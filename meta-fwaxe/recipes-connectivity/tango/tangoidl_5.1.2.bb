DESCRIPTION = "tango control IDL parser"
LICENSE = "GPLv3 | LGPLv3"
LIC_FILES_CHKSUM = " \
		 file://LICENSE;md5=6c9432eab6a070a936cf9da6191d6db6 \
"

DEPENDS += ""

SRC_URI = "\
	git://gitlab.com/tango-controls/tango-idl.git;protocol=https;branch=main \
"
SRCREV="89225b5ef6293d038e633414dd2ea917a99aea74"
S="${WORKDIR}/git"

inherit cmake pkgconfig 

BBCLASSEXTEND = "native"