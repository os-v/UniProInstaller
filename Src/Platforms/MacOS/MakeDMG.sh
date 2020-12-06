
CurDir=$(cd "$(dirname "$0")"; pwd)

ConfigName=
BasePath=../../../Bin/$ConfigName
OutputPath=$CurDir/Image
CertName="Apple Development: XXXXXXXXX (YYYYYYYYY)"

rm "$BasePath/UPIDemo.dmg"

rm -f -R "$OutputPath/UPIDemo Installer.app"
rm -f -R "$OutputPath/UPIDemo Uninstaller.app"
mkdir $OutputPath

cp -a "$BasePath/Install.app" "$OutputPath/UPIDemo Installer.app"
cp -a "$BasePath/Uninstall.app" "$OutputPath/UPIDemo Uninstaller.app"

codesign --force --verify --verbose --sign "$CertName" "$OutputPath/UPIDemo Uninstaller.app"
codesign --force --verify --verbose --sign "$CertName" "$OutputPath/UPIDemo Installer.app"

if [ "$1" == "-w" ]; then
   hdiutil create -size 100m -format UDRW UPIDemo.dmg -volname "UPIDemo" -fs HFS+ -srcfolder "$OutputPath"
else
   hdiutil create UPIDemo.dmg -volname "UPIDemo" -fs HFS+ -srcfolder "$OutputPath"
fi

codesign --force --verify --verbose --sign "$CertName" ./UPIDemo.dmg
codesign -vv --deep-verify ./UPIDemo.dmg
spctl --verbose=4 --assess --type execute -v ./UPIDemo.dmg

mv ./UPIDemo.dmg "$BasePath"

rm -f -R "$OutputPath/UPIDemo Installer.app"
rm -f -R "$OutputPath/UPIDemo Uninstaller.app"
