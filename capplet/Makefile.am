NULL =

INCLUDES = \
	-I$(top_srcdir)/common \
	-DVINO_LOCALEDIR=\""$(prefix)/$(DATADIRNAME)/locale"\"  \
	-DVINO_UIDIR=\""$(datadir)/vino"\" \
	-DVINO_ICONDIR=\""$(datadir)/pixmaps\"" \
	$(VINO_CAPPLET_CFLAGS) \
	$(VINO_KEYRING_CFLAGS) \
	$(VINO_HTTP_CFLAGS) \
	$(VINO_LIBUNIQUE_CFLAGS) \
	$(WARN_CFLAGS) \
	$(DISABLE_DEPRECATED) \
	$(NULL)

bin_PROGRAMS = vino-preferences

vino_preferences_SOURCES = \
	../common/vino-dbus.c		\
	../common/vino-keyring.c	\
	vino-connectivity-info.h	\
	vino-connectivity-info.c	\
	vino-url-webservice.h		\
	vino-url-webservice.c		\
	vino-radio-button.h		\
	vino-radio-button.c		\
	vino-message-box.h		\
	vino-message-box.c		\
	vino-preferences.c		\
	$(NULL)

vino_preferences_LDADD = \
	$(VINO_CAPPLET_LIBS) \
	$(VINO_KEYRING_LIBS) \
	$(VINO_LIBUNIQUE_LIBS) \
	$(X_LIBS) \
	$(NULL)

uidir   = $(datadir)/vino
ui_DATA = vino-preferences.ui webservices

desktopdir       = $(datadir)/applications
desktop_in_files = vino-preferences.desktop.in
desktop_DATA     = $(desktop_in_files:.desktop.in=.desktop)

@INTLTOOL_DESKTOP_RULE@

CLEANFILES = \
	vino-preferences.desktop \
	$(NULL)

EXTRA_DIST = \
	vino-preferences.ui \
	vino-preferences.desktop.in \
	webservices \
	$(NULL)
