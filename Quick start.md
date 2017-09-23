# ARToolKit v6.x Quick Start
1. [macOS](#macos)
2. [iOS](#ios)
3. [Linux](#linux)
4. [Android](#android)
5. [Next Steps](#next-steps)

## macOS
* Open `Examples` for double-clickable example apps.
* Xcode project for each example is in `Examples/Source/*example name*/macOS`. 

## iOS 
* Open `Examples`. Because apps must be signed for your iOS device, no prebuilt app is provided.
* Xcode project for each example is in `Examples/Source/*example name*/iOS`.
    1. Open the Xcode project.
    2. Navigate to the project settings
    3. Select the settings for the example app (under "Targets")
    4. Select the "General" tab.
    5. Set the development team to one of your registered Apple Developer accounts
    6. Choose a bundle ID that your account is authorized to sign. 
* Connect your device, select it as the target for building and running, and build and run.

## Linux
* Install the packages 'artoolkit6-lib', 'artoolkit6-dev', and 'artoolkit6-examples' using your package manager:
    
    For Debian-based systems, this will be via `sudo dpkg -i artoolkit6-lib_VERSION_PLATFORM.deb`.
    
    For Redhat-based systems this will be via `yum -y install artoolkit6-lib_VERSION_PLATFORM.rpm`
    
    where VERSION is the ARToolKit version number triplet, e.g. 6.0.2, and PLATFORM is the target platform, e.g. 'amd64'.
* The executable for the examples will be in /usr/bin, and the source in /usr/src.

## Android
* Open `Examples` for prebuilt .apks. These may be installed on your device via `adb install *example name*.apk` where EXAMPLE is the example name.
* An Android Studio project for each example is in `Examples/Source/*example name*/Android`.

## Next steps

For more documentation, see the wiki at <https://github.com/artoolkit/ar6-wiki/wiki>.

----

