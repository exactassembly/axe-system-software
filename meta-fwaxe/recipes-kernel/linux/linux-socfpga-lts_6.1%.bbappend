SRC_URI += "file://socfpga_mitysom5cse_l23y8_fwaxe.dts;subdir=git/arch/${ARCH}/boot/dts"
SRC_URI += "file://005-Enable_vitesse_phys.cfg"
#SRC_URI += "file://100_fix_spi_cpol.patch"

FILESEXTRAPATHS:prepend := "${THISDIR}/linux-socfpga-fwaxe:"

KERNEL_DEVICETREE += "\
			socfpga_mitysom5cse_l23y8_fwaxe.dtb \
			"
