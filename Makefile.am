ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}
SUBDIRS = common server capplet tools docs po

DISTCHECK_CONFIGURE_FLAGS = --with-avahi --with-libnotify --with-gnome-keyring --with-network-manager --with-telepathy --with-gnutls --with-gcrypt --with-zlib --with-jpeg --enable-debug --enable-http-server

EXTRA_DIST =                \
	MAINTAINERS         \
	intltool-extract.in \
	intltool-merge.in   \
	intltool-update.in

DISTCLEANFILES =		\
	intltool-extract	\
	intltool-merge		\
	intltool-update
