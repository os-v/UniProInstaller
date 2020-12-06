//
//  @file Resources.cpp
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 15.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#ifndef RESOURCES_H
#define RESOURCES_H

#include "../Common/incbin.h"

#ifndef DEFUNINSTALL
	INCBIN(ResIcon, "Src/Resources/IconI.png");
	INCBIN(ResLicense, "Src/Resources/License.rtf");
#else
	INCBIN(ResIcon, "Src/Resources/IconU.png");
#endif

INCBIN(ResBanner, "Src/Resources/Banner.png");

unsigned char *ResIconData()
{
	return (unsigned char*)gResIconData;
}

unsigned int ResIconSize()
{
	return gResIconSize;
}

unsigned char *ResBannerData()
{
	return (unsigned char*)gResBannerData;
}

unsigned int ResBannerSize()
{
	return gResBannerSize;
}

#ifndef DEFUNINSTALL
unsigned char *ResLicenseData()
{
	return (unsigned char*)gResLicenseData;
}

unsigned int ResLicenseSize()
{
	return gResLicenseSize;
}
#endif
unsigned char *ResBundleInfoData()
{
	return 0;
}

unsigned int ResBundleInfoSize()
{
	return 0;
}

#endif

