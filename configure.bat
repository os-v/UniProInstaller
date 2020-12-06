
@echo off

@del Makefile
@del Demo\UPIClient\Makefile
@del Demo\UPIService\Makefile

@copy Makefile.win32 .\Makefile
@copy Demo\UPIClient\Makefile.win32 Demo\UPIClient\Makefile
@copy Demo\UPIService\Makefile.win32 Demo\UPIService\Makefile

