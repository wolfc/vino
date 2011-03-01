ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}
SUBDIRS = docs po

DISTCHECK_CONFIGURE_FLAGS = --with-avahi --with-libnotify --with-gnome-keyring --with-network-manager --with-telepathy --with-gnutls --with-gcrypt --with-zlib --with-jpeg --enable-debug --enable-http-server

smclient_platform_defines = -DEGG_SM_CLIENT_BACKEND_XSMP
smclient_cppflags = \
	-DG_LOG_DOMAIN=\""EggSMClient"\" \
	$(smclient_platform_defines) \
	$(EGG_SMCLIENT_CFLAGS) \
	-I$(top_builddir)
smclient_platform_libs = libeggdesktopfile.la -lSM -lICE
smclient_platform_sources = server/smclient/eggsmclient-xsmp.c

if VINO_HAVE_SELF_IFADDRS
ifaddrs_ltlibraries = libifaddrs.la
endif
noinst_LTLIBRARIES = \
	libeggsmclient.la \
	libeggdesktopfile.la \
	libminiupnp.la \
	libvncserver.la \
	$(ifaddrs_ltlibraries)

libeggsmclient_la_CPPFLAGS = \
	$(smclient_cppflags)

libeggsmclient_la_LIBADD = \
	$(EGG_SMCLIENT_LIBS) \
	$(smclient_platform_libs)

libeggsmclient_la_SOURCES = \
	server/smclient/eggsmclient.c \
	server/smclient/eggsmclient.h \
	server/smclient/eggsmclient-private.h \
	$(smclient_platform_sources)

libeggdesktopfile_la_CPPFLAGS = \
	$(smclient_cppflags)

libeggdesktopfile_la_LIBADD = \
	$(EGG_LIBS)

libeggdesktopfile_la_SOURCES = \
	server/smclient/eggdesktopfile.c \
	server/smclient/eggdesktopfile.h

libminiupnp_la_CPPFLAGS = -DNDEBUG

libminiupnp_la_SOURCES = \
	server/miniupnp/bsdqueue.h \
	server/miniupnp/codelength.h \
	server/miniupnp/declspec.h \
	server/miniupnp/igd_desc_parse.c \
	server/miniupnp/igd_desc_parse.h \
	server/miniupnp/minisoap.c \
	server/miniupnp/minisoap.h \
	server/miniupnp/minissdpc.c \
	server/miniupnp/minissdpc.h \
	server/miniupnp/miniupnpc.c \
	server/miniupnp/miniupnpc.h \
	server/miniupnp/miniwget.c \
	server/miniupnp/miniwget.h \
	server/miniupnp/minixml.c \
	server/miniupnp/minixml.h \
	server/miniupnp/upnpcommands.c \
	server/miniupnp/upnpcommands.h \
	server/miniupnp/upnpreplyparse.c \
	server/miniupnp/upnpreplyparse.h

libifaddrs_la_SOURCES = \
	server/libvncserver/ifaddr/getifaddrs.c \
	server/libvncserver/ifaddr/ifaddrs.h

rfb_headers = \
	server/libvncserver/rfb/rfb.h \
	server/libvncserver/rfb/rfbregion.h \
	server/libvncserver/rfb/rfbproto.h \
	server/libvncserver/rfb/keysym.h

libvncserver_la_CPPFLAGS = \
	$(WARN_CFLAGS) \
	$(GNUTLS_CFLAGS) \
	$(GCRYPT_CFLAGS) \
	-I$(top_builddir)

libvncserver_la_SOURCES = \
	server/libvncserver/auth.c \
	server/libvncserver/corre.c \
	server/libvncserver/cursor.c \
	server/libvncserver/cutpaste.c \
	server/libvncserver/d3des.c \
	server/libvncserver/d3des.h \
	server/libvncserver/hextile.c \
	server/libvncserver/main.c \
	server/libvncserver/rfbregion.c \
	server/libvncserver/rfbserver.c \
	server/libvncserver/rre.c \
	server/libvncserver/sockets.c \
	server/libvncserver/stats.c \
	server/libvncserver/tight.c \
	server/libvncserver/translate.c \
	server/libvncserver/vncauth.c \
	server/libvncserver/zlib.c \
	server/libvncserver/zrle.c \
	server/libvncserver/zrleoutstream.c \
	server/libvncserver/zrleoutstream.h \
	server/libvncserver/zrlepalettehelper.c \
	server/libvncserver/zrlepalettehelper.h \
	server/libvncserver/zrletypes.h \
	$(rfb_headers)

if VINO_HAVE_SELF_IFADDRS
SELF_IFADDRS_LIBS = server/libvncserver/ifaddr/libifaddrs.la
endif

libvncserver_la_LIBADD = \
	$(GNUTLS_LIBS) \
	$(GCRYPT_LIBS) \
	$(JPEG_LIBS) \
	$(SELF_IFADDRS_LIBS)

vino_server_CPPFLAGS = \
	-I$(top_srcdir) \
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
	-DVINO_CLIENTDIR=\""$(pkgdatadir)"\" \
	-DVINO_UIDIR=\""$(pkgdatadir)"\"

libexec_PROGRAMS = vino-server

vino_server_LDADD = \
	libvncserver.la \
	libminiupnp.la \
	libeggsmclient.la \
	$(VINO_SERVER_LIBS) \
	$(LIBNOTIFY_LIBS) \
	$(KEYRING_LIBS) \
	$(AVAHI_LIBS) \
	$(NETWORKMANAGER_LIBS) \
	$(GNUTLS_LIBS) \
	$(GCRYPT_LIBS) \
	$(X_LIBS) $(XTEST_LIBS) $(XSHM_LIBS) $(XDAMAGE_LIBS)

if VINO_ENABLE_HTTP_SERVER
VINO_HTTP_SRC = \
	server/vino-http.c \
	server/vino-http.h
endif

if VINO_HAVE_TELEPATHY_GLIB
vino_server_CPPFLAGS += $(TELEPATHY_GLIB_CFLAGS)
vino_server_LDADD += $(TELEPATHY_GLIB_LIBS)
VINO_TELEPATHY_GLIB_SRC = \
	server/vino-tube-servers-manager.c \
	server/vino-tube-servers-manager.h \
	server/vino-tube-server.c \
	server/vino-tube-server.h \
	server/vino-status-tube-icon.c \
	server/vino-status-tube-icon.h	\
	server/vino-types.h

clientfiledir = $(datarootdir)/telepathy/clients
dist_clientfile_DATA = server/Vino.client
endif

vino_server_SOURCES = \
	common/vino-dbus.c \
	common/vino-dbus.h \
	server/vino-cursor.c \
	server/vino-cursor.h \
	server/vino-enums.c \
	server/vino-enums.h \
	server/vino-fb.c \
	server/vino-fb.h \
	server/vino-input.c \
	server/vino-input.h \
	server/vino-main.c \
	server/vino-marshal.c \
	server/vino-marshal.h \
	server/vino-mdns.c \
	server/vino-mdns.h \
	server/vino-prefs.c \
	server/vino-prefs.h \
	server/vino-prompt.c \
	server/vino-prompt.h \
	server/vino-server.c \
	server/vino-server.h \
	server/vino-util.c \
	server/vino-util.h \
	server/vino-status-icon.c \
	server/vino-status-icon.h \
	server/vino-dbus-listener.c \
	server/vino-dbus-listener.h \
	server/vino-dbus-error.c \
	server/vino-dbus-error.h \
	server/vino-upnp.h \
	server/vino-upnp.c \
	$(VINO_HTTP_SRC) \
	$(VINO_TELEPATHY_GLIB_SRC)

server/vino-marshal.h: server/vino-marshal.list
	$(AM_V_GEN)$(GLIB_GENMARSHAL) $< --header --prefix=vino_marshal > $@

server/vino-marshal.c: server/vino-marshal.list
	$(AM_V_GEN)echo "#include \"vino-marshal.h\"" > $@ && \
	$(GLIB_GENMARSHAL) $< --body --prefix=vino_marshal >> $@

vino_enum_headers = \
	server/vino-server.h \
	server/vino-prompt.h \
	server/vino-status-icon.h \
	server/vino-status-tube-icon.h

server/vino-enums.c: $(vino_enum_headers)
	$(AM_V_GEN)$(GLIB_MKENUMS)    --fhead "#include <glib-object.h>\n" \
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

server/vino-enums.h: $(vino_enum_headers)
	$(AM_V_GEN)$(GLIB_MKENUMS)    --fhead "#ifndef __VINO_ENUMS_H__\n" \
			--fhead "#define __VINO_ENUMS_H__ 1\n\n" \
			--fhead "G_BEGIN_DECLS\n\n" \
			--ftail "G_END_DECLS\n\n" \
			--ftail "#endif /* __VINO_ENUMS_H__ */\n" \
			--fprod "\n/* --- @filename@ --- */" \
			--eprod "#define VINO_TYPE_@ENUMSHORT@ @enum_name@_get_type()\n" \
			--eprod "GType @enum_name@_get_type (void);\n" \
		$(vino_enum_headers) >  $@

server/vino-server.desktop.in: server/vino-server.desktop.in.in
	$(AM_V_GEN)sed -e "s|\@LIBEXECDIR\@|$(libexecdir)|" $< > $@

servicedir = $(datadir)/dbus-1/services
nodist_service_DATA = server/org.freedesktop.Telepathy.Client.Vino.service

server/org.freedesktop.Telepathy.Client.Vino.service: server/org.freedesktop.Telepathy.Client.Vino.service.in \
		Makefile
	@sed -e "s|[@]libexecdir[@]|$(libexecdir)|" $< > $@

BUILT_SOURCES = \
	server/vino-enums.c \
	server/vino-enums.h \
	server/vino-marshal.c \
	server/vino-marshal.h

bin_PROGRAMS = \
	vino-preferences \
	vino-passwd

vino_preferences_CPPFLAGS = \
	-I$(top_srcdir)/common \
	-I$(top_builddir) \
	-DVINO_LOCALEDIR=\""$(prefix)/$(DATADIRNAME)/locale"\"  \
	-DVINO_UIDIR=\""$(pkgdatadir)"\" \
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

vino_passwd_CPPFLAGS = \
	-I$(top_builddir) \
	-DVINO_LOCALEDIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	$(VINO_TOOLS_CFLAGS) \
	$(WARN_CFLAGS) \
	$(DISABLE_DEPRECATED)

vino_passwd_SOURCES = \
	tools/vino-passwd.c

vino_passwd_LDADD = \
	$(VINO_TOOLS_LIBS)

uidir = $(pkgdatadir)
dist_ui_DATA = \
	capplet/vino-preferences.ui \
	capplet/webservices \
	server/vino-prompt.ui

autostartdir = $(sysconfdir)/xdg/autostart
autostart_in_files = server/vino-server.desktop.in
nodist_autostart_DATA = $(autostart_in_files:.desktop.in=.desktop)

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
	server/vino-marshal.list \
	server/vino-server.desktop.in.in \
	server/org.freedesktop.Telepathy.Client.Vino.service.in \
	server/dbus-interface.xml \
	server/libvncserver/tableinit24.c \
	server/libvncserver/tableinittctemplate.c \
	server/libvncserver/tabletrans24template.c \
	server/libvncserver/tabletranstemplate.c \
        server/libvncserver/tableinitcmtemplate.c \
        server/libvncserver/zrleencodetemplate.c \
	server/miniupnp/README \
	server/miniupnp/LICENCE \
	\
	MAINTAINERS \
	intltool-extract.in \
	intltool-merge.in \
	intltool-update.in

dist_noinst_SCRIPTS = autogen.sh

CLEANFILES = \
	$(nodist_desktop_DATA) \
	$(gsettings_SCHEMAS) \
	$(nodist_service_DATA) \
	server/vino-server.desktop \
	server/vino-server.desktop.in

DISTCLEANFILES = \
	$(BUILT_SOURCES) \
	intltool-extract \
	intltool-merge \
	intltool-update \
	po/.intltool-merge-cache

MAINTAINERCLEANFILES = $(gsettings_SCHEMAS:.xml=.valid)
