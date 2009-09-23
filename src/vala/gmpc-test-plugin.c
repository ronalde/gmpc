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

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>
#include <gtktransition.h>
#include <gmpc_easy_download.h>
#include <libmpd/libmpdclient.h>
#include <libmpd/libmpd.h>
#include <metadata.h>
#include <config.h>
#include <stdlib.h>
#include <string.h>
#include <glib/gi18n-lib.h>
#include <gdk-pixbuf/gdk-pixdata.h>
#include <stdio.h>
#include <glib/gstdio.h>
#include <float.h>
#include <math.h>
#include <metadata-cache.h>
#include <main.h>
#include <pango/pango.h>
#include <gmpc-plugin.h>
#include <plugin.h>
#include <config1.h>


#define GMPC_META_DATA_TYPE_EDIT_WINDOW (gmpc_meta_data_edit_window_get_type ())
#define GMPC_META_DATA_EDIT_WINDOW(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), GMPC_META_DATA_TYPE_EDIT_WINDOW, GmpcMetaDataEditWindow))
#define GMPC_META_DATA_EDIT_WINDOW_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), GMPC_META_DATA_TYPE_EDIT_WINDOW, GmpcMetaDataEditWindowClass))
#define GMPC_META_DATA_IS_EDIT_WINDOW(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GMPC_META_DATA_TYPE_EDIT_WINDOW))
#define GMPC_META_DATA_IS_EDIT_WINDOW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GMPC_META_DATA_TYPE_EDIT_WINDOW))
#define GMPC_META_DATA_EDIT_WINDOW_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), GMPC_META_DATA_TYPE_EDIT_WINDOW, GmpcMetaDataEditWindowClass))

typedef struct _GmpcMetaDataEditWindow GmpcMetaDataEditWindow;
typedef struct _GmpcMetaDataEditWindowClass GmpcMetaDataEditWindowClass;
typedef struct _GmpcMetaDataEditWindowPrivate GmpcMetaDataEditWindowPrivate;
#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))
#define _g_list_free0(var) ((var == NULL) ? NULL : (var = (g_list_free (var), NULL)))
#define _mpd_freeSong0(var) ((var == NULL) ? NULL : (var = (mpd_freeSong (var), NULL)))
#define _g_free0(var) (var = (g_free (var), NULL))
#define _g_error_free0(var) ((var == NULL) ? NULL : (var = (g_error_free (var), NULL)))
#define _meta_data_free0(var) ((var == NULL) ? NULL : (var = (meta_data_free (var), NULL)))

#define GMPC_TYPE_TEST_PLUGIN (gmpc_test_plugin_get_type ())
#define GMPC_TEST_PLUGIN(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), GMPC_TYPE_TEST_PLUGIN, GmpcTestPlugin))
#define GMPC_TEST_PLUGIN_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), GMPC_TYPE_TEST_PLUGIN, GmpcTestPluginClass))
#define GMPC_IS_TEST_PLUGIN(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GMPC_TYPE_TEST_PLUGIN))
#define GMPC_IS_TEST_PLUGIN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GMPC_TYPE_TEST_PLUGIN))
#define GMPC_TEST_PLUGIN_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), GMPC_TYPE_TEST_PLUGIN, GmpcTestPluginClass))

typedef struct _GmpcTestPlugin GmpcTestPlugin;
typedef struct _GmpcTestPluginClass GmpcTestPluginClass;
typedef struct _GmpcTestPluginPrivate GmpcTestPluginPrivate;
#define __g_list_free_gtk_tree_path_free0(var) ((var == NULL) ? NULL : (var = (_g_list_free_gtk_tree_path_free (var), NULL)))

struct _GmpcMetaDataEditWindow {
	GtkWindow parent_instance;
	GmpcMetaDataEditWindowPrivate * priv;
};

struct _GmpcMetaDataEditWindowClass {
	GtkWindowClass parent_class;
};

struct _GmpcMetaDataEditWindowPrivate {
	GtkListStore* model;
	GtkTreeView* tree;
	GList* downloads;
	mpd_Song* song;
	MetaDataType query_type;
	void* handle;
	void* handle2;
	GtkHBox* pbox;
	GtkLabel* warning_label;
	GtkEntry* artist_entry;
	GtkEntry* album_entry;
	GtkEntry* title_entry;
	GtkButton* cancel;
	GtkButton* refresh;
	GtkComboBox* combo;
	GtkProgressBar* bar;
	GtkTreeViewColumn* column;
};

struct _GmpcTestPlugin {
	GmpcPluginBase parent_instance;
	GmpcTestPluginPrivate * priv;
};

struct _GmpcTestPluginClass {
	GmpcPluginBaseClass parent_class;
};


static gpointer gmpc_meta_data_edit_window_parent_class = NULL;
static gpointer gmpc_test_plugin_parent_class = NULL;
static GmpcPluginToolMenuIfaceIface* gmpc_test_plugin_gmpc_plugin_tool_menu_iface_parent_iface = NULL;
static GmpcPluginSongListIfaceIface* gmpc_test_plugin_gmpc_plugin_song_list_iface_parent_iface = NULL;

#define use_transition TRUE
GType gmpc_meta_data_edit_window_get_type (void);
#define GMPC_META_DATA_EDIT_WINDOW_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), GMPC_META_DATA_TYPE_EDIT_WINDOW, GmpcMetaDataEditWindowPrivate))
enum  {
	GMPC_META_DATA_EDIT_WINDOW_DUMMY_PROPERTY
};
#define GMPC_META_DATA_EDIT_WINDOW_some_unique_name VERSION
static void gmpc_meta_data_edit_window_add_entry_image (GmpcMetaDataEditWindow* self, const char* provider, const char* uri, GdkPixbufFormat* format, GdkPixbuf* pb);
static void gmpc_meta_data_edit_window_add_entry_text (GmpcMetaDataEditWindow* self, const char* provider, const char* uri, const char* text);
void gmpc_meta_data_edit_window_image_downloaded (GmpcMetaDataEditWindow* self, const GEADAsyncHandler* handle, GEADStatus status);
static void _gmpc_meta_data_edit_window_image_downloaded_gmpc_async_download_callback (const GEADAsyncHandler* handle, GEADStatus status, gpointer self);
void gmpc_meta_data_edit_window_callback (GmpcMetaDataEditWindow* self, void* handle, const char* plugin_name, GList* list);
void gmpc_meta_data_edit_window_store_image (GmpcMetaDataEditWindow* self, const GEADAsyncHandler* handle, GEADStatus status);
static void _gmpc_meta_data_edit_window_store_image_gmpc_async_download_callback (const GEADAsyncHandler* handle, GEADStatus status, gpointer self);
static void gmpc_meta_data_edit_window_set_metadata (GmpcMetaDataEditWindow* self, GtkButton* button);
void gmpc_meta_data_edit_window_destroy_popup (GmpcMetaDataEditWindow* self, GtkButton* button);
static void _gmpc_meta_data_edit_window_callback_gmpc_meta_data_callback (void* handle, const char* plugin_name, GList* list, gpointer self);
void gmpc_meta_data_edit_window_refresh_query (GmpcMetaDataEditWindow* self, GtkButton* button);
static void gmpc_meta_data_edit_window_combo_box_changed (GmpcMetaDataEditWindow* self, GtkComboBox* comb);
void gmpc_meta_data_edit_window_b_cancel (GmpcMetaDataEditWindow* self);
static void _gmpc_meta_data_edit_window_b_cancel_gtk_button_clicked (GtkButton* _sender, gpointer self);
static void _gmpc_meta_data_edit_window_destroy_popup_gtk_button_clicked (GtkButton* _sender, gpointer self);
static void _gmpc_meta_data_edit_window_set_metadata_gtk_button_clicked (GtkButton* _sender, gpointer self);
static void _gmpc_meta_data_edit_window_combo_box_changed_gtk_combo_box_changed (GtkComboBox* _sender, gpointer self);
static void _gmpc_meta_data_edit_window_refresh_query_gtk_button_clicked (GtkButton* _sender, gpointer self);
GmpcMetaDataEditWindow* gmpc_meta_data_edit_window_new (const mpd_Song* song, MetaDataType type);
GmpcMetaDataEditWindow* gmpc_meta_data_edit_window_construct (GType object_type, const mpd_Song* song, MetaDataType type);
static GObject * gmpc_meta_data_edit_window_constructor (GType type, guint n_construct_properties, GObjectConstructParam * construct_properties);
static void gmpc_meta_data_edit_window_finalize (GObject* obj);
GType gmpc_test_plugin_get_type (void);
enum  {
	GMPC_TEST_PLUGIN_DUMMY_PROPERTY
};
static gint* gmpc_test_plugin_real_get_version (GmpcPluginBase* base, int* result_length1);
static const char* gmpc_test_plugin_real_get_name (GmpcPluginBase* base);
static void gmpc_test_plugin_real_save_yourself (GmpcPluginBase* base);
static gboolean gmpc_test_plugin_real_get_enabled (GmpcPluginBase* base);
static void gmpc_test_plugin_real_set_enabled (GmpcPluginBase* base, gboolean state);
static void _g_list_free_gtk_tree_path_free (GList* self);
static void gmpc_test_plugin_menu_activate_tree (GmpcTestPlugin* self, GtkMenuItem* item);
static void _gmpc_test_plugin_menu_activate_tree_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self);
static gint gmpc_test_plugin_real_song_list (GmpcPluginSongListIface* base, GtkWidget* tree, GtkMenu* menu);
void gmpc_test_plugin_menu_activated_album (GmpcTestPlugin* self, GtkMenuItem* item);
static void _gmpc_test_plugin_menu_activated_album_gtk_menu_item_activate (GtkMenuItem* _sender, gpointer self);
static gint gmpc_test_plugin_real_tool_menu_integration (GmpcPluginToolMenuIface* base, GtkMenu* menu);
GmpcTestPlugin* gmpc_test_plugin_new (void);
GmpcTestPlugin* gmpc_test_plugin_construct (GType object_type);
static GObject * gmpc_test_plugin_constructor (GType type, guint n_construct_properties, GObjectConstructParam * construct_properties);

static const gint GMPC_TEST_PLUGIN_version[] = {0, 0, 2};


static void gmpc_meta_data_edit_window_add_entry_image (GmpcMetaDataEditWindow* self, const char* provider, const char* uri, GdkPixbufFormat* format, GdkPixbuf* pb) {
	GtkTreeIter iter = {0};
	char* a;
	char* _tmp0_;
	gint new_h;
	gint new_w;
	GdkPixbuf* _tmp7_;
	g_return_if_fail (self != NULL);
	g_return_if_fail (uri != NULL);
	g_return_if_fail (pb != NULL);
	a = NULL;
	a = (_tmp0_ = g_markup_printf_escaped ("<b>%s</b>: %s", _ ("Uri"), uri), _g_free0 (a), _tmp0_);
	if (provider != NULL) {
		char* _tmp2_;
		char* _tmp1_;
		a = (_tmp2_ = g_strconcat (a, _tmp1_ = g_markup_printf_escaped ("\n<b>%s</b>:  %s", _ ("Provider"), provider), NULL), _g_free0 (a), _tmp2_);
		_g_free0 (_tmp1_);
	}
	if (format != NULL) {
		char* _tmp4_;
		char* _tmp3_;
		a = (_tmp4_ = g_strconcat (a, _tmp3_ = g_markup_printf_escaped ("\n<b>%s</b>: %s", _ ("Filetype"), gdk_pixbuf_format_get_name (format)), NULL), _g_free0 (a), _tmp4_);
		_g_free0 (_tmp3_);
	}
	if (pb != NULL) {
		char* _tmp6_;
		char* _tmp5_;
		a = (_tmp6_ = g_strconcat (a, _tmp5_ = g_strdup_printf ("\n<b>%s</b>: %ix%i (%s)", _ ("Size"), gdk_pixbuf_get_width (pb), gdk_pixbuf_get_height (pb), _ ("wxh")), NULL), _g_free0 (a), _tmp6_);
		_g_free0 (_tmp5_);
	}
	new_h = 0;
	new_w = 0;
	if (gdk_pixbuf_get_width (pb) < gdk_pixbuf_get_height (pb)) {
		new_h = 150;
		new_w = (gint) ((150.0 / ((double) gdk_pixbuf_get_height (pb))) * gdk_pixbuf_get_width (pb));
	} else {
		new_w = 150;
		new_h = (gint) ((150.0 / ((double) gdk_pixbuf_get_width (pb))) * gdk_pixbuf_get_height (pb));
	}
	gtk_list_store_append (self->priv->model, &iter);
	gtk_list_store_set (self->priv->model, &iter, 0, _tmp7_ = gdk_pixbuf_scale_simple (pb, new_w, new_h, GDK_INTERP_BILINEAR), 1, uri, 2, a, -1, -1);
	_g_object_unref0 (_tmp7_);
	_g_free0 (a);
}


static void gmpc_meta_data_edit_window_add_entry_text (GmpcMetaDataEditWindow* self, const char* provider, const char* uri, const char* text) {
	GtkTreeIter iter = {0};
	char* a;
	char* _tmp0_;
	g_return_if_fail (self != NULL);
	g_return_if_fail (uri != NULL);
	g_return_if_fail (text != NULL);
	a = NULL;
	a = (_tmp0_ = g_strdup_printf ("<b>%s</b>: %s", _ ("Uri"), uri), _g_free0 (a), _tmp0_);
	if (provider != NULL) {
		char* _tmp2_;
		char* _tmp1_;
		a = (_tmp2_ = g_strconcat (a, _tmp1_ = g_strdup_printf ("\n<b>%s</b>:  %s", _ ("Provider"), provider), NULL), _g_free0 (a), _tmp2_);
		_g_free0 (_tmp1_);
	}
	gtk_list_store_append (self->priv->model, &iter);
	gtk_list_store_set (self->priv->model, &iter, 3, text, 1, uri, 2, a, -1, -1);
	_g_free0 (a);
}


static gpointer _g_object_ref0 (gpointer self) {
	return self ? g_object_ref (self) : NULL;
}


void gmpc_meta_data_edit_window_image_downloaded (GmpcMetaDataEditWindow* self, const GEADAsyncHandler* handle, GEADStatus status) {
	GError * _inner_error_;
	gboolean _tmp3_;
	gboolean _tmp4_;
	g_return_if_fail (self != NULL);
	g_return_if_fail (handle != NULL);
	_inner_error_ = NULL;
	if (status == GEAD_PROGRESS) {
		return;
	}
	self->priv->downloads = g_list_remove (self->priv->downloads, handle);
	gtk_progress_bar_pulse (self->priv->bar);
	if (status == GEAD_DONE) {
		guchar* _tmp1_;
		gint data_size;
		gint data_length1;
		gint _tmp0_;
		guchar* data;
		gboolean _tmp2_;
		data = (_tmp1_ = gmpc_easy_handler_get_data_vala_wrap (handle, &_tmp0_), data_length1 = _tmp0_, data_size = data_length1, _tmp1_);
		_tmp2_ = FALSE;
		if (self->priv->query_type == META_ALBUM_ART) {
			_tmp2_ = TRUE;
		} else {
			_tmp2_ = self->priv->query_type == META_ARTIST_ART;
		}
		if (_tmp2_) {
			{
				GdkPixbufLoader* load;
				GdkPixbuf* pb;
				load = gdk_pixbuf_loader_new ();
				{
					gdk_pixbuf_loader_write (load, data, data_length1, &_inner_error_);
					if (_inner_error_ != NULL) {
						goto __catch1_g_error;
						goto __finally1;
					}
				}
				goto __finally1;
				__catch1_g_error:
				{
					GError * e;
					e = _inner_error_;
					_inner_error_ = NULL;
					{
						fprintf (stdout, "Failed to load file: %s::%s\n", e->message, gmpc_easy_handler_get_uri (handle));
						_g_error_free0 (e);
					}
				}
				__finally1:
				if (_inner_error_ != NULL) {
					_g_object_unref0 (load);
					goto __catch0_g_error;
					goto __finally0;
				}
				gdk_pixbuf_loader_close (load, &_inner_error_);
				if (_inner_error_ != NULL) {
					_g_object_unref0 (load);
					goto __catch0_g_error;
					goto __finally0;
				}
				pb = _g_object_ref0 (gdk_pixbuf_loader_get_pixbuf (load));
				if (pb != NULL) {
					gmpc_meta_data_edit_window_add_entry_image (self, (const char*) gmpc_easy_handler_get_user_data (handle), gmpc_easy_handler_get_uri (handle), gdk_pixbuf_loader_get_format (load), pb);
				}
				_g_object_unref0 (load);
				_g_object_unref0 (pb);
			}
			goto __finally0;
			__catch0_g_error:
			{
				GError * e;
				e = _inner_error_;
				_inner_error_ = NULL;
				{
					fprintf (stdout, "Failed to load file: %s::%s\n", e->message, gmpc_easy_handler_get_uri (handle));
					_g_error_free0 (e);
				}
			}
			__finally0:
			if (_inner_error_ != NULL) {
				g_critical ("file %s: line %d: uncaught error: %s", __FILE__, __LINE__, _inner_error_->message);
				g_clear_error (&_inner_error_);
				return;
			}
		} else {
			gmpc_meta_data_edit_window_add_entry_text (self, (const char*) gmpc_easy_handler_get_user_data (handle), gmpc_easy_handler_get_uri (handle), (const char*) data);
		}
	}
	_tmp3_ = FALSE;
	_tmp4_ = FALSE;
	if (self->priv->handle == NULL) {
		_tmp4_ = self->priv->handle2 == NULL;
	} else {
		_tmp4_ = FALSE;
	}
	if (_tmp4_) {
		_tmp3_ = self->priv->downloads == NULL;
	} else {
		_tmp3_ = FALSE;
	}
	if (_tmp3_) {
		gtk_widget_hide ((GtkWidget*) self->priv->pbox);
		g_object_set ((GtkWidget*) self->priv->refresh, "sensitive", TRUE, NULL);
		g_object_set ((GtkWidget*) self->priv->combo, "sensitive", TRUE, NULL);
	}
}


static void _gmpc_meta_data_edit_window_image_downloaded_gmpc_async_download_callback (const GEADAsyncHandler* handle, GEADStatus status, gpointer self) {
	gmpc_meta_data_edit_window_image_downloaded (self, handle, status);
}


void gmpc_meta_data_edit_window_callback (GmpcMetaDataEditWindow* self, void* handle, const char* plugin_name, GList* list) {
	GError * _inner_error_;
	g_return_if_fail (self != NULL);
	_inner_error_ = NULL;
	gtk_progress_bar_pulse (self->priv->bar);
	if (list == NULL) {
		if (self->priv->handle == handle) {
			gboolean _tmp0_;
			self->priv->handle = NULL;
			_tmp0_ = FALSE;
			if (self->priv->handle == NULL) {
				_tmp0_ = self->priv->downloads == NULL;
			} else {
				_tmp0_ = FALSE;
			}
			if (_tmp0_) {
				gtk_widget_hide ((GtkWidget*) self->priv->pbox);
				g_object_set ((GtkWidget*) self->priv->refresh, "sensitive", TRUE, NULL);
				g_object_set ((GtkWidget*) self->priv->combo, "sensitive", TRUE, NULL);
			}
		}
		if (self->priv->handle2 == handle) {
			gboolean _tmp1_;
			self->priv->handle2 = NULL;
			_tmp1_ = FALSE;
			if (self->priv->handle == NULL) {
				_tmp1_ = self->priv->downloads == NULL;
			} else {
				_tmp1_ = FALSE;
			}
			if (_tmp1_) {
				gtk_widget_hide ((GtkWidget*) self->priv->pbox);
				g_object_set ((GtkWidget*) self->priv->combo, "sensitive", TRUE, NULL);
				g_object_set ((GtkWidget*) self->priv->refresh, "sensitive", TRUE, NULL);
			}
		}
	}
	{
		GList* md_collection;
		GList* md_it;
		md_collection = list;
		for (md_it = md_collection; md_it != NULL; md_it = md_it->next) {
			const MetaData* md;
			md = (const MetaData*) md_it->data;
			{
				gboolean _tmp2_;
				_tmp2_ = FALSE;
				if (self->priv->query_type == META_ALBUM_ART) {
					_tmp2_ = TRUE;
				} else {
					_tmp2_ = self->priv->query_type == META_ARTIST_ART;
				}
				if (_tmp2_) {
					const char* uri;
					uri = meta_data_get_uri (md);
					if (md->content_type == META_DATA_CONTENT_URI) {
						if (g_utf8_get_char (g_utf8_offset_to_pointer (uri, 0)) == '/') {
							{
								GdkPixbuf* pb;
								pb = gdk_pixbuf_new_from_file (uri, &_inner_error_);
								if (_inner_error_ != NULL) {
									goto __catch2_g_error;
									goto __finally2;
								}
								if (pb != NULL) {
									gint w;
									gint h;
									w = 0;
									h = 0;
									gmpc_meta_data_edit_window_add_entry_image (self, plugin_name, uri, gdk_pixbuf_get_file_info (uri, &w, &h), pb);
								}
								_g_object_unref0 (pb);
							}
							goto __finally2;
							__catch2_g_error:
							{
								GError * e;
								e = _inner_error_;
								_inner_error_ = NULL;
								{
									_g_error_free0 (e);
								}
							}
							__finally2:
							if (_inner_error_ != NULL) {
								g_critical ("file %s: line %d: uncaught error: %s", __FILE__, __LINE__, _inner_error_->message);
								g_clear_error (&_inner_error_);
								return;
							}
						} else {
							GEADAsyncHandler* h;
							h = gmpc_easy_async_downloader (uri, _gmpc_meta_data_edit_window_image_downloaded_gmpc_async_download_callback, self);
							if (h != NULL) {
								gmpc_easy_handler_set_user_data (h, md->plugin_name);
								self->priv->downloads = g_list_append (self->priv->downloads, h);
							}
						}
					}
				} else {
					if (md->content_type == META_DATA_CONTENT_TEXT) {
						const char* uri;
						uri = meta_data_get_text (md);
						gmpc_meta_data_edit_window_add_entry_text (self, plugin_name, "n/a", uri);
					} else {
						if (md->content_type == META_DATA_CONTENT_URI) {
							const char* uri;
							uri = meta_data_get_uri (md);
							if (g_utf8_get_char (g_utf8_offset_to_pointer (uri, 0)) == '/') {
								{
									char* content;
									char* _tmp5_;
									gboolean _tmp4_;
									char* _tmp3_;
									gboolean _tmp6_;
									content = NULL;
									_tmp3_ = NULL;
									_tmp6_ = (_tmp4_ = g_file_get_contents (uri, &_tmp3_, NULL, &_inner_error_), content = (_tmp5_ = _tmp3_, _g_free0 (content), _tmp5_), _tmp4_);
									if (_inner_error_ != NULL) {
										_g_free0 (content);
										goto __catch3_g_error;
										goto __finally3;
									}
									if (_tmp6_) {
										gmpc_meta_data_edit_window_add_entry_text (self, plugin_name, uri, content);
									}
									_g_free0 (content);
								}
								goto __finally3;
								__catch3_g_error:
								{
									GError * e;
									e = _inner_error_;
									_inner_error_ = NULL;
									{
										_g_error_free0 (e);
									}
								}
								__finally3:
								if (_inner_error_ != NULL) {
									g_critical ("file %s: line %d: uncaught error: %s", __FILE__, __LINE__, _inner_error_->message);
									g_clear_error (&_inner_error_);
									return;
								}
							}
						}
					}
				}
			}
		}
	}
}


void gmpc_meta_data_edit_window_store_image (GmpcMetaDataEditWindow* self, const GEADAsyncHandler* handle, GEADStatus status) {
	GError * _inner_error_;
	g_return_if_fail (self != NULL);
	g_return_if_fail (handle != NULL);
	_inner_error_ = NULL;
	if (status == GEAD_PROGRESS) {
		guchar* _tmp1_;
		gint data_size;
		gint data_length1;
		gint _tmp0_;
		guchar* data;
		gint64 total_size;
		gboolean _tmp2_;
		data = (_tmp1_ = gmpc_easy_handler_get_data_vala_wrap (handle, &_tmp0_), data_length1 = _tmp0_, data_size = data_length1, _tmp1_);
		g_object_set ((GtkWidget*) self, "sensitive", FALSE, NULL);
		gtk_widget_show ((GtkWidget*) self->priv->pbox);
		total_size = gmpc_easy_handler_get_content_size (handle);
		_tmp2_ = FALSE;
		if (data_length1 > 0) {
			_tmp2_ = total_size > 0;
		} else {
			_tmp2_ = FALSE;
		}
		if (_tmp2_) {
			double progress;
			progress = data_length1 / ((double) total_size);
			gtk_progress_bar_set_fraction (self->priv->bar, progress);
		} else {
			gtk_progress_bar_pulse (self->priv->bar);
		}
		return;
	}
	self->priv->downloads = g_list_remove (self->priv->downloads, handle);
	if (status == GEAD_DONE) {
		guchar* _tmp4_;
		gint data_size;
		gint data_length1;
		gint _tmp3_;
		guchar* data;
		char* file;
		data = (_tmp4_ = gmpc_easy_handler_get_data_vala_wrap (handle, &_tmp3_), data_length1 = _tmp3_, data_size = data_length1, _tmp4_);
		file = gmpc_get_metadata_filename (self->priv->query_type, self->priv->song, NULL);
		{
			MetaData* met;
			MetaData* met_false;
			g_file_set_contents (file, (const char*) data, (glong) data_length1, &_inner_error_);
			if (_inner_error_ != NULL) {
				goto __catch4_g_error;
				goto __finally4;
			}
			met = meta_data_new ();
			met->type = self->priv->query_type;
			met->plugin_name = "User set";
			met->content_type = META_DATA_CONTENT_URI;
			meta_data_set_uri (met, file);
			meta_data_set_cache (self->priv->song, META_DATA_AVAILABLE, met);
			met_false = meta_data_new ();
			met_false->type = self->priv->query_type;
			met_false->plugin_name = "User set";
			met_false->content_type = META_DATA_CONTENT_EMPTY;
			g_signal_emit_by_name (gmw, "data-changed", self->priv->song, self->priv->query_type, META_DATA_UNAVAILABLE, met_false);
			g_signal_emit_by_name (gmw, "data-changed", self->priv->song, self->priv->query_type, META_DATA_AVAILABLE, met);
			_meta_data_free0 (met);
			_meta_data_free0 (met_false);
		}
		goto __finally4;
		__catch4_g_error:
		{
			GError * e;
			e = _inner_error_;
			_inner_error_ = NULL;
			{
				_g_error_free0 (e);
			}
		}
		__finally4:
		if (_inner_error_ != NULL) {
			_g_free0 (file);
			g_critical ("file %s: line %d: uncaught error: %s", __FILE__, __LINE__, _inner_error_->message);
			g_clear_error (&_inner_error_);
			return;
		}
		_g_free0 (file);
	}
	gtk_widget_hide ((GtkWidget*) self->priv->pbox);
	g_object_set ((GtkWidget*) self, "sensitive", TRUE, NULL);
}


static void _gmpc_meta_data_edit_window_store_image_gmpc_async_download_callback (const GEADAsyncHandler* handle, GEADStatus status, gpointer self) {
	gmpc_meta_data_edit_window_store_image (self, handle, status);
}


static void gmpc_meta_data_edit_window_set_metadata (GmpcMetaDataEditWindow* self, GtkButton* button) {
	GError * _inner_error_;
	GtkTreeIter iter = {0};
	GtkTreeSelection* sel;
	char* path;
	GtkTreeModel* model;
	GtkTreeModel* _tmp2_;
	gboolean _tmp1_;
	GtkTreeModel* _tmp0_;
	g_return_if_fail (self != NULL);
	g_return_if_fail (button != NULL);
	_inner_error_ = NULL;
	sel = _g_object_ref0 (gtk_tree_view_get_selection (self->priv->tree));
	path = NULL;
	model = NULL;
	_tmp0_ = NULL;
	if ((_tmp1_ = gtk_tree_selection_get_selected (sel, &_tmp0_, &iter), model = (_tmp2_ = _g_object_ref0 (_tmp0_), _g_object_unref0 (model), _tmp2_), _tmp1_)) {
		gboolean _tmp3_;
		gtk_tree_model_get ((GtkTreeModel*) self->priv->model, &iter, 1, &path, -1);
		_tmp3_ = FALSE;
		if (self->priv->query_type == META_ALBUM_ART) {
			_tmp3_ = TRUE;
		} else {
			_tmp3_ = self->priv->query_type == META_ARTIST_ART;
		}
		if (_tmp3_) {
			if (g_utf8_get_char (g_utf8_offset_to_pointer (path, 0)) == '/') {
				MetaData* met;
				MetaData* met_false;
				met = meta_data_new ();
				met->type = self->priv->query_type;
				met->plugin_name = "User set";
				met->content_type = META_DATA_CONTENT_URI;
				meta_data_set_uri (met, path);
				meta_data_set_cache (self->priv->song, META_DATA_AVAILABLE, met);
				met_false = meta_data_new ();
				met_false->type = self->priv->query_type;
				met_false->plugin_name = "User set";
				met_false->content_type = META_DATA_CONTENT_EMPTY;
				g_signal_emit_by_name (gmw, "data-changed", self->priv->song, self->priv->query_type, META_DATA_UNAVAILABLE, met_false);
				g_signal_emit_by_name (gmw, "data-changed", self->priv->song, self->priv->query_type, META_DATA_AVAILABLE, met);
				_meta_data_free0 (met);
				_meta_data_free0 (met_false);
			} else {
				GEADAsyncHandler* h;
				h = gmpc_easy_async_downloader (path, _gmpc_meta_data_edit_window_store_image_gmpc_async_download_callback, self);
				if (h != NULL) {
					self->priv->downloads = g_list_append (self->priv->downloads, h);
				}
			}
		} else {
			char* lyric;
			char* file;
			lyric = NULL;
			gtk_tree_model_get ((GtkTreeModel*) self->priv->model, &iter, 3, &lyric, -1);
			file = gmpc_get_metadata_filename (self->priv->query_type, self->priv->song, NULL);
			{
				MetaData* met;
				MetaData* met_false;
				g_file_set_contents (file, lyric, (glong) (-1), &_inner_error_);
				if (_inner_error_ != NULL) {
					goto __catch5_g_error;
					goto __finally5;
				}
				met = meta_data_new ();
				met->type = self->priv->query_type;
				met->plugin_name = "User set";
				met->content_type = META_DATA_CONTENT_URI;
				meta_data_set_uri (met, file);
				meta_data_set_cache (self->priv->song, META_DATA_AVAILABLE, met);
				met_false = meta_data_new ();
				met_false->type = self->priv->query_type;
				met_false->plugin_name = "User set";
				met_false->content_type = META_DATA_CONTENT_EMPTY;
				g_signal_emit_by_name (gmw, "data-changed", self->priv->song, self->priv->query_type, META_DATA_UNAVAILABLE, met_false);
				g_signal_emit_by_name (gmw, "data-changed", self->priv->song, self->priv->query_type, META_DATA_AVAILABLE, met);
				_meta_data_free0 (met);
				_meta_data_free0 (met_false);
			}
			goto __finally5;
			__catch5_g_error:
			{
				GError * e;
				e = _inner_error_;
				_inner_error_ = NULL;
				{
					_g_error_free0 (e);
				}
			}
			__finally5:
			if (_inner_error_ != NULL) {
				_g_free0 (lyric);
				_g_free0 (file);
				_g_object_unref0 (sel);
				_g_free0 (path);
				_g_object_unref0 (model);
				g_critical ("file %s: line %d: uncaught error: %s", __FILE__, __LINE__, _inner_error_->message);
				g_clear_error (&_inner_error_);
				return;
			}
			_g_free0 (lyric);
			_g_free0 (file);
		}
	}
	_g_object_unref0 (sel);
	_g_free0 (path);
	_g_object_unref0 (model);
}


void gmpc_meta_data_edit_window_destroy_popup (GmpcMetaDataEditWindow* self, GtkButton* button) {
	g_return_if_fail (self != NULL);
	g_return_if_fail (button != NULL);
	gtk_object_destroy ((GtkObject*) self);
}


static gpointer _mpd_songDup0 (gpointer self) {
	return self ? mpd_songDup (self) : NULL;
}


static void _gmpc_meta_data_edit_window_callback_gmpc_meta_data_callback (void* handle, const char* plugin_name, GList* list, gpointer self) {
	gmpc_meta_data_edit_window_callback (self, handle, plugin_name, list);
}


void gmpc_meta_data_edit_window_refresh_query (GmpcMetaDataEditWindow* self, GtkButton* button) {
	mpd_Song* ss;
	char* _tmp0_;
	char* _tmp1_;
	char* _tmp2_;
	gboolean _tmp3_;
	g_return_if_fail (self != NULL);
	g_return_if_fail (button != NULL);
	gtk_list_store_clear (self->priv->model);
	ss = _mpd_songDup0 (self->priv->song);
	ss->artist = (_tmp0_ = g_strdup (gtk_entry_get_text (self->priv->artist_entry)), _g_free0 (ss->artist), _tmp0_);
	ss->album = (_tmp1_ = g_strdup (gtk_entry_get_text (self->priv->album_entry)), _g_free0 (ss->album), _tmp1_);
	ss->title = (_tmp2_ = g_strdup (gtk_entry_get_text (self->priv->title_entry)), _g_free0 (ss->title), _tmp2_);
	_tmp3_ = FALSE;
	if (self->priv->handle == NULL) {
		_tmp3_ = self->priv->handle2 == NULL;
	} else {
		_tmp3_ = FALSE;
	}
	if (_tmp3_) {
		self->priv->handle = metadata_get_list (ss, self->priv->query_type, _gmpc_meta_data_edit_window_callback_gmpc_meta_data_callback, self);
		g_object_set ((GtkWidget*) self->priv->refresh, "sensitive", FALSE, NULL);
		g_object_set ((GtkWidget*) self->priv->combo, "sensitive", FALSE, NULL);
		gtk_widget_show ((GtkWidget*) self->priv->pbox);
	}
	_mpd_freeSong0 (ss);
}


static void gmpc_meta_data_edit_window_combo_box_changed (GmpcMetaDataEditWindow* self, GtkComboBox* comb) {
	gint active;
	g_return_if_fail (self != NULL);
	g_return_if_fail (comb != NULL);
	active = gtk_combo_box_get_active (comb);
	gtk_list_store_clear (self->priv->model);
	g_object_set ((GtkWidget*) self->priv->title_entry, "sensitive", FALSE, NULL);
	g_object_set ((GtkWidget*) self->priv->album_entry, "sensitive", FALSE, NULL);
	g_object_set ((GtkWidget*) self->priv->artist_entry, "sensitive", FALSE, NULL);
	g_object_set ((GtkWidget*) self->priv->refresh, "sensitive", FALSE, NULL);
	gtk_widget_hide ((GtkWidget*) self->priv->warning_label);
	if (self->priv->column != NULL) {
		GtkTreeViewColumn* _tmp0_;
		gtk_tree_view_remove_column (self->priv->tree, self->priv->column);
		gtk_object_destroy ((GtkObject*) self->priv->column);
		self->priv->column = (_tmp0_ = NULL, _g_object_unref0 (self->priv->column), _tmp0_);
	}
	if (active == 0) {
		self->priv->query_type = META_ARTIST_ART;
		if (self->priv->song->artist != NULL) {
			g_object_set ((GtkWidget*) self->priv->artist_entry, "sensitive", TRUE, NULL);
			g_object_set ((GtkWidget*) self->priv->refresh, "sensitive", TRUE, NULL);
		} else {
			gtk_widget_show ((GtkWidget*) self->priv->warning_label);
		}
	} else {
		if (active == 1) {
			gboolean _tmp1_;
			self->priv->query_type = META_ALBUM_ART;
			_tmp1_ = FALSE;
			if (self->priv->song->artist != NULL) {
				_tmp1_ = self->priv->song->album != NULL;
			} else {
				_tmp1_ = FALSE;
			}
			if (_tmp1_) {
				g_object_set ((GtkWidget*) self->priv->artist_entry, "sensitive", TRUE, NULL);
				g_object_set ((GtkWidget*) self->priv->album_entry, "sensitive", TRUE, NULL);
				g_object_set ((GtkWidget*) self->priv->refresh, "sensitive", TRUE, NULL);
			} else {
				gtk_widget_show ((GtkWidget*) self->priv->warning_label);
			}
		} else {
			if (active == 2) {
				gboolean _tmp2_;
				self->priv->query_type = META_SONG_TXT;
				_tmp2_ = FALSE;
				if (self->priv->song->artist != NULL) {
					_tmp2_ = self->priv->song->title != NULL;
				} else {
					_tmp2_ = FALSE;
				}
				if (_tmp2_) {
					g_object_set ((GtkWidget*) self->priv->artist_entry, "sensitive", TRUE, NULL);
					g_object_set ((GtkWidget*) self->priv->title_entry, "sensitive", TRUE, NULL);
					g_object_set ((GtkWidget*) self->priv->refresh, "sensitive", TRUE, NULL);
				} else {
					gtk_widget_show ((GtkWidget*) self->priv->warning_label);
				}
			} else {
				if (active == 3) {
					gboolean _tmp3_;
					self->priv->query_type = META_ALBUM_TXT;
					_tmp3_ = FALSE;
					if (self->priv->song->artist != NULL) {
						_tmp3_ = self->priv->song->album != NULL;
					} else {
						_tmp3_ = FALSE;
					}
					if (_tmp3_) {
						g_object_set ((GtkWidget*) self->priv->artist_entry, "sensitive", TRUE, NULL);
						g_object_set ((GtkWidget*) self->priv->album_entry, "sensitive", TRUE, NULL);
						g_object_set ((GtkWidget*) self->priv->refresh, "sensitive", TRUE, NULL);
					} else {
						gtk_widget_show ((GtkWidget*) self->priv->warning_label);
					}
				} else {
					if (active == 4) {
						self->priv->query_type = META_ARTIST_TXT;
						if (self->priv->song->artist != NULL) {
							g_object_set ((GtkWidget*) self->priv->artist_entry, "sensitive", TRUE, NULL);
							g_object_set ((GtkWidget*) self->priv->refresh, "sensitive", TRUE, NULL);
						} else {
							gtk_widget_show ((GtkWidget*) self->priv->warning_label);
						}
					} else {
						if (active == 5) {
							gboolean _tmp4_;
							self->priv->query_type = META_SONG_GUITAR_TAB;
							_tmp4_ = FALSE;
							if (self->priv->song->artist != NULL) {
								_tmp4_ = self->priv->song->title != NULL;
							} else {
								_tmp4_ = FALSE;
							}
							if (_tmp4_) {
								g_object_set ((GtkWidget*) self->priv->artist_entry, "sensitive", TRUE, NULL);
								g_object_set ((GtkWidget*) self->priv->title_entry, "sensitive", TRUE, NULL);
								g_object_set ((GtkWidget*) self->priv->refresh, "sensitive", TRUE, NULL);
							} else {
								gtk_widget_show ((GtkWidget*) self->priv->warning_label);
							}
						}
					}
				}
			}
		}
	}
	if (active < 2) {
		GtkCellRendererPixbuf* renderer;
		GtkTreeViewColumn* _tmp5_;
		renderer = g_object_ref_sink ((GtkCellRendererPixbuf*) gtk_cell_renderer_pixbuf_new ());
		self->priv->column = (_tmp5_ = g_object_ref_sink (gtk_tree_view_column_new ()), _g_object_unref0 (self->priv->column), _tmp5_);
		gtk_tree_view_column_set_resizable (self->priv->column, TRUE);
		gtk_cell_layout_pack_start ((GtkCellLayout*) self->priv->column, (GtkCellRenderer*) renderer, FALSE);
		g_object_set ((GObject*) renderer, "xalign", 0.0f, NULL);
		g_object_set ((GObject*) renderer, "yalign", 0.0f, NULL);
		gtk_tree_view_append_column (self->priv->tree, self->priv->column);
		gtk_tree_view_column_set_title (self->priv->column, _ ("Cover"));
		gtk_cell_layout_add_attribute ((GtkCellLayout*) self->priv->column, (GtkCellRenderer*) renderer, "pixbuf", 0);
		_g_object_unref0 (renderer);
	} else {
		GtkCellRendererText* renderer;
		GtkTreeViewColumn* _tmp6_;
		renderer = g_object_ref_sink ((GtkCellRendererText*) gtk_cell_renderer_text_new ());
		self->priv->column = (_tmp6_ = g_object_ref_sink (gtk_tree_view_column_new ()), _g_object_unref0 (self->priv->column), _tmp6_);
		gtk_tree_view_column_set_resizable (self->priv->column, TRUE);
		gtk_cell_layout_pack_start ((GtkCellLayout*) self->priv->column, (GtkCellRenderer*) renderer, FALSE);
		gtk_tree_view_append_column (self->priv->tree, self->priv->column);
		g_object_set ((GObject*) renderer, "wrap-mode", PANGO_WRAP_WORD, NULL);
		g_object_set ((GObject*) renderer, "wrap-width", 300, NULL);
		gtk_tree_view_column_set_title (self->priv->column, _ ("Lyric"));
		gtk_cell_layout_add_attribute ((GtkCellLayout*) self->priv->column, (GtkCellRenderer*) renderer, "text", 3);
		_g_object_unref0 (renderer);
	}
}


static void _gmpc_meta_data_edit_window_b_cancel_gtk_button_clicked (GtkButton* _sender, gpointer self) {
	gmpc_meta_data_edit_window_b_cancel (self);
}


static void _gmpc_meta_data_edit_window_destroy_popup_gtk_button_clicked (GtkButton* _sender, gpointer self) {
	gmpc_meta_data_edit_window_destroy_popup (self, _sender);
}


static void _gmpc_meta_data_edit_window_set_metadata_gtk_button_clicked (GtkButton* _sender, gpointer self) {
	gmpc_meta_data_edit_window_set_metadata (self, _sender);
}


static void _gmpc_meta_data_edit_window_combo_box_changed_gtk_combo_box_changed (GtkComboBox* _sender, gpointer self) {
	gmpc_meta_data_edit_window_combo_box_changed (self, _sender);
}


static void _gmpc_meta_data_edit_window_refresh_query_gtk_button_clicked (GtkButton* _sender, gpointer self) {
	gmpc_meta_data_edit_window_refresh_query (self, _sender);
}


GmpcMetaDataEditWindow* gmpc_meta_data_edit_window_construct (GType object_type, const mpd_Song* song, MetaDataType type) {
	GmpcMetaDataEditWindow * self;
	GtkVBox* vbox;
	mpd_Song* _tmp0_;
	GtkHBox* _tmp1_;
	GtkProgressBar* _tmp2_;
	GtkButton* _tmp3_;
	GtkListStore* _tmp4_;
	GtkScrolledWindow* sw;
	GtkTreeView* iv;
	GtkTreeView* _tmp5_;
	GtkCellRendererText* rendererpb;
	GtkTreeViewColumn* column;
	GtkHBox* hbox;
	GtkButton* button;
	GtkButton* _tmp6_;
	GtkLabel* _tmp7_;
	char* _tmp8_;
	GtkSizeGroup* group;
	GtkHBox* qhbox;
	GtkLabel* label;
	GtkComboBox* _tmp9_;
	GtkHBox* _tmp10_;
	GtkLabel* _tmp11_;
	GtkEntry* _tmp12_;
	GtkButton* _tmp20_;
	GtkButton* _tmp19_;
	GtkAlignment* ali;
	g_return_val_if_fail (song != NULL, NULL);
	self = g_object_newv (object_type, 0, NULL);
	vbox = g_object_ref_sink ((GtkVBox*) gtk_vbox_new (FALSE, 6));
	self->priv->song = (_tmp0_ = _mpd_songDup0 (song), _mpd_freeSong0 (self->priv->song), _tmp0_);
	self->priv->query_type = type;
	self->priv->pbox = (_tmp1_ = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 6)), _g_object_unref0 (self->priv->pbox), _tmp1_);
	self->priv->bar = (_tmp2_ = g_object_ref_sink ((GtkProgressBar*) gtk_progress_bar_new ()), _g_object_unref0 (self->priv->bar), _tmp2_);
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) self->priv->pbox, FALSE, FALSE, (guint) 0);
	self->priv->cancel = (_tmp3_ = g_object_ref_sink ((GtkButton*) gtk_button_new_from_stock ("gtk-cancel")), _g_object_unref0 (self->priv->cancel), _tmp3_);
	g_signal_connect_object (self->priv->cancel, "clicked", (GCallback) _gmpc_meta_data_edit_window_b_cancel_gtk_button_clicked, self, 0);
	gtk_box_pack_start ((GtkBox*) self->priv->pbox, (GtkWidget*) self->priv->bar, TRUE, TRUE, (guint) 0);
	gtk_box_pack_start ((GtkBox*) self->priv->pbox, (GtkWidget*) self->priv->cancel, FALSE, FALSE, (guint) 0);
	gtk_widget_show ((GtkWidget*) self->priv->bar);
	gtk_widget_show ((GtkWidget*) self->priv->cancel);
	gtk_widget_set_no_show_all ((GtkWidget*) self->priv->pbox, TRUE);
	gtk_widget_hide ((GtkWidget*) self->priv->pbox);
	self->priv->model = (_tmp4_ = gtk_list_store_new (4, GDK_TYPE_PIXBUF, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, NULL), _g_object_unref0 (self->priv->model), _tmp4_);
	sw = g_object_ref_sink ((GtkScrolledWindow*) gtk_scrolled_window_new (NULL, NULL));
	iv = g_object_ref_sink ((GtkTreeView*) gtk_tree_view_new ());
	self->priv->tree = (_tmp5_ = _g_object_ref0 (iv), _g_object_unref0 (self->priv->tree), _tmp5_);
	gtk_scrolled_window_set_policy (sw, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type (sw, GTK_SHADOW_ETCHED_IN);
	gtk_tree_view_set_model (iv, (GtkTreeModel*) self->priv->model);
	gtk_tree_view_set_rules_hint (self->priv->tree, TRUE);
	rendererpb = g_object_ref_sink ((GtkCellRendererText*) gtk_cell_renderer_text_new ());
	column = g_object_ref_sink (gtk_tree_view_column_new ());
	gtk_tree_view_column_set_resizable (column, TRUE);
	gtk_tree_view_column_set_min_width (column, 250);
	g_object_set ((GObject*) rendererpb, "xalign", 0.0f, NULL);
	g_object_set ((GObject*) rendererpb, "yalign", 0.0f, NULL);
	g_object_set ((GObject*) rendererpb, "ellipsize", PANGO_ELLIPSIZE_END, NULL);
	gtk_cell_layout_pack_start ((GtkCellLayout*) column, (GtkCellRenderer*) rendererpb, TRUE);
	gtk_tree_view_append_column (iv, column);
	gtk_tree_view_column_set_title (column, _ ("Information"));
	gtk_cell_layout_add_attribute ((GtkCellLayout*) column, (GtkCellRenderer*) rendererpb, "markup", 2);
	hbox = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 6));
	button = g_object_ref_sink ((GtkButton*) gtk_button_new_from_stock ("gtk-quit"));
	g_signal_connect_object (button, "clicked", (GCallback) _gmpc_meta_data_edit_window_destroy_popup_gtk_button_clicked, self, 0);
	gtk_box_pack_end ((GtkBox*) hbox, (GtkWidget*) button, FALSE, FALSE, (guint) 0);
	button = (_tmp6_ = g_object_ref_sink ((GtkButton*) gtk_button_new_with_mnemonic ("_Set")), _g_object_unref0 (button), _tmp6_);
	g_signal_connect_object (button, "clicked", (GCallback) _gmpc_meta_data_edit_window_set_metadata_gtk_button_clicked, self, 0);
	gtk_box_pack_end ((GtkBox*) hbox, (GtkWidget*) button, FALSE, FALSE, (guint) 0);
	gtk_box_pack_end ((GtkBox*) vbox, (GtkWidget*) hbox, FALSE, FALSE, (guint) 0);
	self->priv->warning_label = (_tmp7_ = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), _g_object_unref0 (self->priv->warning_label), _tmp7_);
	gtk_label_set_markup (self->priv->warning_label, _tmp8_ = g_strdup_printf ("<span size='x-large'>%s</span>", _ ("Insufficient information to store/fetch this metadata")));
	_g_free0 (_tmp8_);
	gtk_misc_set_alignment ((GtkMisc*) self->priv->warning_label, 0.0f, 0.5f);
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) self->priv->warning_label, FALSE, FALSE, (guint) 0);
	gtk_widget_hide ((GtkWidget*) self->priv->warning_label);
	group = gtk_size_group_new (GTK_SIZE_GROUP_HORIZONTAL);
	qhbox = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 6));
	label = g_object_ref_sink ((GtkLabel*) gtk_label_new ("Type"));
	gtk_size_group_add_widget (group, (GtkWidget*) label);
	gtk_box_pack_start ((GtkBox*) qhbox, (GtkWidget*) label, FALSE, FALSE, (guint) 0);
	self->priv->combo = (_tmp9_ = g_object_ref_sink ((GtkComboBox*) gtk_combo_box_new_text ()), _g_object_unref0 (self->priv->combo), _tmp9_);
	gtk_box_pack_start ((GtkBox*) qhbox, (GtkWidget*) self->priv->combo, FALSE, FALSE, (guint) 0);
	gtk_combo_box_append_text (self->priv->combo, _ ("Artist art"));
	gtk_combo_box_append_text (self->priv->combo, _ ("Album art"));
	gtk_combo_box_append_text (self->priv->combo, _ ("Song Lyrics"));
	gtk_combo_box_append_text (self->priv->combo, _ ("Album Info"));
	gtk_combo_box_append_text (self->priv->combo, _ ("Artist Biography"));
	gtk_combo_box_append_text (self->priv->combo, _ ("Guitar Tab"));
	g_signal_connect_object (self->priv->combo, "changed", (GCallback) _gmpc_meta_data_edit_window_combo_box_changed_gtk_combo_box_changed, self, 0);
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) qhbox, FALSE, FALSE, (guint) 0);
	qhbox = (_tmp10_ = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 6)), _g_object_unref0 (qhbox), _tmp10_);
	label = (_tmp11_ = g_object_ref_sink ((GtkLabel*) gtk_label_new (_ ("Artist"))), _g_object_unref0 (label), _tmp11_);
	gtk_size_group_add_widget (group, (GtkWidget*) label);
	gtk_box_pack_start ((GtkBox*) qhbox, (GtkWidget*) label, FALSE, FALSE, (guint) 0);
	self->priv->artist_entry = (_tmp12_ = g_object_ref_sink ((GtkEntry*) gtk_entry_new ()), _g_object_unref0 (self->priv->artist_entry), _tmp12_);
	gtk_entry_set_text (self->priv->artist_entry, song->artist);
	gtk_box_pack_start ((GtkBox*) qhbox, (GtkWidget*) self->priv->artist_entry, TRUE, TRUE, (guint) 0);
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) qhbox, FALSE, FALSE, (guint) 0);
	{
		GtkHBox* _tmp13_;
		GtkLabel* _tmp14_;
		GtkEntry* _tmp15_;
		qhbox = (_tmp13_ = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 6)), _g_object_unref0 (qhbox), _tmp13_);
		label = (_tmp14_ = g_object_ref_sink ((GtkLabel*) gtk_label_new (_ ("Album"))), _g_object_unref0 (label), _tmp14_);
		gtk_size_group_add_widget (group, (GtkWidget*) label);
		gtk_box_pack_start ((GtkBox*) qhbox, (GtkWidget*) label, FALSE, FALSE, (guint) 0);
		self->priv->album_entry = (_tmp15_ = g_object_ref_sink ((GtkEntry*) gtk_entry_new ()), _g_object_unref0 (self->priv->album_entry), _tmp15_);
		if (song->album != NULL) {
			gtk_entry_set_text (self->priv->album_entry, song->album);
		}
		gtk_box_pack_start ((GtkBox*) qhbox, (GtkWidget*) self->priv->album_entry, TRUE, TRUE, (guint) 0);
		gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) qhbox, FALSE, FALSE, (guint) 0);
	}
	{
		GtkHBox* _tmp16_;
		GtkLabel* _tmp17_;
		GtkEntry* _tmp18_;
		qhbox = (_tmp16_ = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 6)), _g_object_unref0 (qhbox), _tmp16_);
		label = (_tmp17_ = g_object_ref_sink ((GtkLabel*) gtk_label_new (_ ("Title"))), _g_object_unref0 (label), _tmp17_);
		gtk_size_group_add_widget (group, (GtkWidget*) label);
		gtk_box_pack_start ((GtkBox*) qhbox, (GtkWidget*) label, FALSE, FALSE, (guint) 0);
		self->priv->title_entry = (_tmp18_ = g_object_ref_sink ((GtkEntry*) gtk_entry_new ()), _g_object_unref0 (self->priv->title_entry), _tmp18_);
		if (song->title != NULL) {
			gtk_entry_set_text (self->priv->title_entry, song->title);
		}
		gtk_box_pack_start ((GtkBox*) qhbox, (GtkWidget*) self->priv->title_entry, TRUE, TRUE, (guint) 0);
		gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) qhbox, FALSE, FALSE, (guint) 0);
	}
	if (type != META_ALBUM_ART) {
		g_object_set ((GtkWidget*) self->priv->album_entry, "sensitive", FALSE, NULL);
	}
	self->priv->refresh = (_tmp20_ = _g_object_ref0 (button = (_tmp19_ = g_object_ref_sink ((GtkButton*) gtk_button_new_with_label (_ ("Query"))), _g_object_unref0 (button), _tmp19_)), _g_object_unref0 (self->priv->refresh), _tmp20_);
	ali = g_object_ref_sink ((GtkAlignment*) gtk_alignment_new (1.0f, 0.5f, 0.0f, 0.0f));
	gtk_container_add ((GtkContainer*) ali, (GtkWidget*) button);
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) ali, FALSE, FALSE, (guint) 0);
	g_signal_connect_object (button, "clicked", (GCallback) _gmpc_meta_data_edit_window_refresh_query_gtk_button_clicked, self, 0);
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) sw, TRUE, TRUE, (guint) 0);
	gtk_container_add ((GtkContainer*) self, (GtkWidget*) vbox);
	gtk_widget_hide_on_delete ((GtkWidget*) self);
	gtk_container_add ((GtkContainer*) sw, (GtkWidget*) iv);
	gtk_widget_show_all ((GtkWidget*) self);
	if (type == META_ARTIST_ART) {
		gtk_combo_box_set_active (self->priv->combo, 0);
	} else {
		if (type == META_ALBUM_ART) {
			gtk_combo_box_set_active (self->priv->combo, 1);
		} else {
			if (type == META_SONG_TXT) {
				gtk_combo_box_set_active (self->priv->combo, 2);
			} else {
				if (type == META_ALBUM_TXT) {
					gtk_combo_box_set_active (self->priv->combo, 3);
				} else {
					if (type == META_ARTIST_TXT) {
						gtk_combo_box_set_active (self->priv->combo, 4);
					} else {
						if (type == META_SONG_GUITAR_TAB) {
							gtk_combo_box_set_active (self->priv->combo, 5);
						}
					}
				}
			}
		}
	}
	_g_object_unref0 (vbox);
	_g_object_unref0 (sw);
	_g_object_unref0 (iv);
	_g_object_unref0 (rendererpb);
	_g_object_unref0 (column);
	_g_object_unref0 (hbox);
	_g_object_unref0 (button);
	_g_object_unref0 (group);
	_g_object_unref0 (qhbox);
	_g_object_unref0 (label);
	_g_object_unref0 (ali);
	return self;
}


GmpcMetaDataEditWindow* gmpc_meta_data_edit_window_new (const mpd_Song* song, MetaDataType type) {
	return gmpc_meta_data_edit_window_construct (GMPC_META_DATA_TYPE_EDIT_WINDOW, song, type);
}


void gmpc_meta_data_edit_window_b_cancel (GmpcMetaDataEditWindow* self) {
	g_return_if_fail (self != NULL);
	if (self->priv->handle != NULL) {
		metadata_get_list_cancel (self->priv->handle);
		self->priv->handle = NULL;
	}
	if (self->priv->handle2 != NULL) {
		metadata_get_list_cancel (self->priv->handle2);
		self->priv->handle2 = NULL;
	}
	g_list_first (self->priv->downloads);
	while (TRUE) {
		GEADAsyncHandler* handle;
		if (!(self->priv->downloads != NULL)) {
			break;
		}
		handle = (const GEADAsyncHandler*) self->priv->downloads->data;
		gmpc_easy_async_cancel (handle);
		g_list_first (self->priv->downloads);
	}
	gtk_widget_hide ((GtkWidget*) self->priv->pbox);
	g_object_set ((GtkWidget*) self->priv->refresh, "sensitive", TRUE, NULL);
	g_object_set ((GtkWidget*) self->priv->combo, "sensitive", TRUE, NULL);
}


static GObject * gmpc_meta_data_edit_window_constructor (GType type, guint n_construct_properties, GObjectConstructParam * construct_properties) {
	GObject * obj;
	GObjectClass * parent_class;
	GmpcMetaDataEditWindow * self;
	parent_class = G_OBJECT_CLASS (gmpc_meta_data_edit_window_parent_class);
	obj = parent_class->constructor (type, n_construct_properties, construct_properties);
	self = GMPC_META_DATA_EDIT_WINDOW (obj);
	{
		g_object_set ((GtkWindow*) self, "type", GTK_WINDOW_TOPLEVEL, NULL);
		gtk_window_set_default_size ((GtkWindow*) self, 650, 800);
		gtk_container_set_border_width ((GtkContainer*) self, (guint) 8);
	}
	return obj;
}


static void gmpc_meta_data_edit_window_class_init (GmpcMetaDataEditWindowClass * klass) {
	gmpc_meta_data_edit_window_parent_class = g_type_class_peek_parent (klass);
	g_type_class_add_private (klass, sizeof (GmpcMetaDataEditWindowPrivate));
	G_OBJECT_CLASS (klass)->constructor = gmpc_meta_data_edit_window_constructor;
	G_OBJECT_CLASS (klass)->finalize = gmpc_meta_data_edit_window_finalize;
}


static void gmpc_meta_data_edit_window_instance_init (GmpcMetaDataEditWindow * self) {
	self->priv = GMPC_META_DATA_EDIT_WINDOW_GET_PRIVATE (self);
	self->priv->model = NULL;
	self->priv->tree = NULL;
	self->priv->downloads = NULL;
	self->priv->song = NULL;
	self->priv->query_type = META_ALBUM_ART;
	self->priv->handle = NULL;
	self->priv->handle2 = NULL;
	self->priv->pbox = NULL;
	self->priv->warning_label = NULL;
	self->priv->cancel = NULL;
	self->priv->refresh = NULL;
	self->priv->combo = NULL;
	self->priv->bar = NULL;
	self->priv->column = NULL;
}


static void gmpc_meta_data_edit_window_finalize (GObject* obj) {
	GmpcMetaDataEditWindow * self;
	self = GMPC_META_DATA_EDIT_WINDOW (obj);
	{
		gmpc_meta_data_edit_window_b_cancel (self);
	}
	_g_object_unref0 (self->priv->model);
	_g_object_unref0 (self->priv->tree);
	_g_list_free0 (self->priv->downloads);
	_mpd_freeSong0 (self->priv->song);
	_g_object_unref0 (self->priv->pbox);
	_g_object_unref0 (self->priv->warning_label);
	_g_object_unref0 (self->priv->artist_entry);
	_g_object_unref0 (self->priv->album_entry);
	_g_object_unref0 (self->priv->title_entry);
	_g_object_unref0 (self->priv->cancel);
	_g_object_unref0 (self->priv->refresh);
	_g_object_unref0 (self->priv->combo);
	_g_object_unref0 (self->priv->bar);
	_g_object_unref0 (self->priv->column);
	G_OBJECT_CLASS (gmpc_meta_data_edit_window_parent_class)->finalize (obj);
}


GType gmpc_meta_data_edit_window_get_type (void) {
	static GType gmpc_meta_data_edit_window_type_id = 0;
	if (gmpc_meta_data_edit_window_type_id == 0) {
		static const GTypeInfo g_define_type_info = { sizeof (GmpcMetaDataEditWindowClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) gmpc_meta_data_edit_window_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (GmpcMetaDataEditWindow), 0, (GInstanceInitFunc) gmpc_meta_data_edit_window_instance_init, NULL };
		gmpc_meta_data_edit_window_type_id = g_type_register_static (GTK_TYPE_WINDOW, "GmpcMetaDataEditWindow", &g_define_type_info, 0);
	}
	return gmpc_meta_data_edit_window_type_id;
}


static gint* gmpc_test_plugin_real_get_version (GmpcPluginBase* base, int* result_length1) {
	GmpcTestPlugin * self;
	gint* result;
	gint* _tmp0_;
	self = (GmpcTestPlugin*) base;
	result = (_tmp0_ = GMPC_TEST_PLUGIN_version, *result_length1 = G_N_ELEMENTS (GMPC_TEST_PLUGIN_version), _tmp0_);
	return result;
}


static const char* gmpc_test_plugin_real_get_name (GmpcPluginBase* base) {
	GmpcTestPlugin * self;
	const char* result;
	self = (GmpcTestPlugin*) base;
	result = "Vala test plugin";
	return result;
}


static void gmpc_test_plugin_real_save_yourself (GmpcPluginBase* base) {
	GmpcTestPlugin * self;
	self = (GmpcTestPlugin*) base;
}


static gboolean gmpc_test_plugin_real_get_enabled (GmpcPluginBase* base) {
	GmpcTestPlugin * self;
	gboolean result;
	self = (GmpcTestPlugin*) base;
	result = (gboolean) cfg_get_single_value_as_int_with_default (config, gmpc_plugin_base_get_name ((GmpcPluginBase*) self), "enabled", 1);
	return result;
}


static void gmpc_test_plugin_real_set_enabled (GmpcPluginBase* base, gboolean state) {
	GmpcTestPlugin * self;
	self = (GmpcTestPlugin*) base;
	cfg_set_single_value_as_int (config, gmpc_plugin_base_get_name ((GmpcPluginBase*) self), "enabled", (gint) state);
}


static void _g_list_free_gtk_tree_path_free (GList* self) {
	g_list_foreach (self, (GFunc) gtk_tree_path_free, NULL);
	g_list_free (self);
}


static void gmpc_test_plugin_menu_activate_tree (GmpcTestPlugin* self, GtkMenuItem* item) {
	GtkTreeView* tv;
	GtkTreeModel* model;
	GtkTreeSelection* selection;
	g_return_if_fail (self != NULL);
	g_return_if_fail (item != NULL);
	tv = _g_object_ref0 (GTK_TREE_VIEW (g_object_get_data ((GObject*) item, "treeview")));
	model = _g_object_ref0 (gtk_tree_view_get_model (tv));
	selection = _g_object_ref0 (gtk_tree_view_get_selection (tv));
	{
		GtkTreeModel* _tmp2_;
		GList* _tmp1_;
		GtkTreeModel* _tmp0_;
		GList* path_collection;
		GList* path_it;
		_tmp0_ = NULL;
		path_collection = (_tmp1_ = gtk_tree_selection_get_selected_rows (selection, &_tmp0_), model = (_tmp2_ = _g_object_ref0 (_tmp0_), _g_object_unref0 (model), _tmp2_), _tmp1_);
		for (path_it = path_collection; path_it != NULL; path_it = path_it->next) {
			const GtkTreePath* path;
			path = (const GtkTreePath*) path_it->data;
			{
				GtkTreeIter iter = {0};
				if (gtk_tree_model_get_iter (model, &iter, path)) {
					const mpd_Song* song;
					song = NULL;
					gtk_tree_model_get (model, &iter, 0, &song, -1);
					if (song != NULL) {
						GmpcMetaDataEditWindow* _tmp3_;
						_tmp3_ = g_object_ref_sink (gmpc_meta_data_edit_window_new (song, META_ALBUM_ART));
						_g_object_unref0 (_tmp3_);
					}
				}
			}
		}
		__g_list_free_gtk_tree_path_free0 (path_collection);
	}
	_g_object_unref0 (tv);
	_g_object_unref0 (model);
	_g_object_unref0 (selection);
}


static void _gmpc_test_plugin_menu_activate_tree_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self) {
	gmpc_test_plugin_menu_activate_tree (self, _sender);
}


static gint gmpc_test_plugin_real_song_list (GmpcPluginSongListIface* base, GtkWidget* tree, GtkMenu* menu) {
	GmpcTestPlugin * self;
	gint result;
	GtkTreeView* tv;
	GtkTreeSelection* selection;
	self = (GmpcTestPlugin*) base;
	g_return_val_if_fail (tree != NULL, 0);
	g_return_val_if_fail (menu != NULL, 0);
	tv = _g_object_ref0 (GTK_TREE_VIEW (tree));
	selection = _g_object_ref0 (gtk_tree_view_get_selection (tv));
	if (gtk_tree_selection_count_selected_rows (selection) > 0) {
		GtkImageMenuItem* item;
		GtkImage* _tmp0_;
		item = g_object_ref_sink ((GtkImageMenuItem*) gtk_image_menu_item_new_with_label (_ ("Metadata selector")));
		gtk_image_menu_item_set_image (item, (GtkWidget*) (_tmp0_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_stock ("gtk-edit", GTK_ICON_SIZE_MENU))));
		_g_object_unref0 (_tmp0_);
		g_object_set_data ((GObject*) item, "treeview", tv);
		gtk_menu_shell_append ((GtkMenuShell*) menu, (GtkWidget*) ((GtkMenuItem*) item));
		g_signal_connect_object ((GtkMenuItem*) item, "activate", (GCallback) _gmpc_test_plugin_menu_activate_tree_gtk_menu_item_activate, self, 0);
		result = 1;
		_g_object_unref0 (item);
		_g_object_unref0 (tv);
		_g_object_unref0 (selection);
		return result;
	}
	result = 0;
	_g_object_unref0 (tv);
	_g_object_unref0 (selection);
	return result;
}


void gmpc_test_plugin_menu_activated_album (GmpcTestPlugin* self, GtkMenuItem* item) {
	const mpd_Song* song;
	GmpcMetaDataEditWindow* _tmp0_;
	g_return_if_fail (self != NULL);
	g_return_if_fail (item != NULL);
	song = mpd_playlist_get_current_song (connection);
	_tmp0_ = g_object_ref_sink (gmpc_meta_data_edit_window_new (song, META_ALBUM_ART));
	_g_object_unref0 (_tmp0_);
}


static void _gmpc_test_plugin_menu_activated_album_gtk_menu_item_activate (GtkMenuItem* _sender, gpointer self) {
	gmpc_test_plugin_menu_activated_album (self, _sender);
}


static gint gmpc_test_plugin_real_tool_menu_integration (GmpcPluginToolMenuIface* base, GtkMenu* menu) {
	GmpcTestPlugin * self;
	gint result;
	GtkMenuItem* item;
	self = (GmpcTestPlugin*) base;
	g_return_val_if_fail (menu != NULL, 0);
	item = g_object_ref_sink ((GtkMenuItem*) gtk_menu_item_new_with_label ("Edit metadata current song"));
	gtk_menu_shell_append ((GtkMenuShell*) menu, (GtkWidget*) item);
	g_signal_connect_object (item, "activate", (GCallback) _gmpc_test_plugin_menu_activated_album_gtk_menu_item_activate, self, 0);
	result = 2;
	_g_object_unref0 (item);
	return result;
}


GmpcTestPlugin* gmpc_test_plugin_construct (GType object_type) {
	GmpcTestPlugin * self;
	self = g_object_newv (object_type, 0, NULL);
	return self;
}


GmpcTestPlugin* gmpc_test_plugin_new (void) {
	return gmpc_test_plugin_construct (GMPC_TYPE_TEST_PLUGIN);
}


static GObject * gmpc_test_plugin_constructor (GType type, guint n_construct_properties, GObjectConstructParam * construct_properties) {
	GObject * obj;
	GObjectClass * parent_class;
	GmpcTestPlugin * self;
	parent_class = G_OBJECT_CLASS (gmpc_test_plugin_parent_class);
	obj = parent_class->constructor (type, n_construct_properties, construct_properties);
	self = GMPC_TEST_PLUGIN (obj);
	{
		((GmpcPluginBase*) self)->plugin_type = 8 + 4;
	}
	return obj;
}


static void gmpc_test_plugin_class_init (GmpcTestPluginClass * klass) {
	gmpc_test_plugin_parent_class = g_type_class_peek_parent (klass);
	GMPC_PLUGIN_BASE_CLASS (klass)->get_version = gmpc_test_plugin_real_get_version;
	GMPC_PLUGIN_BASE_CLASS (klass)->get_name = gmpc_test_plugin_real_get_name;
	GMPC_PLUGIN_BASE_CLASS (klass)->save_yourself = gmpc_test_plugin_real_save_yourself;
	GMPC_PLUGIN_BASE_CLASS (klass)->get_enabled = gmpc_test_plugin_real_get_enabled;
	GMPC_PLUGIN_BASE_CLASS (klass)->set_enabled = gmpc_test_plugin_real_set_enabled;
	G_OBJECT_CLASS (klass)->constructor = gmpc_test_plugin_constructor;
}


static void gmpc_test_plugin_gmpc_plugin_tool_menu_iface_interface_init (GmpcPluginToolMenuIfaceIface * iface) {
	gmpc_test_plugin_gmpc_plugin_tool_menu_iface_parent_iface = g_type_interface_peek_parent (iface);
	iface->tool_menu_integration = gmpc_test_plugin_real_tool_menu_integration;
}


static void gmpc_test_plugin_gmpc_plugin_song_list_iface_interface_init (GmpcPluginSongListIfaceIface * iface) {
	gmpc_test_plugin_gmpc_plugin_song_list_iface_parent_iface = g_type_interface_peek_parent (iface);
	iface->song_list = gmpc_test_plugin_real_song_list;
}


static void gmpc_test_plugin_instance_init (GmpcTestPlugin * self) {
}


GType gmpc_test_plugin_get_type (void) {
	static GType gmpc_test_plugin_type_id = 0;
	if (gmpc_test_plugin_type_id == 0) {
		static const GTypeInfo g_define_type_info = { sizeof (GmpcTestPluginClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) gmpc_test_plugin_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (GmpcTestPlugin), 0, (GInstanceInitFunc) gmpc_test_plugin_instance_init, NULL };
		static const GInterfaceInfo gmpc_plugin_tool_menu_iface_info = { (GInterfaceInitFunc) gmpc_test_plugin_gmpc_plugin_tool_menu_iface_interface_init, (GInterfaceFinalizeFunc) NULL, NULL};
		static const GInterfaceInfo gmpc_plugin_song_list_iface_info = { (GInterfaceInitFunc) gmpc_test_plugin_gmpc_plugin_song_list_iface_interface_init, (GInterfaceFinalizeFunc) NULL, NULL};
		gmpc_test_plugin_type_id = g_type_register_static (GMPC_PLUGIN_TYPE_BASE, "GmpcTestPlugin", &g_define_type_info, 0);
		g_type_add_interface_static (gmpc_test_plugin_type_id, GMPC_PLUGIN_TYPE_TOOL_MENU_IFACE, &gmpc_plugin_tool_menu_iface_info);
		g_type_add_interface_static (gmpc_test_plugin_type_id, GMPC_PLUGIN_TYPE_SONG_LIST_IFACE, &gmpc_plugin_song_list_iface_info);
	}
	return gmpc_test_plugin_type_id;
}




