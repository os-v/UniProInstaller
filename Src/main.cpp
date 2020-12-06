//
//  @file main.cpp
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 15.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#include "Wizard/WizardSheet.h"
#include "../Common/UtilsMisc.h"
#include "HelperDefs.h"

DefLogScope(UPInstall);

int main(int argc, char **argv)
{

	//putenv("LANG=zh_CN.UTF-8");
	//putenv("LANG=hi_IN.UTF-8");
	//putenv("LANG=en_US.UTF-8");

	CStdString sSelfPath = GetFullPath(argv[0]);
	argv[0] = &sSelfPath[0];

	bool fLogStd = false;
	CStdString sLogPath = "";//"/tmp/UPInstall.log";
	for(int iArg = 1; iArg < argc; iArg++)
	{
		if(!strcmp(argv[iArg], "-l"))
			sLogPath = *(argv[iArg] + 2) ? argv[iArg] + 2 : sSelfPath + ".log";
		else if(!strcmp(argv[iArg], "-v"))
			fLogStd = true;
	}

	if(!sLogPath.empty())
		LogInitialize("UPInstall", sLogPath.c_str(), fLogStd);

	LogMessage("Main()");

	CInstallerBase pInstaller;
	if(!pInstaller.Prepare(argc, argv))
		return pInstaller.GetResult();

	LogMessage("Main() -> Show");

	CWizardSheet pDlg(&pInstaller);
	pDlg.Create();
	pDlg.Exec();
	pDlg.Destroy();

	pInstaller.Finalize();

	LogMessage("Main() -> Exit");

	return pInstaller.GetResult();
}

