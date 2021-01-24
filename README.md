
# UniProInstaller - Cross platform open source universal product installer.

## Current Features
- Cross platform (Windows, macOS, Linux, FreeBSD)
- Multilingual
- DPI Aware UI (High DPI)
- Install, Repair, Update, Uninstall
- Pre/Post actions
- Silent commandline mode
- Logging

## Required Features
- Package manager integration on macOS, Ubuntu, FreeBSD

## Build Process
* Prerequisites:
	* Unix: gcc
	* Windows: Platform SDK
* For OSX configure DevIDUser, DevIDPass, DevIDProv, CertName variables in ./Src/Platforms/MacOS/BuildDMG.sh
* Download, build and install FLTK: 
	* https://www.fltk.org/software.php
	* https://www.fltk.org/doc-1.3/intro.html
	* make sure all libs are built (for Unix: fltk fltk_images fltk_png fltk_z, for Windows: fltk fltkimages fltkpng fltkzlib)
	* copy all FLTK headers and libs into include and libs path
* Clone and build UniProInstaller:
	* Unix:
	```
	git clone https://github.com/os-v/UniProInstaller
	./configure
	make
	```
	* Windows (in Platform SDK command line):
	```
	git clone https://github.com/os-v/UniProInstaller
	./configure
	nmake
	```
	* Installer should appears in ./Bin folder

## Screenshots
<p align="center">
	<img alt="MacOS DMG" src="https://user-images.githubusercontent.com/73893487/101396289-ade0bc00-38fd-11eb-97aa-c6cdb83d8102.png"/>
	<img alt="MacOS Language" src="https://user-images.githubusercontent.com/73893487/101396306-b1744300-38fd-11eb-9644-a19ac3a6d8ff.png"/>
	<img alt="Windows Welcome" src="https://user-images.githubusercontent.com/73893487/101396311-b33e0680-38fd-11eb-82a2-9c89884b642c.png"/>
	<img alt="Ubuntu License" src="https://user-images.githubusercontent.com/73893487/101396318-b507ca00-38fd-11eb-8e72-a2fc92700efd.png"/>
	<img alt="FreeBSD ConfigType" src="https://user-images.githubusercontent.com/73893487/101396323-b6d18d80-38fd-11eb-8769-6dab717aa371.png"/>
	<img alt="FreeBSD Install" src="https://user-images.githubusercontent.com/73893487/101396555-1465da00-38fe-11eb-9670-450f5329e10c.png"/>
	<img alt="Windows Welcome" src="https://user-images.githubusercontent.com/73893487/101397844-d073d480-38ff-11eb-80f2-3f602a46b722.png"/>
	<img alt="MacOS Uninstall" src="https://user-images.githubusercontent.com/73893487/101396330-b89b5100-38fd-11eb-90cb-c32d605169ca.png"/>
</p>

## License
This software is distributed under the [LGPL 3.0](https://github.com/os-v/UniProInstaller/blob/master/LICENSE) license. Please read LICENSE for information on the software availability and distribution.


