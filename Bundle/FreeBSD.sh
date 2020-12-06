
PLATFORM=$1

rm -f -R $PLATFORM

unzip $PLATFORM.zip -d $PLATFORM

cp -f ../Bin/UPIClient ./$PLATFORM
cp -f ../Bin/UPIService ./$PLATFORM
cp -f ../Bin/UPUninstall ./$PLATFORM/Uninstall

cd ./$PLATFORM

zip -r $PLATFORM.zip ./*

mv -f ./$PLATFORM.zip ..

cd ..

rm -f -R $PLATFORM


