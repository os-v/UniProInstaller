
NAME=$1
TYPE=$2
SRCAPP=../../../Bin/$NAME
APPNAME=Install
APPICON=../../Resources/IconI.png

if [ "$TYPE" = "" ]; then
	echo "usage: MakeApp.sh Installer|Uninstaller"
	exit 
fi 

if [ $TYPE = "Uninstaller" ]; then 
	APPNAME=Uninstall
	APPICON=../../Resources/IconU.png
fi

DSTAPP=$APPNAME.app

rm -f -R "$TYPE/AppIcon.iconset"
mkdir "$TYPE/AppIcon.iconset"
sips -z 32 32 "$APPICON" --out "$TYPE/AppIcon.iconset/icon_32x32.png"
sips -z 64 64 "$APPICON" --out "$TYPE/AppIcon.iconset/icon_64x64.png"
sips -z 128 128 "$APPICON" --out "$TYPE/AppIcon.iconset/icon_128x128.png"
sips -z 256 256 "$APPICON" --out "$TYPE/AppIcon.iconset/icon_256x256.png"
sips -z 512 512 "$APPICON" --out "$TYPE/AppIcon.iconset/icon_512x512.png"

rm -f -R "$DSTAPP"
rm -f -R App.template
unzip AppTemplate.zip
mv App.template $DSTAPP

cp "$SRCAPP" "$DSTAPP/Contents/MacOS"
cp "$TYPE/Info.plist" "$DSTAPP/Contents"
iconutil -c icns -o "$DSTAPP/Contents/Resources/AppIcon.icns" "$TYPE/AppIcon.iconset"
rm -f -R "$TYPE/AppIcon.iconset"

rm -f -R "../../../Bin/$DSTAPP"
mv "$DSTAPP" ../../../Bin

