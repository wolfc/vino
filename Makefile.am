ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}
SUBDIRS = common server capplet tools docs po

DISTCHECK_CONFIGURE_FLAGS = --enable-avahi --enable-libnotify --enable-gnome-keyring --enable-http-server --enable-network-manager=yes --enable-telepathy --enable-debug

EXTRA_DIST =                \
	MAINTAINERS         \
	intltool-extract.in \
	intltool-merge.in   \
	intltool-update.in

DISTCLEANFILES =		\
	intltool-extract	\
	intltool-merge		\
	intltool-update
