ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}
SUBDIRS = server capplet tools docs po

DISTCHECK_CONFIGURE_FLAGS = --with-avahi --with-libnotify --with-gnome-keyring --with-network-manager --with-telepathy --with-gnutls --with-gcrypt --with-zlib --with-jpeg --enable-debug --enable-http-server

# GSettings schemas, enum files and conversion file
gsettings_ENUM_NAMESPACE = org.gnome.Vino
gsettings_ENUM_FILES = capplet/vino-preferences.c
gsettings_in_file = common/org.gnome.Vino.gschema.xml.in
gsettings_SCHEMAS = $(gsettings_in_file:.xml.in=.xml)

@INTLTOOL_XML_NOMERGE_RULE@
@GSETTINGS_RULES@

dist_noinst_DATA = \
	common/org.gnome.Vino.gschema.xml \
	common/vino-dbus.h \
	common/vino-dbus.c \
	common/vino-keyring.h \
	common/vino-keyring.c \
	$(gsettings_in_file) \
	\
	MAINTAINERS \
	intltool-extract.in \
	intltool-merge.in \
	intltool-update.in

dist_noinst_SCRIPTS = autogen.sh

CLEANFILES = \
	$(gsettings_SCHEMAS)

DISTCLEANFILES = \
	intltool-extract \
	intltool-merge \
	intltool-update \
	po/.intltool-merge-cache

MAINTAINERCLEANFILES = $(gsettings_SCHEMAS:.xml=.valid)
