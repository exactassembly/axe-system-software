IMAGE_INSTALL += "packagegroup-sysdbgtools"
EXTRA_IMAGE_FEATURES += "debug-tweaks"
DISTRO_FEATURES:remove = "3g alsa bluetooth irda nfc x11 wayland wifi"


inherit mitysom-squashfs-image