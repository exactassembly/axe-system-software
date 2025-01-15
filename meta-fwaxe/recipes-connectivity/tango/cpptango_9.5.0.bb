DESCRIPTION = "tango control system"
LICENSE = "LGPLv3"
LIC_FILES_CHKSUM = " \
		 file://LICENSE;md5=8195c2d9416e5fc98eb30ee334511b73 \
"

DEPENDS += " zeromq cppzmq tangoidl-native omniorb-native omniorb jpeg"

SRC_URI = "\
	git://gitlab.com/tango-controls/cppTango.git;protocol=https;branch=bugfix/init-host-name-more-debug-output \
	git://gitlab.com/tango-controls/TangoCMakeModules.git;protocol=https;branch=main;destsuffix=git/TangoCMakeModules;name=tangocmake"
SRCREV="cdf38febf26fb5e8d10a4203f9560192a8462da6"
SRCREV_tangocmake="892198999f0cf6fbc12f1024af85aeb16bf39abc"

EXTRA_OECMAKE:append = "-DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Release"

S="${WORKDIR}/git"
inherit cmake pkgconfig 
