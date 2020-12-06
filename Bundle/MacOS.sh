
PLATFORM=$1

rm -f -R $PLATFORM

unzip $PLATFORM.zip -d $PLATFORM

rm -f -R ./$PLATFORM/Uninstall.app

cp -f ../Bin/UPIClient ./$PLATFORM
cp -f ../Bin/UPIService ./$PLATFORM
cp -a -f ../Bin/Uninstall.app ./$PLATFORM

cd ./$PLATFORM

zip -r $PLATFORM.zip ./*

mv -f ./$PLATFORM.zip ..

cd ..

rm -f -R $PLATFORM

