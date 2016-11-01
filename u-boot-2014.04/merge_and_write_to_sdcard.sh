#!/bin/bash

MERGE_TOOL=./tools/uboot_merge


$MERGE_TOOL ./spl/tiny210-spl.bin ./u-boot.bin

sudo dd iflag=dsync oflag=dsync if=./tiny210_u-boot.bin of=/dev/mmcblk0 seek=1

