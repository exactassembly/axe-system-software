SUMMARY = "System/configuration tools"
DESCRIPTION = "Packages which provide OS/system level debugging utilities"
PR = "r1"

inherit packagegroup

PACKAGE_ARCH = ""

RDEPENDS:${PN} = "\
    devmem2 \
    i2c-tools \
    python3-smbus2 \
    spitools \
    mdiotool \
    libgpiod-tools \
    openocd \
    mosquitto \
    python3-paho-mqtt \
    minicom \
"
