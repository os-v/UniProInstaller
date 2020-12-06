//
//  @file main.cpp
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 22.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#include <stdio.h>
#include "../Common.h"
#include "../../Common/UtilsMisc.h"

#ifdef WIN32
	#include <windows.h>
#endif

DefLogScope(Main)

int main(int argc, char *argv[])
{

	LogInitialize("UPIClient", LOGFILEPATH, false, ELogLevelMessage, true);

	for(uint32_t iCycle = 0; ; iCycle++)
	{
		LogMessage("main() -> %d", iCycle);
		Sleep(1000);
	}

	return 0;
}

#if defined(WIN32)

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	return main(__argc, __argv);
}

#endif
