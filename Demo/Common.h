//
//  @file Common.h
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 22.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#ifndef COMMON_H
#define COMMON_H

#if defined(WIN32)
	#define LOGFILEPATH			"%ProgramFiles%\\UPIDemo\\UPIDemo.log"
#elif defined(__APPLE__)
	#define LOGFILEPATH			"/Library/Application Support/UPIDemo/UPIDemo.log"
#elif defined(__linux__)
	#define LOGFILEPATH			"/usr/share/UPIDemo/UPIDemo.log"
#elif defined(__FreeBSD__)
	#define LOGFILEPATH			"/usr/share/UPIDemo/UPIDemo.log"
#endif

#endif
