DESCRIPTION = "tango control system"
LICENSE = "GPLv3"
LIC_FILES_CHKSUM = " \
		 file://LICENSE;md5=1ebbd3e34237af26da5dc08a4e440464 \
"

DEPENDS += "zeromq cppzmq omniorb-native omniorb cpptango"
#DEPENDS += " zeromq cppzmq tangoidl-native omniorb-native omniorb "

SRC_URI = "\
	git://gitlab.com/tango-controls/starter.git;protocol=https;branch=main \
	git://gitlab.com/tango-controls/TangoCMakeModules.git;protocol=https;branch=main;destsuffix=git/TangoCMakeModules;name=tangocmake"
SRCREV="c9905e9bd227e5a077ae859e5f1a02a359a6a0c6"
SRCREV_tangocmake="892198999f0cf6fbc12f1024af85aeb16bf39abc"

EXTRA_OECMAKE:append = "-DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Release"


S="${WORKDIR}/git"
inherit cmake pkgconfig 
