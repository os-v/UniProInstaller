//
//  @file InstallerSysImpl.cpp
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 19.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#include "InstallerSysImpl.h"
#include <pwd.h>
#include <grp.h>

#include "../../../Common/UtilsMisc.h"
#include "../../../Common/incbin.h"
#include "../../Language.h"

#ifndef DEFUNINSTALL
	INCBIN(ResBundle, "Bundle/Linux.zip");
#endif

INCBIN(ResBundleInfo, "Bundle/Linux.info");

DefLogScope(CInstallerSysImpl)

CInstallerSysImpl::CInstallerSysImpl()
{

	m_eType = EInstallType::Install;
	m_sInstalledVersion = "";
	m_sPathApp = "";
	m_sPathTemp = "";

	m_pOnProgressUpdate = 0;
	m_pProgressContext = 0;

}

CInstallerSysImpl::~CInstallerSysImpl()
{
}

bool CInstallerSysImpl::IsRoot()
{
	uid_t nResult = geteuid();
	LogMessage("CInstallerSysImpl::IsRoot() -> %d", nResult);
	return !nResult;
}

bool CInstallerSysImpl::MakeRoot(int nArgc, char *pArgv[])
{

	DetachProcess();

	CStdString sCommand = "pkexec env DISPLAY=$DISPLAY XAUTHORITY=$XAUTHORITY";
	for(int iArg = 0; iArg < nArgc; iArg++)
		sCommand += CStdStrFormat(" \"%s\"", pArgv[iArg]);

	System(sCommand.c_str());

	return true;
}

int CInstallerSysImpl::ExecRoot(const char *lpCommand)
{

	uid_t eUID = getuid();
	uid_t eEUID = geteuid();

	int nResult = setuid(0);

	LogMessage("CInstallerSysImpl::ExecRoot() -> %d, %d, %d", eUID, eEUID, nResult);

	return System(lpCommand);
}

int CInstallerSysImpl::ExecUser(const char *lpCommand)
{
	return 0;
}

bool CInstallerSysImpl::Prepare(EStartupType eStartupType, int nArgc, char *pArgv[])
{

	LogMessage("CInstallerSysImpl::Prepare(%d)", eStartupType);

	m_eType = eStartupType == EStartupType::Uninstall || eStartupType == EStartupType::SilentUninstall ? EInstallType::Uninstall : EInstallType::Install;

	m_pBundleInfo.Load(CStdString(gResBundleInfoData, gResBundleInfoData + gResBundleInfoSize).c_str());

	m_sPathApp = m_pBundleInfo.ProductPath;
	m_sPathTemp = GetTempFolder();

	setenv("APPPATH", m_sPathApp.c_str(), true);
	setenv("APPTEMP", m_sPathTemp.c_str(), true);
	setenv("APPBUNDLE", (m_sPathTemp + "\\Bundle").c_str(), true);

	System("rm -f -R \"%s\"", m_sPathTemp.c_str());
	System("mkdir \"%s\"", m_sPathTemp.c_str());

	CConfigFileConf pConfig;
	if(pConfig.Load(GetUninstallConfPath().c_str()))
	{
		m_sInstalledVersion = pConfig.GetStr("Version", m_pBundleInfo.ProductVersion.c_str());
		m_pBundleInfo.Language = pConfig.GetStr("Language", m_pBundleInfo.Language.c_str());
		if(m_eType != EInstallType::Uninstall && !m_sInstalledVersion.empty())
			m_eType = m_sInstalledVersion < m_pBundleInfo.ProductVersion ? EInstallType::Update : EInstallType::Reinstall;
	}

	LogMessage("CInstallerSysImpl::Prepare() -> %d", m_eType);

	return true;
}

void CInstallerSysImpl::Finalize()
{

	if(!m_sPathTemp.empty())
		System("rm -f -R \"%s\"", m_sPathTemp.c_str());

}

bool CInstallerSysImpl::Create(FOnInstallProgress pOnProgressUpdate, void *pContextContext)
{

	m_pOnProgressUpdate = pOnProgressUpdate;
	m_pProgressContext = pContextContext;

	return true;
}

bool CInstallerSysImpl::PerformInstall(bool fInternal)
{
#ifndef DEFUNINSTALL
	CStdString sPathBundle = ConcatPath(m_sPathTemp, "Bundle");
	CStdString sPathBundleZip = ConcatPath(m_sPathTemp, "Bundle.zip");

	if(!OnProgress(EStepPrepare, GSTR(PreparingWait)))
		return false;

	if(!InstallPrepare())
		return false;

	SaveFile(sPathBundleZip.c_str(), (char*)gResBundleData, gResBundleSize);

	if(!OnProgress(EStepExtractFiles, GSTR(ExtractingFiles)))
		return false;
	System("unzip -o -qq \"%s\" -d \"%s\"", sPathBundleZip.c_str(), sPathBundle.c_str());

	if(!ProcessFiles(true, sPathBundle.c_str()))
		return false;

	if(!OnProgress(EStepRegister, GSTR(Registering)) || !InstallRegister())
		return false;

	if(!OnProgress(EStepConfigure, GSTR(Configuring)) || !InstallConfigure())
		return false;

	if(!OnProgress(EStepFinalize, GSTR(Finalizing)) || !InstallFinalize())
		return false;

	Sleep(500);

	if(!fInternal)
		OnProgressSuccess("Finished");
#endif
	return true;
}

bool CInstallerSysImpl::PerformReinstall()
{

	LogMessage("CInstallerSysImpl::PerformReinstall()");

	CStdString sBackupPath = GetTempFolder();

	System("rm -f -R \"%s\"", sBackupPath.c_str());
	System("mkdir \"%s\"", sBackupPath.c_str());

	setenv("APPBACK", sBackupPath.c_str(), true);

	if(!ProcessCommands(m_pBundleInfo.RepairPreCommands))
		return OnProgressFailed(GSTR(CommandFailed));

	if(!PerformUninstall(true))
		return false;

	m_sInstalledVersion = "";

	if(!PerformInstall(true))
		return false;

	if(!ProcessCommands(m_pBundleInfo.RepairPostCommands))
		return OnProgressFailed(GSTR(CommandFailed));

	System("rm -f -R \"%s\"", sBackupPath.c_str());

	OnProgressSuccess("Finished");

	return true;
}

bool CInstallerSysImpl::PerformUpdate()
{

	LogMessage("CInstallerSysImpl::PerformUpdate()");

	CStdString sBackupPath = GetTempFolder();

	System("rm -f -R \"%s\"", sBackupPath.c_str());
	System("mkdir \"%s\"", sBackupPath.c_str());

	setenv("APPBACK", sBackupPath.c_str(), true);

	if(!ProcessCommands(m_pBundleInfo.UpdatePreCommands))
		return OnProgressFailed(GSTR(CommandFailed));

	if(!PerformUninstall(true))
		return false;

	m_sInstalledVersion = "";

	if(!PerformInstall(true))
		return false;

	if(!ProcessCommands(m_pBundleInfo.UpdatePostCommands))
		return OnProgressFailed(GSTR(CommandFailed));

	System("rm -f -R \"%s\"", sBackupPath.c_str());

	OnProgressSuccess("Finished");

	return true;
}

bool CInstallerSysImpl::PerformUninstall(bool fInternal)
{

	LogMessage("CInstallerSysImpl::PerformUninstall()");

	if(!OnProgress(EStepPrepare, GSTR(PreparingWait)))
		return false;

	if(!ProcessCommands(m_pBundleInfo.UninstallPreCommands))
		return OnProgressFailed(GSTR(CommandFailed));

	if(!OnProgress(EStepCopyFilesStart, GSTR(RemovingFiles)))
		return false;

	ProcessFiles(false);

	if(!ProcessCommands(m_pBundleInfo.UninstallPostCommands))
		return OnProgressFailed(GSTR(CommandFailed));

	if(!OnProgress(EStepFinalize, GSTR(Finalizing)))
		return false;

	System("rm -f -R \"%s\"", m_sPathApp.c_str());
	System("rm -f \"%s\"", GetUninstallConfPath().c_str());

	if(!fInternal && !OnProgressSuccess("Finished"))
		return false;

	return true;
}

bool CInstallerSysImpl::InstallPrepare()
{

	if(!m_sInstalledVersion.empty())
	{
		FOnInstallProgress pOnProgressUpdate = m_pOnProgressUpdate;
		m_pOnProgressUpdate = 0;
		bool fUninstall = PerformUninstall();
		m_pOnProgressUpdate = pOnProgressUpdate;
		if(!fUninstall)
		{
			OnProgressFailed(GSTR(UnableToUninstall));
			return false;
		}
	}

	System("mkdir \"%s\"", m_sPathApp.c_str());
	System("chmod 777 \"%s\"", m_sPathApp.c_str());

	if(!ProcessCommands(m_pBundleInfo.InstallPreCommands))
		return OnProgressFailed(GSTR(CommandFailed));

	return true;
}

bool CInstallerSysImpl::InstallRegister()
{

	CConfigFileConf pConfig;
	pConfig.SetStr("UUID", m_pBundleInfo.ProductUUID.c_str());
	pConfig.SetStr("Version", m_pBundleInfo.ProductVersion.c_str());
	pConfig.SetStr("Language", m_pBundleInfo.Language.c_str());
	pConfig.Save(GetUninstallConfPath().c_str());

	return true;
}

bool CInstallerSysImpl::InstallConfigure()
{

	if(!ProcessCommands(m_pBundleInfo.InstallPostCommands))
		return OnProgressFailed(GSTR(CommandFailed));

	return true;
}

bool CInstallerSysImpl::InstallFinalize()
{
	return true;
}

bool CInstallerSysImpl::ProcessFile(const CStdString &sSrcName, const CStdString &sDstName, const CStdString &sDstPath, const CStdString &sAccessMod, const CStdString &sOwner, bool fInstall, const char *lpBundle)
{

	CStdString sDstFile = ConcatPath(sDstPath, sDstName);

	System("rm -f -R \"%s\"", sDstFile.c_str());
	if(!fInstall)
		return true;

	CStdString sSrcFile = ConcatPath(lpBundle, sSrcName);

	System("cp -a \"%s\" \"%s\"", sSrcFile.c_str(), (sDstName != sSrcName ? sDstFile : sDstPath).c_str());

	if(!sAccessMod.empty())
		System("chmod %s \"%s\"", sAccessMod.c_str(), sDstFile.c_str());

	if(!sOwner.empty())
		System("chown %s \"%s\"", sOwner.c_str(), sDstFile.c_str());

	return true;
}

bool CInstallerSysImpl::ProcessFiles(bool fInstall, const char *lpBundle)
{

	uint32_t nFiles = m_pBundleInfo.GetFilesCount();
	for(uint32_t iFile = 0; iFile < nFiles; iFile++)
	{
		CStdString sSrcName, sDstName, sDstPath, sAccessMod, sOwner;
		m_pBundleInfo.GetFilesInfo(iFile, sSrcName, sDstName, sDstPath, sAccessMod, sOwner);
		uint32_t nStep = EStepCopyFilesStart + iFile * (EStepCopyFilesEnd - EStepCopyFilesStart + 1) / nFiles;
		if(!OnProgress(nStep, (fInstall ? GSTR(CopyingFiles) : GSTR(RemovingFiles))))
			return false;
		if(!ProcessFile(sSrcName, sDstName, sDstPath, sAccessMod, sOwner, fInstall, lpBundle))
			return false;
	}

	return true;
}

bool CInstallerSysImpl::ProcessCommands(const CStdStringArray &pCommands)
{

	for(uint32_t iCommand = 0; iCommand < pCommands.size(); iCommand++)
	{
		if(!CPackageInfo::ProcessCommand(pCommands[iCommand].c_str()))
			return false;
	}

	return true;
}

void CInstallerSysImpl::OpenURL(const char *lpURL)
{

	LogMessage("CInstallerSysImpl::OpenURL(%s)", lpURL);

	const char *lpUserName = getlogin();

	System("pkexec --user %s env DISPLAY=$DISPLAY XAUTHORITY=$XAUTHORITY \"%s\" -Hyperlink \"%s\"", lpUserName, CInstallerBase::SelfPath().c_str(), lpURL);

}

CStdString CInstallerSysImpl::GetTempFolder()
{
	timespec pTimeSpec;
	clock_gettime(CLOCK_MONOTONIC, &pTimeSpec);
	uint64_t nTickCount = pTimeSpec.tv_nsec / 1000000;
	nTickCount += pTimeSpec.tv_sec * 1000;
	char *lpTempDir = getenv("TMPDIR");
	CStdString sTempPath = lpTempDir ? lpTempDir : "/tmp";
	return ConcatPath(sTempPath, CStdStrFormat("%llX", nTickCount));
}

CStdString CInstallerSysImpl::GetUninstallConfPath()
{
	return CStdStrFormat("/etc/%s.info", m_pBundleInfo.ProductName.c_str());
}

