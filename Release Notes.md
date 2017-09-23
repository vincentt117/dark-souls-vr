# ARToolKit v6.x Release Notes

------------------------------------------------

## Version 6.0.2
### 2017-03-31

This release welcomes participants in the ARToolKit v6 general-access beta-release program. At present, ARToolKit is available in binary SDK form. Upcoming releases will progressively include source code for the SDK.

This package may not be redistributed without prior written permission of DAQRI, LLC. This overrides any and all license terms mentioned elsewhere in this package.

### ARToolKit Camera Calibration utility
- Alongside this SDK release, we are progressively releaseing the ARToolKit Camera Calibration utility for Android, macOS, iOS and Linux. This utility allows users to create, save, and share camera calibration information with the ARToolKit community. This utility replaces all previous utilities made available with ARToolKit v5. A reference server containing calibrations for many cameras is being provided by DAQRI. A future release will include full source for the calibraiton server, allowing you to implement your own server to meet the needs of your user base in terms of uptime, calibration quality, and device support.

------------------------------------------------
## Version 6.0.1
### 2017-03-13

This is a second pre-release version specifically for DAQRI ARToolKit Early Access Program (EAP) participants.

This package may not be redistributed without prior written permission of DAQRI, LLC. This overrides any and all license terms mentioned elsewhere in this package.

### ARToolKit Image Database utility
- This utility allows users to quickly and easily create and edit ARToolKit v6 image databases. An image database is a pre-processed set of images which can be quickly loaded into the ARToolKit v6 2D image recognizer, greatly easing the overhead of loading a large number of images for recognition and tracking.  The utility runs on macOS or Linux, and the image database itself is supported on all platforms.

------------------------------------------------

## Version 6.0.0
### 2017-02-24

This is a pre-release version specifically for DAQRI ARToolKit Early Access Program (EAP) participants.

This package may not be redistributed without prior written permission of DAQRI, LLC. This overrides any and all license terms mentioned elsewhere in this package.

### New Image Tracker
-   Can recognize and track multiple images.
-   High stability pose estimation with P3P.
-   Template-matching tracker with optical flow fallback.
-   FREAK-based recognition for initialization.
-   Fast on-the-fly tracking data generation (10s of ms).
-   Support for up to 100 recognized images.
-   Works completely offline (calibration, recognition, and tracking).
-   Dynamic saving and loading of image sets or individual images.

### New Tools
-   Camera Calibration Tool
    -   Calibrate your own cameras and devices easily with our new tool, improved from version 5.
-   All calibrations made will play with Open Lens Model database (more on this later) so that once you calibrate, everybody can gain benefit from that calibration with an active internet connection.

### New API Structure
-   Consistent state-machine-like API structure to allow the same API across all platforms and devices.
-   Sane defaults with deep customization options to configure your AR app just for your use-case.

Check out the documentation for [ARToolKit 6](https://github.com/artoolkit/ar6-wiki/wiki) and [ARToolKit 6 for Unity](https://github.com/artoolkit/ar6unity-wiki/wiki) for more details.

------------------------------------------------


