# Following UTAustinX UT.6.10x: Embedded Systems - Shape The World: Microcontroller Input/Output in 2024

Note: This is intended for Windows users.

## Software Installation

The instructions given by the EdX course [here](http://edx-org-utaustinx.s3.amazonaws.com/UT601x/download.html) are out of date, and the link to uVision 4.74 [here](https://www.keil.com/demo/eval/armv4.htm) no longer seems to work.

Instead, the next best approach is to install uVision 5. Despite the course site saying otherwise, it is possible to run the labs after installing some extra software. You can download Version 5 [here](https://www.keil.com/demo/eval/arm.htm). Note that you will need to make an account with Keil and wait a few days for a confirmation.

From here, follow the remaining instructions from EdX to install the Lab Graders and Windows Drivers.

## Modify uVision Keil to run older projects

### Version 4 Legacy Pack

At this point, if you try to run any of the course labs, you will discover that uVision cannot run the `.uvproj` files, as they require uVision Version 4. Fortunately, Keil offers support for these situations. Follow [this link](https://developer.arm.com/documentation/101407/0540/Creating-Applications/Tips-and-Tricks/Use-MDK-Version-4-Projects) and download the Arm Cortex-M Legacy pack. This will download an executable that, when run, will enable your uVision software to run the course labs.

### ARM Compiler Version 5

At this point, you will be able to open and edit the course projects. However, you will probably have trouble building the target and running the project in debug. This is because the softare to run the targets also needs to be downloaded separately. [This](https://community.arm.com/support-forums/f/keil-forum/52719/how-can-i-install-compiler-version-5-for-keil-vision-5) is a support forum thread about the downloading the ARM Compiler (Version 5) that is required by this course, and [this](https://developer.arm.com/documentation/ka002827/latest) is Keil's official documentation on the matter.

To download the older ARM Compilers, you will need to make an ARM Developer account. An index of historical compiler versions (you can also find this by following the links in the last paragraph) is [here](https://developer.arm.com/documentation/ka005198/latest). I used [Arm Compiler 5.06 update 7 (build 960)](https://developer.arm.com/downloads/view/ACOMP5?revision=r5p6-07rel1), which seems to be the most updated version that is compatible with the course lab files. Make sure to follow the "Install" directions [here](https://developer.arm.com/documentation/ka002827/latest); it will not be as simple as simply running the executable.

### Stellaris Drivers

Finally, at this point, you should be able to build and run the lab files in simulation, but you will almost certainly run into an error when trying to run on the microcontroller. This is because Keil uVision 5 and the ARM Compiler versions we have installed do not include a key driver for Stellaris, which you will see referenced in the course videos. This is a known ARM issue, as referenced [here](https://developer.arm.com/documentation/ka002280/latest). To download this last component, check the link and download the executable. The executable will check the library that it is directed to and either create a BIN file or add the necessary driver file to an existing BIN file. Make sure that the new driver file is added to the ARM Compiler that you installed in the last paragraph.

# Conclusion

Congratulations! With these installations and modifications, it should now be possible to take "Embedded Systems - Shape The World" in 2024.