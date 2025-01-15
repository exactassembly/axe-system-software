DESCRIPTION = "OmniORB High Performance ORB"
SECTION = "devel"
LICENSE = "GPLv2"
DEPENDS += "omniorb-native"
PR = "r2"

SRC_URI = "http://downloads.sourceforge.net/omniorb/omniORB-${PV}.tar.bz2; \
"
SRC_URI[sha256sum] = "1c745330d01904afd7a1ed0a5896b9a6e53ac1a4b864a48503b93c7eecbf1fa8"
LIC_FILES_CHKSUM="file://COPYING;md5=1b422f7cda3870b9c4b040b68ba1c0fe"
S = "${WORKDIR}/omniORB-${PV}"

inherit autotools pkgconfig 

EXTRA_OECONF:append = "--disable-longdouble"

BBCLASSEXTEND = "native"

FILES_${PN}-dev += "${datadir}/idl/omniORB/*"