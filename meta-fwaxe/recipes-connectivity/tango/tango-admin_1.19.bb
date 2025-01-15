DESCRIPTION = "tango controls database admin tool"
LICENSE = "GPLv3 | LGPLv3"
LIC_FILES_CHKSUM = " \
		 file://LICENSE;md5=1ebbd3e34237af26da5dc08a4e440464 \
"

DEPENDS += "cpptango"
#zeromq cppzmq omniorb-native omniorb 
SRC_URI = "\
	git://gitlab.com/tango-controls/tango_admin.git;protocol=https;branch=main \
	git://gitlab.com/tango-controls/TangoCMakeModules.git;protocol=https;branch=main;destsuffix=git/TangoCMakeModules;name=tangocmake \
"
SRCREV="fbd3465aa7eac0d2de55c9918ce66ccc1bcc048f"
SRCREV_tangocmake="892198999f0cf6fbc12f1024af85aeb16bf39abc"
S="${WORKDIR}/git"

EXTRA_OECMAKE:append = "-DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Release"

inherit cmake pkgconfig 
