#
#  @file Notarize.sh
#  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
#  @see https://github.com/os-v/UniProInstaller/
#
#  Created on 05.01.21.
#  Copyright 2021 Sergii Oryshchenko. All rights reserved.
#

CurDir=$(cd "$(dirname "$0")"; pwd)

ProductName=$1
DevIDUser=$2
DevIDPass=$3
DevIDProv=$4

BasePath=../../../Bin
DMGFile=$ProductName.dmg
BundleID="com.$ProductName"
RequestTimeout=60s

echo "Uploading, please wait ..."

UploadResult=$(xcrun altool --notarize-app --primary-bundle-id "$BundleID" --username "$DevIDUser" --password "$DevIDPass" --asc-provider "$DevIDProv" --file "$BasePath/$DMGFile" 2>&1)
UploadReturn=$?

echo "$UploadResult"

if [ $UploadReturn -ne 0 ]; then
	echo "Failed to upload, error: $UploadReturn"
	exit $UploadReturn
fi

#echo "Enter your request ID:"
#read RequestID

#echo "$UploadResult" | awk '{if ($1 == "RequestID:") print $2}'
#echo "$UploadResult" | awk '/RequestID:/ {print $2}'
#echo "$UploadResult" | grep "RequestID" | sed -n "s/RequestID:[[:space:]]*\(.*\)/\1/p"

RequestUUID=$(echo "$UploadResult" | grep "RequestUUID" | sed -n "s/RequestUUID = *\(.*\)/\1/p")

echo "RequestUUID is $RequestUUID"

if [ -z $RequestUUID ]; then
	echo "Invalid RequestUUID"
	exit -1
fi

for ((iAttempt=1; ; iAttempt++))
do
	echo "Requesting notarization result. Attempt #$iAttempt"
	RequestResult=$(xcrun altool --notarization-info "$RequestUUID" -u "$DevIDUser" -p "$DevIDPass" 2>&1)
	RequestReturn=$?
	Status=$(echo "$RequestResult" | grep "Status:" | sed -n "s/[[:space:]]*Status: *\(.*\)/\1/p")
	StatusCode=$(echo "$RequestResult" | grep "Status Code:" | sed -n "s/[[:space:]]*Status Code: *\(.*\)/\1/p")
	echo "   Status=$Status, Status Code=$StatusCode"
	if [ $RequestReturn -ne 0 ]; then
		echo "$RequestResult"
		echo "Request failed, error: $RequestReturn"
		exit $RequestReturn
	fi
	if [ "$Status" == "invalid" ]; then
		echo "FullMessage: $RequestResult"
		exit -1
	fi
	if [ "$Status" == "success" ] && [ $StatusCode -eq 0 ]; then
		break
	fi
	echo "   Suspending for $RequestTimeout"
	sleep $RequestTimeout
done

echo "Notarization succeeded, stapling ..."

xcrun stapler staple "$BasePath/$AppFile"

echo "Notarization finished"

