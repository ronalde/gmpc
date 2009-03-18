/* Gnome Music Player Client (GMPC)
 * Copyright (C) 2004-2009 Qball Cow <qball@sarine.nl>
 * Project homepage: http://gmpc.wikia.com/
 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef __GMPC_TEST_PLUGIN_H__
#define __GMPC_TEST_PLUGIN_H__

#include <glib.h>
#include <glib-object.h>
#include <gmpc-plugin.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gmpc_easy_download.h>

G_BEGIN_DECLS


#define GMPC_TYPE_TEST_PLUGIN (gmpc_test_plugin_get_type ())
#define GMPC_TEST_PLUGIN(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), GMPC_TYPE_TEST_PLUGIN, GmpcTestPlugin))
#define GMPC_TEST_PLUGIN_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), GMPC_TYPE_TEST_PLUGIN, GmpcTestPluginClass))
#define GMPC_IS_TEST_PLUGIN(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GMPC_TYPE_TEST_PLUGIN))
#define GMPC_IS_TEST_PLUGIN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GMPC_TYPE_TEST_PLUGIN))
#define GMPC_TEST_PLUGIN_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), GMPC_TYPE_TEST_PLUGIN, GmpcTestPluginClass))

typedef struct _GmpcTestPlugin GmpcTestPlugin;
typedef struct _GmpcTestPluginClass GmpcTestPluginClass;
typedef struct _GmpcTestPluginPrivate GmpcTestPluginPrivate;

struct _GmpcTestPlugin {
	GmpcPluginBase parent_instance;
	GmpcTestPluginPrivate * priv;
};

struct _GmpcTestPluginClass {
	GmpcPluginBaseClass parent_class;
};


void gmpc_test_plugin_image_downloaded (GmpcTestPlugin* self, const GEADAsyncHandler* handle, GEADStatus status);
void gmpc_test_plugin_callback (GmpcTestPlugin* self, GList* list);
gboolean gmpc_test_plugin_window_delete_event (GmpcTestPlugin* self, GtkWindow* win);
void gmpc_test_plugin_menu_activated (GmpcTestPlugin* self, GtkMenuItem* item);
GmpcTestPlugin* gmpc_test_plugin_construct (GType object_type);
GmpcTestPlugin* gmpc_test_plugin_new (void);
GType gmpc_test_plugin_get_type (void);

static const gint GMPC_TEST_PLUGIN_version[] = {0, 0, 2};

G_END_DECLS

#endif
