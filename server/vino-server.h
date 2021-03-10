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

#ifndef __VINO_SERVER_H__
#define __VINO_SERVER_H__

#include <glib-object.h>
#include <gdk/gdk.h>

#include "vino-types.h"

G_BEGIN_DECLS

#define VINO_SERVER_DEFAULT_PORT  5900
#define VINO_SERVER_MIN_PORT      5000
#define VINO_SERVER_MAX_PORT      50000
#define VINO_SERVER_VALID_PORT(p) ((p) > VINO_SERVER_MIN_PORT && (p) < VINO_SERVER_MAX_PORT)

#define VINO_TYPE_SERVER         (vino_server_get_type ())
#define VINO_SERVER(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), VINO_TYPE_SERVER, VinoServer))
#define VINO_SERVER_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), VINO_TYPE_SERVER, VinoServerClass))
#define VINO_IS_SERVER(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), VINO_TYPE_SERVER))
#define VINO_IS_SERVER_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), VINO_TYPE_SERVER))
#define VINO_SERVER_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), VINO_TYPE_SERVER, VinoServerClass))

typedef struct _VinoServerClass   VinoServerClass;
typedef struct _VinoServerPrivate VinoServerPrivate;
typedef struct _VinoClient        VinoClient;

typedef enum
{
  VINO_AUTH_INVALID = 0,
  VINO_AUTH_NONE    = 1 << 0,
  VINO_AUTH_VNC     = 1 << 1
} VinoAuthMethod;

struct _VinoServer
{
  GObject            base;

  VinoServerPrivate *priv;
};

struct _VinoServerClass
{
  GObjectClass base_class;
};

GType                vino_server_get_type                 (void) G_GNUC_CONST;

VinoServer          *vino_server_new                      (GdkScreen      *screen,
							   gboolean        view_only);
GdkScreen           *vino_server_get_screen               (VinoServer     *server);
gboolean             vino_server_get_has_clients          (VinoServer     *server);
void                 vino_server_set_on_hold              (VinoServer     *server,
							   gboolean        on_hold);
gboolean             vino_server_get_on_hold              (VinoServer     *server);
void                 vino_server_set_prompt_enabled       (VinoServer     *server,
							   gboolean        enable_prompt);
gboolean             vino_server_get_prompt_enabled       (VinoServer     *server);
void                 vino_server_set_view_only            (VinoServer     *server,
							   gboolean        view_only);
gboolean             vino_server_get_view_only            (VinoServer     *server);
void                 vino_server_set_display_status_icon  (VinoServer     *server,
                                                           gboolean        display_status_icon);
void                 vino_server_set_use_dbus_listener    (VinoServer     *server,
                                                           gboolean        use_dbus_listener);
gboolean             vino_server_get_use_alternative_port (VinoServer     *server);
void                 vino_server_set_use_alternative_port (VinoServer     *server,
							   gboolean        use_alternative_port);
int                  vino_server_get_alternative_port     (VinoServer     *server);
void                 vino_server_set_alternative_port     (VinoServer     *server,
							   int             alternative_port);
int                  vino_server_get_port                 (VinoServer     *server);
gchar *              vino_server_get_external_ip          (VinoServer     *server);
guint16              vino_server_get_external_port        (VinoServer     *server);

void                 vino_server_set_network_interface    (VinoServer     *server,
                                                           const char     *network_interface);
const char          *vino_server_get_network_interface    (VinoServer     *server);
void                 vino_server_set_require_encryption   (VinoServer     *server,
							   gboolean        require_encryption);
gboolean             vino_server_get_require_encryption   (VinoServer     *server);
void                 vino_server_set_auth_methods         (VinoServer     *server,
							   VinoAuthMethod  auth_methods);
VinoAuthMethod       vino_server_get_auth_methods         (VinoServer     *server);
void                 vino_server_set_vnc_password         (VinoServer     *server,
							   const char     *vnc_password);
const char          *vino_server_get_vnc_password         (VinoServer     *server);
void                 vino_server_set_lock_screen          (VinoServer     *server,
							   gboolean        lock_screen);
gboolean             vino_server_get_lock_screen          (VinoServer     *server);

void                 vino_server_set_disable_background   (VinoServer     *server,
                                                           gboolean        disable_background);
gboolean             vino_server_get_disable_background   (VinoServer     *server);

void                 vino_server_set_use_upnp             (VinoServer     *server,
                                                           gboolean        use_upnp);
gboolean             vino_server_get_use_upnp             (VinoServer     *server);

void                 vino_server_set_disable_xdamage      (VinoServer     *server,
                                                           gboolean        disable_xdamage);
gboolean             vino_server_get_disable_xdamage      (VinoServer     *server);
gboolean             vino_server_get_notify_on_connect    (VinoServer     *server);
void                 vino_server_set_reject_incoming      (VinoServer     *server,
                                                           gboolean        reject);
gboolean             vino_server_get_reject_incoming      (VinoServer     *server);
void                 vino_server_set_accepted_hosts       (VinoServer     *server,
							   const char     *accepted_hosts);

#include "vino-status-icon.h"
VinoStatusIcon      *vino_server_get_status_icon          (VinoServer      *server);

const char          *vino_client_get_hostname (VinoClient *client);
void                 vino_client_disconnect   (VinoClient *client);

G_END_DECLS

#endif /* __VINO_SERVER_H__ */
