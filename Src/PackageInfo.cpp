//
//  @file PackageInfo.cpp
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 19.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#include "PackageInfo.h"
#include "Resources.h"
#include "InstallerBase.h"
#include "Wizard/WizardSheet.h"

#define KEY_COMMON				"Common"
#define KEY_VERSION				"Version"
#define KEY_APPPATH				"AppPath"
#define KEY_PRODUCTNAME				"ProductName"
#define KEY_DISPLAYNAME				"DisplayName"
#define KEY_PUBLISHER				"Publisher"
#define KEY_UUID				"UUID"
#define KEY_LANGUAGE			"Language"
#define KEY_FILES				"Files"
#define KEY_AUTORUN				"AutoRun"
#define KEY_SHORTCUTS				"Shortcuts"
#define KEY_INSTALL				"Install"
#define KEY_INSTALLPRECOMMANDS			"Install.PreCommands"
#define KEY_INSTALLPOSTCOMMANDS			"Install.PostCommands"
#define KEY_UPDATEPRECOMMANDS			"Update.PreCommands"
#define KEY_UPDATEPOSTCOMMANDS			"Update.PostCommands"
#define KEY_REPAIRPRECOMMANDS			"Repair.PreCommands"
#define KEY_REPAIRPOSTCOMMANDS			"Repair.PostCommands"
#define KEY_UNINSTALLPRECOMMANDS		"Uninstall.PreCommands"
#define KEY_UNINSTALLPOSTCOMMANDS		"Uninstall.PostCommands"
#define KEY_FINISHPROMPT			"FinishPrompt"
#define KEY_EXTRACONFIG				"ExtraConfig"
#define KEY_PRECOMMANDS				"PreCommands"
#define KEY_POSTCOMMANDS			"PostCommands"
#define KEY_UPDATE				"Update"
#define KEY_REPAIR				"Repair"
#define KEY_UNINSTALL				"Uninstall"

DefLogScope(CPackageInfo)

CPackageInfo::CPackageInfo()
{

	Language = "";

	ProductPath = "";
	ProductVersion = "";
	ProductUUID = "";
	ProductName = "";
	ProductPublisher = "";

	InstallExtraConfig = false;
	InstallFinishPrompt = "";
	UninstallFinishPrompt = "";

}

CPackageInfo::~CPackageInfo()
{

}

bool CPackageInfo::Load(const char *lpData)
{

	CStdString sData;
	if(!lpData)
	{
		sData.assign(ResBundleInfoData(), ResBundleInfoData() + ResBundleInfoSize());
		lpData = sData.c_str();
	}

	m_pBundleInfo.LoadData(lpData);

	ProductPath = m_pBundleInfo.GetStr(KEY_COMMON, KEY_APPPATH);
	ProductVersion = m_pBundleInfo.GetStr(KEY_COMMON, KEY_VERSION, "1.0.1");
	ProductUUID = m_pBundleInfo.GetStr(KEY_COMMON, KEY_UUID);
	ProductName = m_pBundleInfo.GetStr(KEY_COMMON, KEY_PRODUCTNAME);
	ProductPublisher = m_pBundleInfo.GetStr(KEY_COMMON, KEY_PUBLISHER);
	Language = m_pBundleInfo.GetStr(KEY_COMMON, KEY_LANGUAGE);

	InstallExtraConfig = m_pBundleInfo.GetBool(KEY_INSTALL, KEY_EXTRACONFIG);
	InstallFinishPrompt = m_pBundleInfo.GetStr(KEY_INSTALL, KEY_FINISHPROMPT);
	UninstallFinishPrompt = m_pBundleInfo.GetStr(KEY_UNINSTALL, KEY_FINISHPROMPT);

	m_pProductFiles = m_pBundleInfo.EnumItems(KEY_FILES);

	InstallPreCommands = m_pBundleInfo.GetItemsArray(KEY_INSTALLPRECOMMANDS);
	InstallPostCommands = m_pBundleInfo.GetItemsArray(KEY_INSTALLPOSTCOMMANDS);
	UpdatePreCommands = m_pBundleInfo.GetItemsArray(KEY_UPDATEPRECOMMANDS);
	UpdatePostCommands = m_pBundleInfo.GetItemsArray(KEY_UPDATEPOSTCOMMANDS);
	RepairPreCommands = m_pBundleInfo.GetItemsArray(KEY_REPAIRPRECOMMANDS);
	RepairPostCommands = m_pBundleInfo.GetItemsArray(KEY_REPAIRPOSTCOMMANDS);
	UninstallPreCommands = m_pBundleInfo.GetItemsArray(KEY_UNINSTALLPRECOMMANDS);
	UninstallPostCommands = m_pBundleInfo.GetItemsArray(KEY_UNINSTALLPOSTCOMMANDS);

	return !ProductUUID.empty() && !ProductName.empty();
}

uint32_t CPackageInfo::GetFilesCount()
{
	return (uint32_t)m_pProductFiles.size();
}

bool CPackageInfo::GetFilesInfo(uint32_t iFile, CStdString &sSrcName, CStdString &sDstName, CStdString &sDstPath, CStdString &sAccessMask, CStdString &sOwner)
{

	if(iFile >= m_pProductFiles.size())
		return false;

	CStdString sFileDesc = m_pBundleInfo.GetStr(KEY_FILES, m_pProductFiles[iFile].c_str());
	CStdStringArray pFileDesc = StrSplit(sFileDesc.c_str(), ',', 0, false);

	enum { ARG_DSTNAME = 0, ARG_DSTPATH, ARG_ACCESSMASK, ARG_OWNER, ARG_COUNT };

	sSrcName = m_pProductFiles[iFile];
	sDstName = pFileDesc.size() > ARG_DSTNAME && !pFileDesc[ARG_DSTNAME].empty() ? pFileDesc[ARG_DSTNAME] : sSrcName;
	sDstPath = pFileDesc.size() > ARG_DSTPATH ? pFileDesc[ARG_DSTPATH] : "";
	sAccessMask = pFileDesc.size() > ARG_ACCESSMASK ? pFileDesc[ARG_ACCESSMASK] : "";
	sOwner = pFileDesc.size() > ARG_OWNER ? pFileDesc[ARG_OWNER] : "";

	return true;
}

bool CPackageInfo::ProcessCommand(const char *lpCommand)
{

	int nExecType = 0;
	int nCheckResult = 0;
	CStdString sCommand;
	bool fCheckResult = CPackageInfo::ParseCommand(lpCommand, nExecType, nCheckResult, sCommand);

	int nResult = 0;
	if(nExecType == CPackageInfo::EExecRoot)
		nResult = CInstallerBase::ExecRoot(sCommand.c_str());
	else if(nExecType == CPackageInfo::EExecUser)
		nResult = CInstallerBase::ExecUser(sCommand.c_str());
	else
		nResult = SystemInvoke(sCommand.c_str());
	if(fCheckResult && nCheckResult != nResult)
	{
		LogMessage("CPackageInfo::ProcessCommand() -> failed: %d, %d", nCheckResult, nResult);
		return false;
	}

	return true;
}

bool CPackageInfo::ParseCommand(const char *lpString, int &nType, int &nResult, CStdString &sCommand)
{
	const char *lpNumber = lpString;
	const char *lpPtr = lpString;
	if((nType = nType = *lpString == '^' ? EExecRoot : (*lpString == '~' ? EExecUser : EExecNormal)) != EExecNormal)
		lpNumber++,lpPtr++;
	bool fResult = false;
	if(*lpPtr == '-')
		lpPtr++;
	for(; *lpPtr && *lpPtr != ':' && isdigit(*lpPtr); lpPtr++);
	if(!*lpPtr || *lpPtr != ':')
	{
		nType = EExecNormal;
		nResult = 0;
		sCommand = lpString;
	}
	else
	{
		CStdString sResult(lpNumber, lpPtr);
		nResult = atoi(sResult.c_str());
		sCommand = lpPtr + 1;
		fResult = !sResult.empty();
	}
	return fResult;
}
