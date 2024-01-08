SRC_URI += "file://socfpga_mitysom5cse_l23y8_fwaxe.dts;subdir=git/arch/${ARCH}/boot/dts"
#SRC_URI += "file://00-BootRequired.cfg"

FILESEXTRAPATHS:prepend := "${THISDIR}/linux-socfpga-fwaxe:"

KERNEL_DEVICETREE += "\
			socfpga_mitysom5cse_l23y8_fwaxe.dtb \
			"
