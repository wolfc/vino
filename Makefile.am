ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}
SUBDIRS = server tools docs po

DISTCHECK_CONFIGURE_FLAGS = --with-avahi --with-libnotify --with-gnome-keyring --with-network-manager --with-telepathy --with-gnutls --with-gcrypt --with-zlib --with-jpeg --enable-debug --enable-http-server

bin_PROGRAMS = vino-preferences

vino_preferences_CPPFLAGS = \
	-I$(top_srcdir)/common \
	-I$(top_builddir) \
	-DVINO_LOCALEDIR=\""$(prefix)/$(DATADIRNAME)/locale"\"  \
	-DVINO_UIDIR=\""$(datadir)/vino"\" \
	-DVINO_ICONDIR=\""$(datadir)/pixmaps\"" \
	$(VINO_CAPPLET_CFLAGS) \
	$(VINO_KEYRING_CFLAGS) \
	$(VINO_HTTP_CFLAGS) \
	$(VINO_LIBUNIQUE_CFLAGS) \
	$(WARN_CFLAGS) \
	$(DISABLE_DEPRECATED)

vino_preferences_SOURCES = \
	common/vino-dbus.h \
	common/vino-dbus.c \
	common/vino-keyring.h \
	common/vino-keyring.c \
	capplet/vino-connectivity-info.h \
	capplet/vino-connectivity-info.c \
	capplet/vino-url-webservice.h \
	capplet/vino-url-webservice.c \
	capplet/vino-radio-button.h \
	capplet/vino-radio-button.c \
	capplet/vino-message-box.h \
	capplet/vino-message-box.c \
	capplet/vino-preferences.c

vino_preferences_LDADD = \
	$(VINO_CAPPLET_LIBS) \
	$(VINO_KEYRING_LIBS) \
	$(VINO_LIBUNIQUE_LIBS) \
	$(X_LIBS)

uidir = $(pkgdatadir)
dist_ui_DATA = capplet/vino-preferences.ui capplet/webservices

desktopdir = $(datadir)/applications
desktop_in_files = capplet/vino-preferences.desktop.in
nodist_desktop_DATA = $(desktop_in_files:.desktop.in=.desktop)

@INTLTOOL_DESKTOP_RULE@

# GSettings schemas, enum files and conversion file
gsettings_ENUM_NAMESPACE = org.gnome.Vino
gsettings_ENUM_FILES = capplet/vino-preferences.c
gsettings_in_file = common/org.gnome.Vino.gschema.xml.in
gsettings_SCHEMAS = $(gsettings_in_file:.xml.in=.xml)

@INTLTOOL_XML_NOMERGE_RULE@
@GSETTINGS_RULES@

dist_noinst_DATA = \
	$(desktop_in_files) \
	\
	common/org.gnome.Vino.gschema.xml \
	$(gsettings_in_file) \
	\
	MAINTAINERS \
	intltool-extract.in \
	intltool-merge.in \
	intltool-update.in

dist_noinst_SCRIPTS = autogen.sh

CLEANFILES = \
	$(nodist_desktop_DATA) \
	$(gsettings_SCHEMAS)

DISTCLEANFILES = \
	intltool-extract \
	intltool-merge \
	intltool-update \
	po/.intltool-merge-cache

MAINTAINERCLEANFILES = $(gsettings_SCHEMAS:.xml=.valid)
