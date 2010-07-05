/*
 * Copyright (C) 2003 Sun Microsystems, Inc.
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
 *      Mark McLoughlin <mark@skynet.ie>
 */

#include "config.h"

#include "vino-prefs.h"


#include <string.h>
#include <glib/gstdio.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>

#include "vino-prefs.h"
#include "vino-util.h"
#include "vino-mdns.h"
#include "vino-status-icon.h"

#define VINO_PREFS_LOCKFILE               "vino-server.lock"

static GSettings *background_settings = NULL;
static GSettings *vino_settings       = NULL;
static gboolean   force_view_only;

static void
vino_prefs_restart_mdns (VinoServer *server,
                         gpointer    data)
{
  vino_mdns_stop ();
  vino_mdns_add_service ("_rfb._tcp", vino_server_get_port (server));
  vino_mdns_start (vino_server_get_network_interface (server));
}

static gboolean
get_inverted_boolean (GValue   *value,
                      GVariant *variant,
                      gpointer  user_data)
{
  g_value_set_boolean (value, !g_variant_get_boolean (variant));
  return TRUE;
}

void
vino_prefs_create_server (GdkScreen *screen)
{
  VinoServer     *server;
  VinoStatusIcon *icon;

  /* Start the server 'on-hold' until all the settings are in, then go. */
  server = g_object_new (VINO_TYPE_SERVER,
                         "view-only", force_view_only,
                         "on-hold", TRUE,
                         "screen", screen,
                         NULL);
  icon = vino_server_get_status_icon (server);

  g_settings_bind (vino_settings, "prompt-enabled",
                   server, "prompt-enabled", G_SETTINGS_BIND_GET);

  if (!force_view_only)
    g_settings_bind (vino_settings, "view-only",
                     server, "view-only", G_SETTINGS_BIND_GET);

  g_settings_bind (vino_settings, "network-interface",
                   server, "network-interface", G_SETTINGS_BIND_GET);
  g_settings_bind (vino_settings, "use-alternative-port",
                   server, "use-alternative-port", G_SETTINGS_BIND_GET);
  g_settings_bind (vino_settings, "alternative-port",
                   server, "alternative-port", G_SETTINGS_BIND_GET);
  g_settings_bind (vino_settings, "authentication-methods",
                   server, "auth-methods", G_SETTINGS_BIND_GET);
  g_settings_bind (vino_settings, "require-encryption",
                   server, "require-encryption", G_SETTINGS_BIND_GET);
  g_settings_bind (vino_settings, "require-encryption",
                   server, "require-encryption", G_SETTINGS_BIND_GET);
  g_settings_bind (vino_settings, "vnc-password",
                   server, "vnc-password", G_SETTINGS_BIND_GET);
  g_settings_bind (vino_settings, "lock-screen-on-disconnect",
                   server, "lock-screen", G_SETTINGS_BIND_GET);
  g_settings_bind (vino_settings, "disable-background",
                   server, "disable-background", G_SETTINGS_BIND_GET);
  g_settings_bind (vino_settings, "use-upnp",
                   server, "use-upnp", G_SETTINGS_BIND_GET);
  g_settings_bind (vino_settings, "disable-xdamage",
                   server, "disable-xdamage", G_SETTINGS_BIND_GET);
  g_settings_bind (vino_settings, "icon-visibility",
                   icon, "visibility", G_SETTINGS_BIND_GET);

  /* bind this one last */
  g_settings_bind_with_mapping (vino_settings, "enabled",
                                server, "on-hold",
                                G_SETTINGS_BIND_GET,
                                get_inverted_boolean,
                                NULL, NULL, NULL);

  g_signal_connect (server, "notify::alternative-port", G_CALLBACK (vino_prefs_restart_mdns), NULL);
  g_signal_connect (server, "notify::use-alternative-port", G_CALLBACK(vino_prefs_restart_mdns), NULL);
  g_signal_connect (server, "notify::network-interface", G_CALLBACK (vino_prefs_restart_mdns), NULL);
}

static void
vino_prefs_restore_background (void)
{
  if (g_settings_get_boolean (vino_settings, "disable-background"))
    g_settings_set_boolean (background_settings, "draw-background", TRUE);
}

static gchar *
vino_prefs_lock_filename (void)
{
  gchar *dir;

  dir = g_build_filename (g_get_user_data_dir (),
			 "vino",
			  NULL);
  if (!g_file_test (dir, G_FILE_TEST_EXISTS))
    g_mkdir_with_parents (dir, 0755);

  g_free (dir);

  return g_build_filename (g_get_user_data_dir (),
			   "vino",
			    VINO_PREFS_LOCKFILE,
			    NULL);
}

static gboolean
vino_prefs_lock (void)
{
  gchar    *lockfile;
  gboolean  res;

  res = FALSE;
  lockfile = vino_prefs_lock_filename ();

  if (g_file_test (lockfile, G_FILE_TEST_EXISTS))
    {
      dprintf (PREFS, "WARNING: The lock file (%s) already exists\n", lockfile);
    }
  else
    {
      g_creat (lockfile, 0644);
      res = TRUE;
    }

  g_free (lockfile);
  return res;
}

static gboolean
vino_prefs_unlock (void)
{
  gchar    *lockfile;
  gboolean  res;

  res = FALSE;
  lockfile = vino_prefs_lock_filename ();

  if (!g_file_test (lockfile, G_FILE_TEST_EXISTS))
    {
      dprintf (PREFS, "WARNING: Lock file (%s) not found!\n", lockfile);
    }
  else
    {
      g_unlink (lockfile);
      res = TRUE;
    }

  g_free (lockfile);
  return res;
}

static void
vino_prefs_sighandler (int sig)
{
  g_message (_("Received signal %d, exiting...\n"), sig);
  vino_prefs_restore_background ();
  vino_mdns_shutdown ();
  vino_prefs_shutdown ();
  exit (0);
}

void
vino_prefs_init (gboolean view_only)
{
  background_settings = g_settings_new ("org.gnome.desktop.background");
  vino_settings = g_settings_new ("org.gnome.Vino");

  signal (SIGINT,  vino_prefs_sighandler); /* Ctrl+C */
  signal (SIGQUIT, vino_prefs_sighandler);
  signal (SIGTERM, vino_prefs_sighandler); /* kill -15 */
  signal (SIGSEGV, vino_prefs_sighandler); /* Segmentation fault */

  if(!vino_prefs_lock ())
    vino_prefs_restore_background ();

  force_view_only = view_only;
}

void
vino_prefs_shutdown (void)
{
  g_object_unref (background_settings);
  g_object_unref (vino_settings);
  background_settings = NULL;
  vino_settings = NULL;

  vino_prefs_unlock ();
}
