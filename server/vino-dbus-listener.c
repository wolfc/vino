/*
 * Copyright © 2010 Codethink Limited
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
 * Author: Ryan Lortie <desrt@desrt.ca>
 */

#include "config.h"

#include "vino-dbus-listener.h"
#include "vino-dbus.h"

#ifdef VINO_HAVE_TELEPATHY_GLUB
#include "vino-tube-servers-manager.h"
#endif

#include "vino-util.h"
#ifdef VINO_ENABLE_HTTP_SERVER
#include "vino-http.h"
#endif

#include "vino-mdns.h"

struct _VinoDBusListener
{
  GObject  parent_instance;

  GDBusConnection *connection;
  gchar           *path;
  gint             screen;

  VinoServer      *server;

#ifdef VINO_HAVE_TELEPATHY_GLUB
  VinoTubeServersManager *manager;
#endif
};

typedef GObjectClass VinoDBusListenerClass;

static GType vino_dbus_listener_get_type (void);

G_DEFINE_TYPE (VinoDBusListener, vino_dbus_listener, G_TYPE_OBJECT)

static void
vino_dbus_listener_finalize (GObject *object)
{
  VinoDBusListener *listener = (VinoDBusListener *) object;

  g_object_unref (listener->connection);
  g_free (listener->path);
  if (listener->server)
    g_object_unref (listener->server);


#ifdef VINO_HAVE_TELEPATHY_GLUB
  if (listener->manager != NULL)
    {
      g_object_unref (listener->manager);
      listener->manager = NULL;
    }
#endif

  G_OBJECT_CLASS (vino_dbus_listener_parent_class)
    ->finalize (object);
}

static void
vino_dbus_listener_init (VinoDBusListener *listener)
{
#ifdef VINO_HAVE_TELEPATHY_GLUB
  listener->manager = vino_tube_servers_manager_new ();
#endif
}

static void
vino_dbus_listener_class_init (GObjectClass *class)
{
  class->finalize = vino_dbus_listener_finalize;
}

static guint16
vino_dbus_listener_get_port (VinoDBusListener *listener)
{
#ifdef VINO_ENABLE_HTTP_SERVER
  return vino_get_http_server_port ();
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

  if (listener->server == NULL)
    /* We (reasonably) assume that nobody will be doing Vino property
     * queries during the extremely short period of time between
     * connecting to the bus and acquiring our well-known name.
     *
     * As soon as the well-known name is acquired, GDBus invokes our
     * name_acquired function (in vino-main.c) and that blocks until all
     * of the servers have been setup.  That completes before any more
     * messages (ie: to the well-known name) can be dispatched.
     *
     * That means that the only possibility that we see property queries
     * here without listener->server being set is in the case that the
     * property is being queried against our unique name (which we
     * assume won't happen).
     */
    {
      g_warning ("Somebody queried vino server properties "
                 "(%s, property %s) before unique name was acquired.",
                 object_path, property_name);
      return NULL;
    }

  if (strcmp (property_name, "Host") == 0)
    {
      gchar *local_hostname;
      const gchar *iface;
      GVariant *result;

      iface = vino_server_get_network_interface (listener->server);
      local_hostname = vino_util_get_local_hostname (iface);
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
    return g_variant_new_string (vino_mdns_get_hostname());

  else
    g_assert_not_reached ();
}

void
vino_dbus_listener_set_server (VinoDBusListener *listener,
                               VinoServer       *server)
{
  g_return_if_fail (listener->server == NULL);
  g_return_if_fail (VINO_IS_SERVER (server));
  g_return_if_fail (listener->screen ==
                    gdk_screen_get_number (vino_server_get_screen (server)));

  listener->server = g_object_ref (server);

  /* We need not notify for property changes here since we assume that
   * nobody will have checked properties before now (see large comment
   * above in get_property()).
   */
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
