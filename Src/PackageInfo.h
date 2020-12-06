//
//  @file PackageInfo.h
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 19.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#ifndef PACKAGEINFO_H
#define PACKAGEINFO_H

#include "../Common/ConfigFileInfo.h"
#include "../Common/UtilsMisc.h"

class CConfigFileInfo;

class CPackageInfo
{
public:

	enum { EExecNormal = 0, EExecRoot, EExecUser };

	CStdString Language;

	CStdString ProductPath;
	CStdString ProductVersion;
	CStdString ProductUUID;
	CStdString ProductName;
	CStdString ProductPublisher;

	bool InstallExtraConfig;
	CStdString InstallFinishPrompt;
	CStdString UninstallFinishPrompt;

	CStdStringArray InstallPreCommands;
	CStdStringArray InstallPostCommands;
	CStdStringArray UpdatePreCommands;
	CStdStringArray UpdatePostCommands;
	CStdStringArray RepairPreCommands;
	CStdStringArray RepairPostCommands;
	CStdStringArray UninstallPreCommands;
	CStdStringArray UninstallPostCommands;

	CPackageInfo();
	~CPackageInfo();

	bool Load(const char *lpData = 0);

	uint32_t GetFilesCount();
	bool GetFilesInfo(uint32_t iFile, CStdString &sSrcName, CStdString &sDstName, CStdString &sDstPath, CStdString &sAccessMask, CStdString &sOwner);

	static bool ProcessCommand(const char *lpCommand);
	static bool ParseCommand(const char *lpString, int &nType, int &nResult, CStdString &sCommand);

private:

	CConfigFileInfo m_pBundleInfo;

	CStdStringArray m_pProductFiles;

};

#endif
