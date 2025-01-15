DESCRIPTION = "tango controls database system"
LICENSE = "GPLv3"
LIC_FILES_CHKSUM = " \
		 file://LICENSE;md5=1ebbd3e34237af26da5dc08a4e440464 \
"

DEPENDS += " zlib zeromq cppzmq omniorb-native omniorb mysql5 cpptango"

SRC_URI = "git://gitlab.com/tango-controls/TangoDatabase.git;protocol=https;branch=main"
SRCREV="45073afcf64c87e1c1504b2c3c1f31ae6101dc39"
SRC_URI[sha256sum] = "bd6f2e2ba5dd002dca036ab2d240d5f6fc7861e7c4cd88351be8cd646c0fb69e"
S="${WORKDIR}/git"

inherit cmake pkgconfig
FILES:${PN} += "\
  /usr/share \
  /usr/share/tango \
  /usr/share/tango/db \
  /usr/share/tango/db/create_db.sql \
  /usr/share/tango/db/create_db.sh \
  /usr/share/tango/db/update_db_from_9.2.5_to_9.3.4.sql \
  /usr/share/tango/db/update_db_from_6_to_9.3.4.sql \
  /usr/share/tango/db/update_db_from_8_to_9.3.4.sql \
  /usr/share/tango/db/rem_history.sql \
  /usr/share/tango/db/update_db_from_5_to_9.3.4.sql \
  /usr/share/tango/db/update_db_from_7_to_9.3.4.sql \
  /usr/share/tango/db/create_db_tables.sql \
  /usr/share/tango/db/update_db.sh \
  /usr/share/tango/db/stored_proc.sql \
  /usr/share/tango/db/update_db.sql \
  /usr/share/tango/db/migrate_db_tables_innodb.sql \
"