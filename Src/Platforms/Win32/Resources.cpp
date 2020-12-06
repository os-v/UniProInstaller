//
//  @file Resources.cpp
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 19.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#ifndef RESOURCES_H
#define RESOURCES_H

#include <windows.h>

#include "AppRes.h"

#define GetResourceSize(sName)		SizeofResource(GModuleHandle, FindResource(GModuleHandle, sName, L"FILE"))
#define GetResourceData(sName)		LockResource(LoadResource(GModuleHandle, FindResource(GModuleHandle, sName, L"FILE")))

HINSTANCE GModuleHandle = GetModuleHandle(0);

unsigned char *ResIconData()
{
	return (unsigned char*)GetResourceData(MAKEINTRESOURCE(IDR_FILE_RESICON));
}

unsigned int ResIconSize()
{
	return GetResourceSize(MAKEINTRESOURCE(IDR_FILE_RESICON));
}

unsigned char *ResBannerData()
{
	return (unsigned char*)GetResourceData(MAKEINTRESOURCE(IDR_FILE_BANNER));
}

unsigned int ResBannerSize()
{
	return GetResourceSize(MAKEINTRESOURCE(IDR_FILE_BANNER));
}

unsigned char *ResLicenseData()
{
	return (unsigned char*)GetResourceData(MAKEINTRESOURCE(IDR_FILE_LICENSE));
}

unsigned int ResLicenseSize()
{
	return GetResourceSize(MAKEINTRESOURCE(IDR_FILE_LICENSE));
}

unsigned char *ResBundlePackData()
{
	return (unsigned char*)GetResourceData(MAKEINTRESOURCE(IDR_FILE_BUNDLEPACK));
}

unsigned int ResBundlePackSize()
{
	return GetResourceSize(MAKEINTRESOURCE(IDR_FILE_BUNDLEPACK));
}

unsigned char *ResBundleInfoData()
{
	return (unsigned char*)GetResourceData(MAKEINTRESOURCE(IDR_FILE_BUNDLEINFO));
}

unsigned int ResBundleInfoSize()
{
	return GetResourceSize(MAKEINTRESOURCE(IDR_FILE_BUNDLEINFO));
}

#endif

