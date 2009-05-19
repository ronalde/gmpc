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

#include "gmpc-metadata-browser2.h"
#include <gtktransition.h>
#include <config.h>
#include <glib/gi18n-lib.h>
#include <gdk/gdk.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <gmpc-mpddata-model.h>
#include <plugin.h>
#include <config1.h>
#include <libmpd/libmpd.h>
#include <main.h>
#include <libmpd/libmpdclient.h>
#include <misc.h>
#include <gmpc-metaimage.h>
#include <metadata.h>
#include <gmpc-meta-text-view.h>
#include <gmpc-stats-label.h>
#include <gmpc-connection.h>
#include "gmpc-favorites.h"
#include "gmpc-song-links.h"




struct _GmpcWidgetMorePrivate {
	GtkAlignment* ali;
	gint expand_state;
	GtkButton* expand_button;
	gint max_height;
	GtkEventBox* eventbox;
	GtkWidget* pchild;
};

#define GMPC_WIDGET_MORE_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), GMPC_WIDGET_TYPE_MORE, GmpcWidgetMorePrivate))
enum  {
	GMPC_WIDGET_MORE_DUMMY_PROPERTY
};
static void gmpc_widget_more_expand (GmpcWidgetMore* self, GtkButton* but);
static void gmpc_widget_more_size_changed (GmpcWidgetMore* self, GtkWidget* child, const GdkRectangle* alloc);
static void gmpc_widget_more_bg_style_changed (GmpcWidgetMore* self, GtkWidget* frame, GtkStyle* style);
static void _gmpc_widget_more_bg_style_changed_gtk_widget_style_set (GmpcWidgetMore* _sender, GtkStyle* previous_style, gpointer self);
static void _gmpc_widget_more_expand_gtk_button_clicked (GtkButton* _sender, gpointer self);
static void _gmpc_widget_more_size_changed_gtk_widget_size_allocate (GtkWidget* _sender, const GdkRectangle* allocation, gpointer self);
static GmpcWidgetMore* gmpc_widget_more_construct (GType object_type, const char* markup, GtkWidget* child);
static GmpcWidgetMore* gmpc_widget_more_new (const char* markup, GtkWidget* child);
static gpointer gmpc_widget_more_parent_class = NULL;
static void gmpc_widget_more_finalize (GObject* obj);
struct _GmpcMetadataBrowserPrivate {
	GtkTreeRowReference* rref;
	GtkPaned* paned;
	GtkBox* browser_box;
	GtkTreeView* tree_artist;
	GmpcMpdDataModel* model_artist;
	GtkTreeModelFilter* model_filter_artist;
	GtkEntry* artist_filter_entry;
	GtkTreeView* tree_album;
	GmpcMpdDataModel* model_albums;
	GtkTreeModelFilter* model_filter_album;
	GtkEntry* album_filter_entry;
	GtkTreeView* tree_songs;
	GmpcMpdDataModel* model_songs;
	GtkScrolledWindow* metadata_sw;
	GtkEventBox* metadata_box;
	gboolean selected;
};

#define GMPC_METADATA_BROWSER_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), GMPC_TYPE_METADATA_BROWSER, GmpcMetadataBrowserPrivate))
enum  {
	GMPC_METADATA_BROWSER_DUMMY_PROPERTY
};
static gint* gmpc_metadata_browser_real_get_version (GmpcPluginBase* base, int* result_length1);
static const char* gmpc_metadata_browser_real_get_name (GmpcPluginBase* base);
static void gmpc_metadata_browser_real_save_yourself (GmpcPluginBase* base);
static void gmpc_metadata_browser_browser_bg_style_changed (GmpcMetadataBrowser* self, GtkScrolledWindow* bg, GtkStyle* style);
static gboolean gmpc_metadata_browser_browser_button_press_event (GmpcMetadataBrowser* self, GtkTreeView* tree, const GdkEventButton* event);
static gboolean gmpc_metadata_browser_visible_func_artist (GmpcMetadataBrowser* self, GtkTreeModel* model, GtkTreeIter* iter);
static gboolean gmpc_metadata_browser_visible_func_album (GmpcMetadataBrowser* self, GtkTreeModel* model, GtkTreeIter* iter);
static gboolean gmpc_metadata_browser_browser_artist_key_press_event (GmpcMetadataBrowser* self, GtkWidget* widget, const GdkEventKey* event);
static gboolean gmpc_metadata_browser_browser_album_key_press_event (GmpcMetadataBrowser* self, GtkWidget* widget, const GdkEventKey* event);
static void gmpc_metadata_browser_browser_artist_entry_changed (GmpcMetadataBrowser* self, GtkEntry* entry);
static void gmpc_metadata_browser_browser_album_entry_changed (GmpcMetadataBrowser* self, GtkEntry* entry);
static void _gmpc_metadata_browser_browser_artist_entry_changed_gtk_editable_changed (GtkEntry* _sender, gpointer self);
static gboolean _gmpc_metadata_browser_visible_func_artist_gtk_tree_model_filter_visible_func (GtkTreeModel* model, GtkTreeIter* iter, gpointer self);
static gboolean _gmpc_metadata_browser_browser_button_press_event_gtk_widget_button_press_event (GtkTreeView* _sender, const GdkEventButton* event, gpointer self);
static gboolean _gmpc_metadata_browser_browser_artist_key_press_event_gtk_widget_key_press_event (GtkTreeView* _sender, const GdkEventKey* event, gpointer self);
static void _gmpc_metadata_browser_browser_artist_changed_gtk_tree_selection_changed (GtkTreeSelection* _sender, gpointer self);
static void _gmpc_metadata_browser_browser_album_entry_changed_gtk_editable_changed (GtkEntry* _sender, gpointer self);
static gboolean _gmpc_metadata_browser_visible_func_album_gtk_tree_model_filter_visible_func (GtkTreeModel* model, GtkTreeIter* iter, gpointer self);
static gboolean _gmpc_metadata_browser_browser_album_key_press_event_gtk_widget_key_press_event (GtkTreeView* _sender, const GdkEventKey* event, gpointer self);
static void _gmpc_metadata_browser_browser_album_changed_gtk_tree_selection_changed (GtkTreeSelection* _sender, gpointer self);
static void _gmpc_metadata_browser_browser_songs_changed_gtk_tree_selection_changed (GtkTreeSelection* _sender, gpointer self);
static void _gmpc_metadata_browser_browser_bg_style_changed_gtk_widget_style_set (GtkScrolledWindow* _sender, GtkStyle* previous_style, gpointer self);
static void gmpc_metadata_browser_browser_init (GmpcMetadataBrowser* self);
static void gmpc_metadata_browser_reload_browsers (GmpcMetadataBrowser* self);
static char* gmpc_metadata_browser_browser_get_selected_artist (GmpcMetadataBrowser* self);
static char* gmpc_metadata_browser_browser_get_selected_album (GmpcMetadataBrowser* self);
static mpd_Song* gmpc_metadata_browser_browser_get_selected_song (GmpcMetadataBrowser* self);
static void gmpc_metadata_browser_browser_artist_changed (GmpcMetadataBrowser* self, GtkTreeSelection* sel);
static void gmpc_metadata_browser_browser_album_changed (GmpcMetadataBrowser* self, GtkTreeSelection* album_sel);
static void gmpc_metadata_browser_browser_songs_changed (GmpcMetadataBrowser* self, GtkTreeSelection* song_sel);
static void gmpc_metadata_browser_play_selected_song (GmpcMetadataBrowser* self, GtkButton* button);
static void gmpc_metadata_browser_add_selected_song (GmpcMetadataBrowser* self, GtkButton* button);
static void gmpc_metadata_browser_replace_selected_song (GmpcMetadataBrowser* self, GtkButton* button);
static void gmpc_metadata_browser_metadata_box_clear (GmpcMetadataBrowser* self);
static void _gmpc_metadata_browser_play_selected_song_gtk_button_clicked (GtkButton* _sender, gpointer self);
static void _gmpc_metadata_browser_add_selected_song_gtk_button_clicked (GtkButton* _sender, gpointer self);
static void _gmpc_metadata_browser_replace_selected_song_gtk_button_clicked (GtkButton* _sender, gpointer self);
static void gmpc_metadata_browser_metadata_box_show_song (GmpcMetadataBrowser* self, const mpd_Song* song);
static void gmpc_metadata_browser_metadata_box_show_album (GmpcMetadataBrowser* self, const char* artist, const char* album);
static void gmpc_metadata_browser_metadata_box_show_artist (GmpcMetadataBrowser* self, const char* artist);
static void gmpc_metadata_browser_metadata_box_update (GmpcMetadataBrowser* self);
static void gmpc_metadata_browser_real_browser_add (GmpcPluginBrowserIface* base, GtkWidget* category_tree);
static void gmpc_metadata_browser_real_browser_selected (GmpcPluginBrowserIface* base, GtkContainer* container);
static void gmpc_metadata_browser_real_browser_unselected (GmpcPluginBrowserIface* base, GtkContainer* container);
static void gmpc_metadata_browser_con_changed (GmpcMetadataBrowser* self, GmpcConnection* conn, MpdObj* server, gint connect);
static void gmpc_metadata_browser_status_changed (GmpcMetadataBrowser* self, GmpcConnection* conn, MpdObj* server, ChangedStatusType what);
static void _gmpc_metadata_browser_con_changed_gmpc_connection_connection_changed (GmpcConnection* _sender, MpdObj* server, gint connect, gpointer self);
static void _gmpc_metadata_browser_status_changed_gmpc_connection_status_changed (GmpcConnection* _sender, MpdObj* server, ChangedStatusType what, gpointer self);
static GObject * gmpc_metadata_browser_constructor (GType type, guint n_construct_properties, GObjectConstructParam * construct_properties);
static gpointer gmpc_metadata_browser_parent_class = NULL;
static GmpcPluginBrowserIfaceIface* gmpc_metadata_browser_gmpc_plugin_browser_iface_parent_iface = NULL;
static void gmpc_metadata_browser_finalize (GObject* obj);



static void gmpc_widget_more_expand (GmpcWidgetMore* self, GtkButton* but) {
	g_return_if_fail (self != NULL);
	g_return_if_fail (but != NULL);
	if (self->priv->expand_state == 0) {
		gtk_button_set_label (but, _ ("(less)"));
		gtk_widget_set_size_request ((GtkWidget*) self->priv->ali, -1, -1);
		self->priv->expand_state = 1;
	} else {
		gtk_button_set_label (but, _ ("(more)"));
		gtk_widget_set_size_request ((GtkWidget*) self->priv->ali, -1, self->priv->max_height);
		self->priv->expand_state = 0;
	}
}


static void gmpc_widget_more_size_changed (GmpcWidgetMore* self, GtkWidget* child, const GdkRectangle* alloc) {
	g_return_if_fail (self != NULL);
	g_return_if_fail (child != NULL);
	fprintf (stdout, "height: %i\n", (*alloc).height);
	if ((*alloc).height < (self->priv->max_height - 12)) {
		gtk_widget_set_size_request ((GtkWidget*) self->priv->ali, -1, -1);
		gtk_widget_hide ((GtkWidget*) self->priv->expand_button);
	} else {
		if (self->priv->expand_state == 0) {
			gtk_widget_set_size_request ((GtkWidget*) self->priv->ali, -1, self->priv->max_height);
		}
		gtk_widget_show ((GtkWidget*) self->priv->expand_button);
	}
}


static void gmpc_widget_more_bg_style_changed (GmpcWidgetMore* self, GtkWidget* frame, GtkStyle* style) {
	GdkColor _tmp0 = {0};
	GdkColor _tmp1 = {0};
	GdkColor _tmp2 = {0};
	GdkColor _tmp3 = {0};
	g_return_if_fail (self != NULL);
	g_return_if_fail (frame != NULL);
	gtk_widget_modify_bg (self->priv->pchild, GTK_STATE_NORMAL, (_tmp0 = gtk_widget_get_style ((GtkWidget*) gtk_widget_get_parent ((GtkWidget*) self))->mid[GTK_STATE_NORMAL], &_tmp0));
	gtk_widget_modify_base (self->priv->pchild, GTK_STATE_NORMAL, (_tmp1 = gtk_widget_get_style ((GtkWidget*) gtk_widget_get_parent ((GtkWidget*) self))->mid[GTK_STATE_NORMAL], &_tmp1));
	gtk_widget_modify_bg ((GtkWidget*) self->priv->eventbox, GTK_STATE_NORMAL, (_tmp2 = gtk_widget_get_style ((GtkWidget*) gtk_widget_get_parent ((GtkWidget*) self))->dark[GTK_STATE_NORMAL], &_tmp2));
	gtk_widget_modify_base ((GtkWidget*) self->priv->eventbox, GTK_STATE_NORMAL, (_tmp3 = gtk_widget_get_style ((GtkWidget*) gtk_widget_get_parent ((GtkWidget*) self))->dark[GTK_STATE_NORMAL], &_tmp3));
}


static void _gmpc_widget_more_bg_style_changed_gtk_widget_style_set (GmpcWidgetMore* _sender, GtkStyle* previous_style, gpointer self) {
	gmpc_widget_more_bg_style_changed (self, _sender, previous_style);
}


static void _gmpc_widget_more_expand_gtk_button_clicked (GtkButton* _sender, gpointer self) {
	gmpc_widget_more_expand (self, _sender);
}


static void _gmpc_widget_more_size_changed_gtk_widget_size_allocate (GtkWidget* _sender, const GdkRectangle* allocation, gpointer self) {
	gmpc_widget_more_size_changed (self, _sender, allocation);
}


static GmpcWidgetMore* gmpc_widget_more_construct (GType object_type, const char* markup, GtkWidget* child) {
	GmpcWidgetMore * self;
	GtkWidget* _tmp1;
	GtkWidget* _tmp0;
	GtkAlignment* _tmp2;
	GtkEventBox* _tmp3;
	GtkHBox* hbox;
	GtkLabel* label;
	GtkButton* _tmp4;
	g_return_val_if_fail (markup != NULL, NULL);
	g_return_val_if_fail (child != NULL, NULL);
	self = g_object_newv (object_type, 0, NULL);
	gtk_frame_set_shadow_type ((GtkFrame*) self, GTK_SHADOW_NONE);
	_tmp1 = NULL;
	_tmp0 = NULL;
	self->priv->pchild = (_tmp1 = (_tmp0 = child, (_tmp0 == NULL) ? NULL : g_object_ref (_tmp0)), (self->priv->pchild == NULL) ? NULL : (self->priv->pchild = (g_object_unref (self->priv->pchild), NULL)), _tmp1);
	_tmp2 = NULL;
	self->priv->ali = (_tmp2 = g_object_ref_sink ((GtkAlignment*) gtk_alignment_new (0.f, 0.f, 1.f, 0.f)), (self->priv->ali == NULL) ? NULL : (self->priv->ali = (g_object_unref (self->priv->ali), NULL)), _tmp2);
	gtk_alignment_set_padding (self->priv->ali, (guint) 1, (guint) 1, (guint) 1, (guint) 1);
	_tmp3 = NULL;
	self->priv->eventbox = (_tmp3 = g_object_ref_sink ((GtkEventBox*) gtk_event_box_new ()), (self->priv->eventbox == NULL) ? NULL : (self->priv->eventbox = (g_object_unref (self->priv->eventbox), NULL)), _tmp3);
	gtk_event_box_set_visible_window (self->priv->eventbox, TRUE);
	gtk_container_add ((GtkContainer*) self, (GtkWidget*) self->priv->eventbox);
	gtk_container_add ((GtkContainer*) self->priv->eventbox, (GtkWidget*) self->priv->ali);
	gtk_widget_set_size_request ((GtkWidget*) self->priv->ali, -1, self->priv->max_height);
	gtk_container_add ((GtkContainer*) self->priv->ali, child);
	g_signal_connect_object ((GtkWidget*) self, "style-set", (GCallback) _gmpc_widget_more_bg_style_changed_gtk_widget_style_set, self, 0);
	hbox = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 6));
	label = g_object_ref_sink ((GtkLabel*) gtk_label_new (""));
	gtk_label_set_markup (label, markup);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) label, FALSE, FALSE, (guint) 0);
	_tmp4 = NULL;
	self->priv->expand_button = (_tmp4 = g_object_ref_sink ((GtkButton*) gtk_button_new_with_label (_ ("(more)"))), (self->priv->expand_button == NULL) ? NULL : (self->priv->expand_button = (g_object_unref (self->priv->expand_button), NULL)), _tmp4);
	gtk_button_set_relief (self->priv->expand_button, GTK_RELIEF_NONE);
	g_signal_connect_object (self->priv->expand_button, "clicked", (GCallback) _gmpc_widget_more_expand_gtk_button_clicked, self, 0);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) self->priv->expand_button, FALSE, FALSE, (guint) 0);
	gtk_frame_set_label_widget ((GtkFrame*) self, (GtkWidget*) hbox);
	g_signal_connect_object (child, "size-allocate", (GCallback) _gmpc_widget_more_size_changed_gtk_widget_size_allocate, self, 0);
	return self;
}


static GmpcWidgetMore* gmpc_widget_more_new (const char* markup, GtkWidget* child) {
	return gmpc_widget_more_construct (GMPC_WIDGET_TYPE_MORE, markup, child);
}


static void gmpc_widget_more_class_init (GmpcWidgetMoreClass * klass) {
	gmpc_widget_more_parent_class = g_type_class_peek_parent (klass);
	g_type_class_add_private (klass, sizeof (GmpcWidgetMorePrivate));
	G_OBJECT_CLASS (klass)->finalize = gmpc_widget_more_finalize;
}


static void gmpc_widget_more_instance_init (GmpcWidgetMore * self) {
	self->priv = GMPC_WIDGET_MORE_GET_PRIVATE (self);
	self->priv->ali = NULL;
	self->priv->expand_state = 0;
	self->priv->expand_button = NULL;
	self->priv->max_height = 100;
	self->priv->eventbox = NULL;
	self->priv->pchild = NULL;
}


static void gmpc_widget_more_finalize (GObject* obj) {
	GmpcWidgetMore * self;
	self = GMPC_WIDGET_MORE (obj);
	(self->priv->ali == NULL) ? NULL : (self->priv->ali = (g_object_unref (self->priv->ali), NULL));
	(self->priv->expand_button == NULL) ? NULL : (self->priv->expand_button = (g_object_unref (self->priv->expand_button), NULL));
	(self->priv->eventbox == NULL) ? NULL : (self->priv->eventbox = (g_object_unref (self->priv->eventbox), NULL));
	(self->priv->pchild == NULL) ? NULL : (self->priv->pchild = (g_object_unref (self->priv->pchild), NULL));
	G_OBJECT_CLASS (gmpc_widget_more_parent_class)->finalize (obj);
}


GType gmpc_widget_more_get_type (void) {
	static GType gmpc_widget_more_type_id = 0;
	if (gmpc_widget_more_type_id == 0) {
		static const GTypeInfo g_define_type_info = { sizeof (GmpcWidgetMoreClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) gmpc_widget_more_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (GmpcWidgetMore), 0, (GInstanceInitFunc) gmpc_widget_more_instance_init, NULL };
		gmpc_widget_more_type_id = g_type_register_static (GTK_TYPE_FRAME, "GmpcWidgetMore", &g_define_type_info, 0);
	}
	return gmpc_widget_more_type_id;
}


static gint* gmpc_metadata_browser_real_get_version (GmpcPluginBase* base, int* result_length1) {
	GmpcMetadataBrowser * self;
	gint* _tmp0;
	self = (GmpcMetadataBrowser*) base;
	_tmp0 = NULL;
	return (_tmp0 = GMPC_METADATA_BROWSER_version, *result_length1 = G_N_ELEMENTS (GMPC_METADATA_BROWSER_version), _tmp0);
}


static const char* gmpc_metadata_browser_real_get_name (GmpcPluginBase* base) {
	GmpcMetadataBrowser * self;
	self = (GmpcMetadataBrowser*) base;
	return N_ ("Metadata Browser 2");
}


static void gmpc_metadata_browser_real_save_yourself (GmpcPluginBase* base) {
	GmpcMetadataBrowser * self;
	self = (GmpcMetadataBrowser*) base;
	if (self->priv->paned != NULL) {
		gint pos;
		pos = gtk_paned_get_position (self->priv->paned);
		cfg_set_single_value_as_int (config, gmpc_plugin_base_get_name ((GmpcPluginBase*) self), "pane-pos", pos);
	}
	if (self->priv->model_artist != NULL) {
		gmpc_mpddata_model_set_mpd_data (self->priv->model_artist, NULL);
	}
	if (self->priv->model_albums != NULL) {
		gmpc_mpddata_model_set_mpd_data (self->priv->model_albums, NULL);
	}
	if (self->priv->rref != NULL) {
		GtkTreePath* path;
		path = gtk_tree_row_reference_get_path (self->priv->rref);
		if (path != NULL) {
			gint* _tmp0;
			gint indices_size;
			gint indices_length1;
			gint* indices;
			_tmp0 = NULL;
			indices = (_tmp0 = gtk_tree_path_get_indices (path), indices_length1 = -1, indices_size = indices_length1, _tmp0);
			cfg_set_single_value_as_int (config, gmpc_plugin_base_get_name ((GmpcPluginBase*) self), "position", indices[0]);
		}
		(path == NULL) ? NULL : (path = (gtk_tree_path_free (path), NULL));
	}
}


/**
     * This builds the browser
     */
static void gmpc_metadata_browser_browser_bg_style_changed (GmpcMetadataBrowser* self, GtkScrolledWindow* bg, GtkStyle* style) {
	GdkColor _tmp0 = {0};
	g_return_if_fail (self != NULL);
	g_return_if_fail (bg != NULL);
	gtk_widget_modify_bg ((GtkWidget*) self->priv->metadata_box, GTK_STATE_NORMAL, (_tmp0 = gtk_widget_get_style ((GtkWidget*) self->priv->metadata_sw)->base[GTK_STATE_NORMAL], &_tmp0));
}


/* This hack makes clicking a selected row again, unselect it */
static gboolean gmpc_metadata_browser_browser_button_press_event (GmpcMetadataBrowser* self, GtkTreeView* tree, const GdkEventButton* event) {
	GtkTreePath* path;
	GtkTreePath* _tmp3;
	gboolean _tmp2;
	GtkTreePath* _tmp1;
	gboolean _tmp5;
	g_return_val_if_fail (self != NULL, FALSE);
	g_return_val_if_fail (tree != NULL, FALSE);
	path = NULL;
	if ((*event).button != 1) {
		gboolean _tmp0;
		return (_tmp0 = FALSE, (path == NULL) ? NULL : (path = (gtk_tree_path_free (path), NULL)), _tmp0);
	}
	_tmp3 = NULL;
	_tmp1 = NULL;
	if ((_tmp2 = gtk_tree_view_get_path_at_pos (tree, (gint) (*event).x, (gint) (*event).y, &_tmp1, NULL, NULL, NULL), path = (_tmp3 = _tmp1, (path == NULL) ? NULL : (path = (gtk_tree_path_free (path), NULL)), _tmp3), _tmp2)) {
		if (gtk_tree_selection_path_is_selected (gtk_tree_view_get_selection (tree), path)) {
			gboolean _tmp4;
			gtk_tree_selection_unselect_path (gtk_tree_view_get_selection (tree), path);
			return (_tmp4 = TRUE, (path == NULL) ? NULL : (path = (gtk_tree_path_free (path), NULL)), _tmp4);
		}
	}
	return (_tmp5 = FALSE, (path == NULL) ? NULL : (path = (gtk_tree_path_free (path), NULL)), _tmp5);
}


static gboolean gmpc_metadata_browser_visible_func_artist (GmpcMetadataBrowser* self, GtkTreeModel* model, GtkTreeIter* iter) {
	const char* _tmp0;
	char* text;
	char* str;
	gboolean visible;
	gboolean _tmp2;
	gboolean _tmp7;
	g_return_val_if_fail (self != NULL, FALSE);
	g_return_val_if_fail (model != NULL, FALSE);
	_tmp0 = NULL;
	text = (_tmp0 = gtk_entry_get_text (self->priv->artist_filter_entry), (_tmp0 == NULL) ? NULL : g_strdup (_tmp0));
	/* Visible if row is non-empty and first column is "HI" */
	str = NULL;
	visible = FALSE;
	if (g_utf8_get_char (g_utf8_offset_to_pointer (text, 0)) == '\0') {
		gboolean _tmp1;
		return (_tmp1 = TRUE, text = (g_free (text), NULL), str = (g_free (str), NULL), _tmp1);
	}
	gtk_tree_model_get (model, &(*iter), 7, &str, -1, -1);
	_tmp2 = FALSE;
	if (str != NULL) {
		char* _tmp6;
		char* _tmp5;
		char* _tmp4;
		char* _tmp3;
		_tmp6 = NULL;
		_tmp5 = NULL;
		_tmp4 = NULL;
		_tmp3 = NULL;
		_tmp2 = strstr (_tmp4 = g_utf8_normalize (_tmp3 = g_utf8_casefold (str, -1), -1, G_NORMALIZE_DEFAULT), _tmp6 = g_utf8_normalize (_tmp5 = g_utf8_casefold (text, -1), -1, G_NORMALIZE_DEFAULT)) != NULL;
		_tmp6 = (g_free (_tmp6), NULL);
		_tmp5 = (g_free (_tmp5), NULL);
		_tmp4 = (g_free (_tmp4), NULL);
		_tmp3 = (g_free (_tmp3), NULL);
	} else {
		_tmp2 = FALSE;
	}
	if (_tmp2) {
		visible = TRUE;
	}
	return (_tmp7 = visible, text = (g_free (text), NULL), str = (g_free (str), NULL), _tmp7);
}


static gboolean gmpc_metadata_browser_visible_func_album (GmpcMetadataBrowser* self, GtkTreeModel* model, GtkTreeIter* iter) {
	const char* _tmp0;
	char* text;
	char* str;
	gboolean visible;
	gboolean _tmp2;
	gboolean _tmp7;
	g_return_val_if_fail (self != NULL, FALSE);
	g_return_val_if_fail (model != NULL, FALSE);
	_tmp0 = NULL;
	text = (_tmp0 = gtk_entry_get_text (self->priv->album_filter_entry), (_tmp0 == NULL) ? NULL : g_strdup (_tmp0));
	/* Visible if row is non-empty and first column is "HI" */
	str = NULL;
	visible = FALSE;
	if (g_utf8_get_char (g_utf8_offset_to_pointer (text, 0)) == '\0') {
		gboolean _tmp1;
		return (_tmp1 = TRUE, text = (g_free (text), NULL), str = (g_free (str), NULL), _tmp1);
	}
	gtk_tree_model_get (model, &(*iter), 7, &str, -1, -1);
	_tmp2 = FALSE;
	if (str != NULL) {
		char* _tmp6;
		char* _tmp5;
		char* _tmp4;
		char* _tmp3;
		_tmp6 = NULL;
		_tmp5 = NULL;
		_tmp4 = NULL;
		_tmp3 = NULL;
		_tmp2 = strstr (_tmp4 = g_utf8_normalize (_tmp3 = g_utf8_casefold (str, -1), -1, G_NORMALIZE_DEFAULT), _tmp6 = g_utf8_normalize (_tmp5 = g_utf8_casefold (text, -1), -1, G_NORMALIZE_DEFAULT)) != NULL;
		_tmp6 = (g_free (_tmp6), NULL);
		_tmp5 = (g_free (_tmp5), NULL);
		_tmp4 = (g_free (_tmp4), NULL);
		_tmp3 = (g_free (_tmp3), NULL);
	} else {
		_tmp2 = FALSE;
	}
	if (_tmp2) {
		visible = TRUE;
	}
	return (_tmp7 = visible, text = (g_free (text), NULL), str = (g_free (str), NULL), _tmp7);
}


static gboolean gmpc_metadata_browser_browser_artist_key_press_event (GmpcMetadataBrowser* self, GtkWidget* widget, const GdkEventKey* event) {
	gunichar uc;
	g_return_val_if_fail (self != NULL, FALSE);
	g_return_val_if_fail (widget != NULL, FALSE);
	uc = (gunichar) gdk_keyval_to_unicode ((*event).keyval);
	fprintf (stdout, "set key press event artist");
	if (uc > 0) {
		char* outbuf;
		gint i;
		gboolean _tmp0;
		outbuf = g_strdup ("       ");
		i = g_unichar_to_utf8 (uc, outbuf);
		((gchar*) outbuf)[i] = '\0';
		gtk_entry_set_text (self->priv->artist_filter_entry, outbuf);
		gtk_widget_grab_focus ((GtkWidget*) self->priv->artist_filter_entry);
		gtk_editable_set_position ((GtkEditable*) self->priv->artist_filter_entry, 1);
		return (_tmp0 = TRUE, outbuf = (g_free (outbuf), NULL), _tmp0);
	}
	return FALSE;
}


static gboolean gmpc_metadata_browser_browser_album_key_press_event (GmpcMetadataBrowser* self, GtkWidget* widget, const GdkEventKey* event) {
	gunichar uc;
	g_return_val_if_fail (self != NULL, FALSE);
	g_return_val_if_fail (widget != NULL, FALSE);
	uc = (gunichar) gdk_keyval_to_unicode ((*event).keyval);
	fprintf (stdout, "set key press event album");
	if (uc > 0) {
		char* outbuf;
		gint i;
		gboolean _tmp0;
		outbuf = g_strdup ("       ");
		i = g_unichar_to_utf8 (uc, outbuf);
		((gchar*) outbuf)[i] = '\0';
		gtk_entry_set_text (self->priv->album_filter_entry, outbuf);
		gtk_widget_grab_focus ((GtkWidget*) self->priv->album_filter_entry);
		gtk_editable_set_position ((GtkEditable*) self->priv->album_filter_entry, 1);
		return (_tmp0 = TRUE, outbuf = (g_free (outbuf), NULL), _tmp0);
	}
	return FALSE;
}


static void gmpc_metadata_browser_browser_artist_entry_changed (GmpcMetadataBrowser* self, GtkEntry* entry) {
	const char* _tmp0;
	char* text;
	g_return_if_fail (self != NULL);
	g_return_if_fail (entry != NULL);
	_tmp0 = NULL;
	text = (_tmp0 = gtk_entry_get_text (entry), (_tmp0 == NULL) ? NULL : g_strdup (_tmp0));
	if (strlen (text) > 0) {
		gtk_widget_show ((GtkWidget*) entry);
	} else {
		gtk_widget_hide ((GtkWidget*) entry);
		gtk_widget_grab_focus ((GtkWidget*) self->priv->tree_artist);
	}
	gtk_tree_model_filter_refilter (self->priv->model_filter_artist);
	text = (g_free (text), NULL);
}


static void gmpc_metadata_browser_browser_album_entry_changed (GmpcMetadataBrowser* self, GtkEntry* entry) {
	const char* _tmp0;
	char* text;
	g_return_if_fail (self != NULL);
	g_return_if_fail (entry != NULL);
	_tmp0 = NULL;
	text = (_tmp0 = gtk_entry_get_text (entry), (_tmp0 == NULL) ? NULL : g_strdup (_tmp0));
	if (strlen (text) > 0) {
		gtk_widget_show ((GtkWidget*) entry);
	} else {
		gtk_widget_hide ((GtkWidget*) entry);
		gtk_widget_grab_focus ((GtkWidget*) self->priv->tree_album);
	}
	gtk_tree_model_filter_refilter (self->priv->model_filter_album);
	text = (g_free (text), NULL);
}


static void _gmpc_metadata_browser_browser_artist_entry_changed_gtk_editable_changed (GtkEntry* _sender, gpointer self) {
	gmpc_metadata_browser_browser_artist_entry_changed (self, _sender);
}


static gboolean _gmpc_metadata_browser_visible_func_artist_gtk_tree_model_filter_visible_func (GtkTreeModel* model, GtkTreeIter* iter, gpointer self) {
	return gmpc_metadata_browser_visible_func_artist (self, model, iter);
}


static gboolean _gmpc_metadata_browser_browser_button_press_event_gtk_widget_button_press_event (GtkTreeView* _sender, const GdkEventButton* event, gpointer self) {
	return gmpc_metadata_browser_browser_button_press_event (self, _sender, event);
}


static gboolean _gmpc_metadata_browser_browser_artist_key_press_event_gtk_widget_key_press_event (GtkTreeView* _sender, const GdkEventKey* event, gpointer self) {
	return gmpc_metadata_browser_browser_artist_key_press_event (self, _sender, event);
}


static void _gmpc_metadata_browser_browser_artist_changed_gtk_tree_selection_changed (GtkTreeSelection* _sender, gpointer self) {
	gmpc_metadata_browser_browser_artist_changed (self, _sender);
}


static void _gmpc_metadata_browser_browser_album_entry_changed_gtk_editable_changed (GtkEntry* _sender, gpointer self) {
	gmpc_metadata_browser_browser_album_entry_changed (self, _sender);
}


static gboolean _gmpc_metadata_browser_visible_func_album_gtk_tree_model_filter_visible_func (GtkTreeModel* model, GtkTreeIter* iter, gpointer self) {
	return gmpc_metadata_browser_visible_func_album (self, model, iter);
}


static gboolean _gmpc_metadata_browser_browser_album_key_press_event_gtk_widget_key_press_event (GtkTreeView* _sender, const GdkEventKey* event, gpointer self) {
	return gmpc_metadata_browser_browser_album_key_press_event (self, _sender, event);
}


static void _gmpc_metadata_browser_browser_album_changed_gtk_tree_selection_changed (GtkTreeSelection* _sender, gpointer self) {
	gmpc_metadata_browser_browser_album_changed (self, _sender);
}


static void _gmpc_metadata_browser_browser_songs_changed_gtk_tree_selection_changed (GtkTreeSelection* _sender, gpointer self) {
	gmpc_metadata_browser_browser_songs_changed (self, _sender);
}


static void _gmpc_metadata_browser_browser_bg_style_changed_gtk_widget_style_set (GtkScrolledWindow* _sender, GtkStyle* previous_style, gpointer self) {
	gmpc_metadata_browser_browser_bg_style_changed (self, _sender, previous_style);
}


static void gmpc_metadata_browser_browser_init (GmpcMetadataBrowser* self) {
	g_return_if_fail (self != NULL);
	if (self->priv->paned == NULL) {
		GtkPaned* _tmp0;
		GtkBox* _tmp1;
		GtkVBox* box;
		GtkEntry* _tmp2;
		GtkScrolledWindow* sw;
		GmpcMpdDataModel* _tmp3;
		GtkTreeModelFilter* _tmp4;
		GtkTreeView* _tmp5;
		GtkTreeViewColumn* column;
		GtkCellRendererPixbuf* prenderer;
		GtkCellRendererText* trenderer;
		GtkVBox* _tmp6;
		GtkEntry* _tmp7;
		GtkScrolledWindow* _tmp8;
		GmpcMpdDataModel* _tmp9;
		GtkTreeModelFilter* _tmp10;
		GtkTreeView* _tmp11;
		GtkTreeViewColumn* _tmp12;
		GtkCellRendererPixbuf* _tmp13;
		GtkCellRendererText* _tmp14;
		GtkScrolledWindow* _tmp15;
		GmpcMpdDataModel* _tmp16;
		GtkTreeView* _tmp17;
		GtkTreeViewColumn* _tmp18;
		GtkCellRendererPixbuf* _tmp19;
		GtkCellRendererText* _tmp20;
		GtkCellRendererText* _tmp21;
		GtkScrolledWindow* _tmp22;
		GtkEventBox* _tmp23;
		_tmp0 = NULL;
		self->priv->paned = (_tmp0 = (GtkPaned*) g_object_ref_sink ((GtkHPaned*) gtk_hpaned_new ()), (self->priv->paned == NULL) ? NULL : (self->priv->paned = (g_object_unref (self->priv->paned), NULL)), _tmp0);
		gtk_paned_set_position (self->priv->paned, cfg_get_single_value_as_int_with_default (config, gmpc_plugin_base_get_name ((GmpcPluginBase*) self), "pane-pos", 150));
		/* Bow with browsers */
		_tmp1 = NULL;
		self->priv->browser_box = (_tmp1 = (GtkBox*) g_object_ref_sink ((GtkVBox*) gtk_vbox_new (TRUE, 6)), (self->priv->browser_box == NULL) ? NULL : (self->priv->browser_box = (g_object_unref (self->priv->browser_box), NULL)), _tmp1);
		gtk_paned_add1 (self->priv->paned, (GtkWidget*) self->priv->browser_box);
		/* Artist list  */
		box = g_object_ref_sink ((GtkVBox*) gtk_vbox_new (FALSE, 6));
		gtk_box_pack_start (self->priv->browser_box, (GtkWidget*) box, TRUE, TRUE, (guint) 0);
		_tmp2 = NULL;
		self->priv->artist_filter_entry = (_tmp2 = g_object_ref_sink ((GtkEntry*) gtk_entry_new ()), (self->priv->artist_filter_entry == NULL) ? NULL : (self->priv->artist_filter_entry = (g_object_unref (self->priv->artist_filter_entry), NULL)), _tmp2);
		gtk_widget_set_no_show_all ((GtkWidget*) self->priv->artist_filter_entry, TRUE);
		g_signal_connect_object ((GtkEditable*) self->priv->artist_filter_entry, "changed", (GCallback) _gmpc_metadata_browser_browser_artist_entry_changed_gtk_editable_changed, self, 0);
		gtk_box_pack_start ((GtkBox*) box, (GtkWidget*) self->priv->artist_filter_entry, FALSE, FALSE, (guint) 0);
		sw = g_object_ref_sink ((GtkScrolledWindow*) gtk_scrolled_window_new (NULL, NULL));
		gtk_scrolled_window_set_policy (sw, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
		gtk_scrolled_window_set_shadow_type (sw, GTK_SHADOW_ETCHED_IN);
		gtk_box_pack_start ((GtkBox*) box, (GtkWidget*) sw, TRUE, TRUE, (guint) 0);
		_tmp3 = NULL;
		self->priv->model_artist = (_tmp3 = gmpc_mpddata_model_new (), (self->priv->model_artist == NULL) ? NULL : (self->priv->model_artist = (g_object_unref (self->priv->model_artist), NULL)), _tmp3);
		_tmp4 = NULL;
		self->priv->model_filter_artist = (_tmp4 = (GtkTreeModelFilter*) gtk_tree_model_filter_new ((GtkTreeModel*) self->priv->model_artist, NULL), (self->priv->model_filter_artist == NULL) ? NULL : (self->priv->model_filter_artist = (g_object_unref (self->priv->model_filter_artist), NULL)), _tmp4);
		gtk_tree_model_filter_set_visible_func (self->priv->model_filter_artist, _gmpc_metadata_browser_visible_func_artist_gtk_tree_model_filter_visible_func, g_object_ref (self), g_object_unref);
		_tmp5 = NULL;
		self->priv->tree_artist = (_tmp5 = g_object_ref_sink ((GtkTreeView*) gtk_tree_view_new_with_model ((GtkTreeModel*) self->priv->model_filter_artist)), (self->priv->tree_artist == NULL) ? NULL : (self->priv->tree_artist = (g_object_unref (self->priv->tree_artist), NULL)), _tmp5);
		g_signal_connect_object ((GtkWidget*) self->priv->tree_artist, "button-press-event", (GCallback) _gmpc_metadata_browser_browser_button_press_event_gtk_widget_button_press_event, self, 0);
		g_signal_connect_object ((GtkWidget*) self->priv->tree_artist, "key-press-event", (GCallback) _gmpc_metadata_browser_browser_artist_key_press_event_gtk_widget_key_press_event, self, 0);
		gtk_container_add ((GtkContainer*) sw, (GtkWidget*) self->priv->tree_artist);
		/* setup the columns */
		column = g_object_ref_sink (gtk_tree_view_column_new ());
		prenderer = g_object_ref_sink ((GtkCellRendererPixbuf*) gtk_cell_renderer_pixbuf_new ());
		g_object_set ((GObject*) prenderer, "height", self->priv->model_artist->icon_size, NULL);
		gtk_cell_layout_pack_start ((GtkCellLayout*) column, (GtkCellRenderer*) prenderer, FALSE);
		gtk_cell_layout_add_attribute ((GtkCellLayout*) column, (GtkCellRenderer*) prenderer, "pixbuf", 27);
		trenderer = g_object_ref_sink ((GtkCellRendererText*) gtk_cell_renderer_text_new ());
		gtk_cell_layout_pack_start ((GtkCellLayout*) column, (GtkCellRenderer*) trenderer, TRUE);
		gtk_cell_layout_add_attribute ((GtkCellLayout*) column, (GtkCellRenderer*) trenderer, "text", 7);
		gtk_tree_view_append_column (self->priv->tree_artist, column);
		gtk_tree_view_column_set_title (column, _ ("Artist"));
		g_signal_connect_object (gtk_tree_view_get_selection (self->priv->tree_artist), "changed", (GCallback) _gmpc_metadata_browser_browser_artist_changed_gtk_tree_selection_changed, self, 0);
		gtk_tree_view_set_search_column (self->priv->tree_artist, 7);
		/* set fixed height mode */
		gtk_tree_view_column_set_sizing (column, GTK_TREE_VIEW_COLUMN_FIXED);
		gtk_tree_view_set_fixed_height_mode (self->priv->tree_artist, TRUE);
		/* Album list */
		_tmp6 = NULL;
		box = (_tmp6 = g_object_ref_sink ((GtkVBox*) gtk_vbox_new (FALSE, 6)), (box == NULL) ? NULL : (box = (g_object_unref (box), NULL)), _tmp6);
		gtk_box_pack_start (self->priv->browser_box, (GtkWidget*) box, TRUE, TRUE, (guint) 0);
		_tmp7 = NULL;
		self->priv->album_filter_entry = (_tmp7 = g_object_ref_sink ((GtkEntry*) gtk_entry_new ()), (self->priv->album_filter_entry == NULL) ? NULL : (self->priv->album_filter_entry = (g_object_unref (self->priv->album_filter_entry), NULL)), _tmp7);
		gtk_widget_set_no_show_all ((GtkWidget*) self->priv->album_filter_entry, TRUE);
		g_signal_connect_object ((GtkEditable*) self->priv->album_filter_entry, "changed", (GCallback) _gmpc_metadata_browser_browser_album_entry_changed_gtk_editable_changed, self, 0);
		gtk_box_pack_start ((GtkBox*) box, (GtkWidget*) self->priv->album_filter_entry, FALSE, FALSE, (guint) 0);
		_tmp8 = NULL;
		sw = (_tmp8 = g_object_ref_sink ((GtkScrolledWindow*) gtk_scrolled_window_new (NULL, NULL)), (sw == NULL) ? NULL : (sw = (g_object_unref (sw), NULL)), _tmp8);
		gtk_scrolled_window_set_policy (sw, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
		gtk_scrolled_window_set_shadow_type (sw, GTK_SHADOW_ETCHED_IN);
		gtk_box_pack_start ((GtkBox*) box, (GtkWidget*) sw, TRUE, TRUE, (guint) 0);
		_tmp9 = NULL;
		self->priv->model_albums = (_tmp9 = gmpc_mpddata_model_new (), (self->priv->model_albums == NULL) ? NULL : (self->priv->model_albums = (g_object_unref (self->priv->model_albums), NULL)), _tmp9);
		_tmp10 = NULL;
		self->priv->model_filter_album = (_tmp10 = (GtkTreeModelFilter*) gtk_tree_model_filter_new ((GtkTreeModel*) self->priv->model_albums, NULL), (self->priv->model_filter_album == NULL) ? NULL : (self->priv->model_filter_album = (g_object_unref (self->priv->model_filter_album), NULL)), _tmp10);
		gtk_tree_model_filter_set_visible_func (self->priv->model_filter_album, _gmpc_metadata_browser_visible_func_album_gtk_tree_model_filter_visible_func, g_object_ref (self), g_object_unref);
		_tmp11 = NULL;
		self->priv->tree_album = (_tmp11 = g_object_ref_sink ((GtkTreeView*) gtk_tree_view_new_with_model ((GtkTreeModel*) self->priv->model_filter_album)), (self->priv->tree_album == NULL) ? NULL : (self->priv->tree_album = (g_object_unref (self->priv->tree_album), NULL)), _tmp11);
		g_signal_connect_object ((GtkWidget*) self->priv->tree_album, "button-press-event", (GCallback) _gmpc_metadata_browser_browser_button_press_event_gtk_widget_button_press_event, self, 0);
		g_signal_connect_object ((GtkWidget*) self->priv->tree_album, "key-press-event", (GCallback) _gmpc_metadata_browser_browser_album_key_press_event_gtk_widget_key_press_event, self, 0);
		gtk_container_add ((GtkContainer*) sw, (GtkWidget*) self->priv->tree_album);
		/* setup the columns */
		_tmp12 = NULL;
		column = (_tmp12 = g_object_ref_sink (gtk_tree_view_column_new ()), (column == NULL) ? NULL : (column = (g_object_unref (column), NULL)), _tmp12);
		_tmp13 = NULL;
		prenderer = (_tmp13 = g_object_ref_sink ((GtkCellRendererPixbuf*) gtk_cell_renderer_pixbuf_new ()), (prenderer == NULL) ? NULL : (prenderer = (g_object_unref (prenderer), NULL)), _tmp13);
		g_object_set ((GObject*) prenderer, "height", self->priv->model_albums->icon_size, NULL);
		gtk_cell_layout_pack_start ((GtkCellLayout*) column, (GtkCellRenderer*) prenderer, FALSE);
		gtk_cell_layout_add_attribute ((GtkCellLayout*) column, (GtkCellRenderer*) prenderer, "pixbuf", 27);
		_tmp14 = NULL;
		trenderer = (_tmp14 = g_object_ref_sink ((GtkCellRendererText*) gtk_cell_renderer_text_new ()), (trenderer == NULL) ? NULL : (trenderer = (g_object_unref (trenderer), NULL)), _tmp14);
		gtk_cell_layout_pack_start ((GtkCellLayout*) column, (GtkCellRenderer*) trenderer, TRUE);
		gtk_cell_layout_add_attribute ((GtkCellLayout*) column, (GtkCellRenderer*) trenderer, "text", 7);
		gtk_tree_view_append_column (self->priv->tree_album, column);
		gtk_tree_view_set_search_column (self->priv->tree_album, 7);
		gtk_tree_view_column_set_title (column, _ ("Album"));
		g_signal_connect_object (gtk_tree_view_get_selection (self->priv->tree_album), "changed", (GCallback) _gmpc_metadata_browser_browser_album_changed_gtk_tree_selection_changed, self, 0);
		/* set fixed height mode */
		gtk_tree_view_column_set_sizing (column, GTK_TREE_VIEW_COLUMN_FIXED);
		gtk_tree_view_set_fixed_height_mode (self->priv->tree_album, TRUE);
		/* Song list */
		_tmp15 = NULL;
		sw = (_tmp15 = g_object_ref_sink ((GtkScrolledWindow*) gtk_scrolled_window_new (NULL, NULL)), (sw == NULL) ? NULL : (sw = (g_object_unref (sw), NULL)), _tmp15);
		gtk_scrolled_window_set_policy (sw, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
		gtk_scrolled_window_set_shadow_type (sw, GTK_SHADOW_ETCHED_IN);
		gtk_box_pack_start (self->priv->browser_box, (GtkWidget*) sw, TRUE, TRUE, (guint) 0);
		_tmp16 = NULL;
		self->priv->model_songs = (_tmp16 = gmpc_mpddata_model_new (), (self->priv->model_songs == NULL) ? NULL : (self->priv->model_songs = (g_object_unref (self->priv->model_songs), NULL)), _tmp16);
		_tmp17 = NULL;
		self->priv->tree_songs = (_tmp17 = g_object_ref_sink ((GtkTreeView*) gtk_tree_view_new_with_model ((GtkTreeModel*) self->priv->model_songs)), (self->priv->tree_songs == NULL) ? NULL : (self->priv->tree_songs = (g_object_unref (self->priv->tree_songs), NULL)), _tmp17);
		g_signal_connect_object ((GtkWidget*) self->priv->tree_songs, "button-press-event", (GCallback) _gmpc_metadata_browser_browser_button_press_event_gtk_widget_button_press_event, self, 0);
		gtk_container_add ((GtkContainer*) sw, (GtkWidget*) self->priv->tree_songs);
		/* setup the columns */
		_tmp18 = NULL;
		column = (_tmp18 = g_object_ref_sink (gtk_tree_view_column_new ()), (column == NULL) ? NULL : (column = (g_object_unref (column), NULL)), _tmp18);
		_tmp19 = NULL;
		prenderer = (_tmp19 = g_object_ref_sink ((GtkCellRendererPixbuf*) gtk_cell_renderer_pixbuf_new ()), (prenderer == NULL) ? NULL : (prenderer = (g_object_unref (prenderer), NULL)), _tmp19);
		gtk_cell_layout_pack_start ((GtkCellLayout*) column, (GtkCellRenderer*) prenderer, FALSE);
		gtk_cell_layout_add_attribute ((GtkCellLayout*) column, (GtkCellRenderer*) prenderer, "icon-name", 23);
		_tmp20 = NULL;
		trenderer = (_tmp20 = g_object_ref_sink ((GtkCellRendererText*) gtk_cell_renderer_text_new ()), (trenderer == NULL) ? NULL : (trenderer = (g_object_unref (trenderer), NULL)), _tmp20);
		gtk_cell_layout_pack_start ((GtkCellLayout*) column, (GtkCellRenderer*) trenderer, FALSE);
		gtk_cell_layout_add_attribute ((GtkCellLayout*) column, (GtkCellRenderer*) trenderer, "text", 10);
		_tmp21 = NULL;
		trenderer = (_tmp21 = g_object_ref_sink ((GtkCellRendererText*) gtk_cell_renderer_text_new ()), (trenderer == NULL) ? NULL : (trenderer = (g_object_unref (trenderer), NULL)), _tmp21);
		gtk_cell_layout_pack_start ((GtkCellLayout*) column, (GtkCellRenderer*) trenderer, TRUE);
		gtk_cell_layout_add_attribute ((GtkCellLayout*) column, (GtkCellRenderer*) trenderer, "text", 7);
		gtk_tree_view_append_column (self->priv->tree_songs, column);
		gtk_tree_view_set_search_column (self->priv->tree_songs, 7);
		gtk_tree_view_column_set_title (column, _ ("Songs"));
		/* set fixed height mode */
		gtk_tree_view_column_set_sizing (column, GTK_TREE_VIEW_COLUMN_FIXED);
		gtk_tree_view_set_fixed_height_mode (self->priv->tree_songs, TRUE);
		g_signal_connect_object (gtk_tree_view_get_selection (self->priv->tree_songs), "changed", (GCallback) _gmpc_metadata_browser_browser_songs_changed_gtk_tree_selection_changed, self, 0);
		/* The right view */
		_tmp22 = NULL;
		self->priv->metadata_sw = (_tmp22 = g_object_ref_sink ((GtkScrolledWindow*) gtk_scrolled_window_new (NULL, NULL)), (self->priv->metadata_sw == NULL) ? NULL : (self->priv->metadata_sw = (g_object_unref (self->priv->metadata_sw), NULL)), _tmp22);
		gtk_scrolled_window_set_policy (self->priv->metadata_sw, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
		g_signal_connect_object ((GtkWidget*) self->priv->metadata_sw, "style-set", (GCallback) _gmpc_metadata_browser_browser_bg_style_changed_gtk_widget_style_set, self, 0);
		_tmp23 = NULL;
		self->priv->metadata_box = (_tmp23 = g_object_ref_sink ((GtkEventBox*) gtk_event_box_new ()), (self->priv->metadata_box == NULL) ? NULL : (self->priv->metadata_box = (g_object_unref (self->priv->metadata_box), NULL)), _tmp23);
		gtk_event_box_set_visible_window (self->priv->metadata_box, TRUE);
		gtk_scrolled_window_add_with_viewport (self->priv->metadata_sw, (GtkWidget*) self->priv->metadata_box);
		gtk_paned_add2 (self->priv->paned, (GtkWidget*) self->priv->metadata_sw);
		gmpc_metadata_browser_reload_browsers (self);
		(box == NULL) ? NULL : (box = (g_object_unref (box), NULL));
		(sw == NULL) ? NULL : (sw = (g_object_unref (sw), NULL));
		(column == NULL) ? NULL : (column = (g_object_unref (column), NULL));
		(prenderer == NULL) ? NULL : (prenderer = (g_object_unref (prenderer), NULL));
		(trenderer == NULL) ? NULL : (trenderer = (g_object_unref (trenderer), NULL));
	}
	gtk_widget_show_all ((GtkWidget*) self->priv->paned);
}


static void gmpc_metadata_browser_reload_browsers (GmpcMetadataBrowser* self) {
	MpdData* data;
	MpdData* _tmp2;
	const MpdData* _tmp1;
	MpdData* _tmp0;
	MpdData* _tmp3;
	g_return_if_fail (self != NULL);
	if (self->priv->paned == NULL) {
		return;
	}
	gmpc_mpddata_model_set_mpd_data (self->priv->model_songs, NULL);
	gmpc_mpddata_model_set_mpd_data (self->priv->model_albums, NULL);
	gmpc_mpddata_model_set_mpd_data (self->priv->model_artist, NULL);
	gtk_entry_set_text (self->priv->artist_filter_entry, "");
	gtk_entry_set_text (self->priv->album_filter_entry, "");
	/* Fill in the first browser */
	mpd_database_search_field_start (connection, MPD_TAG_ITEM_ARTIST);
	data = mpd_database_search_commit (connection);
	_tmp2 = NULL;
	_tmp1 = NULL;
	_tmp0 = NULL;
	data = (_tmp2 = (_tmp1 = misc_sort_mpddata_by_album_disc_track ((_tmp0 = data, data = NULL, _tmp0)), (_tmp1 == NULL) ? NULL :  (_tmp1)), (data == NULL) ? NULL : (data = (mpd_data_free (data), NULL)), _tmp2);
	_tmp3 = NULL;
	gmpc_mpddata_model_set_mpd_data (self->priv->model_artist, (_tmp3 = data, data = NULL, _tmp3));
	(data == NULL) ? NULL : (data = (mpd_data_free (data), NULL));
}


static char* gmpc_metadata_browser_browser_get_selected_artist (GmpcMetadataBrowser* self) {
	GtkTreeIter iter = {0};
	GtkTreeSelection* _tmp0;
	GtkTreeSelection* sel;
	GtkTreeModel* model;
	GtkTreeModel* _tmp4;
	GtkTreeModel* _tmp3;
	gboolean _tmp2;
	GtkTreeModel* _tmp1;
	char* _tmp6;
	g_return_val_if_fail (self != NULL, NULL);
	_tmp0 = NULL;
	sel = (_tmp0 = gtk_tree_view_get_selection (self->priv->tree_artist), (_tmp0 == NULL) ? NULL : g_object_ref (_tmp0));
	model = NULL;
	/*this.model_artist;*/
	_tmp4 = NULL;
	_tmp3 = NULL;
	_tmp1 = NULL;
	if ((_tmp2 = gtk_tree_selection_get_selected (sel, &_tmp1, &iter), model = (_tmp3 = (_tmp4 = _tmp1, (_tmp4 == NULL) ? NULL : g_object_ref (_tmp4)), (model == NULL) ? NULL : (model = (g_object_unref (model), NULL)), _tmp3), _tmp2)) {
		char* artist;
		char* _tmp5;
		artist = NULL;
		gtk_tree_model_get (model, &iter, 7, &artist, -1, -1);
		_tmp5 = NULL;
		return (_tmp5 = artist, (sel == NULL) ? NULL : (sel = (g_object_unref (sel), NULL)), (model == NULL) ? NULL : (model = (g_object_unref (model), NULL)), _tmp5);
	}
	_tmp6 = NULL;
	return (_tmp6 = NULL, (sel == NULL) ? NULL : (sel = (g_object_unref (sel), NULL)), (model == NULL) ? NULL : (model = (g_object_unref (model), NULL)), _tmp6);
}


static char* gmpc_metadata_browser_browser_get_selected_album (GmpcMetadataBrowser* self) {
	GtkTreeIter iter = {0};
	GtkTreeSelection* _tmp0;
	GtkTreeSelection* sel;
	GtkTreeModel* model;
	GtkTreeModel* _tmp4;
	GtkTreeModel* _tmp3;
	gboolean _tmp2;
	GtkTreeModel* _tmp1;
	char* _tmp6;
	g_return_val_if_fail (self != NULL, NULL);
	_tmp0 = NULL;
	sel = (_tmp0 = gtk_tree_view_get_selection (self->priv->tree_album), (_tmp0 == NULL) ? NULL : g_object_ref (_tmp0));
	model = NULL;
	/*this.model_albums;*/
	_tmp4 = NULL;
	_tmp3 = NULL;
	_tmp1 = NULL;
	if ((_tmp2 = gtk_tree_selection_get_selected (sel, &_tmp1, &iter), model = (_tmp3 = (_tmp4 = _tmp1, (_tmp4 == NULL) ? NULL : g_object_ref (_tmp4)), (model == NULL) ? NULL : (model = (g_object_unref (model), NULL)), _tmp3), _tmp2)) {
		char* album;
		char* _tmp5;
		album = NULL;
		gtk_tree_model_get (model, &iter, 7, &album, -1, -1);
		_tmp5 = NULL;
		return (_tmp5 = album, (sel == NULL) ? NULL : (sel = (g_object_unref (sel), NULL)), (model == NULL) ? NULL : (model = (g_object_unref (model), NULL)), _tmp5);
	}
	_tmp6 = NULL;
	return (_tmp6 = NULL, (sel == NULL) ? NULL : (sel = (g_object_unref (sel), NULL)), (model == NULL) ? NULL : (model = (g_object_unref (model), NULL)), _tmp6);
}


static mpd_Song* gmpc_metadata_browser_browser_get_selected_song (GmpcMetadataBrowser* self) {
	GtkTreeIter iter = {0};
	GtkTreeSelection* _tmp0;
	GtkTreeSelection* sel;
	GmpcMpdDataModel* _tmp4;
	GmpcMpdDataModel* _tmp3;
	gboolean _tmp2;
	GtkTreeModel* _tmp1;
	mpd_Song* _tmp7;
	g_return_val_if_fail (self != NULL, NULL);
	_tmp0 = NULL;
	sel = (_tmp0 = gtk_tree_view_get_selection (self->priv->tree_songs), (_tmp0 == NULL) ? NULL : g_object_ref (_tmp0));
	_tmp4 = NULL;
	_tmp3 = NULL;
	_tmp1 = NULL;
	if ((_tmp2 = gtk_tree_selection_get_selected (sel, &_tmp1, &iter), self->priv->model_songs = (_tmp3 = (_tmp4 = (GmpcMpdDataModel*) _tmp1, (_tmp4 == NULL) ? NULL : g_object_ref (_tmp4)), (self->priv->model_songs == NULL) ? NULL : (self->priv->model_songs = (g_object_unref (self->priv->model_songs), NULL)), _tmp3), _tmp2)) {
		const mpd_Song* songs;
		const mpd_Song* _tmp5;
		mpd_Song* _tmp6;
		songs = NULL;
		gtk_tree_model_get ((GtkTreeModel*) self->priv->model_songs, &iter, 0, &songs, -1, -1);
		_tmp5 = NULL;
		_tmp6 = NULL;
		return (_tmp6 = (_tmp5 = songs, (_tmp5 == NULL) ? NULL : mpd_songDup (_tmp5)), (sel == NULL) ? NULL : (sel = (g_object_unref (sel), NULL)), _tmp6);
	}
	_tmp7 = NULL;
	return (_tmp7 = NULL, (sel == NULL) ? NULL : (sel = (g_object_unref (sel), NULL)), _tmp7);
}


static void gmpc_metadata_browser_browser_artist_changed (GmpcMetadataBrowser* self, GtkTreeSelection* sel) {
	char* artist;
	g_return_if_fail (self != NULL);
	g_return_if_fail (sel != NULL);
	gmpc_mpddata_model_set_mpd_data (self->priv->model_albums, NULL);
	gmpc_mpddata_model_set_mpd_data (self->priv->model_songs, NULL);
	gmpc_metadata_browser_metadata_box_clear (self);
	artist = gmpc_metadata_browser_browser_get_selected_artist (self);
	if (artist != NULL) {
		MpdData* data;
		MpdData* _tmp2;
		const MpdData* _tmp1;
		MpdData* _tmp0;
		MpdData* _tmp3;
		MpdData* _tmp4;
		MpdData* _tmp7;
		const MpdData* _tmp6;
		MpdData* _tmp5;
		MpdData* _tmp8;
		/* Fill in the first browser */
		mpd_database_search_field_start (connection, MPD_TAG_ITEM_ALBUM);
		mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ARTIST, artist);
		data = mpd_database_search_commit (connection);
		_tmp2 = NULL;
		_tmp1 = NULL;
		_tmp0 = NULL;
		data = (_tmp2 = (_tmp1 = misc_sort_mpddata_by_album_disc_track ((_tmp0 = data, data = NULL, _tmp0)), (_tmp1 == NULL) ? NULL :  (_tmp1)), (data == NULL) ? NULL : (data = (mpd_data_free (data), NULL)), _tmp2);
		gmpc_mpddata_model_set_request_artist (self->priv->model_albums, artist);
		_tmp3 = NULL;
		gmpc_mpddata_model_set_mpd_data (self->priv->model_albums, (_tmp3 = data, data = NULL, _tmp3));
		mpd_database_search_start (connection, TRUE);
		mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ARTIST, artist);
		_tmp4 = NULL;
		data = (_tmp4 = mpd_database_search_commit (connection), (data == NULL) ? NULL : (data = (mpd_data_free (data), NULL)), _tmp4);
		_tmp7 = NULL;
		_tmp6 = NULL;
		_tmp5 = NULL;
		data = (_tmp7 = (_tmp6 = misc_sort_mpddata_by_album_disc_track ((_tmp5 = data, data = NULL, _tmp5)), (_tmp6 == NULL) ? NULL :  (_tmp6)), (data == NULL) ? NULL : (data = (mpd_data_free (data), NULL)), _tmp7);
		_tmp8 = NULL;
		gmpc_mpddata_model_set_mpd_data (self->priv->model_songs, (_tmp8 = data, data = NULL, _tmp8));
		(data == NULL) ? NULL : (data = (mpd_data_free (data), NULL));
	}
	gmpc_metadata_browser_metadata_box_update (self);
	artist = (g_free (artist), NULL);
}


static void gmpc_metadata_browser_browser_album_changed (GmpcMetadataBrowser* self, GtkTreeSelection* album_sel) {
	char* artist;
	g_return_if_fail (self != NULL);
	g_return_if_fail (album_sel != NULL);
	gmpc_mpddata_model_set_mpd_data (self->priv->model_songs, NULL);
	gmpc_metadata_browser_metadata_box_clear (self);
	artist = gmpc_metadata_browser_browser_get_selected_artist (self);
	if (artist != NULL) {
		char* album;
		MpdData* data;
		MpdData* _tmp2;
		const MpdData* _tmp1;
		MpdData* _tmp0;
		MpdData* _tmp3;
		album = gmpc_metadata_browser_browser_get_selected_album (self);
		/* Fill in the first browser */
		mpd_database_search_start (connection, TRUE);
		mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ARTIST, artist);
		if (album != NULL) {
			mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ALBUM, album);
		}
		data = mpd_database_search_commit (connection);
		_tmp2 = NULL;
		_tmp1 = NULL;
		_tmp0 = NULL;
		data = (_tmp2 = (_tmp1 = misc_sort_mpddata_by_album_disc_track ((_tmp0 = data, data = NULL, _tmp0)), (_tmp1 == NULL) ? NULL :  (_tmp1)), (data == NULL) ? NULL : (data = (mpd_data_free (data), NULL)), _tmp2);
		_tmp3 = NULL;
		gmpc_mpddata_model_set_mpd_data (self->priv->model_songs, (_tmp3 = data, data = NULL, _tmp3));
		album = (g_free (album), NULL);
		(data == NULL) ? NULL : (data = (mpd_data_free (data), NULL));
	}
	gmpc_metadata_browser_metadata_box_update (self);
	artist = (g_free (artist), NULL);
}


static void gmpc_metadata_browser_browser_songs_changed (GmpcMetadataBrowser* self, GtkTreeSelection* song_sel) {
	g_return_if_fail (self != NULL);
	g_return_if_fail (song_sel != NULL);
	gmpc_metadata_browser_metadata_box_clear (self);
	gmpc_metadata_browser_metadata_box_update (self);
}


/** 
     * Metadata box
     */
static void gmpc_metadata_browser_play_selected_song (GmpcMetadataBrowser* self, GtkButton* button) {
	mpd_Song* song;
	g_return_if_fail (self != NULL);
	g_return_if_fail (button != NULL);
	song = gmpc_metadata_browser_browser_get_selected_song (self);
	if (song != NULL) {
		play_path (song->file);
	}
	(song == NULL) ? NULL : (song = (mpd_freeSong (song), NULL));
}


static void gmpc_metadata_browser_add_selected_song (GmpcMetadataBrowser* self, GtkButton* button) {
	char* artist;
	char* album;
	mpd_Song* song;
	g_return_if_fail (self != NULL);
	g_return_if_fail (button != NULL);
	artist = gmpc_metadata_browser_browser_get_selected_artist (self);
	album = gmpc_metadata_browser_browser_get_selected_album (self);
	song = gmpc_metadata_browser_browser_get_selected_song (self);
	if (song != NULL) {
		mpd_playlist_add (connection, song->file);
	}
	if (artist != NULL) {
		MpdData* data;
		mpd_database_search_field_start (connection, MPD_TAG_ITEM_FILENAME);
		mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ARTIST, artist);
		if (album != NULL) {
			mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ALBUM, album);
		}
		data = mpd_database_search_commit (connection);
		if (data != NULL) {
			const MpdData* iter;
			iter = mpd_data_get_first (data);
			do {
				mpd_playlist_queue_add (connection, iter->tag);
			} while ((iter = mpd_data_get_next_real (iter, FALSE)) != NULL);
			mpd_playlist_queue_commit (connection);
		}
		(data == NULL) ? NULL : (data = (mpd_data_free (data), NULL));
	}
	artist = (g_free (artist), NULL);
	album = (g_free (album), NULL);
	(song == NULL) ? NULL : (song = (mpd_freeSong (song), NULL));
}


static void gmpc_metadata_browser_replace_selected_song (GmpcMetadataBrowser* self, GtkButton* button) {
	char* artist;
	char* album;
	mpd_Song* song;
	g_return_if_fail (self != NULL);
	g_return_if_fail (button != NULL);
	artist = gmpc_metadata_browser_browser_get_selected_artist (self);
	album = gmpc_metadata_browser_browser_get_selected_album (self);
	song = gmpc_metadata_browser_browser_get_selected_song (self);
	if (song != NULL) {
		mpd_playlist_clear (connection);
		mpd_playlist_add (connection, song->file);
	}
	if (artist != NULL) {
		MpdData* data;
		mpd_database_search_field_start (connection, MPD_TAG_ITEM_FILENAME);
		mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ARTIST, artist);
		if (album != NULL) {
			mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ALBUM, album);
		}
		data = mpd_database_search_commit (connection);
		if (data != NULL) {
			const MpdData* iter;
			iter = mpd_data_get_first (data);
			mpd_playlist_clear (connection);
			do {
				mpd_playlist_queue_add (connection, iter->tag);
			} while ((iter = mpd_data_get_next_real (iter, FALSE)) != NULL);
			mpd_playlist_queue_commit (connection);
			mpd_player_play (connection);
		}
		(data == NULL) ? NULL : (data = (mpd_data_free (data), NULL));
	}
	artist = (g_free (artist), NULL);
	album = (g_free (album), NULL);
	(song == NULL) ? NULL : (song = (mpd_freeSong (song), NULL));
}


static void gmpc_metadata_browser_metadata_box_clear (GmpcMetadataBrowser* self) {
	GList* list;
	g_return_if_fail (self != NULL);
	list = gtk_container_get_children ((GtkContainer*) self->priv->metadata_box);
	{
		GList* child_collection;
		GList* child_it;
		child_collection = list;
		for (child_it = child_collection; child_it != NULL; child_it = child_it->next) {
			GtkWidget* _tmp0;
			GtkWidget* child;
			_tmp0 = NULL;
			child = (_tmp0 = (GtkWidget*) child_it->data, (_tmp0 == NULL) ? NULL : g_object_ref (_tmp0));
			{
				gtk_object_destroy ((GtkObject*) child);
				(child == NULL) ? NULL : (child = (g_object_unref (child), NULL));
			}
		}
	}
	(list == NULL) ? NULL : (list = (g_list_free (list), NULL));
}


static void _gmpc_metadata_browser_play_selected_song_gtk_button_clicked (GtkButton* _sender, gpointer self) {
	gmpc_metadata_browser_play_selected_song (self, _sender);
}


static void _gmpc_metadata_browser_add_selected_song_gtk_button_clicked (GtkButton* _sender, gpointer self) {
	gmpc_metadata_browser_add_selected_song (self, _sender);
}


static void _gmpc_metadata_browser_replace_selected_song_gtk_button_clicked (GtkButton* _sender, gpointer self) {
	gmpc_metadata_browser_replace_selected_song (self, _sender);
}


static void gmpc_metadata_browser_metadata_box_show_song (GmpcMetadataBrowser* self, const mpd_Song* song) {
	GtkVBox* vbox;
	GtkLabel* label;
	char* _tmp0;
	GtkHBox* hbox;
	GtkAlignment* ali;
	GmpcMetaImage* artist_image;
	GtkTable* info_box;
	gint i;
	GtkLabel* pt_label;
	GtkLabel* _tmp1;
	char* _tmp2;
	GtkLabel* _tmp6;
	GtkLabel* _tmp7;
	char* _tmp8;
	GmpcFavoritesButton* fav_button;
	GtkLabel* _tmp27;
	char* _tmp28;
	GtkAlignment* _tmp29;
	GtkButton* button;
	GtkHBox* _tmp33;
	GtkButton* _tmp34;
	GtkButton* _tmp35;
	GtkImage* _tmp36;
	GmpcMetaTextView* text_view;
	char* _tmp37;
	GmpcWidgetMore* _tmp38;
	GmpcWidgetMore* frame;
	GmpcSongLinks* song_links;
	g_return_if_fail (self != NULL);
	g_return_if_fail (song != NULL);
	vbox = g_object_ref_sink ((GtkVBox*) gtk_vbox_new (FALSE, 6));
	gtk_container_set_border_width ((GtkContainer*) vbox, (guint) 8);
	label = g_object_ref_sink ((GtkLabel*) gtk_label_new (""));
	_tmp0 = NULL;
	gtk_label_set_markup (label, _tmp0 = g_markup_printf_escaped ("<span size='xx-large' weight='bold'>%s</span>", song->title));
	_tmp0 = (g_free (_tmp0), NULL);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) label, FALSE, FALSE, (guint) 0);
	/* Artist image */
	hbox = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 6));
	ali = g_object_ref_sink ((GtkAlignment*) gtk_alignment_new (0.f, 0.f, 0.f, 0.f));
	artist_image = g_object_ref_sink (gmpc_metaimage_new_size (META_ALBUM_ART, 250));
	gmpc_metaimage_set_squared (artist_image, FALSE);
	gmpc_metaimage_update_cover_from_song (artist_image, song);
	gtk_container_add ((GtkContainer*) ali, (GtkWidget*) artist_image);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) ali, FALSE, FALSE, (guint) 0);
	/* Artist information */
	info_box = g_object_ref_sink ((GtkTable*) gtk_table_new ((guint) 4, (guint) 2, FALSE));
	gtk_table_set_row_spacings (info_box, (guint) 3);
	gtk_table_set_col_spacings (info_box, (guint) 8);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) info_box, FALSE, FALSE, (guint) 0);
	i = 0;
	/* Artist label */
	pt_label = g_object_ref_sink ((GtkLabel*) gtk_label_new (song->artist));
	_tmp1 = NULL;
	label = (_tmp1 = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp1);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
	_tmp2 = NULL;
	gtk_label_set_markup (label, _tmp2 = g_markup_printf_escaped ("<b>%s:</b>", _ ("Artist")));
	_tmp2 = (g_free (_tmp2), NULL);
	gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
	gtk_label_set_line_wrap (pt_label, TRUE);
	gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	i++;
	/* AlbumArtist label */
	if (song->albumartist != NULL) {
		GtkLabel* _tmp3;
		GtkLabel* _tmp4;
		char* _tmp5;
		_tmp3 = NULL;
		pt_label = (_tmp3 = g_object_ref_sink ((GtkLabel*) gtk_label_new (song->albumartist)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), _tmp3);
		_tmp4 = NULL;
		label = (_tmp4 = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp4);
		gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
		_tmp5 = NULL;
		gtk_label_set_markup (label, _tmp5 = g_markup_printf_escaped ("<b>%s:</b>", _ ("Album artist")));
		_tmp5 = (g_free (_tmp5), NULL);
		gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
		gtk_label_set_line_wrap (pt_label, TRUE);
		gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		i++;
	}
	/* Album */
	_tmp6 = NULL;
	pt_label = (_tmp6 = g_object_ref_sink ((GtkLabel*) gtk_label_new (song->album)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), _tmp6);
	_tmp7 = NULL;
	label = (_tmp7 = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp7);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
	_tmp8 = NULL;
	gtk_label_set_markup (label, _tmp8 = g_markup_printf_escaped ("<b>%s:</b>", _ ("Album")));
	_tmp8 = (g_free (_tmp8), NULL);
	gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
	gtk_label_set_line_wrap (pt_label, TRUE);
	gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	i++;
	/* track */
	if (song->track != NULL) {
		GtkLabel* _tmp9;
		GtkLabel* _tmp10;
		char* _tmp11;
		_tmp9 = NULL;
		pt_label = (_tmp9 = g_object_ref_sink ((GtkLabel*) gtk_label_new (song->track)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), _tmp9);
		_tmp10 = NULL;
		label = (_tmp10 = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp10);
		gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
		_tmp11 = NULL;
		gtk_label_set_markup (label, _tmp11 = g_markup_printf_escaped ("<b>%s:</b>", _ ("Track")));
		_tmp11 = (g_free (_tmp11), NULL);
		gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
		gtk_label_set_line_wrap (pt_label, TRUE);
		gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		i++;
	}
	/* date */
	if (song->date != NULL) {
		GtkLabel* _tmp12;
		GtkLabel* _tmp13;
		char* _tmp14;
		_tmp12 = NULL;
		pt_label = (_tmp12 = g_object_ref_sink ((GtkLabel*) gtk_label_new (song->date)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), _tmp12);
		_tmp13 = NULL;
		label = (_tmp13 = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp13);
		gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
		_tmp14 = NULL;
		gtk_label_set_markup (label, _tmp14 = g_markup_printf_escaped ("<b>%s:</b>", _ ("Date")));
		_tmp14 = (g_free (_tmp14), NULL);
		gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
		gtk_label_set_line_wrap (pt_label, TRUE);
		gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		i++;
	}
	/* performer */
	if (song->performer != NULL) {
		GtkLabel* _tmp15;
		GtkLabel* _tmp16;
		char* _tmp17;
		_tmp15 = NULL;
		pt_label = (_tmp15 = g_object_ref_sink ((GtkLabel*) gtk_label_new (song->performer)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), _tmp15);
		_tmp16 = NULL;
		label = (_tmp16 = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp16);
		gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
		_tmp17 = NULL;
		gtk_label_set_markup (label, _tmp17 = g_markup_printf_escaped ("<b>%s:</b>", _ ("Performer")));
		_tmp17 = (g_free (_tmp17), NULL);
		gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
		gtk_label_set_line_wrap (pt_label, TRUE);
		gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		i++;
	}
	/* disc */
	if (song->disc != NULL) {
		GtkLabel* _tmp18;
		GtkLabel* _tmp19;
		char* _tmp20;
		_tmp18 = NULL;
		pt_label = (_tmp18 = g_object_ref_sink ((GtkLabel*) gtk_label_new (song->disc)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), _tmp18);
		_tmp19 = NULL;
		label = (_tmp19 = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp19);
		gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
		_tmp20 = NULL;
		gtk_label_set_markup (label, _tmp20 = g_markup_printf_escaped ("<b>%s:</b>", _ ("Disc")));
		_tmp20 = (g_free (_tmp20), NULL);
		gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
		gtk_label_set_line_wrap (pt_label, TRUE);
		gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		i++;
	}
	/* Genre */
	if (song->genre != NULL) {
		GtkLabel* _tmp21;
		GtkLabel* _tmp22;
		char* _tmp23;
		_tmp21 = NULL;
		pt_label = (_tmp21 = g_object_ref_sink ((GtkLabel*) gtk_label_new (song->genre)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), _tmp21);
		_tmp22 = NULL;
		label = (_tmp22 = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp22);
		gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
		_tmp23 = NULL;
		gtk_label_set_markup (label, _tmp23 = g_markup_printf_escaped ("<b>%s:</b>", _ ("Genre")));
		_tmp23 = (g_free (_tmp23), NULL);
		gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
		gtk_label_set_line_wrap (pt_label, TRUE);
		gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		i++;
	}
	/* Genre */
	if (song->file != NULL) {
		GtkLabel* _tmp24;
		GtkLabel* _tmp25;
		char* _tmp26;
		_tmp24 = NULL;
		pt_label = (_tmp24 = g_object_ref_sink ((GtkLabel*) gtk_label_new (song->file)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), _tmp24);
		_tmp25 = NULL;
		label = (_tmp25 = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp25);
		gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.0f);
		_tmp26 = NULL;
		gtk_label_set_markup (label, _tmp26 = g_markup_printf_escaped ("<b>%s:</b>", _ ("Path")));
		_tmp26 = (g_free (_tmp26), NULL);
		gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
		gtk_label_set_line_wrap (pt_label, TRUE);
		gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		i++;
	}
	/* Favored button */
	fav_button = g_object_ref_sink (gmpc_favorites_button_new ());
	gmpc_favorites_button_set_song (fav_button, song);
	_tmp27 = NULL;
	label = (_tmp27 = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp27);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
	_tmp28 = NULL;
	gtk_label_set_markup (label, _tmp28 = g_markup_printf_escaped ("<b>%s:</b>", _ ("Favored")));
	_tmp28 = (g_free (_tmp28), NULL);
	gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	_tmp29 = NULL;
	ali = (_tmp29 = g_object_ref_sink ((GtkAlignment*) gtk_alignment_new (0.0f, 0.5f, 0.f, 0.f)), (ali == NULL) ? NULL : (ali = (g_object_unref (ali), NULL)), _tmp29);
	gtk_container_add ((GtkContainer*) ali, (GtkWidget*) fav_button);
	gtk_table_attach (info_box, (GtkWidget*) ali, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	i++;
	/* Comment */
	if (song->comment != NULL) {
		GtkLabel* _tmp30;
		GtkLabel* _tmp31;
		char* _tmp32;
		_tmp30 = NULL;
		pt_label = (_tmp30 = g_object_ref_sink ((GtkLabel*) gtk_label_new (song->comment)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), _tmp30);
		_tmp31 = NULL;
		label = (_tmp31 = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp31);
		gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.0f);
		_tmp32 = NULL;
		gtk_label_set_markup (label, _tmp32 = g_markup_printf_escaped ("<b>%s:</b>", _ ("Comment")));
		_tmp32 = (g_free (_tmp32), NULL);
		gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
		gtk_label_set_line_wrap (pt_label, TRUE);
		gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		i++;
	}
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) hbox, FALSE, FALSE, (guint) 0);
	/* Player controls */
	button = g_object_ref_sink ((GtkButton*) gtk_button_new_from_stock ("gtk-media-play"));
	gtk_button_set_relief (button, GTK_RELIEF_NONE);
	g_signal_connect_object (button, "clicked", (GCallback) _gmpc_metadata_browser_play_selected_song_gtk_button_clicked, self, 0);
	_tmp33 = NULL;
	hbox = (_tmp33 = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 6)), (hbox == NULL) ? NULL : (hbox = (g_object_unref (hbox), NULL)), _tmp33);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) button, FALSE, FALSE, (guint) 0);
	_tmp34 = NULL;
	button = (_tmp34 = g_object_ref_sink ((GtkButton*) gtk_button_new_from_stock ("gtk-add")), (button == NULL) ? NULL : (button = (g_object_unref (button), NULL)), _tmp34);
	gtk_button_set_relief (button, GTK_RELIEF_NONE);
	g_signal_connect_object (button, "clicked", (GCallback) _gmpc_metadata_browser_add_selected_song_gtk_button_clicked, self, 0);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) button, FALSE, FALSE, (guint) 0);
	_tmp35 = NULL;
	button = (_tmp35 = g_object_ref_sink ((GtkButton*) gtk_button_new_with_mnemonic ("_Replace")), (button == NULL) ? NULL : (button = (g_object_unref (button), NULL)), _tmp35);
	_tmp36 = NULL;
	gtk_button_set_image (button, (GtkWidget*) (_tmp36 = g_object_ref_sink ((GtkImage*) gtk_image_new_from_stock ("gtk-redo", GTK_ICON_SIZE_BUTTON))));
	(_tmp36 == NULL) ? NULL : (_tmp36 = (g_object_unref (_tmp36), NULL));
	gtk_button_set_relief (button, GTK_RELIEF_NONE);
	g_signal_connect_object (button, "clicked", (GCallback) _gmpc_metadata_browser_replace_selected_song_gtk_button_clicked, self, 0);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) button, FALSE, FALSE, (guint) 0);
	gtk_table_attach (info_box, (GtkWidget*) hbox, (guint) 0, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	i++;
	/* Lyrics */
	text_view = g_object_ref_sink (gmpc_meta_text_view_new (META_SONG_TXT));
	gtk_text_view_set_left_margin ((GtkTextView*) text_view, 8);
	_tmp37 = NULL;
	_tmp38 = NULL;
	frame = (_tmp38 = g_object_ref_sink (gmpc_widget_more_new (_tmp37 = g_markup_printf_escaped ("<b>%s:</b>", _ ("Lyrics")), (GtkWidget*) text_view)), _tmp37 = (g_free (_tmp37), NULL), _tmp38);
	gmpc_meta_text_view_query_text_from_song (text_view, song);
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) frame, FALSE, FALSE, (guint) 0);
	song_links = g_object_ref_sink (gmpc_song_links_new (GMPC_SONG_LINKS_TYPE_SONG, song));
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) song_links, FALSE, FALSE, (guint) 0);
	/**
	         * Add it to the view
	         */
	gtk_container_add ((GtkContainer*) self->priv->metadata_box, (GtkWidget*) vbox);
	gtk_widget_show_all ((GtkWidget*) self->priv->metadata_sw);
	(vbox == NULL) ? NULL : (vbox = (g_object_unref (vbox), NULL));
	(label == NULL) ? NULL : (label = (g_object_unref (label), NULL));
	(hbox == NULL) ? NULL : (hbox = (g_object_unref (hbox), NULL));
	(ali == NULL) ? NULL : (ali = (g_object_unref (ali), NULL));
	(artist_image == NULL) ? NULL : (artist_image = (g_object_unref (artist_image), NULL));
	(info_box == NULL) ? NULL : (info_box = (g_object_unref (info_box), NULL));
	(pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL));
	(fav_button == NULL) ? NULL : (fav_button = (g_object_unref (fav_button), NULL));
	(button == NULL) ? NULL : (button = (g_object_unref (button), NULL));
	(text_view == NULL) ? NULL : (text_view = (g_object_unref (text_view), NULL));
	(frame == NULL) ? NULL : (frame = (g_object_unref (frame), NULL));
	(song_links == NULL) ? NULL : (song_links = (g_object_unref (song_links), NULL));
}


static void gmpc_metadata_browser_metadata_box_show_album (GmpcMetadataBrowser* self, const char* artist, const char* album) {
	GtkVBox* vbox;
	GtkLabel* label;
	char* _tmp0;
	GtkHBox* hbox;
	GtkAlignment* ali;
	GmpcMetaImage* artist_image;
	mpd_Song* song;
	char* _tmp2;
	const char* _tmp1;
	char* _tmp4;
	const char* _tmp3;
	GtkTable* info_box;
	gint i;
	GmpcStatsLabel* pt_label;
	GtkLabel* _tmp5;
	char* _tmp6;
	GmpcStatsLabel* _tmp7;
	GtkLabel* _tmp8;
	char* _tmp9;
	GmpcStatsLabel* _tmp10;
	GtkLabel* _tmp11;
	char* _tmp12;
	GmpcStatsLabel* _tmp13;
	GtkLabel* _tmp14;
	char* _tmp15;
	GtkHBox* _tmp16;
	GtkButton* button;
	GtkButton* _tmp17;
	GtkImage* _tmp18;
	GmpcMetaTextView* text_view;
	char* _tmp19;
	GmpcWidgetMore* _tmp20;
	GmpcWidgetMore* frame;
	GmpcSongLinks* song_links;
	g_return_if_fail (self != NULL);
	g_return_if_fail (artist != NULL);
	g_return_if_fail (album != NULL);
	vbox = g_object_ref_sink ((GtkVBox*) gtk_vbox_new (FALSE, 6));
	gtk_container_set_border_width ((GtkContainer*) vbox, (guint) 8);
	label = g_object_ref_sink ((GtkLabel*) gtk_label_new (""));
	_tmp0 = NULL;
	gtk_label_set_markup (label, _tmp0 = g_markup_printf_escaped ("<span size='xx-large' weight='bold'>%s - %s</span>", artist, album));
	_tmp0 = (g_free (_tmp0), NULL);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) label, FALSE, FALSE, (guint) 0);
	/* Artist image */
	hbox = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 6));
	ali = g_object_ref_sink ((GtkAlignment*) gtk_alignment_new (0.f, 0.f, 0.f, 0.f));
	artist_image = g_object_ref_sink (gmpc_metaimage_new_size (META_ALBUM_ART, 250));
	gmpc_metaimage_set_squared (artist_image, FALSE);
	song = mpd_newSong ();
	_tmp2 = NULL;
	_tmp1 = NULL;
	song->artist = (_tmp2 = (_tmp1 = artist, (_tmp1 == NULL) ? NULL : g_strdup (_tmp1)), song->artist = (g_free (song->artist), NULL), _tmp2);
	_tmp4 = NULL;
	_tmp3 = NULL;
	song->album = (_tmp4 = (_tmp3 = album, (_tmp3 == NULL) ? NULL : g_strdup (_tmp3)), song->album = (g_free (song->album), NULL), _tmp4);
	gmpc_metaimage_update_cover_from_song (artist_image, song);
	gtk_container_add ((GtkContainer*) ali, (GtkWidget*) artist_image);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) ali, FALSE, FALSE, (guint) 0);
	/* Artist information */
	info_box = g_object_ref_sink ((GtkTable*) gtk_table_new ((guint) 4, (guint) 2, FALSE));
	gtk_table_set_row_spacings (info_box, (guint) 3);
	gtk_table_set_col_spacings (info_box, (guint) 8);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) info_box, FALSE, FALSE, (guint) 0);
	i = 0;
	/* Genres of songs */
	pt_label = g_object_ref_sink (gmpc_stats_label_new (ALBUM_GENRES_SONGS, song));
	_tmp5 = NULL;
	label = (_tmp5 = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp5);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
	_tmp6 = NULL;
	gtk_label_set_markup (label, _tmp6 = g_markup_printf_escaped ("<b>%s:</b>", _ ("Genres")));
	_tmp6 = (g_free (_tmp6), NULL);
	gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
	gtk_label_set_line_wrap ((GtkLabel*) pt_label, TRUE);
	gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	i++;
	/* Dates of songs */
	_tmp7 = NULL;
	pt_label = (_tmp7 = g_object_ref_sink (gmpc_stats_label_new (ALBUM_DATES_SONGS, song)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), _tmp7);
	gtk_label_set_line_wrap ((GtkLabel*) pt_label, TRUE);
	_tmp8 = NULL;
	label = (_tmp8 = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp8);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.0f);
	_tmp9 = NULL;
	gtk_label_set_markup (label, _tmp9 = g_markup_printf_escaped ("<b>%s:</b>", _ ("Dates")));
	_tmp9 = (g_free (_tmp9), NULL);
	gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
	gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	i++;
	/* Total number of songs */
	_tmp10 = NULL;
	pt_label = (_tmp10 = g_object_ref_sink (gmpc_stats_label_new (ALBUM_NUM_SONGS, song)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), _tmp10);
	gtk_label_set_line_wrap ((GtkLabel*) pt_label, TRUE);
	_tmp11 = NULL;
	label = (_tmp11 = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp11);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.0f);
	_tmp12 = NULL;
	gtk_label_set_markup (label, _tmp12 = g_markup_printf_escaped ("<b>%s:</b>", _ ("Songs")));
	_tmp12 = (g_free (_tmp12), NULL);
	gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
	gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	i++;
	/* Total playtime */
	_tmp13 = NULL;
	pt_label = (_tmp13 = g_object_ref_sink (gmpc_stats_label_new (ALBUM_PLAYTIME_SONGS, song)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), _tmp13);
	gtk_label_set_line_wrap ((GtkLabel*) pt_label, TRUE);
	_tmp14 = NULL;
	label = (_tmp14 = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp14);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.0f);
	_tmp15 = NULL;
	gtk_label_set_markup (label, _tmp15 = g_markup_printf_escaped ("<b>%s:</b>", _ ("Playtime")));
	_tmp15 = (g_free (_tmp15), NULL);
	gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
	gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	i++;
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) hbox, FALSE, FALSE, (guint) 0);
	/* Player controls */
	_tmp16 = NULL;
	hbox = (_tmp16 = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 6)), (hbox == NULL) ? NULL : (hbox = (g_object_unref (hbox), NULL)), _tmp16);
	button = g_object_ref_sink ((GtkButton*) gtk_button_new_from_stock ("gtk-add"));
	gtk_button_set_relief (button, GTK_RELIEF_NONE);
	g_signal_connect_object (button, "clicked", (GCallback) _gmpc_metadata_browser_add_selected_song_gtk_button_clicked, self, 0);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) button, FALSE, FALSE, (guint) 0);
	_tmp17 = NULL;
	button = (_tmp17 = g_object_ref_sink ((GtkButton*) gtk_button_new_with_mnemonic ("_Replace")), (button == NULL) ? NULL : (button = (g_object_unref (button), NULL)), _tmp17);
	_tmp18 = NULL;
	gtk_button_set_image (button, (GtkWidget*) (_tmp18 = g_object_ref_sink ((GtkImage*) gtk_image_new_from_stock ("gtk-redo", GTK_ICON_SIZE_BUTTON))));
	(_tmp18 == NULL) ? NULL : (_tmp18 = (g_object_unref (_tmp18), NULL));
	gtk_button_set_relief (button, GTK_RELIEF_NONE);
	g_signal_connect_object (button, "clicked", (GCallback) _gmpc_metadata_browser_replace_selected_song_gtk_button_clicked, self, 0);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) button, FALSE, FALSE, (guint) 0);
	gtk_table_attach (info_box, (GtkWidget*) hbox, (guint) 0, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	i++;
	text_view = g_object_ref_sink (gmpc_meta_text_view_new (META_ALBUM_TXT));
	gtk_text_view_set_left_margin ((GtkTextView*) text_view, 8);
	_tmp19 = NULL;
	_tmp20 = NULL;
	frame = (_tmp20 = g_object_ref_sink (gmpc_widget_more_new (_tmp19 = g_markup_printf_escaped ("<b>%s:</b>", _ ("Album information")), (GtkWidget*) text_view)), _tmp19 = (g_free (_tmp19), NULL), _tmp20);
	gmpc_meta_text_view_query_text_from_song (text_view, song);
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) frame, FALSE, FALSE, (guint) 0);
	song_links = g_object_ref_sink (gmpc_song_links_new (GMPC_SONG_LINKS_TYPE_ALBUM, song));
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) song_links, FALSE, FALSE, (guint) 0);
	/**
	         * Add it to the view
	         */
	gtk_container_add ((GtkContainer*) self->priv->metadata_box, (GtkWidget*) vbox);
	gtk_widget_show_all ((GtkWidget*) self->priv->metadata_sw);
	(vbox == NULL) ? NULL : (vbox = (g_object_unref (vbox), NULL));
	(label == NULL) ? NULL : (label = (g_object_unref (label), NULL));
	(hbox == NULL) ? NULL : (hbox = (g_object_unref (hbox), NULL));
	(ali == NULL) ? NULL : (ali = (g_object_unref (ali), NULL));
	(artist_image == NULL) ? NULL : (artist_image = (g_object_unref (artist_image), NULL));
	(song == NULL) ? NULL : (song = (mpd_freeSong (song), NULL));
	(info_box == NULL) ? NULL : (info_box = (g_object_unref (info_box), NULL));
	(pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL));
	(button == NULL) ? NULL : (button = (g_object_unref (button), NULL));
	(text_view == NULL) ? NULL : (text_view = (g_object_unref (text_view), NULL));
	(frame == NULL) ? NULL : (frame = (g_object_unref (frame), NULL));
	(song_links == NULL) ? NULL : (song_links = (g_object_unref (song_links), NULL));
}


static void gmpc_metadata_browser_metadata_box_show_artist (GmpcMetadataBrowser* self, const char* artist) {
	GtkVBox* vbox;
	GtkLabel* label;
	char* _tmp0;
	GtkHBox* hbox;
	GtkAlignment* ali;
	GmpcMetaImage* artist_image;
	mpd_Song* song;
	char* _tmp2;
	const char* _tmp1;
	GtkTable* info_box;
	gint i;
	GmpcStatsLabel* pt_label;
	GtkLabel* _tmp3;
	char* _tmp4;
	GmpcStatsLabel* _tmp5;
	GtkLabel* _tmp6;
	char* _tmp7;
	GmpcStatsLabel* _tmp8;
	GtkLabel* _tmp9;
	char* _tmp10;
	GmpcStatsLabel* _tmp11;
	GtkLabel* _tmp12;
	char* _tmp13;
	GtkHBox* _tmp14;
	GtkButton* button;
	GtkButton* _tmp15;
	GtkImage* _tmp16;
	GmpcMetaTextView* text_view;
	char* _tmp17;
	GmpcWidgetMore* _tmp18;
	GmpcWidgetMore* frame;
	GmpcSongLinks* song_links;
	g_return_if_fail (self != NULL);
	g_return_if_fail (artist != NULL);
	vbox = g_object_ref_sink ((GtkVBox*) gtk_vbox_new (FALSE, 6));
	gtk_container_set_border_width ((GtkContainer*) vbox, (guint) 8);
	label = g_object_ref_sink ((GtkLabel*) gtk_label_new (""));
	_tmp0 = NULL;
	gtk_label_set_markup (label, _tmp0 = g_markup_printf_escaped ("<span size='xx-large' weight='bold'>%s</span>", artist));
	_tmp0 = (g_free (_tmp0), NULL);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) label, FALSE, FALSE, (guint) 0);
	/* Artist image */
	hbox = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 6));
	ali = g_object_ref_sink ((GtkAlignment*) gtk_alignment_new (0.f, 0.f, 0.f, 0.f));
	artist_image = g_object_ref_sink (gmpc_metaimage_new_size (META_ARTIST_ART, 250));
	gmpc_metaimage_set_squared (artist_image, FALSE);
	song = mpd_newSong ();
	_tmp2 = NULL;
	_tmp1 = NULL;
	song->artist = (_tmp2 = (_tmp1 = artist, (_tmp1 == NULL) ? NULL : g_strdup (_tmp1)), song->artist = (g_free (song->artist), NULL), _tmp2);
	gmpc_metaimage_update_cover_from_song (artist_image, song);
	gtk_container_add ((GtkContainer*) ali, (GtkWidget*) artist_image);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) ali, FALSE, FALSE, (guint) 0);
	/* Artist information */
	info_box = g_object_ref_sink ((GtkTable*) gtk_table_new ((guint) 4, (guint) 2, FALSE));
	gtk_table_set_row_spacings (info_box, (guint) 3);
	gtk_table_set_col_spacings (info_box, (guint) 8);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) info_box, FALSE, FALSE, (guint) 0);
	i = 0;
	/* Genres of songs */
	pt_label = g_object_ref_sink (gmpc_stats_label_new (ARTIST_GENRES_SONGS, song));
	_tmp3 = NULL;
	label = (_tmp3 = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp3);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
	_tmp4 = NULL;
	gtk_label_set_markup (label, _tmp4 = g_markup_printf_escaped ("<b>%s:</b>", _ ("Genres")));
	_tmp4 = (g_free (_tmp4), NULL);
	gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
	gtk_label_set_line_wrap ((GtkLabel*) pt_label, TRUE);
	gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	i++;
	/* Dates of songs */
	_tmp5 = NULL;
	pt_label = (_tmp5 = g_object_ref_sink (gmpc_stats_label_new (ARTIST_DATES_SONGS, song)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), _tmp5);
	gtk_label_set_line_wrap ((GtkLabel*) pt_label, TRUE);
	_tmp6 = NULL;
	label = (_tmp6 = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp6);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.0f);
	_tmp7 = NULL;
	gtk_label_set_markup (label, _tmp7 = g_markup_printf_escaped ("<b>%s:</b>", _ ("Dates")));
	_tmp7 = (g_free (_tmp7), NULL);
	gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
	gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	i++;
	/* Total number of songs */
	_tmp8 = NULL;
	pt_label = (_tmp8 = g_object_ref_sink (gmpc_stats_label_new (ARTIST_NUM_SONGS, song)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), _tmp8);
	gtk_label_set_line_wrap ((GtkLabel*) pt_label, TRUE);
	_tmp9 = NULL;
	label = (_tmp9 = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp9);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.0f);
	_tmp10 = NULL;
	gtk_label_set_markup (label, _tmp10 = g_markup_printf_escaped ("<b>%s:</b>", _ ("Songs")));
	_tmp10 = (g_free (_tmp10), NULL);
	gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
	gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	i++;
	/* Total playtime */
	_tmp11 = NULL;
	pt_label = (_tmp11 = g_object_ref_sink (gmpc_stats_label_new (ARTIST_PLAYTIME_SONGS, song)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), _tmp11);
	gtk_label_set_line_wrap ((GtkLabel*) pt_label, TRUE);
	_tmp12 = NULL;
	label = (_tmp12 = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp12);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.0f);
	_tmp13 = NULL;
	gtk_label_set_markup (label, _tmp13 = g_markup_printf_escaped ("<b>%s:</b>", _ ("Playtime")));
	_tmp13 = (g_free (_tmp13), NULL);
	gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
	gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	i++;
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) hbox, FALSE, FALSE, (guint) 0);
	/* Player controls */
	_tmp14 = NULL;
	hbox = (_tmp14 = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 6)), (hbox == NULL) ? NULL : (hbox = (g_object_unref (hbox), NULL)), _tmp14);
	button = g_object_ref_sink ((GtkButton*) gtk_button_new_from_stock ("gtk-add"));
	gtk_button_set_relief (button, GTK_RELIEF_NONE);
	g_signal_connect_object (button, "clicked", (GCallback) _gmpc_metadata_browser_add_selected_song_gtk_button_clicked, self, 0);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) button, FALSE, FALSE, (guint) 0);
	_tmp15 = NULL;
	button = (_tmp15 = g_object_ref_sink ((GtkButton*) gtk_button_new_with_mnemonic ("_Replace")), (button == NULL) ? NULL : (button = (g_object_unref (button), NULL)), _tmp15);
	_tmp16 = NULL;
	gtk_button_set_image (button, (GtkWidget*) (_tmp16 = g_object_ref_sink ((GtkImage*) gtk_image_new_from_stock ("gtk-redo", GTK_ICON_SIZE_BUTTON))));
	(_tmp16 == NULL) ? NULL : (_tmp16 = (g_object_unref (_tmp16), NULL));
	gtk_button_set_relief (button, GTK_RELIEF_NONE);
	g_signal_connect_object (button, "clicked", (GCallback) _gmpc_metadata_browser_replace_selected_song_gtk_button_clicked, self, 0);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) button, FALSE, FALSE, (guint) 0);
	gtk_table_attach (info_box, (GtkWidget*) hbox, (guint) 0, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	i++;
	text_view = g_object_ref_sink (gmpc_meta_text_view_new (META_ARTIST_TXT));
	gtk_text_view_set_left_margin ((GtkTextView*) text_view, 8);
	_tmp17 = NULL;
	_tmp18 = NULL;
	frame = (_tmp18 = g_object_ref_sink (gmpc_widget_more_new (_tmp17 = g_markup_printf_escaped ("<b>%s:</b>", _ ("Artist information")), (GtkWidget*) text_view)), _tmp17 = (g_free (_tmp17), NULL), _tmp18);
	gmpc_meta_text_view_query_text_from_song (text_view, song);
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) frame, FALSE, FALSE, (guint) 0);
	song_links = g_object_ref_sink (gmpc_song_links_new (GMPC_SONG_LINKS_TYPE_ARTIST, song));
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) song_links, FALSE, FALSE, (guint) 0);
	/**
	         * Add it to the view
	         */
	gtk_container_add ((GtkContainer*) self->priv->metadata_box, (GtkWidget*) vbox);
	gtk_widget_show_all ((GtkWidget*) self->priv->metadata_sw);
	(vbox == NULL) ? NULL : (vbox = (g_object_unref (vbox), NULL));
	(label == NULL) ? NULL : (label = (g_object_unref (label), NULL));
	(hbox == NULL) ? NULL : (hbox = (g_object_unref (hbox), NULL));
	(ali == NULL) ? NULL : (ali = (g_object_unref (ali), NULL));
	(artist_image == NULL) ? NULL : (artist_image = (g_object_unref (artist_image), NULL));
	(song == NULL) ? NULL : (song = (mpd_freeSong (song), NULL));
	(info_box == NULL) ? NULL : (info_box = (g_object_unref (info_box), NULL));
	(pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL));
	(button == NULL) ? NULL : (button = (g_object_unref (button), NULL));
	(text_view == NULL) ? NULL : (text_view = (g_object_unref (text_view), NULL));
	(frame == NULL) ? NULL : (frame = (g_object_unref (frame), NULL));
	(song_links == NULL) ? NULL : (song_links = (g_object_unref (song_links), NULL));
}


static void gmpc_metadata_browser_metadata_box_update (GmpcMetadataBrowser* self) {
	char* artist;
	char* album;
	mpd_Song* song;
	g_return_if_fail (self != NULL);
	artist = gmpc_metadata_browser_browser_get_selected_artist (self);
	album = gmpc_metadata_browser_browser_get_selected_album (self);
	song = gmpc_metadata_browser_browser_get_selected_song (self);
	if (song != NULL) {
		gmpc_metadata_browser_metadata_box_show_song (self, song);
	} else {
		gboolean _tmp0;
		_tmp0 = FALSE;
		if (album != NULL) {
			_tmp0 = artist != NULL;
		} else {
			_tmp0 = FALSE;
		}
		if (_tmp0) {
			gmpc_metadata_browser_metadata_box_show_album (self, artist, album);
		} else {
			if (artist != NULL) {
				gmpc_metadata_browser_metadata_box_show_artist (self, artist);
			} else {
				mpd_Song* _tmp2;
				const mpd_Song* _tmp1;
				_tmp2 = NULL;
				_tmp1 = NULL;
				song = (_tmp2 = (_tmp1 = mpd_playlist_get_current_song (connection), (_tmp1 == NULL) ? NULL : mpd_songDup (_tmp1)), (song == NULL) ? NULL : (song = (mpd_freeSong (song), NULL)), _tmp2);
				if (song != NULL) {
					gmpc_metadata_browser_metadata_box_show_song (self, song);
				}
			}
		}
	}
	artist = (g_free (artist), NULL);
	album = (g_free (album), NULL);
	(song == NULL) ? NULL : (song = (mpd_freeSong (song), NULL));
}


/** 
     * Browser Interface bindings
     */
static void gmpc_metadata_browser_real_browser_add (GmpcPluginBrowserIface* base, GtkWidget* category_tree) {
	GmpcMetadataBrowser * self;
	GtkTreeView* _tmp0;
	GtkTreeView* tree;
	GtkListStore* _tmp1;
	GtkListStore* store;
	GtkTreeModel* _tmp2;
	GtkTreeModel* model;
	GtkTreeIter iter = {0};
	GtkTreeRowReference* _tmp4;
	GtkTreePath* _tmp3;
	self = (GmpcMetadataBrowser*) base;
	g_return_if_fail (category_tree != NULL);
	_tmp0 = NULL;
	tree = (_tmp0 = GTK_TREE_VIEW (category_tree), (_tmp0 == NULL) ? NULL : g_object_ref (_tmp0));
	_tmp1 = NULL;
	store = (_tmp1 = GTK_LIST_STORE (gtk_tree_view_get_model (tree)), (_tmp1 == NULL) ? NULL : g_object_ref (_tmp1));
	_tmp2 = NULL;
	model = (_tmp2 = gtk_tree_view_get_model (tree), (_tmp2 == NULL) ? NULL : g_object_ref (_tmp2));
	playlist3_insert_browser (&iter, cfg_get_single_value_as_int_with_default (config, gmpc_plugin_base_get_name ((GmpcPluginBase*) self), "position", 100));
	/* Create a row reference */
	_tmp4 = NULL;
	_tmp3 = NULL;
	self->priv->rref = (_tmp4 = gtk_tree_row_reference_new (model, _tmp3 = gtk_tree_model_get_path (model, &iter)), (self->priv->rref == NULL) ? NULL : (self->priv->rref = (gtk_tree_row_reference_free (self->priv->rref), NULL)), _tmp4);
	(_tmp3 == NULL) ? NULL : (_tmp3 = (gtk_tree_path_free (_tmp3), NULL));
	gtk_list_store_set (store, &iter, 0, ((GmpcPluginBase*) self)->id, 1, _ (gmpc_plugin_base_get_name ((GmpcPluginBase*) self)), 3, "gtk-info", -1);
	(tree == NULL) ? NULL : (tree = (g_object_unref (tree), NULL));
	(store == NULL) ? NULL : (store = (g_object_unref (store), NULL));
	(model == NULL) ? NULL : (model = (g_object_unref (model), NULL));
}


static void gmpc_metadata_browser_real_browser_selected (GmpcPluginBrowserIface* base, GtkContainer* container) {
	GmpcMetadataBrowser * self;
	char* artist;
	char* _tmp0;
	self = (GmpcMetadataBrowser*) base;
	g_return_if_fail (container != NULL);
	artist = NULL;
	self->priv->selected = TRUE;
	gmpc_metadata_browser_browser_init (self);
	fprintf (stdout, "blub\n");
	gtk_container_add (container, (GtkWidget*) self->priv->paned);
	/* update if non selected */
	_tmp0 = NULL;
	artist = (_tmp0 = gmpc_metadata_browser_browser_get_selected_artist (self), artist = (g_free (artist), NULL), _tmp0);
	if (artist == NULL) {
		gmpc_metadata_browser_metadata_box_clear (self);
		gmpc_metadata_browser_metadata_box_update (self);
	}
	artist = (g_free (artist), NULL);
}


static void gmpc_metadata_browser_real_browser_unselected (GmpcPluginBrowserIface* base, GtkContainer* container) {
	GmpcMetadataBrowser * self;
	self = (GmpcMetadataBrowser*) base;
	g_return_if_fail (container != NULL);
	self->priv->selected = FALSE;
	fprintf (stdout, "blob\n");
	gtk_container_remove (container, (GtkWidget*) self->priv->paned);
}


static void gmpc_metadata_browser_con_changed (GmpcMetadataBrowser* self, GmpcConnection* conn, MpdObj* server, gint connect) {
	g_return_if_fail (self != NULL);
	g_return_if_fail (conn != NULL);
	g_return_if_fail (server != NULL);
	if (self->priv->paned == NULL) {
		return;
	}
	gmpc_metadata_browser_reload_browsers (self);
	gmpc_metadata_browser_metadata_box_clear (self);
	gmpc_metadata_browser_metadata_box_update (self);
}


static void gmpc_metadata_browser_status_changed (GmpcMetadataBrowser* self, GmpcConnection* conn, MpdObj* server, ChangedStatusType what) {
	gboolean _tmp0;
	g_return_if_fail (self != NULL);
	g_return_if_fail (conn != NULL);
	g_return_if_fail (server != NULL);
	if (self->priv->paned == NULL) {
		return;
	}
	_tmp0 = FALSE;
	if ((what & MPD_CST_SONGID) == MPD_CST_SONGID) {
		_tmp0 = self->priv->selected;
	} else {
		_tmp0 = FALSE;
	}
	if (_tmp0) {
		char* artist;
		artist = gmpc_metadata_browser_browser_get_selected_artist (self);
		if (artist == NULL) {
			gmpc_metadata_browser_metadata_box_clear (self);
			gmpc_metadata_browser_metadata_box_update (self);
		}
		artist = (g_free (artist), NULL);
	}
}


GmpcMetadataBrowser* gmpc_metadata_browser_construct (GType object_type) {
	GmpcMetadataBrowser * self;
	self = g_object_newv (object_type, 0, NULL);
	return self;
}


GmpcMetadataBrowser* gmpc_metadata_browser_new (void) {
	return gmpc_metadata_browser_construct (GMPC_TYPE_METADATA_BROWSER);
}


static void _gmpc_metadata_browser_con_changed_gmpc_connection_connection_changed (GmpcConnection* _sender, MpdObj* server, gint connect, gpointer self) {
	gmpc_metadata_browser_con_changed (self, _sender, server, connect);
}


static void _gmpc_metadata_browser_status_changed_gmpc_connection_status_changed (GmpcConnection* _sender, MpdObj* server, ChangedStatusType what, gpointer self) {
	gmpc_metadata_browser_status_changed (self, _sender, server, what);
}


static GObject * gmpc_metadata_browser_constructor (GType type, guint n_construct_properties, GObjectConstructParam * construct_properties) {
	GObject * obj;
	GmpcMetadataBrowserClass * klass;
	GObjectClass * parent_class;
	GmpcMetadataBrowser * self;
	klass = GMPC_METADATA_BROWSER_CLASS (g_type_class_peek (GMPC_TYPE_METADATA_BROWSER));
	parent_class = G_OBJECT_CLASS (g_type_class_peek_parent (klass));
	obj = parent_class->constructor (type, n_construct_properties, construct_properties);
	self = GMPC_METADATA_BROWSER (obj);
	{
		/* Set the plugin as an internal one and of type pl_browser */
		((GmpcPluginBase*) self)->plugin_type = 2 | 8;
		g_signal_connect_object (gmpcconn, "connection-changed", (GCallback) _gmpc_metadata_browser_con_changed_gmpc_connection_connection_changed, self, 0);
		g_signal_connect_object (gmpcconn, "status-changed", (GCallback) _gmpc_metadata_browser_status_changed_gmpc_connection_status_changed, self, 0);
	}
	return obj;
}


static void gmpc_metadata_browser_class_init (GmpcMetadataBrowserClass * klass) {
	gmpc_metadata_browser_parent_class = g_type_class_peek_parent (klass);
	g_type_class_add_private (klass, sizeof (GmpcMetadataBrowserPrivate));
	G_OBJECT_CLASS (klass)->constructor = gmpc_metadata_browser_constructor;
	G_OBJECT_CLASS (klass)->finalize = gmpc_metadata_browser_finalize;
	GMPC_PLUGIN_BASE_CLASS (klass)->get_version = gmpc_metadata_browser_real_get_version;
	GMPC_PLUGIN_BASE_CLASS (klass)->get_name = gmpc_metadata_browser_real_get_name;
	GMPC_PLUGIN_BASE_CLASS (klass)->save_yourself = gmpc_metadata_browser_real_save_yourself;
}


static void gmpc_metadata_browser_gmpc_plugin_browser_iface_interface_init (GmpcPluginBrowserIfaceIface * iface) {
	gmpc_metadata_browser_gmpc_plugin_browser_iface_parent_iface = g_type_interface_peek_parent (iface);
	iface->browser_add = gmpc_metadata_browser_real_browser_add;
	iface->browser_selected = gmpc_metadata_browser_real_browser_selected;
	iface->browser_unselected = gmpc_metadata_browser_real_browser_unselected;
}


static void gmpc_metadata_browser_instance_init (GmpcMetadataBrowser * self) {
	self->priv = GMPC_METADATA_BROWSER_GET_PRIVATE (self);
	self->priv->rref = NULL;
	self->priv->paned = NULL;
	self->priv->browser_box = NULL;
	self->priv->tree_artist = NULL;
	self->priv->model_artist = NULL;
	self->priv->model_filter_artist = NULL;
	self->priv->artist_filter_entry = NULL;
	self->priv->tree_album = NULL;
	self->priv->model_albums = NULL;
	self->priv->model_filter_album = NULL;
	self->priv->album_filter_entry = NULL;
	self->priv->tree_songs = NULL;
	self->priv->model_songs = NULL;
	self->priv->metadata_sw = NULL;
	self->priv->metadata_box = NULL;
	self->priv->selected = FALSE;
}


static void gmpc_metadata_browser_finalize (GObject* obj) {
	GmpcMetadataBrowser * self;
	self = GMPC_METADATA_BROWSER (obj);
	(self->priv->rref == NULL) ? NULL : (self->priv->rref = (gtk_tree_row_reference_free (self->priv->rref), NULL));
	(self->priv->paned == NULL) ? NULL : (self->priv->paned = (g_object_unref (self->priv->paned), NULL));
	(self->priv->browser_box == NULL) ? NULL : (self->priv->browser_box = (g_object_unref (self->priv->browser_box), NULL));
	(self->priv->tree_artist == NULL) ? NULL : (self->priv->tree_artist = (g_object_unref (self->priv->tree_artist), NULL));
	(self->priv->model_artist == NULL) ? NULL : (self->priv->model_artist = (g_object_unref (self->priv->model_artist), NULL));
	(self->priv->model_filter_artist == NULL) ? NULL : (self->priv->model_filter_artist = (g_object_unref (self->priv->model_filter_artist), NULL));
	(self->priv->artist_filter_entry == NULL) ? NULL : (self->priv->artist_filter_entry = (g_object_unref (self->priv->artist_filter_entry), NULL));
	(self->priv->tree_album == NULL) ? NULL : (self->priv->tree_album = (g_object_unref (self->priv->tree_album), NULL));
	(self->priv->model_albums == NULL) ? NULL : (self->priv->model_albums = (g_object_unref (self->priv->model_albums), NULL));
	(self->priv->model_filter_album == NULL) ? NULL : (self->priv->model_filter_album = (g_object_unref (self->priv->model_filter_album), NULL));
	(self->priv->album_filter_entry == NULL) ? NULL : (self->priv->album_filter_entry = (g_object_unref (self->priv->album_filter_entry), NULL));
	(self->priv->tree_songs == NULL) ? NULL : (self->priv->tree_songs = (g_object_unref (self->priv->tree_songs), NULL));
	(self->priv->model_songs == NULL) ? NULL : (self->priv->model_songs = (g_object_unref (self->priv->model_songs), NULL));
	(self->priv->metadata_sw == NULL) ? NULL : (self->priv->metadata_sw = (g_object_unref (self->priv->metadata_sw), NULL));
	(self->priv->metadata_box == NULL) ? NULL : (self->priv->metadata_box = (g_object_unref (self->priv->metadata_box), NULL));
	G_OBJECT_CLASS (gmpc_metadata_browser_parent_class)->finalize (obj);
}


GType gmpc_metadata_browser_get_type (void) {
	static GType gmpc_metadata_browser_type_id = 0;
	if (gmpc_metadata_browser_type_id == 0) {
		static const GTypeInfo g_define_type_info = { sizeof (GmpcMetadataBrowserClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) gmpc_metadata_browser_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (GmpcMetadataBrowser), 0, (GInstanceInitFunc) gmpc_metadata_browser_instance_init, NULL };
		static const GInterfaceInfo gmpc_plugin_browser_iface_info = { (GInterfaceInitFunc) gmpc_metadata_browser_gmpc_plugin_browser_iface_interface_init, (GInterfaceFinalizeFunc) NULL, NULL};
		gmpc_metadata_browser_type_id = g_type_register_static (GMPC_PLUGIN_TYPE_BASE, "GmpcMetadataBrowser", &g_define_type_info, 0);
		g_type_add_interface_static (gmpc_metadata_browser_type_id, GMPC_PLUGIN_TYPE_BROWSER_IFACE, &gmpc_plugin_browser_iface_info);
	}
	return gmpc_metadata_browser_type_id;
}




