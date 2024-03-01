FILESEXTRAPATHS:prepend := "${THISDIR}/files:"
SRC_URI += "file://fwaxe.cfg"
SRC_URI += "file://openocd.cfg"

do_deploy:append() {
    install -d ${DEPLOYDIR}
    install -m 744 ${WORKDIR}/fwaxe ${DEPLOYDIR}/usr/share/openocd/scripts/interfaces
}