SUMMARY = "HowTo Sample Console image"
LICENSE = "MIT"

inherit mitysom-squashfs-image

IMAGE_FEATURES += "ssh-server-dropbear debug-tweaks"
EXTRA_IMAGE_FEATURES += " tools-debug"
DISTRO_FEATURES:remove = "3g alsa bluetooth irda nfc x11 wayland wifi"

CORE_IMAGE_EXTRA_INSTALL += "\
    packagegroup-sysdbgtools \
    "