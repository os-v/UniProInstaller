
CurDir=$(cd "$(dirname "$0")"; pwd)

ProductName=$1

ConfigName=
BasePath=../../../Bin/$ConfigName
OutputPath=$CurDir/Image
IOutputPath=$OutputPath/$ProductName\ Installer.app
UOutputPath=$OutputPath/$ProductName\ Uninstaller.app
DMGName=$ProductName.dmg
DMGPath=$BasePath/$DMGName
CertName="Apple Development: XXXXXXXXX (YYYYYYYYY)"

rm "$DMGPath"

rm -f -R "$IOutputPath"
rm -f -R "$UOutputPath"
mkdir $OutputPath

cp -a "$BasePath/Install.app" "$IOutputPath"
cp -a "$BasePath/Uninstall.app" "$UOutputPath"

codesign --force --verify --verbose --sign "$CertName" "$UOutputPath"
codesign --force --verify --verbose --sign "$CertName" "$IOutputPath"

if [ "$2" == "-w" ]; then
   hdiutil create -size 100m -format UDRW $DMGName -volname "$ProductName" -fs HFS+ -srcfolder "$OutputPath"
else
   hdiutil create $DMGName -volname "$ProductName" -fs HFS+ -srcfolder "$OutputPath"
fi

codesign --force --verify --verbose --sign "$CertName" ./$DMGName
codesign -vv --deep-verify ./$DMGName
spctl --verbose=4 --assess --type execute -v ./$DMGName

mv ./$DMGName "$BasePath"

rm -f -R "$IOutputPath"
rm -f -R "$UOutputPath"
