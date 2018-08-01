#!/bin/sh
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- JN-pal5728_defconfig
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- JN-pal5728.dtb
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- zImage
