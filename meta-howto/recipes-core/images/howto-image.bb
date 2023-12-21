SUMMARY = "HowTo Sample Console image"
LICENSE = "MIT"

# qemu seems to be needed by image assembly scripts associatd with udev (must 
# be doing runtime things during the build that execute with ARM binaries)
DEPENDS="qemuwrapper-cross"

IMAGE_FEATURES += "ssh-server-dropbear debug-tweaks"
EXTRA_IMAGE_FEATURES += " tools-debug"
DISTRO_FEATURES:remove = "3g alsa bluetooth irda nfc x11 wayland wifi"

IMAGE_INSTALL = "\
    packagegroup-core-boot \
    kernel-modules \
    ${CORE_IMAGE_EXTRA_INSTALL} \
    "

CORE_IMAGE_EXTRA_INSTALL += "\
    packagegroup-sysdbgtools \
    "

IMAGE_LINGUAS = " "

inherit core-image

ROOTFS_POSTPROCESS_COMMAND += " rootfs_tuning_boot;"

rootfs_tuning_boot() {
    # make it clear this is the squahfs
    echo "Empty/Example Console-Only SquashFS" >> ${IMAGE_ROOTFS}/etc/issue
    # we dont want another copy of the bzImage boot file in here
    # plus the /boot directory will be hidden when we mount the real
    # EFI boot partition
    rm -rf ${IMAGE_ROOTFS}/boot/*

    # remove unnecessary things like NFS mounting
    rm ${IMAGE_ROOTFS}/etc/rc5.d/S15mountnfs.sh
}

IMAGE_BOOT_FILES += " \
        fit_spl_fpga.itb \
        mitysom_a10s.core.rbf \
        mitysom_a10s.periph.rbf \
        "

IMAGE_ROOTFS_SIZE ?= "8192"
IMAGE_ROOTFS_EXTRA_SPACE:append = "${@bb.utils.contains("DISTRO_FEATURES", "systemd", " + 4096", "" ,d)}"
export IMAGE_BASENAME = "application-rootfs"
IMAGE_FSTYPES = "squashfs"