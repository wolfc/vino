EXTRA_DIST = \
	org.gnome.Vino.gschema.xml	\
	vino-dbus.h			\
	vino-dbus.c			\
	vino-keyring.h			\
	vino-keyring.c

# GSettings schemas, enum files and conversion file
gsettings_ENUM_NAMESPACE = org.gnome.Vino
gsettings_ENUM_FILES = $(top_srcdir)/capplet/vino-preferences.c
wgsettings_in_file = org.gnome.Vino.gschema.xml.in
gsettings_SCHEMAS = $(gsettings_in_file:.xml.in=.xml)

@INTLTOOL_XML_NOMERGE_RULE@
@GSETTINGS_RULES@

dist_noinst_DATA = \
	$(gsettings_in_file)

CLEANFILES = \
	$(gsettings_SCHEMAS)

MAINTAINERCLEANFILES = $(gsettings_SCHEMAS:.xml=.valid)