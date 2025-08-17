//
//  @file InstallerSysImpl.cpp
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 19.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#include "InstallerSysImpl.h"
#include "../../../Common/UtilsMisc.h"
#include "../../PackageInfo.h"
#include "../../Resources.h"
#include "../../Language.h"
#include <ShlDisp.h>
#include <ShlObj.h>
#include <comutil.h>

DefLogScope(CInstallerSysImpl)

#define REG_MSUNISNTALL			"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\"

CInstallerSysImpl::CInstallerSysImpl()
{

	m_eType = EInstallType::Install;
	m_sInstalledVersion = "";
	m_sPathApp = "";
	m_sPathSystem = "";
	m_sPathTemp = "";

	m_fUninstalled = false;

	m_pOnProgressUpdate = 0;
	m_pProgressContext = 0;

}

CInstallerSysImpl::~CInstallerSysImpl()
{
}

void CInstallerSysImpl::ActivateApp()
{
}

bool CInstallerSysImpl::IsRoot()
{
	return true;
}

bool CInstallerSysImpl::MakeRoot(int nArgc, char *pArgv[])
{
	return true;
}

int CInstallerSysImpl::ExecRoot(const char *lpCommand)
{
	return 0;
}

int CInstallerSysImpl::ExecUser(const char *lpCommand)
{
	return 0;
}

bool CInstallerSysImpl::Prepare(EStartupType eStartupType, int nArgc, char *pArgv[])
{

	LogMessage("CInstallerSysImpl::Prepare(%d)", eStartupType);

	SetProcessDPIAware();

	m_pBundleInfo.Load();

	TCHAR szPath[MAX_PATH];
	SHGetSpecialFolderPath(0, szPath, CSIDL_PROGRAM_FILES, FALSE);
	SetEnvironmentVariableA("PROGRAMS", GetAltPathName(szPath).c_str());
	m_sPathApp = GetExpandedPath(m_pBundleInfo.ProductPath.c_str());

	GetWindowsDirectory(szPath, MAX_PATH);
	m_sPathSystem= ConcatPath(GetAltPathName(szPath), "System32");

	m_sPathTemp = GetTempPath();

	SetEnvironmentVariableA("APPPATH", m_sPathApp.c_str());
	SetEnvironmentVariableA("APPTEMP", m_sPathTemp.c_str());
	SetEnvironmentVariableA("APPBUNDLE", (m_sPathTemp + "\\Bundle").c_str());
	SetEnvironmentVariableA("SYSTEM32", m_sPathSystem.c_str());

	System("rmdir /S /Q \"%s\"", m_sPathTemp.c_str());
	System("mkdir \"%s\"", m_sPathTemp.c_str());

	m_eType = eStartupType == EStartupType::Uninstall || eStartupType == EStartupType::SilentUninstall ? EInstallType::Uninstall : EInstallType::Install;

	if(RegGetUninstall(m_pBundleInfo.ProductUUID, m_sInstalledVersion, m_pBundleInfo.Language))
	{
		if(m_eType != EInstallType::Uninstall && !m_sInstalledVersion.empty())
			m_eType = m_sInstalledVersion < GetInstallerVersion() ? EInstallType::Update : EInstallType::Reinstall;
	}

	LogMessage("CInstallerSysImpl::Prepare() -> %d", m_eType);

	return true;
}

void CInstallerSysImpl::Finalize()
{

	if(m_eType == EInstallType::Uninstall && m_fUninstalled)
	{
		CStdString sScriptPath = GetTempPath() + ".bat";
		CStdString sScriptCode = CStdStrFormat(
			"timeout 1\r\n"
			"rmdir /S /Q \"%s\"\r\n"
			"del /F /Q \"%s\"",
			m_sPathApp.c_str(),
			sScriptPath.c_str()
		);
		SaveFile(sScriptPath.c_str(), sScriptCode.c_str(), sScriptCode.length());
		ShellExecuteA(0, "open", sScriptPath.c_str(), 0, 0, SW_HIDE);
	}

	if(!m_sPathTemp.empty())
		System("rmdir /S /Q \"%s\"", m_sPathTemp.c_str());

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

	LogMessage("CInstallerSysImpl::PerformInstall()");

	CStdString sPathBundle = ConcatPath(m_sPathTemp, "Bundle");
	CStdString sPathBundleZip = ConcatPath(m_sPathTemp, "Bundle.zip");

	if(!OnProgress(EStepPrepare, GSTR(PreparingWait)))
		return false;

	if(!InstallPrepare())
		return false;

	SaveFile(sPathBundleZip.c_str(), (char*)ResBundlePackData(), ResBundlePackSize());

	if(!OnProgress(EStepExtractFiles, GSTR(ExtractingFiles)))
		return false;

	CreateDirectoryA(sPathBundle.c_str(), 0);

	UnzipPackage(sPathBundleZip, sPathBundle);

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

	CStdString sBackupPath = GetTempPath();

	System("rmdir /S /Q \"%s\"", sBackupPath.c_str());
	System("mkdir \"%s\"", sBackupPath.c_str());

	SetEnvironmentVariableA("APPBACK", sBackupPath.c_str());

	if(!ProcessCommands(m_pBundleInfo.RepairPreCommands))
		return OnProgressFailed(GSTR(CommandFailed));

	if(!PerformUninstall(true))
		return false;

	m_sInstalledVersion = "";

	if(!PerformInstall(true))
		return false;

	if(!ProcessCommands(m_pBundleInfo.RepairPostCommands))
		return OnProgressFailed(GSTR(CommandFailed));

	System("rmdir /S /Q \"%s\"", sBackupPath.c_str());

	OnProgressSuccess("Finished");

	return true;
}

bool CInstallerSysImpl::PerformUpdate()
{

	LogMessage("CInstallerSysImpl::PerformUpdate()");

	CStdString sBackupPath = GetTempPath();

	System("rmdir /S /Q \"%s\"", sBackupPath.c_str());
	System("mkdir \"%s\"", sBackupPath.c_str());

	SetEnvironmentVariableA("APPBACK", sBackupPath.c_str());

	if(!ProcessCommands(m_pBundleInfo.UpdatePreCommands))
		return OnProgressFailed(GSTR(CommandFailed));

	if(!PerformUninstall(true))
		return false;

	m_sInstalledVersion = "";

	if(!PerformInstall(true))
		return false;

	if(!ProcessCommands(m_pBundleInfo.UpdatePostCommands))
		return OnProgressFailed(GSTR(CommandFailed));

	System("rmdir /S /Q \"%s\"", sBackupPath.c_str());

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

	if(!OnProgress(EStepFinalize, GSTR(Finalizing)))
		return false;

	if(!ProcessCommands(m_pBundleInfo.UninstallPostCommands))
		return OnProgressFailed(GSTR(CommandFailed));

	RegDelUnisntall(m_pBundleInfo.ProductUUID);

	m_fUninstalled = true;

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

	if(!ProcessCommands(m_pBundleInfo.InstallPreCommands))
		return OnProgressFailed(GSTR(CommandFailed));

	return true;
}

bool CInstallerSysImpl::InstallRegister()
{

	CStdString sUninstallPath = ConcatPath(m_sPathApp, "Uninstall.exe");
	RegAddUnisntall(m_pBundleInfo.ProductUUID, m_sPathApp, sUninstallPath, m_pBundleInfo.ProductName, m_pBundleInfo.ProductVersion, m_pBundleInfo.ProductPublisher, m_pBundleInfo.Language);

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

	LogMessage("CInstallerSysImpl::ProcessFile(%s, %s, %s)", sSrcName.c_str(), sDstName.c_str(), sDstPath.c_str());

	CStdString sDstFile = ConcatPath(GetExpandedPath(sDstPath.c_str()), sDstName);

	LogMessage("CInstallerSysImpl::ProcessFile() -> %s", sDstFile.c_str());

	DWORD nAttrs = GetFileAttributesA(sDstFile.c_str());
	if(nAttrs != INVALID_FILE_ATTRIBUTES)
	{
		if(nAttrs & FILE_ATTRIBUTE_DIRECTORY)
			System("rmdir /S /Q \"%s\"", sDstFile.c_str());
		else
			System("del /F /Q \"%s\"", sDstFile.c_str());
	}

	if(!fInstall)
		return true;

	CStdString sSrcFile = ConcatPath(lpBundle, sSrcName);

	nAttrs = GetFileAttributesA(sSrcFile.c_str());

	System("xcopy \"%s\" \"%s\" /c /e /h /i /r /q /y", sSrcFile.c_str(), (nAttrs & FILE_ATTRIBUTE_DIRECTORY || sSrcName != sDstName ? sDstFile : sDstPath).c_str());

	if(!sAccessMod.empty())
	{
		CStdString sGroup = sOwner.empty() ? "Administrator:Administrators:Everyone" : sOwner;
		uint32_t nModMask = 0;
		sscanf(sAccessMod.c_str(), "%o", &nModMask);
		if((nModMask & (uint32_t)EFilePerm::OA) == (uint32_t)EFilePerm::OA)
			System("icacls \"%s\" /grant Everyone:(OI)(CI)F", sDstFile.c_str());
		else
		{
			enum { EOwnUser = 0, EOwnGroup, EOwnOther };
			CStdStringArray pName = StrSplit(sOwner.c_str(), ':', 0, false);
			CStdString sUName = pName.size() > EOwnUser ? pName[EOwnUser] : "";//"Administrator";
			CStdString sGName = pName.size() > EOwnGroup ? pName[EOwnGroup] : "";//"Administrators";
			CStdString sOName = pName.size() > EOwnOther ? pName[EOwnOther] : "";//"Everyone";
			CStdString sUMask = (nModMask & (uint32_t)EFilePerm::UA) == (uint32_t)EFilePerm::UA ? "F" : (CStdString(nModMask & (uint32_t)EFilePerm::UX ? "RX" : nModMask & (uint32_t)EFilePerm::UR ? "R" : "") + CStdString(nModMask & (uint32_t)EFilePerm::UW ? "W" : ""));
			CStdString sGMask = (nModMask & (uint32_t)EFilePerm::GA) == (uint32_t)EFilePerm::GA ? "F" : (CStdString(nModMask & (uint32_t)EFilePerm::GX ? "RX" : nModMask & (uint32_t)EFilePerm::GR ? "R" : "") + CStdString(nModMask & (uint32_t)EFilePerm::GW ? "W" : ""));
			CStdString sOMask = (nModMask & (uint32_t)EFilePerm::OA) == (uint32_t)EFilePerm::OA ? "F" : (CStdString(nModMask & (uint32_t)EFilePerm::OX ? "RX" : nModMask & (uint32_t)EFilePerm::OR ? "R" : "") + CStdString(nModMask & (uint32_t)EFilePerm::OW ? "W" : ""));
			CStdString sUGrant = sUMask.empty() || sUName.empty() ? "" : CStdStrFormat(" /grant %s:%s", sUName.c_str(), sUMask.c_str());
			CStdString sGGrant = sGMask.empty() || sGName.empty() ? "" : CStdStrFormat(" /grant %s:%s", sGName.c_str(), sGMask.c_str());
			CStdString sOGrant = sOMask.empty() || sOName.empty() ? "" : CStdStrFormat(" /grant %s:%s", sOName.c_str(), sOMask.c_str());
			System("icacls \"%s\"%s%s%s", sDstFile.c_str(), sUGrant.c_str(), sGGrant.c_str(), sOGrant.c_str());
		}
	}

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
		int nExecType = 0, nCheckResult = 0;
		CStdString sCommand;
		bool fCheckResult = CPackageInfo::ParseCommand(pCommands[iCommand].c_str(), nExecType, nCheckResult, sCommand);
		int nResult = SystemInvoke(sCommand.c_str());
		if(fCheckResult && nCheckResult != nResult)
		{
			LogMessage("CInstallerSysImpl::ProcessCommands() -> failed: %d, %d", nCheckResult, nResult);
			return false;
		}
	}
	
	return true;
}

void CInstallerSysImpl::OpenURL(const char *lpURL)
{

	LogMessage("CInstallerSysImpl::OpenURL(%s)", lpURL);

	fl_open_uri(lpURL, 0, 0);

}

CStdString CInstallerSysImpl::GetTempPath()
{
	TCHAR szTempPath[MAX_PATH];
	::GetTempPath(MAX_PATH, szTempPath);
	return ConcatPath(GetAltPathName(szTempPath), CStdStrFormat("%llX", GetTickCount64()));
}

CStdString CInstallerSysImpl::GetAltPathName(LPCTSTR lpFullPath)
{
	TCHAR szShortPath[MAX_PATH];
	GetShortPathName(lpFullPath, szShortPath, MAX_PATH);
	return CStdString(szShortPath, szShortPath + lstrlen(szShortPath));
}

CStdString CInstallerSysImpl::GetExpandedPath(LPCSTR lpEnvPath)
{
	CHAR szPath[MAX_PATH];
	ExpandEnvironmentStringsA(lpEnvPath, szPath, MAX_PATH);
	return szPath;
}

bool CInstallerSysImpl::UnzipPackage(const CStdString &sZipPath, const CStdString &sDstPath)
{

	LogMessage("CInstallerSysImpl::UnzipPackage()");

	CoInitialize(0);

	HRESULT hr;
	IShellDispatch *pShell = 0;
	hr = CoCreateInstance(CLSID_Shell, 0, CLSCTX_INPROC_SERVER, __uuidof(IShellDispatch), (void**)&pShell);
	if(FAILED(hr))
	{
		LogMessage("CInstallerSysImpl::UnzipPackage() -> Create: %X", hr);
		return false;
	}

	Folder *pZipFile = 0, *pDstFolder = 0;
	hr = pShell->NameSpace(variant_t(sZipPath.c_str()), &pZipFile);
	if(!FAILED(hr))
		hr = pShell->NameSpace(variant_t(sDstPath.c_str()), &pDstFolder);

	if(!FAILED(hr))
	{
		FolderItems *pZipItems = 0;
		hr = pZipFile->Items(&pZipItems);
		if(!FAILED(hr))
			hr = pDstFolder->CopyHere(variant_t(pZipItems), variant_t((long)(2048 | 1024 | 512 | 16 | 4), VT_I4));
	}

	pShell->Release();

	LogMessage("CInstallerSysImpl::UnzipPackage() -> finished");

	return !FAILED(hr);
}

bool CInstallerSysImpl::RegAddUnisntall(const CStdString &sUUID, const CStdString &sInstallPath, const CStdString &sUnisntallPath, const CStdString &sPackageName, const CStdString &sVersion, const CStdString &sPublisher, const CStdString &sLanguage)
{

	CStdString sRegPathA = CStdString(REG_MSUNISNTALL) + "{" + sUUID + "}";
	CStdStringW sRegPath = TOSTRW(sRegPathA);

	RegSetStr(HKEY_LOCAL_MACHINE, sRegPath, L"DisplayIcon", TOSTRW(sUnisntallPath));
	RegSetStr(HKEY_LOCAL_MACHINE, sRegPath, L"DisplayName", TOSTRW(sPackageName));
	RegSetStr(HKEY_LOCAL_MACHINE, sRegPath, L"DisplayVersion", TOSTRW(sVersion));
	RegSetStr(HKEY_LOCAL_MACHINE, sRegPath, L"InstallLocation", TOSTRW(sInstallPath));
	RegSetInt(HKEY_LOCAL_MACHINE, sRegPath, L"NoModify", 1);
	RegSetInt(HKEY_LOCAL_MACHINE, sRegPath, L"NoRepair", 1);
	RegSetStr(HKEY_LOCAL_MACHINE, sRegPath, L"Publisher", TOSTRW(sPublisher));
	RegSetStr(HKEY_LOCAL_MACHINE, sRegPath, L"UninstallString", TOSTRW(sUnisntallPath));
	RegSetStr(HKEY_LOCAL_MACHINE, sRegPath, L"Language", TOSTRW(sLanguage));

	return true;
}

bool CInstallerSysImpl::RegDelUnisntall(const CStdString &sUUID)
{

	CStdString sRegPath = CStdString(REG_MSUNISNTALL) + "{" + sUUID + "}";

	LSTATUS lStatus = RegDeleteKeyA(HKEY_LOCAL_MACHINE, sRegPath.c_str());

	return lStatus == ERROR_SUCCESS;
}

bool CInstallerSysImpl::RegGetUninstall(const CStdString &sUUID, CStdString &sVersion, CStdString &sLanguage)
{

	CStdString sRegPathA = CStdString(REG_MSUNISNTALL) + "{" + sUUID + "}";
	CStdStringW sRegPath = TOSTRW(sRegPathA);

	CStdStringW sVersionW;
	if(!RegGetStr(HKEY_LOCAL_MACHINE, sRegPath.c_str(), L"DisplayVersion", sVersionW))
		return false;

	CStdStringW sLanguageW;
	if(RegGetStr(HKEY_LOCAL_MACHINE, sRegPath.c_str(), L"Language", sLanguageW))
		sLanguage.assign(sLanguageW.begin(), sLanguageW.end());

	sVersion.assign(sVersionW.begin(), sVersionW.end());

	return true;
}

bool CInstallerSysImpl::RegSetStr(HKEY hRoot, const CStdStringW &sKey, const CStdStringW &sName, const CStdStringW &sValue)
{
	return RegSetValue(hRoot, sKey, sName, REG_SZ, (PBYTE)sValue.c_str(), (DWORD)sValue.length() * sizeof(TCHAR));
}

bool CInstallerSysImpl::RegSetInt(HKEY hRoot, const CStdStringW &sKey, const CStdStringW &sName, DWORD nValue)
{
	return RegSetValue(hRoot, sKey, sName, REG_DWORD, (PBYTE)&nValue, sizeof(nValue));
}

bool CInstallerSysImpl::RegSetValue(HKEY hRoot, const CStdStringW &sKey, const CStdStringW &sName, DWORD nType, PBYTE pData, DWORD nSize)
{

	HKEY hKey = 0;
	LSTATUS nResult = RegCreateKeyEx(hRoot, sKey.c_str(), 0, 0, 0, KEY_WRITE, 0, &hKey, 0);
	if (nResult != ERROR_SUCCESS)
		return REG_NONE;

	nResult = RegSetValueEx(hKey, sName.c_str(), 0, nType, pData, nSize);

	RegCloseKey(hKey);

	return nResult == ERROR_SUCCESS;
}

bool CInstallerSysImpl::RegGetStr(HKEY hRoot, const CStdStringW &sKey, const CStdStringW &sName, CStdStringW &sValue)
{

	PBYTE pData = 0;
	DWORD nType = 0, nSize = 0;
	if(!RegGetValue(hRoot, sKey, sName, nType, pData, nSize))
		return false;

	std::auto_ptr<BYTE> aData(pData);

	if(nType != REG_SZ && nType != REG_MULTI_SZ && nType != REG_EXPAND_SZ)
		return false;

	sValue = CStdStringW((WCHAR*)pData);

	return true;
}

bool CInstallerSysImpl::RegGetValue(HKEY hRoot, const CStdStringW &sKey, const CStdStringW &sName, DWORD &nType, PBYTE &pData, DWORD &nSize)
{

	nSize = 0;
	LSTATUS nResult = ::RegGetValueW(hRoot, sKey.c_str(), sName.c_str(), RRF_RT_ANY, &nType, 0, &nSize);
	if(nResult != ERROR_SUCCESS)
		return false;

	pData = new BYTE[nSize];
	nResult = ::RegGetValueW(hRoot, sKey.c_str(), sName.c_str(), RRF_RT_ANY, &nType, pData, &nSize);
	if(nResult != ERROR_SUCCESS)
	{
		delete pData;
		pData = 0;
		nSize = 0;
	}

	return nResult == ERROR_SUCCESS;
}

