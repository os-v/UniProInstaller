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
#include "../../PackageInfo.h"

class CInstallerSysImpl
{
public:

	CInstallerSysImpl();
	~CInstallerSysImpl();

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

private:

	enum { EStepPrepare = 0, EStepExtractFiles, EStepCopyFilesStart, EStepCopyFilesEnd = 5, EStepRegister, EStepConfigure, EStepFinalize, EStepCount =  EStepFinalize };

	EInstallType m_eType;
	CStdString m_sInstalledVersion;
	CStdString m_sPathApp;
	CStdString m_sPathSystem;
	CStdString m_sPathTemp;
	CPackageInfo m_pBundleInfo;

	bool m_fUninstalled;

	FOnInstallProgress m_pOnProgressUpdate;
	void *m_pProgressContext;

	bool InstallPrepare();
	bool InstallRegister();
	bool InstallConfigure();
	bool InstallFinalize();

	bool ProcessFile(const CStdString &sSrcName, const CStdString &sDstName, const CStdString &sDstPath, const CStdString &sAccessMod, const CStdString &sOwner, bool fInstall, const char *lpBundle = 0);
	bool ProcessFiles(bool fInstall, const char *lpBundle = 0);
	bool ProcessCommands(const CStdStringArray &pCommands);

	CStdString GetTempPath();
	CStdString GetAltPathName(LPCTSTR lpFullPath);
	CStdString GetExpandedPath(LPCSTR lpEnvPath);
	bool UnzipPackage(const CStdString &sZipPath, const CStdString &sDstPath);

	bool RegAddUnisntall(const CStdString &sUUID, const CStdString &sInstallPath, const CStdString &sUnisntallPath, const CStdString &sPackageName, const CStdString &sVersion, const CStdString &sPublisher, const CStdString &sLanguage);
	bool RegDelUnisntall(const CStdString &sUUID);
	bool RegGetUninstall(const CStdString &sUUID, CStdString &sVersion, CStdString &sLanguage);
	bool RegSetStr(HKEY hRoot, const CStdStringW &sKey, const CStdStringW &sName, const CStdStringW &sValue);
	bool RegSetInt(HKEY hRoot, const CStdStringW &sKey, const CStdStringW &sName, DWORD nValue);
	bool RegSetValue(HKEY hRoot, const CStdStringW &sKey, const CStdStringW &sName, DWORD nType, PBYTE pData, DWORD nSize);
	bool RegGetStr(HKEY hRoot, const CStdStringW &sKey, const CStdStringW &sName, CStdStringW &sValue);
	bool RegGetValue(HKEY hRoot, const CStdStringW &sKey, const CStdStringW &sName, DWORD &nType, PBYTE &pData, DWORD &nSize);

};

#endif
