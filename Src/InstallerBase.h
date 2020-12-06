//
//  @file InstallerBase.h
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 15.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#ifndef INSTALLERBASE_H
#define INSTALLERBASE_H

#include "../Common/UtilsMisc.h"
#include "../Common/ThreadImpl.h"
#include "PackageInfo.h"
#include <FL/filename.H>

#define CInstallerBase			CInstallerBaseImpl<CInstallerSysImpl>

#define EStepFailed				-1
#define EStepSuccess			1000
#define EStepInvalid			-1000

#define OnProgress(eStep, ...)					\
		(m_pOnProgressUpdate ? m_pOnProgressUpdate(eStep * 100 / EStepCount, CStdStrFormat(__VA_ARGS__), m_pProgressContext) : true)

#define OnProgressFailed(...)			\
		(m_pOnProgressUpdate ? m_pOnProgressUpdate(EStepFailed, CStdStrFormat(__VA_ARGS__), m_pProgressContext) : true)

#define OnProgressSuccess(...)			\
		(m_pOnProgressUpdate ? m_pOnProgressUpdate(EStepSuccess, CStdStrFormat(__VA_ARGS__), m_pProgressContext) : true)

enum class EInstallType { Invalid = 0, Install, Reinstall, Update, Uninstall };
enum class EStartupType { Invalid = 0, Install, Uninstall, SilentInstall, SilentUninstall, StartService, StopService };

enum class EFilePerm { UR = 0400, UW = 0200, UX = 0100, UA = 0700, GR = 040, GW = 020, GX = 010, GA = 070, OR = 04, OW = 02, OX = 01, OA = 07, ALL = 0777 };

typedef bool (*FOnInstallProgress)(int nStep, const CStdString &sText, void *pContext);

template<class TPlatformImpl>
class CInstallerBaseImpl
{
	DefLogScope(CInstallerBaseImpl)
public:

	typedef CInstallerBaseImpl<TPlatformImpl> ClassType;

	CInstallerBaseImpl()
	{

		m_fAbort = false;

		m_nResult = 0;

		m_eStartupType = EStartupType::Invalid;

		m_pOnInstallProgress = 0;
		m_pProgressContext = 0;

		m_pThread = 0;

	}

	~CInstallerBaseImpl()
	{

	}

	bool Prepare(int nArgc, char *pArgv[])
	{
		LogMessage("CInstallerBaseImpl::Prepare(%s)", pArgv[0]);
		if(!ProcessCommandLine(nArgc, pArgv))
			return false;
		LogMessage("CInstallerBaseImpl::Prepare() -> %d", m_eStartupType);
		bool fSilent = m_eStartupType == EStartupType::SilentInstall || m_eStartupType == EStartupType::SilentUninstall;
		if(fSilent && !m_pPlatformImpl.IsRoot())
		{
			printf("Administrator privileges required.\r\n");
			return false;
		}
		if(!m_pPlatformImpl.IsRoot())
		{
			m_pPlatformImpl.MakeRoot(nArgc, pArgv);
			return false;
		}
		bool fResult = m_pPlatformImpl.Prepare(m_eStartupType, nArgc, pArgv);
		if(!fResult)
			return false;
		if(fSilent)
		{
			m_pOnInstallProgress = SOnProgressUpdateStdout;
			m_pProgressContext = this;
			PerformTask();
			return false;
		}
		return true;
	}

	void Finalize()
	{
		m_pPlatformImpl.Finalize();
	}

	void SetUninstallType()
	{
		m_pPlatformImpl.SetUninstallType();
	}

	bool Create(FOnInstallProgress pOnProgressUpdate, void *pProgressContext)
	{
		m_pOnInstallProgress = pOnProgressUpdate;
		m_pProgressContext = pProgressContext;
		return m_pPlatformImpl.Create(SOnProgressUpdate, this);
	}

	EInstallType GetType() {
		return m_pPlatformImpl.GetType();
	}

	bool IsUninstallAllowed() {
		return m_pPlatformImpl.IsUninstallAllowed();
	}

	const char *GetInstalledVersion() {
		return m_pPlatformImpl.GetInstalledVersion();
	}

	const char *GetInstallerVersion() {
		return m_pPlatformImpl.GetInstallerVersion();
	}

	const char *GetProductName() {
		return m_pPlatformImpl.GetProductName();
	}

	CPackageInfo *GetBundleInfo() {
		return m_pPlatformImpl.GetBundleInfo();
	}

	bool Start(bool fAsync)
	{
		if(fAsync)
			m_pThread = CThreadImpl::CreateThread(SOnThreadProcFunc, this);
		else
			PerformTask();
		return true;
	}

	void Stop()
	{
		m_fAbort = true;
		if(m_pThread)
			m_pThread->Abort(-1);
	}

	int GetResult() {
		return m_nResult;
	}

	static CStdString &SelfPath() {
		static CStdString sSelfPath;
		return sSelfPath;
	}

	static int ExecRoot(const char *lpCommand)
	{
		CStdString sLogArg = LogGetLevel() > ELogLevelNone ? CStdStrFormat(" \"-l%s\"", LogFilePath()) : CStdString("");
		return System("\"%s\"%s%s -ExecRoot \"%s\"", SelfPath().c_str(), sLogArg.c_str(), LogIsLogStd() ? " -v" : "", lpCommand);
	}

	static int ExecUser(const char *lpCommand)
	{
		CStdString sLogArg = LogGetLevel() > ELogLevelNone ? CStdStrFormat(" \"-l%s\"", LogFilePath()) : CStdString("");
		return System("\"%s\"%s%s -ExecUser \"%s\"", SelfPath().c_str(), sLogArg.c_str(), LogIsLogStd() ? " -v" : "", lpCommand);
	}

protected:

	bool m_fAbort;

	int m_nResult;

	TPlatformImpl m_pPlatformImpl;

	EStartupType m_eStartupType;

	FOnInstallProgress m_pOnInstallProgress;
	void *m_pProgressContext;

	CThreadImpl *m_pThread;

	static uint32_t SOnThreadProcFunc(CThreadImpl *pThread, void *pContext) {
		ClassType *pThis = ((ClassType*)pContext);
		pThis->PerformTask();
		return 0;
	}

	static bool SOnProgressUpdate(int nStep, const CStdString &sText, void *pContext)
	{
		ClassType *pThis = (ClassType*)pContext;
		if(pThis->m_fAbort)
			return false;
		return pThis->m_pOnInstallProgress(nStep, sText, pThis->m_pProgressContext);
	}

	static bool SOnProgressUpdateStdout(int nStep, const CStdString &sText, void *pContext)
	{
		ClassType *pThis = (ClassType*)pContext;
		if(pThis->m_fAbort)
			return false;
		printf("\r%d: %s", nStep, sText.c_str());
		return true;
	}

	void PerformTask()
	{
		EInstallType eType = GetType();
		if(eType == EInstallType::Install)
			m_pPlatformImpl.PerformInstall();
		else if(eType == EInstallType::Reinstall)
			m_pPlatformImpl.PerformReinstall();
		else if(eType == EInstallType::Update)
			m_pPlatformImpl.PerformUpdate();
		else if(eType == EInstallType::Uninstall)
			m_pPlatformImpl.PerformUninstall();
	}

	bool ProcessCommandLine(int nArgc, char *pArgv[])
	{

		SelfPath() = pArgv[0];

		for(int iArgc = 1; iArgc < nArgc; iArgc++)
		{
			if(!strcmp(pArgv[iArgc], "-i"))
				m_eStartupType = EStartupType::Install;
			else if(!strcmp(pArgv[iArgc], "-is"))
				m_eStartupType = EStartupType::SilentInstall;
			else if(!strcmp(pArgv[iArgc], "-u"))
				m_eStartupType = EStartupType::Uninstall;
			else if(!strcmp(pArgv[iArgc], "-us"))
				m_eStartupType = EStartupType::SilentUninstall;
			else if(StrStartsWith(pArgv[iArgc], "-ExecRoot") && iArgc < nArgc - 1)
			{
				const char *lpCommand = pArgv[++iArgc];
				LogMessage("CInstallerBaseImpl::ProcessCommandLine() -> ExecRoot(%s)", lpCommand);
				m_nResult = m_pPlatformImpl.ExecRoot(lpCommand);
				return false;
			}
			else if(StrStartsWith(pArgv[iArgc], "-ExecUser") && iArgc < nArgc - 1)
			{
				const char *lpCommand = pArgv[++iArgc];
				LogMessage("CInstallerBaseImpl::ProcessCommandLine() -> ExecUser(%s)", lpCommand);
				m_nResult = m_pPlatformImpl.ExecUser(lpCommand);
				return false;
			}
			else if(StrStartsWith(pArgv[iArgc], "-Hyperlink") && iArgc < nArgc - 1)
			{
				const char *lpURL = pArgv[++iArgc];
				LogMessage("CInstallerBaseImpl::ProcessCommandLine() -> OpenHyperlink(%s)", lpURL);
				fl_open_uri(lpURL, 0, 0);
				return false;
			}
		}

		if(m_eStartupType == EStartupType::Invalid)
		{
#ifdef DEFUNINSTALL
			m_eStartupType = EStartupType::Uninstall;
#else
			m_eStartupType = EStartupType::Install;
#endif
		}

		return true;
	}

};

#endif
