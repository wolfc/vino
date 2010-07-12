/*
 * Copyright (C) 2003 Sun Microsystems, Inc.
 * Copyright (C) 2006 Jonh Wendell <wendell@bani.com.br>
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
 * Authors:
 *      Mark McLoughlin <mark@skynet.ie>
 *      Jonh Wendell <wendell@bani.com.br>
 *      Ryan Lortie <desrt@desrt.ca>
 */

#include "config.h"

#include "vino-keyring.h"

#ifdef VINO_ENABLE_KEYRING

/* TODO: canhas async? */

#include <gnome-keyring.h>

char *
vino_keyring_get_password (void)
{
  GnomeKeyringNetworkPasswordData *found_item;
  GnomeKeyringResult               result;
  GList                           *matches;
  char                            *password;

  matches = NULL;

  result = gnome_keyring_find_network_password_sync (
                NULL,           /* user     */
                NULL,           /* domain   */
                "vino.local",   /* server   */
                NULL,           /* object   */
                "rfb",          /* protocol */
                "vnc-password", /* authtype */
                5900,           /* port     */
                &matches);

  if (result != GNOME_KEYRING_RESULT_OK || matches == NULL || matches->data == NULL)
    return NULL;

  found_item = (GnomeKeyringNetworkPasswordData *) matches->data;

  password = g_strdup (found_item->password);

  gnome_keyring_network_password_list_free (matches);

  return password;
}

gboolean
vino_keyring_set_password (const char *password)
{
  GnomeKeyringResult result;
  guint32            item_id;

  result = gnome_keyring_set_network_password_sync (
                NULL,           /* default keyring */
                NULL,           /* user            */
                NULL,           /* domain          */
                "vino.local",   /* server          */
                NULL,           /* object          */
                "rfb",          /* protocol        */
                "vnc-password", /* authtype        */
                5900,           /* port            */
                password,       /* password        */
                &item_id);

  return result == GNOME_KEYRING_RESULT_OK;
}

#else

gchar *
vino_keyring_get_password (void)
{
  return NULL;
}

gboolean
vino_keyring_set_password (const gchar *password)
{
  return FALSE;
}

#endif
