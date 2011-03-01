AC_PREREQ([2.64])
AC_INIT([Vino],[2.99.3],[http://bugzilla.gnome.org/enter_bug.cgi?product=vino],[vino])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_SRCDIR([server/vino-fb.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([1.10 -Wall no-define nostdinc])
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

AC_PROG_CC

# Check whether to enable HTTP server support
AC_ARG_ENABLE([http-server],
  [AS_HELP_STRING([--disable-http-server],
   [build with an HTTP server for serving a Java applet client @<:@default=yes@:>@])],
   [],
   [enable_httpserver=yes])

AS_IF([test "x$enable_httpserver" = "xyes"],
  [AC_DEFINE([VINO_ENABLE_HTTP_SERVER], [], [Enable HTTP server support])])

AM_CONDITIONAL([VINO_ENABLE_HTTP_SERVER], [test "x$enable_httpserver" = "xyes"])

# GNOME common
GNOME_MAINTAINER_MODE_DEFINES
GNOME_COMPILE_WARNINGS([maximum])
GNOME_DEBUG_CHECK

LT_PREREQ([2.2.6])
LT_INIT([disable-static])

PKG_PROG_PKG_CONFIG([0.16])

AC_MSG_CHECKING([for glib-genmarshal script])
GLIB_GENMARSHAL=`$PKG_CONFIG --variable=glib_genmarshal glib-2.0`
AS_IF([test "x$GLIB_GENMARSHAL" = "x"],
  [AC_MSG_ERROR([glib-genmarshal not listed in glib-2.0 pkg-config file])],
  [AC_SUBST([GLIB_GENMARSHAL])
    AC_MSG_RESULT([$GLIB_GENMARSHAL])])

AC_MSG_CHECKING([for glib-mkenums script])
GLIB_MKENUMS=`$PKG_CONFIG --variable=glib_mkenums glib-2.0`
AS_IF([test "x$GLIB_MKENUMS" = "x"],
  [AC_MSG_ERROR([glib-mkenums not listed in glib-2.0 pkg-config file])],
  [AC_SUBST([GLIB_MKENUMS])
    AC_MSG_RESULT([$GLIB_MKENUMS])])

# GNOME libraries
GTK_VERSION=3.0.0
GLIB_VERSION=2.17.0
GIO_VERSION=2.26
DBUS_VERSION=1.2.3
SOUP_VERSION=2.24.0
TELEPATHY_GLIB_VERSION=0.11.6
NETWORKMANAGER_VERSION=0.7

PKG_CHECK_MODULES([VINO_SERVER], [glib-2.0 >= $GLIB_VERSION gio-unix-2.0 gtk+-x11-3.0 >= $GTK_VERSION])

PKG_CHECK_MODULES([VINO_CAPPLET], [glib-2.0 >= $GLIB_VERSION gio-2.0 >= $GIO_VERSION gtk+-3.0 >= $GTK_VERSION libsoup-2.4 >= $SOUP_VERSION])

PKG_CHECK_MODULES([VINO_TOOLS], [glib-2.0 >= $GLIB_VERSION gio-2.0 >= $GIO_VERSION gobject-2.0 >= $GLIB_VERSION gnome-keyring-1])

# EGG stuff
PKG_CHECK_MODULES([EGG], [gtk+-3.0 >= $GTK_VERSION])
PKG_CHECK_MODULES([EGG_SMCLIENT], [gtk+-3.0])

# Check for telepathy-glib
AC_ARG_WITH([telepathy],
  [AS_HELP_STRING([--without-telepathy],
    [ignore the presence of telepathy and disable it])])

AS_IF([test "x$with_telepathy" != "xno"],
  [PKG_CHECK_MODULES([TELEPATHY_GLIB],
    [telepathy-glib >= $TELEPATHY_GLIB_VERSION],
    [have_telepathy=yes],
    [have_telepathy=no])],
  [have_telepathy=no])

AS_IF([test "x$have_telepathy" = "xyes"],
  [AC_DEFINE([VINO_HAVE_TELEPATHY_GLIB], [], [Build with telepathy support])],
  [AS_IF([test "x$with_telepathy" = "xyes"],
    [AC_MSG_ERROR([telepathy support requested but not found])])])

AM_CONDITIONAL([VINO_HAVE_TELEPATHY_GLIB], [test "x$have_telepathy" = "xyes"])

# Check for libnotify
LIBNOTIFY_VERSION=0.4.4

AC_ARG_WITH([libnotify],
  [AS_HELP_STRING([--without-libnotify],
    [ignore the presence of libnotify and disable it])])

AS_IF([test "x$with_libnotify" != "xno"],
  [PKG_CHECK_MODULES([LIBNOTIFY],
    [libnotify >= $LIBNOTIFY_VERSION],
    [have_libnotify=yes],
    [have_libnotify=no])],
  [have_libnotify=no])

AS_IF([test "x$have_libnotify" = "xyes"],
  [AC_DEFINE([VINO_HAVE_LIBNOTIFY], [], [Build with libnotify support])
    PKG_CHECK_EXISTS([libnotify >= 0.7.0],
      [have_libnotify_0_7=yes],
      [have_libnotify_0_7=no])
    AS_IF([test "x$have_libnotify_0_7" = "xyes"],
      [AC_DEFINE([VINO_HAVE_LIBNOTIFY_0_7], [], [Build with libnotify 0.7 or greater support])])],
  [AS_IF([test "x$have_libnotify" = "xyes"],
    [AC_MSG_ERROR([libnotify support requested but not found])])])

# Check for NetworkManager
AC_ARG_WITH([network-manager],
  [AS_HELP_STRING([--without-network-manager],
    [ignore the presence of NetworkManager and disable it])])

AS_IF([test "x$with_networkmanager" != "xno"],
  [PKG_CHECK_MODULES([NETWORKMANAGER],
    [NetworkManager >= $NETWORKMANAGER_VERSION],
    [have_networkmanager=yes],
    [have_networkmanager=no])],
  [have_networkmanager=no])

AS_IF([test "x$have_networkmanager" = "xyes"],
  [AC_DEFINE([VINO_HAVE_NETWORKMANAGER], [], [Build with NetworkManager support])],
  [AS_IF([test "x$with_networkmanager" = "xyes"],
    [AC_MSG_ERROR([NetworkManager support requested but not found])])])

# Check for gnome-keyring
AC_ARG_WITH([gnome-keyring],
  [AS_HELP_STRING([--without-gnome-keyring],
    [ignore the presence of gnome-keyring and disable it])])

AS_IF([test "x$with_gnomekeyring" != "xno"],
  [PKG_CHECK_MODULES([GNOME_KEYRING],
    [gnome-keyring-1],
    [have_gnomekeyring=yes],
    [have_gnomekeyring=no])],
  [have_gnomekeyring=no])

AS_IF([test "x$have_gnomekeyring" != "xno"],
  [AC_DEFINE([VINO_HAVE_GNOME_KEYRING], [], [Build with gnome-keyring support])],
  [AS_IF([test "x$with_gnomekeyring" = "xyes"],
    [AC_MSG_ERROR([gnome-keyring support requested but not found])])])

# If Pango included the shared library dependencies from X11 in
# the pkg-config output, then we use that (to avoid duplicates).
# but if they were omitted to avoid binary compatibility problems
# then we need to repeat the checks.
PKG_CHECK_EXISTS([pangoxft],
  [PANGO_PACKAGES="pangox pangoxft"],
  [PANGO_PACKAGES="pangox"])

AS_CASE([`$PKG_CONFIG --libs $PANGO_PACKAGES`],
  [*-lX11*], [pango_omitted_x_deps=no],
  [pango_omitted_x_deps=yes])

AS_IF([test "x$pango_omitted_x_deps" = "xyes"],
  [AC_PATH_XTRA
    AS_IF([test "x$no_x" = "xyes"],
      [AC_MSG_ERROR([X development libraries not found])],
      [AC_SUBST([X_LIBS], [$X_PRE_LIBS $X_LIBS -lX11 $X_EXTRA_LIBS])])])

# Check for gnutls
GNUTLS_VERSION=1.0.0

AC_ARG_WITH([gnutls],
  [AS_HELP_STRING([--without-gnutls],
    [ignore gnutls support and disable it])])

AS_IF([test "x$with_gnutls" != "xno"],
  [PKG_CHECK_MODULES([GNUTLS],
    [gnutls >= $GNUTLS_VERSION],
    [have_gnutls=yes],
    [have_gnutls=no])],
  [have_gnutls=no
    AC_MSG_WARN([gnutls was not found. You may want to get it from ftp://ftp.gnutls.org/pub/gnutls/])])

AS_IF([test "x$have_gnutls" = "xyes"],
  [AC_DEFINE([VINO_HAVE_GNUTLS], [], [Build with gnutls support])],
  [AS_IF([test "x$with_gnutls" = "xyes"],
    [AC_MSG_ERROR([gnutls support was requested but not found])])])

# Check for gcrypt
GCRYPT_VERSION=1.1.90

AC_ARG_WITH([gcrypt],
  [AS_HELP_STRING([--without-gcrypt],
    [ignore gcrypt support and disable it])])

AS_IF([test "x$with_gcrypt" != "xno"],
  [AM_PATH_LIBGCRYPT([$GCRYPT_VERSION],
    [have_gcrypt=yes],
    [have_gcrypt=no])],
  [have_gcrypt=no
    AC_MSG_WARN([gcrypt was not found. You may want to get it from ftp://ftp.gnupg.org/pub/gcrypt/alpha/libgcrypt/])])

AS_IF([test "x$have_gcrypt" = "xyes"],
  [AC_DEFINE([VINO_HAVE_GCRYPT], [], [Build with gcrypt support])],
  [AS_IF([test "x$with_gcrypt" = "xyes"],
    [AC_MSG_ERROR([gcrypt support requested but not found])])])

# Check for Avahi
AVAHI_VERSION=0.6

AC_ARG_WITH([avahi],
  [AS_HELP_STRING([--without-avahi],
    [ignore avahi support and disable it])])

AS_IF([test "x$with_avahi" != "xno"],
  [PKG_CHECK_MODULES([AVAHI],
    [avahi-client >= $AVAHI_VERSION avahi-glib >= $AVAHI_VERSION],
    [have_avahi=yes],
    [have_avahi=no])],
  [have_avahi=no])

AS_IF([test "x$have_avahi" = "xyes"],
  [AC_DEFINE([VINO_HAVE_AVAHI], [], [Build with avahi support])],
  [AS_IF([test "x$with_avahi" = "xyes"],
    [AC_MSG_ERROR([avahi support requested but not found])])])

# Check for zlib
# TODO: Use pkg-config file installed by newer zlib releases
AC_ARG_WITH([zlib],
  [AS_HELP_STRING([--without-zlib],
    [ignore zlib support and disable it])])

AS_IF([test "x$with_zlib" != "xno"],
  [AC_CHECK_HEADER([zlib.h],
    [AC_CHECK_LIB([z], [deflate], [have_zlib=yes], [have_zlib=no])],
    [have_zlib=no])],
  [have_zlib=no])

AS_IF([test "x$have_zlib" = "xyes"],
  [AC_DEFINE([VINO_HAVE_ZLIB], [], [Build with zlib support])],
  [AS_IF([test "x$with_zlib" = "xyes"],
    [AC_MSG_ERROR([zlib support requested but not found])])])

# Check for jpeg, with deflate support
AC_ARG_WITH([jpeg],
  [AS_HELP_STRING([--without-jpeg],
    [ignore jpeg support and disable it])])

AS_IF([test "x$with_jpeg" != "xno"],
  [AC_CHECK_HEADER([jpeglib.h],
    [AC_CHECK_LIB([jpeg], [jpeg_CreateCompress], [have_jpeg=yes], [have_jpeg=no])],
    [have_jpeg=no])],
  [have_jpeg=no])

AS_IF([test "x$have_jpeg" = "xyes"],
  [AC_DEFINE([VINO_HAVE_JPEG], [], [Build with jpeg support])
    AC_SUBST([JPEG_LIBS], [-ljpeg])],
  [AS_IF([test "x$with_jpeg" = "xyes"],
    [AC_MSG_ERROR([jpeg support requested but not found])])])

# Internationalization
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.17])
AC_SUBST([GETTEXT_PACKAGE],[$PACKAGE_TARNAME])
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$GETTEXT_PACKAGE"], [gettext package])
IT_PROG_INTLTOOL([0.40.0])

# Check for XDAMAGE extension
AC_CHECK_HEADER([X11/extensions/Xdamage.h],
  [AC_CHECK_LIB([Xdamage], [XDamageQueryExtension],
    [AC_DEFINE([VINO_HAVE_XDAMAGE], [], [Defined if the DAMAGE X extension is present])
      AC_SUBST([XDAMAGE_LIBS], ["-lXdamage -lXfixes"])],
    [], [$X_LIBS])],
  [], [#include <X11/Xlib.h>])

# Check for MIT-SHM extension
AC_CHECK_HEADER([X11/extensions/XShm.h],
  [AC_CHECK_LIB([Xext], [XShmQueryExtension],
    [AC_DEFINE([VINO_HAVE_XSHM], [], [Defined if the MIT-SHM X extension is present])
      AC_SUBST([XSHM_LIBS], [-lXext])],
    [], [$X_LIBS])],
  [], [#include <X11/Xlib.h>])

# Check for XTest extension
AC_CHECK_HEADER([X11/extensions/XTest.h],
  [AC_CHECK_LIB([Xtst], [XTestQueryExtension],
    [AC_DEFINE([VINO_HAVE_XTEST], [], [Defined if the XTEST X extension is present])
      AC_SUBST([XTEST_LIBS], [-lXtst])],
    [], [$X_LIBS])])

# Check for XKB extension
AC_CHECK_HEADER([X11/XKBlib.h],
  [AC_CHECK_LIB([X11], [XkbQueryExtension],
    [AC_DEFINE([VINO_HAVE_XKB], [], [Defined if the XKB X extension is present])],
  [], [$X_LIBS])])

AC_CHECK_HEADERS([netinet/in.h sys/time.h fcntl.h unistd.h sys/socket.h ifaddrs.h])

# Check for getifaddrs
AM_CONDITIONAL([VINO_HAVE_SELF_IFADDRS], [test "x$ac_cv_header_ifaddrs_h" != "xyes"])

# RFC2553 introduce sockaddr_storage as ifa_addr member in ifaddrs structure
# Not all distros follow this.
AS_IF([test "x$ac_cv_header_ifaddrs_h" = "xyes"],
  [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
    #include <ifaddrs.h>
    #include <net/if.h>
    ]], [[
    struct ifaddrs *myaddrs;
    getifaddrs (&myaddrs);
    if (myaddrs->ifa_addr->ss_family == AF_INET) {
    }
    ]])],
    [have_sockaddr_storage=yes],
    [have_sockaddr_storage=no])],
  [AS_IF([test "x$have_sockaddr_storage" = "xyes"],
    [AC_DEFINE([RFC2553], [], [Define if RFC2553 is followed])])])

AC_C_BIGENDIAN

AC_CHECK_LIB([resolv], [herror])
AC_SEARCH_LIBS([strerror],[cposix])
AC_CHECK_FUNCS([gettimeofday])

# Check for IPv6 support
AC_MSG_CHECKING([checking for IPv6 support])
AC_ARG_ENABLE([ipv6],
  [AS_HELP_STRING([--disable-ipv6],
    [ignore the presence of IPv6 support and disable it])],
    [],
    [enable_ipv6=yes])

AS_IF([test "x$enable_ipv6" != "xno"],
  [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
    #include <sys/types.h>
    #include <sys/socket.h>
    ]], [[
      socket(AF_INET6, SOCK_STREAM, 0);
    ]])],
    [have_ipv6=yes],
    [have_ipv6=no])])

AS_IF([test "x$have_ipv6" = "xyes"],
  [AC_DEFINE([VINO_ENABLE_IPV6], [], [Define to enable IPv6 support])],
  [AS_IF([test "x$enable_ipv6" = "xyes"],
    [AC_MSG_ERROR([IPv6 support requested but not found])])])
AC_MSG_RESULT([$have_ipv6])

GLIB_GSETTINGS

AC_CONFIG_FILES([
Makefile
server/Makefile
server/libvncserver/Makefile
server/libvncserver/ifaddr/Makefile
server/libvncserver/rfb/Makefile
server/miniupnp/Makefile
server/smclient/Makefile
capplet/Makefile
capplet/vino-preferences.desktop.in
tools/Makefile
docs/Makefile
po/Makefile.in
])

AC_OUTPUT
