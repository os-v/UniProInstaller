
set PLATFORM=Win32
set BINPATH=..\Bin

rmdir /S /Q %PLATFORM%

powershell Expand-Archive %PLATFORM%.zip -DestinationPath %PLATFORM%

xcopy /Y %BINPATH%\UPIClient.exe %PLATFORM%
xcopy /Y %BINPATH%\UPIService.exe %PLATFORM%
xcopy /Y %BINPATH%\UPUninstall.exe %PLATFORM%\Uninstall.exe

powershell Compress-Archive -Update -Path %PLATFORM%\* -DestinationPath %PLATFORM%.zip

rmdir /S /Q %PLATFORM%

