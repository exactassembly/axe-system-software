SUMMARY = "System/configuration tools"
DESCRIPTION = "Packages which provide OS/system level debugging utilities"
PR = "r1"

inherit packagegroup

PACKAGE_ARCH = ""

RDEPENDS:${PN} = "\
    devmem2 \
    i2c-tools \
    spitools \
    mdiotool \
    libgpiod-tools \
    openocd \
    minicom \
"
