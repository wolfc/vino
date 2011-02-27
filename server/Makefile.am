NULL =

SUBDIRS = libvncserver miniupnp smclient

INCLUDES = \
	-I$(top_srcdir)/common \
	-I$(top_srcdir)/server/libvncserver \
	-I$(top_srcdir)/server/miniupnp \
	-I$(top_srcdir)/server/smclient \
	-I$(top_builddir) \
	-I$(top_builddir)/server \
	$(VINO_SERVER_CFLAGS) \
	$(LIBNOTIFY_CFLAGS) \
	$(VINO_KEYRING_CFLAGS) \
	$(VINO_NETWORKMANAGER_CFLAGS) \
	$(AVAHI_CFLAGS) \
	$(GNUTLS_CFLAGS) \
	$(GCRYPT_CFLAGS) \
	$(WARN_CFLAGS) \
	$(DISABLE_DEPRECATED) \
	-DVINO_LOCALEDIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DVINO_ICONDIR=\""$(datadir)/pixmaps\"" \
	-DVINO_CLIENTDIR=\""$(datadir)/vino"\" \
	-DVINO_UIDIR=\""$(datadir)/vino"\" \
	$(NULL)

libexec_PROGRAMS = vino-server

vino_server_LDADD = \
	$(top_builddir)/server/libvncserver/libvncserver.la \
	$(top_builddir)/server/miniupnp/libminiupnp.la \
	$(top_builddir)/server/smclient/libeggsmclient.la \
	$(VINO_SERVER_LIBS) \
	$(LIBNOTIFY_LIBS) \
	$(KEYRING_LIBS) \
	$(AVAHI_LIBS) \
	$(NETWORKMANAGER_LIBS) \
	$(GNUTLS_LIBS) \
	$(GCRYPT_LIBS) \
	$(X_LIBS) $(XTEST_LIBS) $(XSHM_LIBS) $(XDAMAGE_LIBS) \
	$(NULL)

if VINO_ENABLE_HTTP_SERVER
VINO_HTTP_SRC = \
	vino-http.c \
	vino-http.h \
	$(NULL)
endif

vino_server_SOURCES = \
	../common/vino-dbus.c \
	vino-cursor.c \
	vino-cursor.h \
	vino-enums.c \
	vino-enums.h \
	vino-fb.c \
	vino-fb.h \
	vino-input.c \
	vino-input.h \
	vino-main.c \
	vino-marshal.c \
	vino-marshal.h \
	vino-mdns.c \
	vino-mdns.h \
	vino-prefs.c \
	vino-prefs.h \
	vino-prompt.c \
	vino-prompt.h \
	vino-server.c \
	vino-server.h \
	vino-util.c \
	vino-util.h \
	vino-status-icon.c \
	vino-status-icon.h \
	vino-dbus-listener.c \
	vino-dbus-listener.h \
	vino-dbus-error.c \
	vino-dbus-error.h \
	vino-upnp.h \
	vino-upnp.c \
	$(VINO_HTTP_SRC) \
	$(CORBA_SRCLIST) \
	$(NULL)

if VINO_HAVE_TELEPATHY_GLIB

INCLUDES += $(TELEPATHY_GLIB_CFLAGS)
vino_server_LDADD += $(TELEPATHY_GLIB_LIBS)
vino_server_SOURCES += \
	vino-tube-servers-manager.c vino-tube-servers-manager.h		\
	vino-tube-server.c vino-tube-server.h				\
	vino-status-tube-icon.c vino-status-tube-icon.h			\
	vino-types.h \
	$(NULL)

clientfiledir = $(datarootdir)/telepathy/clients
clientfile_DATA = Vino.client

endif

vino-marshal.h: vino-marshal.list
	$(AM_V_GEN)$(GLIB_GENMARSHAL) $< --header --prefix=vino_marshal > $@

vino-marshal.c: vino-marshal.list
	$(AM_V_GEN)echo "#include \"vino-marshal.h\"" > $@ && \
	$(GLIB_GENMARSHAL) $< --body --prefix=vino_marshal >> $@

vino_enum_headers = \
	$(top_srcdir)/server/vino-server.h \
	$(top_srcdir)/server/vino-prompt.h \
	$(top_srcdir)/server/vino-status-icon.h \
	$(top_srcdir)/server/vino-status-tube-icon.h \
	$(NULL)

vino-enums.c: $(vino_enum_headers)
	$(AM_V_GEN)glib-mkenums    --fhead "#include <glib-object.h>\n" \
			--fhead "#include \"vino-enums.h\"\n\n" \
			--fprod "\n/* enumerations from \"@filename@\" */" \
			--fprod "\n#include \"@filename@\"\n" \
			--vhead "static const G@Type@Value _@enum_name@_values[] = {" \
			--vprod "  { @VALUENAME@, \"@VALUENAME@\", \"@valuenick@\" }," \
			--vtail "  { 0, NULL, NULL }\n};\n\n" \
			--vtail "GType\n@enum_name@_get_type (void)\n{\n" \
			--vtail "  static GType type = 0;\n\n" \
			--vtail "  if (!type)\n" \
			--vtail "    type = g_@type@_register_static (\"@EnumName@\", _@enum_name@_values);\n\n" \
			--vtail "  return type;\n}\n\n" \
		$(vino_enum_headers) > $@

vino-enums.h: $(vino_enum_headers)
	$(AM_V_GEN)glib-mkenums    --fhead "#ifndef __VINO_ENUMS_H__\n" \
			--fhead "#define __VINO_ENUMS_H__ 1\n\n" \
			--fhead "G_BEGIN_DECLS\n\n" \
			--ftail "G_END_DECLS\n\n" \
			--ftail "#endif /* __VINO_ENUMS_H__ */\n" \
			--fprod "\n/* --- @filename@ --- */" \
			--eprod "#define VINO_TYPE_@ENUMSHORT@ @enum_name@_get_type()\n" \
			--eprod "GType @enum_name@_get_type (void);\n" \
		$(vino_enum_headers) >  $@

vino-server.desktop.in: vino-server.desktop.in.in
	$(AM_V_GEN)sed -e "s|\@LIBEXECDIR\@|$(libexecdir)|" $< > $@

desktopdir      = $(sysconfdir)/xdg/autostart
desktop_in_files = vino-server.desktop.in
desktop_DATA    = $(desktop_in_files:.desktop.in=.desktop)

servicedir = $(datadir)/dbus-1/services
service_DATA = org.freedesktop.Telepathy.Client.Vino.service

org.freedesktop.Telepathy.Client.Vino.service: org.freedesktop.Telepathy.Client.Vino.service.in \
		Makefile
	@sed -e "s|[@]libexecdir[@]|$(libexecdir)|" $< > $@

@INTLTOOL_DESKTOP_RULE@

uidir   = $(datadir)/vino
ui_DATA = vino-prompt.ui

EXTRA_DIST = \
	vino-prompt.ui \
	vino-marshal.list \
	vino-server.desktop.in.in \
	org.freedesktop.Telepathy.Client.Vino.service.in \
	dbus-interface.xml \
	Vino.client \
	$(NULL)

BUILT_SOURCES = \
	vino-enums.c \
	vino-enums.h \
	vino-marshal.c \
	vino-marshal.h \
	$(NULL)

CLEANFILES = \
	$(service_DATA) \
	vino-server.desktop     \
	vino-server.desktop.in

DISTCLEANFILES = \
	$(BUILT_SOURCES) \
	$(NULL)

dist-hook:
	cd $(distdir) ; rm -f $(DISTCLEANFILES)

$(vino_server_OBJECTS): $(BUILT_SOURCES)
