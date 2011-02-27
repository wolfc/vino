NULL =

SUBDIRS = ifaddr rfb

noinst_LTLIBRARIES = libvncserver.la

INCLUDES = $(WARN_CFLAGS) $(GNUTLS_CFLAGS) $(GCRYPT_CFLAGS) -I$(top_builddir)

libvncserver_la_SOURCES = 	\
	auth.c 			\
	corre.c 		\
	cursor.c 		\
	cutpaste.c 		\
	d3des.c 		\
	d3des.h 		\
	hextile.c 		\
	main.c 			\
	rfbregion.c 		\
	rfbserver.c 		\
	rre.c 			\
	sockets.c 		\
	stats.c 		\
	tight.c 		\
	translate.c 		\
	vncauth.c		\
	zlib.c 			\
	zrle.c 			\
	zrleoutstream.c 	\
	zrleoutstream.h 	\
	zrlepalettehelper.c 	\
	zrlepalettehelper.h 	\
	zrletypes.h 		\
	$(NULL)

libvncserver_la_LIBADD =	\
	$(GNUTLS_LIBS)	\
	$(GCRYPT_LIBS)	\
	$(JPEG_LIBS) \
	$(NULL)

if VINO_HAVE_SELF_IFADDRS
libvncserver_la_LIBADD += ifaddr/libifaddrs.la
endif

EXTRA_DIST =			\
	tableinit24.c		\
	tableinittctemplate.c	\
	tabletrans24template.c 	\
	tabletranstemplate.c 	\
        tableinitcmtemplate.c	\
        zrleencodetemplate.c	\
	$(NULL)

