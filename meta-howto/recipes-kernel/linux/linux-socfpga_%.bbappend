SRC_URI += "file://mitysom_a10s_devkit.dtsi;subdir=git/arch/${ARCH}/boot/dts"
SRC_URI += "file://mitysom_a10s_user_empty.dts;subdir=git/arch/${ARCH}/boot/dts"

FILESEXTRAPATHS:prepend := "${THISDIR}/user-devicetrees:"

PACKAGE_ARCH = "${MACHINE_ARCH}"