//
//  @file main.cpp
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 22.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#include <stdio.h>
#include  <errno.h>
#include "../Common.h"
#include "../../Common/UtilsMisc.h"

#if defined(WIN32)
	#include <windows.h>
#elif defined(__APPLE__)
	#include <signal.h>
#elif defined(__linux__)
	#include <signal.h>
#elif defined(__FreeBSD__)
	#include <fcntl.h>
	#include <libutil.h>
	#include <signal.h>
#endif

#define SERVICE_NAME		"UPIService"

DefLogScope(Main)

int GExitCode = 0;
volatile bool GServiceStop = false;

int main(int argc, char *argv[]);
int ServiceMain(int argc, char *argv[]);
void OnSignalHandler(int eSignal);

#if defined(WIN32)

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	return main(__argc, __argv);
}

DWORD OnServiceHandlerEx(DWORD nControl, DWORD nEventType, LPVOID lpEventData, LPVOID lpContext)
{

	LogMessage("OnServiceHandlerEx() -> %X, %X", nControl, nEventType);

	switch(nControl)
	{
		case SERVICE_CONTROL_STOP:
			GServiceStop = true;
			break;
		default:
			return ERROR_CALL_NOT_IMPLEMENTED;
	}

	return NO_ERROR;
}

VOID OnServiceStart(DWORD nNumServicesArgs, LPSTR *lpServiceArgVectors)
{

	LogMessage("OnServiceStart()");

	SERVICE_STATUS pStatus = { SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, SERVICE_STOPPED, SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE | SERVICE_ACCEPT_POWEREVENT | SERVICE_ACCEPT_SESSIONCHANGE, NO_ERROR, 0, 0, 0 };
	SERVICE_STATUS_HANDLE hStatus = RegisterServiceCtrlHandlerExA(SERVICE_NAME, OnServiceHandlerEx, 0);

	pStatus.dwCurrentState = SERVICE_RUNNING;
	if(!SetServiceStatus(hStatus, &pStatus))
		return;

	GExitCode = ServiceMain((int)nNumServicesArgs, lpServiceArgVectors);

	pStatus.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(hStatus, &pStatus);

}

#endif

int main(int argc, char *argv[])
{

	LogInitialize(SERVICE_NAME, LOGFILEPATH, false);

	LogMessage("main() -> enter");

#if defined(WIN32)

	SERVICE_TABLE_ENTRYA pTable[] = {
		{ (char*)SERVICE_NAME, OnServiceStart },
		{ 0, 0 }
	};
	StartServiceCtrlDispatcherA(pTable);

/*#elif defined(__APPLE__)

	GExitCode = ServiceMain(argc, argv);

#elif defined(__linux__) || defined(__FreeBSD__)
*/
#elif defined(__APPLE__) || defined(__unix__)

	signal(SIGABRT, &OnSignalHandler);
	signal(SIGHUP, &OnSignalHandler);
	signal(SIGTERM, &OnSignalHandler);
	signal(SIGINT, &OnSignalHandler);
	signal(SIGKILL, &OnSignalHandler);
	signal(SIGQUIT, &OnSignalHandler);

#if defined(__FreeBSD__)

	pid_t pPIDID = 0;
	pidfh *pPIDFH = 0;

	pPIDFH = pidfile_open("/var/run/" SERVICE_NAME ".pid", 0600, &pPIDID);
	if(!pPIDFH)
	{
		if (errno == EEXIST)
		{
			LogMessage("main() -> already started");
			return -1;
		}
		LogMessage("main() -> pidfile_open: %d", errno);
	}

	int nResult = daemon(0, 0);
	if (nResult == -1)
	{
		LogMessage("main() -> daemon: %d", errno);
		pidfile_remove(pPIDFH);
		return -1;
	}

	pidfile_write(pPIDFH);

#endif

	GExitCode = ServiceMain(argc, argv);

#endif

	LogMessage("main() -> exit");

	return GExitCode;
}

int ServiceMain(int argc, char *argv[])
{

	LogMessage("ServiceMain() -> enter");

	for(uint32_t iCycle = 0; !GServiceStop; iCycle++)
	{
		LogMessage("ServiceMain() -> idle:%d", iCycle);
		Sleep(1000);
	}

	LogMessage("ServiceMain() -> exit");

	return GExitCode;
}

void OnSignalHandler(int eSignal)
{

	LogMessage("OnSignalHandler() -> %d", eSignal);

	GServiceStop = true;

	GExitCode = eSignal;
	//exit(eSignal);

}
