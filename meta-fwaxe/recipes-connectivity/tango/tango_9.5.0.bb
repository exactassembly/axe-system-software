DESCRIPTION = "tango control system"
LICENSE = "GPLv3 | LGPLv3"
LIC_FILES_CHKSUM = " \
		 file://COPYING;md5=d32239bcb673463ab874e80d47fae504 \
"

DEPENDS += " zlib zeromq cppzmq tangoidl-native omniorb-native omniorb libmysqlclient cpptango"

SRC_URI = "git://gitlab.com/tango-controls/TangoSourceDistribution.git;protocol=https;branch=main"
SRCREV = "9e97c463fac9e9bae0ea9d6ac97320bd38b0ed3e"

EXTRA_OECMAKE = "-DCMAKE_BUILD_TYPE=Release"
EXTRA_OECMAKE += "-DBUILD_SHARED_LIBS=OFF"
EXTRA_OECMAKE += "-DBUILD_TESTING=OFF"
EXTRA_OECMAKE += "-DTSD_JAVA=OFF"
EXTRA_OECMAKE += "-DTDB_DATABASE_SCHEMA=OFF"
EXTRA_OECMAKE += "-DDB_SCRIPTS=ON"

#					-DTANGO_OMNI_BASE=/usr 


S="${WORKDIR}/git/assets"
inherit cmake pkgconfig 
