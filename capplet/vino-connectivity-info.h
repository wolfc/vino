#include <glib.h>

typedef struct _VinoConnectivityInfo VinoConnectivityInfo;

VinoConnectivityInfo * vino_connectivity_info_new (gint screen_number);
gboolean vino_connectivity_info_get (VinoConnectivityInfo  *info,
                                     gchar                **internal_host,
                                     guint16               *internal_port,
                                     gchar                **external_host,
                                     guint16               *external_port,
                                     gchar                **avahi_host);
void vino_connectivity_info_check (VinoConnectivityInfo *info);
