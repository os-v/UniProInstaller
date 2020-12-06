//
//  @file HelperDefs.cpp
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 15.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#include "HelperDefs.h"
#include <FL/Fl.H>
#include "../Common/UtilsMisc.h"

#if defined(WIN32)
	#include <Windows.h>
#elif defined(__APPLE__)
	#include "Platforms/MacOS/InstallerSysImpl.h"
#elif defined(__unix__)
	#include <X11/Xlib.h>
#endif

DefLogScope(HelperDefs)

float GScaleFactor = 1.0;

void UIPrepare()
{

	float fDPIX = 0.0, fDPIY = 0.0;

#if defined(WIN32)
	HDC hDC = GetDC(0);
	fDPIX = (float)GetDeviceCaps(hDC, LOGPIXELSX);
	fDPIY = (float)GetDeviceCaps(hDC, LOGPIXELSY);
	ReleaseDC(0, hDC);
#elif defined(__APPLE__)
	//Fl::screen_dpi(fDPIX, fDPIY);
	fDPIX = fDPIY = CInstallerSysImpl::UIFindScaleFactor();
#elif defined(__unix__)
	Display *pDisplay = XOpenDisplay(0);
	fDPIX = (int)((float)DisplayWidth(pDisplay, 0) * 25.4 / (float)DisplayWidthMM(pDisplay, 0) + 0.5);
	fDPIY = (int)((float)DisplayHeight(pDisplay, 0) * 25.4 / (float)DisplayHeightMM(pDisplay, 0) + 0.5);
	XCloseDisplay(pDisplay);
#endif

	if(fDPIX == 96)
		fDPIX = 100;
	else if(fDPIX == 120)
		fDPIX = 125;

	fDPIX /= 100;

	GScaleFactor = fDPIX;

	FL_NORMAL_SIZE = (int)((float)14 * fDPIX);

	LogMessage("ConfigureScreen() -> %f:%f", fDPIX, fDPIY);

}

int UIScale(int nValue)
{
	return (int)((float)nValue * GScaleFactor);
}

