/*
 * Copyright (C) 2004-2006 William Jon McCann <mccann@jhu.edu>
 * Copyright (C) 2006 Jonh Wendell <wendell@bani.com.br>
 * Copyright (C) 2007 Mark McLoughlin <markmc@skynet.ie>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 *
 * Authors:
 *      William Jon McCann <mccann@jhu.edu>
 *      Jonh Wendell <wendell@bani.com.br>
 *      Mark McLoughlin <mark@skynet.ie>
 *
 * Code taken from gnome-screensaver/src/gs-listener-dbus.c
 */

#include "config.h"

#include "vino-dbus-listener.h"
#include "vino-dbus.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "vino-dbus-error.h"

#ifdef HAVE_TELEPATHY_GLIB
#include "vino-tube-servers-manager.h"
#endif

#include "vino-util.h"
#include "vino-mdns.h"
#ifdef VINO_ENABLE_HTTP_SERVER
#include "vino-http.h"
#endif

#ifdef HAVE_IFADDRS_H
#include <ifaddrs.h>
#else
#include "libvncserver/ifaddr/ifaddrs.h"
#endif

#ifdef RFC2553
#define ADDR_FAMILY_MEMBER ss_family
#else
#define ADDR_FAMILY_MEMBER sa_family
#endif

#define VINO_DBUS_BUS_NAME  "org.gnome.Vino"


struct _VinoDBusListener
{
  GObject  parent_instance;

  GDBusConnection *connection;
  gchar           *path;
  gint             screen;

  VinoServer      *server;
};

typedef GObjectClass VinoDBusListenerClass;

static GType vino_dbus_listener_get_type (void);
G_DEFINE_TYPE (VinoDBusListener, vino_dbus_listener, G_TYPE_OBJECT)

static char *
get_local_hostname (VinoDBusListener *listener)
{
  char                *retval, buf[INET6_ADDRSTRLEN];
  struct ifaddrs      *myaddrs, *ifa; 
  void                *sin;
  const char          *server_iface;
  GHashTable          *ipv4, *ipv6;
  GHashTableIter      iter;
  gpointer            key, value;

  retval = NULL;
  server_iface = vino_server_get_network_interface (listener->server);
  ipv4 = g_hash_table_new_full (g_str_hash, g_str_equal, NULL, g_free);
  ipv6 = g_hash_table_new_full (g_str_hash, g_str_equal, NULL, g_free);

  getifaddrs (&myaddrs);
  for (ifa = myaddrs; ifa != NULL; ifa = ifa->ifa_next)
    {
      if (ifa->ifa_addr == NULL || ifa->ifa_name == NULL || (ifa->ifa_flags & IFF_UP) == 0)
	continue;

      switch (ifa->ifa_addr->ADDR_FAMILY_MEMBER)
	{
	  case AF_INET:
	    sin = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
	    inet_ntop (AF_INET, sin, buf, INET6_ADDRSTRLEN);
	    g_hash_table_insert (ipv4,
				 ifa->ifa_name,
				 g_strdup (buf));
	    break;

	  case AF_INET6:
	    sin = &((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
	    inet_ntop (AF_INET6, sin, buf, INET6_ADDRSTRLEN);
	    g_hash_table_insert (ipv6,
				 ifa->ifa_name,
				 g_strdup (buf));
	    break;
	  default: continue;
	}
    }

  if (server_iface && server_iface[0] != '\0')
    {
      if ((retval = g_strdup (g_hash_table_lookup (ipv4, server_iface))))
	goto the_end;
      if ((retval = g_strdup (g_hash_table_lookup (ipv6, server_iface))))
	goto the_end;
    }

  g_hash_table_iter_init (&iter, ipv4);
  while (g_hash_table_iter_next (&iter, &key, &value))
    {
      if (strncmp (key, "lo", 2) == 0)
	continue;
      retval = g_strdup (value);
      goto the_end;
    }

  g_hash_table_iter_init (&iter, ipv6);
  while (g_hash_table_iter_next (&iter, &key, &value))
    {
      if (strncmp (key, "lo", 2) == 0)
	continue;
      retval = g_strdup (value);
      goto the_end;
    }

  if ((retval = g_strdup (g_hash_table_lookup (ipv4, "lo"))))
    goto the_end;
  if ((retval = g_strdup (g_hash_table_lookup (ipv6, "lo"))))
    goto the_end;

  the_end:
  freeifaddrs (myaddrs); 
  g_hash_table_destroy (ipv4);
  g_hash_table_destroy (ipv6);

  return retval;
}

#ifdef HAVE_TELEPATHY_GLIB

static void
vino_dbus_listener_dispose (GObject *object)
{
  VinoDBusListener *listener = VINO_DBUS_LISTENER (object);

  if (listener->priv->manager != NULL)
    {
      g_object_unref (listener->priv->manager);
      listener->priv->manager = NULL;
    }

  if (G_OBJECT_CLASS (vino_dbus_listener_parent_class)->dispose)
    G_OBJECT_CLASS (vino_dbus_listener_parent_class)->dispose (object);
}
#endif

static void
vino_dbus_listener_init (VinoDBusListener *listener)
{
#ifdef HAVE_TELEPATHY_GLIB
  listener->priv->manager = vino_tube_servers_manager_new ();
#endif
}

static void
vino_dbus_listener_class_init (VinoDBusListenerClass *klass)
{
#ifdef HAVE_TELEPATHY_GLIB
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->dispose = vino_dbus_listener_dispose;
#endif
}

static guint16
vino_dbus_listener_get_port (VinoDBusListener *listener)
{
#ifdef VINO_ENABLE_HTTP_SERVER
  return vino_get_http_server_port (listener->server);
#else
  return vino_server_get_port (listener->server);
#endif
}

static GVariant *
vino_dbus_listener_get_property (GDBusConnection  *connection,
                                 const gchar      *sender,
                                 const gchar      *object_path,
                                 const gchar      *interface_name,
                                 const gchar      *property_name,
                                 GError          **error,
                                 gpointer          user_data)
{
  VinoDBusListener *listener = user_data;

  if (strcmp (property_name, "Host") == 0)
    {
      gchar *local_hostname;
      GVariant *result;

      local_hostname = get_local_hostname (listener);
      if (local_hostname)
        result = g_variant_new_string (local_hostname);
      else
        result = g_variant_new_string ("");
      g_free (local_hostname);

      return result;
    }

  else if (strcmp (property_name, "Port") == 0)
    return g_variant_new_uint16 (vino_dbus_listener_get_port (listener));

  else if (strcmp (property_name, "ExternalHost") == 0)
    {
      gchar *external_ip;
      GVariant *result;

      external_ip = vino_server_get_external_ip (listener->server);
      if (external_ip)
        result = g_variant_new_string (external_ip);
      else
        result = g_variant_new_string ("");
      g_free (external_ip);

      return result;
    }

  else if (strcmp (property_name, "ExternalPort") == 0)
    return g_variant_new_uint16 (vino_server_get_external_port (listener->server));

  else if (strcmp (property_name, "AvahiHost") == 0)
    return g_variant_new_string ("aaa");

  else
    g_assert_not_reached ();
}


VinoDBusListener *
vino_dbus_listener_new (gint screen)
{
  static const GDBusInterfaceVTable vtable = {
    NULL, vino_dbus_listener_get_property
  };
  VinoDBusListener *listener;

  listener = g_object_new (vino_dbus_listener_get_type (), NULL);
  listener->connection = g_bus_get_sync (G_BUS_TYPE_SESSION, NULL, NULL);
  listener->path = g_strdup_printf ("%s%d",
                                    ORG_GNOME_VINO_SCREEN_PATH_PREFIX,
                                    screen);
  listener->screen = screen;

  g_dbus_connection_register_object (listener->connection, listener->path,
                                     ORG_GNOME_VINO_SCREEN_INTERFACE,
                                     &vtable, listener, NULL, NULL);

  return listener;
}


void
vino_dbus_listener_set_server (VinoDBusListener *listener,
                               VinoServer       *server)
{
  g_assert (listener->server == NULL);
  g_assert_cmpint (listener->screen, ==,
                   gdk_screen_get_number (vino_server_get_screen (server)));

  listener->server = server;

  /* XXX: emit property change signal, watch for more changes
   */
}

gboolean
vino_dbus_request_name (void)
{
  g_application_new ("org.gnome.Vino", 0, NULL);
  return TRUE;
}
