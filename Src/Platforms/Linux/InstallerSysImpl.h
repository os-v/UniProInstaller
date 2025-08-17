//
//  @file InstallerSysImpl.h
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 19.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#ifndef INSTALLERSYSIMPL_H
#define INSTALLERSYSIMPL_H

#include "../../InstallerBase.h"
#include "../../../Common/ConfigFileConf.h"

class CInstallerSysImpl
{
public:

	CInstallerSysImpl();
	~CInstallerSysImpl();

	void ActivateApp();

	bool IsRoot();
	bool MakeRoot(int nArgc, char *pArgv[]);
	int ExecRoot(const char *lpCommand);
	int ExecUser(const char *lpCommand);

	bool Prepare(EStartupType eStartupType, int nArgc, char *pArgv[]);
	void Finalize();

	bool Create(FOnInstallProgress pOnProgressUpdate, void *pContextContext);

	bool PerformInstall(bool fInternal = false);
	bool PerformReinstall();
	bool PerformUpdate();
	bool PerformUninstall(bool fInternal = false);

	void SetUninstallType() {
		m_eType = EInstallType::Uninstall;
	}

	EInstallType GetType() {
		return m_eType;
	}

	bool IsUninstallAllowed() {
		return !m_sInstalledVersion.empty();
	}

	const char *GetInstalledVersion() {
		return m_sInstalledVersion.c_str();
	}

	const char *GetInstallerVersion() {
		return m_pBundleInfo.ProductVersion.c_str();
	}

	const char *GetProductName() {
		return m_pBundleInfo.ProductName.c_str();
	}

	CPackageInfo *GetBundleInfo() {
		return &m_pBundleInfo;
	}

	static void OpenURL(const char *lpURL);

protected:

	enum { EStepPrepare = 0, EStepExtractFiles, EStepCopyFilesStart, EStepCopyFilesEnd = 5, EStepRegister, EStepConfigure, EStepFinalize, EStepCount =  EStepFinalize };

	EInstallType m_eType;
	CStdString m_sInstalledVersion;
	CStdString m_sPathApp;
	CStdString m_sPathTemp;
	CPackageInfo m_pBundleInfo;

	FOnInstallProgress m_pOnProgressUpdate;
	void *m_pProgressContext;

	bool InstallPrepare();
	bool InstallRegister();
	bool InstallConfigure();
	bool InstallFinalize();

	bool ProcessFile(const CStdString &sSrcName, const CStdString &sDstName, const CStdString &sDstPath, const CStdString &sAccessMod, const CStdString &sOwner, bool fInstall, const char *lpBundle = 0);
	bool ProcessFiles(bool fInstall, const char *lpBundle = 0);
	bool ProcessCommands(const CStdStringArray &pCommands);

	CStdString GetTempFolder();
	CStdString GetUninstallConfPath();

};

#endif
