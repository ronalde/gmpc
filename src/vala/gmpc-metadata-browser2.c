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
#include <config.h>
#include <libmpd/libmpdclient.h>
#include <libmpd/libmpd.h>
#include <metadata.h>
#include <stdlib.h>
#include <string.h>
#include <glib/gi18n-lib.h>
#include <plugin.h>
#include <mpdinteraction.h>
#include <gdk/gdk.h>
#include <gmpc-mpddata-model.h>
#include <gmpc-meta-watcher.h>
#include <main.h>
#include <gmpc-plugin.h>
#include <misc.h>
#include <gmpc-metaimage.h>
#include <pango/pango.h>
#include <config1.h>
#include <gmpc-connection.h>
#include <float.h>
#include <math.h>
#include <gdk-pixbuf/gdk-pixdata.h>
#include <gmpc-paned-size-group.h>
#include <gmpc-mpddata-treeview-tooltip.h>
#include <gmpc-favorites.h>
#include <gmpc-rating.h>
#include <gmpc-meta-text-view.h>
#include <gmpc-song-links.h>
#include <gmpc-stats-label.h>


#define GMPC_WIDGET_TYPE_SIMILAR_SONGS (gmpc_widget_similar_songs_get_type ())
#define GMPC_WIDGET_SIMILAR_SONGS(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), GMPC_WIDGET_TYPE_SIMILAR_SONGS, GmpcWidgetSimilarSongs))
#define GMPC_WIDGET_SIMILAR_SONGS_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), GMPC_WIDGET_TYPE_SIMILAR_SONGS, GmpcWidgetSimilarSongsClass))
#define GMPC_WIDGET_IS_SIMILAR_SONGS(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GMPC_WIDGET_TYPE_SIMILAR_SONGS))
#define GMPC_WIDGET_IS_SIMILAR_SONGS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GMPC_WIDGET_TYPE_SIMILAR_SONGS))
#define GMPC_WIDGET_SIMILAR_SONGS_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), GMPC_WIDGET_TYPE_SIMILAR_SONGS, GmpcWidgetSimilarSongsClass))

typedef struct _GmpcWidgetSimilarSongs GmpcWidgetSimilarSongs;
typedef struct _GmpcWidgetSimilarSongsClass GmpcWidgetSimilarSongsClass;
typedef struct _GmpcWidgetSimilarSongsPrivate GmpcWidgetSimilarSongsPrivate;
#define _mpd_freeSong0(var) ((var == NULL) ? NULL : (var = (mpd_freeSong (var), NULL)))
#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))
#define _meta_data_free0(var) ((var == NULL) ? NULL : (var = (meta_data_free (var), NULL)))
#define _mpd_data_free0(var) ((var == NULL) ? NULL : (var = (mpd_data_free (var), NULL)))
#define _g_free0(var) (var = (g_free (var), NULL))
#define _gtk_tree_path_free0(var) ((var == NULL) ? NULL : (var = (gtk_tree_path_free (var), NULL)))
#define __g_list_free_gtk_tree_path_free0(var) ((var == NULL) ? NULL : (var = (_g_list_free_gtk_tree_path_free (var), NULL)))

#define GMPC_WIDGET_TYPE_SIMILAR_ARTIST (gmpc_widget_similar_artist_get_type ())
#define GMPC_WIDGET_SIMILAR_ARTIST(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), GMPC_WIDGET_TYPE_SIMILAR_ARTIST, GmpcWidgetSimilarArtist))
#define GMPC_WIDGET_SIMILAR_ARTIST_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), GMPC_WIDGET_TYPE_SIMILAR_ARTIST, GmpcWidgetSimilarArtistClass))
#define GMPC_WIDGET_IS_SIMILAR_ARTIST(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GMPC_WIDGET_TYPE_SIMILAR_ARTIST))
#define GMPC_WIDGET_IS_SIMILAR_ARTIST_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GMPC_WIDGET_TYPE_SIMILAR_ARTIST))
#define GMPC_WIDGET_SIMILAR_ARTIST_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), GMPC_WIDGET_TYPE_SIMILAR_ARTIST, GmpcWidgetSimilarArtistClass))

typedef struct _GmpcWidgetSimilarArtist GmpcWidgetSimilarArtist;
typedef struct _GmpcWidgetSimilarArtistClass GmpcWidgetSimilarArtistClass;
typedef struct _GmpcWidgetSimilarArtistPrivate GmpcWidgetSimilarArtistPrivate;

#define GMPC_TYPE_METADATA_BROWSER (gmpc_metadata_browser_get_type ())
#define GMPC_METADATA_BROWSER(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), GMPC_TYPE_METADATA_BROWSER, GmpcMetadataBrowser))
#define GMPC_METADATA_BROWSER_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), GMPC_TYPE_METADATA_BROWSER, GmpcMetadataBrowserClass))
#define GMPC_IS_METADATA_BROWSER(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GMPC_TYPE_METADATA_BROWSER))
#define GMPC_IS_METADATA_BROWSER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GMPC_TYPE_METADATA_BROWSER))
#define GMPC_METADATA_BROWSER_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), GMPC_TYPE_METADATA_BROWSER, GmpcMetadataBrowserClass))

typedef struct _GmpcMetadataBrowser GmpcMetadataBrowser;
typedef struct _GmpcMetadataBrowserClass GmpcMetadataBrowserClass;
#define __g_list_free_g_object_unref0(var) ((var == NULL) ? NULL : (var = (_g_list_free_g_object_unref (var), NULL)))
#define _g_list_free0(var) ((var == NULL) ? NULL : (var = (g_list_free (var), NULL)))

#define GMPC_WIDGET_TYPE_MORE (gmpc_widget_more_get_type ())
#define GMPC_WIDGET_MORE(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), GMPC_WIDGET_TYPE_MORE, GmpcWidgetMore))
#define GMPC_WIDGET_MORE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), GMPC_WIDGET_TYPE_MORE, GmpcWidgetMoreClass))
#define GMPC_WIDGET_IS_MORE(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GMPC_WIDGET_TYPE_MORE))
#define GMPC_WIDGET_IS_MORE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GMPC_WIDGET_TYPE_MORE))
#define GMPC_WIDGET_MORE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), GMPC_WIDGET_TYPE_MORE, GmpcWidgetMoreClass))

typedef struct _GmpcWidgetMore GmpcWidgetMore;
typedef struct _GmpcWidgetMoreClass GmpcWidgetMoreClass;
typedef struct _GmpcWidgetMorePrivate GmpcWidgetMorePrivate;

#define GMPC_TYPE_NOW_PLAYING (gmpc_now_playing_get_type ())
#define GMPC_NOW_PLAYING(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), GMPC_TYPE_NOW_PLAYING, GmpcNowPlaying))
#define GMPC_NOW_PLAYING_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), GMPC_TYPE_NOW_PLAYING, GmpcNowPlayingClass))
#define GMPC_IS_NOW_PLAYING(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GMPC_TYPE_NOW_PLAYING))
#define GMPC_IS_NOW_PLAYING_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GMPC_TYPE_NOW_PLAYING))
#define GMPC_NOW_PLAYING_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), GMPC_TYPE_NOW_PLAYING, GmpcNowPlayingClass))

typedef struct _GmpcNowPlaying GmpcNowPlaying;
typedef struct _GmpcNowPlayingClass GmpcNowPlayingClass;
typedef struct _GmpcNowPlayingPrivate GmpcNowPlayingPrivate;
#define _gtk_tree_row_reference_free0(var) ((var == NULL) ? NULL : (var = (gtk_tree_row_reference_free (var), NULL)))
#define _g_error_free0(var) ((var == NULL) ? NULL : (var = (g_error_free (var), NULL)))
#define _gtk_icon_info_free0(var) ((var == NULL) ? NULL : (var = (gtk_icon_info_free (var), NULL)))
typedef struct _GmpcMetadataBrowserPrivate GmpcMetadataBrowserPrivate;

#define GMPC_METADATA_BROWSER_TYPE_HITEM (gmpc_metadata_browser_hitem_get_type ())

#define GMPC_METADATA_BROWSER_TYPE_HITEM_TYPE (gmpc_metadata_browser_hitem_type_get_type ())
typedef struct _GmpcMetadataBrowserHitem GmpcMetadataBrowserHitem;
#define __g_list_free_gmpc_metadata_browser_hitem_free0(var) ((var == NULL) ? NULL : (var = (_g_list_free_gmpc_metadata_browser_hitem_free (var), NULL)))
#define _gmpc_metadata_browser_hitem_free0(var) ((var == NULL) ? NULL : (var = (gmpc_metadata_browser_hitem_free (var), NULL)))

struct _GmpcWidgetSimilarSongs {
	GtkExpander parent_instance;
	GmpcWidgetSimilarSongsPrivate * priv;
};

struct _GmpcWidgetSimilarSongsClass {
	GtkExpanderClass parent_class;
};

struct _GmpcWidgetSimilarSongsPrivate {
	mpd_Song* song;
	gboolean filled;
	GtkWidget* pchild;
	guint idle_add;
	MetaData* copy;
	MpdData* item;
	GList* current;
};

struct _GmpcWidgetSimilarArtist {
	GtkTable parent_instance;
	GmpcWidgetSimilarArtistPrivate * priv;
};

struct _GmpcWidgetSimilarArtistClass {
	GtkTableClass parent_class;
};

struct _GmpcWidgetSimilarArtistPrivate {
	mpd_Song* song;
	GmpcMetadataBrowser* browser;
};

struct _GmpcWidgetMore {
	GtkFrame parent_instance;
	GmpcWidgetMorePrivate * priv;
};

struct _GmpcWidgetMoreClass {
	GtkFrameClass parent_class;
};

struct _GmpcWidgetMorePrivate {
	GtkAlignment* ali;
	gint expand_state;
	GtkButton* expand_button;
	gint max_height;
	GtkEventBox* eventbox;
	GtkWidget* pchild;
	char* unique_id;
};

struct _GmpcNowPlaying {
	GmpcPluginBase parent_instance;
	GmpcNowPlayingPrivate * priv;
};

struct _GmpcNowPlayingClass {
	GmpcPluginBaseClass parent_class;
};

struct _GmpcNowPlayingPrivate {
	GtkTreeRowReference* np_ref;
	GmpcMetadataBrowser* browser;
	GtkScrolledWindow* paned;
	GtkEventBox* container;
	gboolean selected;
	char* song_checksum;
};

struct _GmpcMetadataBrowser {
	GmpcPluginBase parent_instance;
	GmpcMetadataBrowserPrivate * priv;
};

struct _GmpcMetadataBrowserClass {
	GmpcPluginBaseClass parent_class;
};

typedef enum  {
	GMPC_METADATA_BROWSER_HITEM_TYPE_CLEAR,
	GMPC_METADATA_BROWSER_HITEM_TYPE_ARTIST,
	GMPC_METADATA_BROWSER_HITEM_TYPE_ALBUM,
	GMPC_METADATA_BROWSER_HITEM_TYPE_SONG
} GmpcMetadataBrowserHitemType;

struct _GmpcMetadataBrowserHitem {
	GmpcMetadataBrowserHitemType type;
	mpd_Song* song;
};

struct _GmpcMetadataBrowserPrivate {
	gint block_update;
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
	guint update_timeout;
	gboolean selected;
	GList* history;
	GList* current;
};


static gpointer gmpc_widget_similar_songs_parent_class = NULL;
static gpointer gmpc_widget_similar_artist_parent_class = NULL;
static gpointer gmpc_widget_more_parent_class = NULL;
static gpointer gmpc_now_playing_parent_class = NULL;
static GmpcPluginBrowserIfaceIface* gmpc_now_playing_gmpc_plugin_browser_iface_parent_iface = NULL;
static gpointer gmpc_metadata_browser_parent_class = NULL;
static GmpcPluginBrowserIfaceIface* gmpc_metadata_browser_gmpc_plugin_browser_iface_parent_iface = NULL;
static GmpcPluginPreferencesIfaceIface* gmpc_metadata_browser_gmpc_plugin_preferences_iface_parent_iface = NULL;

#define use_transition_mb TRUE
#define some_unique_name_mb VERSION
GType gmpc_widget_similar_songs_get_type (void);
#define GMPC_WIDGET_SIMILAR_SONGS_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), GMPC_WIDGET_TYPE_SIMILAR_SONGS, GmpcWidgetSimilarSongsPrivate))
enum  {
	GMPC_WIDGET_SIMILAR_SONGS_DUMMY_PROPERTY
};
GmpcWidgetSimilarSongs* gmpc_widget_similar_songs_new (const mpd_Song* song);
GmpcWidgetSimilarSongs* gmpc_widget_similar_songs_construct (GType object_type, const mpd_Song* song);
static void _g_list_free_gtk_tree_path_free (GList* self);
static void gmpc_widget_similar_songs_add_clicked (GmpcWidgetSimilarSongs* self, GtkImageMenuItem* item);
static void gmpc_widget_similar_songs_play_clicked (GmpcWidgetSimilarSongs* self, GtkImageMenuItem* item);
static void gmpc_widget_similar_songs_replace_clicked (GmpcWidgetSimilarSongs* self, GtkImageMenuItem* item);
static void gmpc_widget_similar_songs_tree_row_activated (GmpcWidgetSimilarSongs* self, GmpcMpdDataTreeview* tree, const GtkTreePath* path, GtkTreeViewColumn* column);
static void _gmpc_widget_similar_songs_play_clicked_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self);
static void _gmpc_widget_similar_songs_add_clicked_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self);
static void _gmpc_widget_similar_songs_replace_clicked_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self);
static gboolean gmpc_widget_similar_songs_tree_right_menu (GmpcWidgetSimilarSongs* self, GmpcMpdDataTreeview* tree, const GdkEventButton* event);
static gboolean _gmpc_widget_similar_songs_tree_right_menu_gtk_widget_button_release_event (GmpcMpdDataTreeview* _sender, const GdkEventButton* event, gpointer self);
static void _gmpc_widget_similar_songs_tree_row_activated_gtk_tree_view_row_activated (GmpcMpdDataTreeview* _sender, const GtkTreePath* path, GtkTreeViewColumn* column, gpointer self);
static gboolean gmpc_widget_similar_songs_update_sim_song (GmpcWidgetSimilarSongs* self);
static gboolean _gmpc_widget_similar_songs_update_sim_song_gsource_func (gpointer self);
static void gmpc_widget_similar_songs_metadata_changed (GmpcWidgetSimilarSongs* self, GmpcMetaWatcher* gmw2, const mpd_Song* song, MetaDataType type, MetaDataResult _result_, const MetaData* met);
static void _gmpc_widget_similar_songs_metadata_changed_gmpc_meta_watcher_data_changed (GmpcMetaWatcher* _sender, const mpd_Song* song, MetaDataType type, MetaDataResult _result_, const MetaData* met, gpointer self);
static void gmpc_widget_similar_songs_update (GmpcWidgetSimilarSongs* self);
static void gmpc_widget_similar_songs_real_activate (GtkExpander* base);
static void gmpc_widget_similar_songs_finalize (GObject* obj);
GType gmpc_widget_similar_artist_get_type (void);
GType gmpc_metadata_browser_get_type (void);
#define GMPC_WIDGET_SIMILAR_ARTIST_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), GMPC_WIDGET_TYPE_SIMILAR_ARTIST, GmpcWidgetSimilarArtistPrivate))
enum  {
	GMPC_WIDGET_SIMILAR_ARTIST_DUMMY_PROPERTY
};
GtkWidget* gmpc_widget_similar_artist_new_artist_button (GmpcWidgetSimilarArtist* self, const char* artist, gboolean in_db);
static void _g_list_free_g_object_unref (GList* self);
static void gmpc_widget_similar_artist_metadata_changed (GmpcWidgetSimilarArtist* self, GmpcMetaWatcher* gmw2, const mpd_Song* song, MetaDataType type, MetaDataResult _result_, const MetaData* met);
void gmpc_metadata_browser_set_artist (GmpcMetadataBrowser* self, const char* artist);
static void gmpc_widget_similar_artist_artist_button_clicked (GmpcWidgetSimilarArtist* self, GtkButton* button);
static gboolean _misc_header_expose_event_gtk_widget_expose_event (GtkWidget* _sender, const GdkEventExpose* event, gpointer self);
static void _gmpc_widget_similar_artist_artist_button_clicked_gtk_button_clicked (GtkButton* _sender, gpointer self);
static void _gmpc_widget_similar_artist_metadata_changed_gmpc_meta_watcher_data_changed (GmpcMetaWatcher* _sender, const mpd_Song* song, MetaDataType type, MetaDataResult _result_, const MetaData* met, gpointer self);
GmpcWidgetSimilarArtist* gmpc_widget_similar_artist_new (GmpcMetadataBrowser* browser, MpdObj* server, const mpd_Song* song);
GmpcWidgetSimilarArtist* gmpc_widget_similar_artist_construct (GType object_type, GmpcMetadataBrowser* browser, MpdObj* server, const mpd_Song* song);
static void gmpc_widget_similar_artist_finalize (GObject* obj);
GType gmpc_widget_more_get_type (void);
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
GmpcWidgetMore* gmpc_widget_more_new (const char* unique_id, const char* markup, GtkWidget* child);
GmpcWidgetMore* gmpc_widget_more_construct (GType object_type, const char* unique_id, const char* markup, GtkWidget* child);
static void gmpc_widget_more_finalize (GObject* obj);
GType gmpc_now_playing_get_type (void);
#define GMPC_NOW_PLAYING_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), GMPC_TYPE_NOW_PLAYING, GmpcNowPlayingPrivate))
enum  {
	GMPC_NOW_PLAYING_DUMMY_PROPERTY
};
static gint* gmpc_now_playing_real_get_version (GmpcPluginBase* base, int* result_length1);
static const char* gmpc_now_playing_real_get_name (GmpcPluginBase* base);
static void gmpc_now_playing_real_save_yourself (GmpcPluginBase* base);
static void gmpc_now_playing_update (GmpcNowPlaying* self);
static void gmpc_now_playing_status_changed (GmpcNowPlaying* self, GmpcConnection* conn, MpdObj* server, ChangedStatusType what);
static void gmpc_now_playing_real_browser_add (GmpcPluginBrowserIface* base, GtkWidget* category_tree);
static void gmpc_now_playing_browser_init (GmpcNowPlaying* self);
static void gmpc_now_playing_real_browser_selected (GmpcPluginBrowserIface* base, GtkContainer* container);
static void gmpc_now_playing_real_browser_unselected (GmpcPluginBrowserIface* base, GtkContainer* container);
static void gmpc_now_playing_browser_bg_style_changed (GmpcNowPlaying* self, GtkScrolledWindow* bg, GtkStyle* style);
static gboolean gmpc_now_playing_browser_key_release_event (GmpcNowPlaying* self, const GdkEventKey* event);
static void _gmpc_now_playing_browser_bg_style_changed_gtk_widget_style_set (GtkScrolledWindow* _sender, GtkStyle* previous_style, gpointer self);
static gboolean _gmpc_now_playing_browser_key_release_event_gtk_widget_key_release_event (GtkScrolledWindow* _sender, const GdkEventKey* event, gpointer self);
GtkWidget* gmpc_metadata_browser_metadata_box_show_song (GmpcMetadataBrowser* self, const mpd_Song* song, gboolean show_controls);
static void gmpc_now_playing_select_now_playing_browser (GmpcNowPlaying* self, GtkImageMenuItem* item);
static void _gmpc_now_playing_select_now_playing_browser_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self);
static gint gmpc_now_playing_real_browser_add_go_menu (GmpcPluginBrowserIface* base, GtkMenu* menu);
GmpcNowPlaying* gmpc_now_playing_new (void);
GmpcNowPlaying* gmpc_now_playing_construct (GType object_type);
static void _gmpc_now_playing_status_changed_gmpc_connection_status_changed (GmpcConnection* _sender, MpdObj* server, ChangedStatusType what, gpointer self);
GmpcMetadataBrowser* gmpc_metadata_browser_new (void);
GmpcMetadataBrowser* gmpc_metadata_browser_construct (GType object_type);
static GObject * gmpc_now_playing_constructor (GType type, guint n_construct_properties, GObjectConstructParam * construct_properties);
static void gmpc_now_playing_finalize (GObject* obj);
static GType gmpc_metadata_browser_hitem_get_type (void);
static GType gmpc_metadata_browser_hitem_type_get_type (void);
static GmpcMetadataBrowserHitem* gmpc_metadata_browser_hitem_dup (const GmpcMetadataBrowserHitem* self);
static void gmpc_metadata_browser_hitem_free (GmpcMetadataBrowserHitem* self);
static void gmpc_metadata_browser_hitem_copy (const GmpcMetadataBrowserHitem* self, GmpcMetadataBrowserHitem* dest);
static void gmpc_metadata_browser_hitem_destroy (GmpcMetadataBrowserHitem* self);
#define GMPC_METADATA_BROWSER_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), GMPC_TYPE_METADATA_BROWSER, GmpcMetadataBrowserPrivate))
enum  {
	GMPC_METADATA_BROWSER_DUMMY_PROPERTY
};
static void _g_list_free_gmpc_metadata_browser_hitem_free (GList* self);
static gint* gmpc_metadata_browser_real_get_version (GmpcPluginBase* base, int* result_length1);
static const char* gmpc_metadata_browser_real_get_name (GmpcPluginBase* base);
static void gmpc_metadata_browser_real_save_yourself (GmpcPluginBase* base);
void gmpc_metadata_browser_select_browser (GmpcMetadataBrowser* self, GtkTreeView* tree);
static void gmpc_metadata_browser_select_metadata_browser (GmpcMetadataBrowser* self, GtkImageMenuItem* item);
static void _gmpc_metadata_browser_select_metadata_browser_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self);
static gint gmpc_metadata_browser_real_browser_add_go_menu (GmpcPluginBrowserIface* base, GtkMenu* menu);
static void gmpc_metadata_browser_browser_bg_style_changed (GmpcMetadataBrowser* self, GtkScrolledWindow* bg, GtkStyle* style);
static gboolean gmpc_metadata_browser_browser_button_press_event (GmpcMetadataBrowser* self, GtkTreeView* tree, const GdkEventButton* event);
static void gmpc_metadata_browser_browser_artist_entry_changed (GmpcMetadataBrowser* self, GtkEntry* entry);
static char* gmpc_metadata_browser_browser_get_selected_artist (GmpcMetadataBrowser* self);
static void gmpc_metadata_browser_artist_add_clicked (GmpcMetadataBrowser* self, GtkImageMenuItem* item);
static void gmpc_metadata_browser_artist_replace_clicked (GmpcMetadataBrowser* self, GtkImageMenuItem* item);
static void _gmpc_metadata_browser_artist_add_clicked_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self);
static void _gmpc_metadata_browser_artist_replace_clicked_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self);
static gboolean gmpc_metadata_browser_artist_browser_button_release_event (GmpcMetadataBrowser* self, GtkTreeView* tree, const GdkEventButton* event);
static gboolean gmpc_metadata_browser_visible_func_artist (GmpcMetadataBrowser* self, GtkTreeModel* model, GtkTreeIter* iter);
static gboolean gmpc_metadata_browser_browser_artist_key_press_event (GmpcMetadataBrowser* self, GtkTreeView* widget, const GdkEventKey* event);
static char* gmpc_metadata_browser_browser_get_selected_album (GmpcMetadataBrowser* self);
static void gmpc_metadata_browser_album_add_clicked (GmpcMetadataBrowser* self, GtkImageMenuItem* item);
static void gmpc_metadata_browser_album_replace_clicked (GmpcMetadataBrowser* self, GtkImageMenuItem* item);
static void _gmpc_metadata_browser_album_add_clicked_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self);
static void _gmpc_metadata_browser_album_replace_clicked_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self);
static gboolean gmpc_metadata_browser_album_browser_button_release_event (GmpcMetadataBrowser* self, GtkTreeView* tree, const GdkEventButton* event);
static gboolean gmpc_metadata_browser_visible_func_album (GmpcMetadataBrowser* self, GtkTreeModel* model, GtkTreeIter* iter);
static gboolean gmpc_metadata_browser_browser_album_key_press_event (GmpcMetadataBrowser* self, GtkTreeView* widget, const GdkEventKey* event);
static void gmpc_metadata_browser_browser_album_entry_changed (GmpcMetadataBrowser* self, GtkEntry* entry);
static mpd_Song* gmpc_metadata_browser_browser_get_selected_song (GmpcMetadataBrowser* self);
static void gmpc_metadata_browser_song_add_clicked (GmpcMetadataBrowser* self, GtkImageMenuItem* item);
static void gmpc_metadata_browser_song_replace_clicked (GmpcMetadataBrowser* self, GtkImageMenuItem* item);
static void _gmpc_metadata_browser_song_add_clicked_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self);
static void _gmpc_metadata_browser_song_replace_clicked_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self);
static gboolean gmpc_metadata_browser_song_browser_button_release_event (GmpcMetadataBrowser* self, GtkTreeView* tree, const GdkEventButton* event);
static void gmpc_metadata_browser_history_previous (GmpcMetadataBrowser* self);
static void gmpc_metadata_browser_history_next (GmpcMetadataBrowser* self);
static gboolean gmpc_metadata_browser_browser_button_release_event (GmpcMetadataBrowser* self, GtkWidget* widget, const GdkEventButton* event);
static void _gmpc_metadata_browser_browser_artist_entry_changed_gtk_editable_changed (GtkEntry* _sender, gpointer self);
static gboolean _gmpc_metadata_browser_visible_func_artist_gtk_tree_model_filter_visible_func (GtkTreeModel* model, GtkTreeIter* iter, gpointer self);
static gboolean _gmpc_metadata_browser_browser_button_press_event_gtk_widget_button_press_event (GtkTreeView* _sender, const GdkEventButton* event, gpointer self);
static gboolean _gmpc_metadata_browser_artist_browser_button_release_event_gtk_widget_button_release_event (GtkTreeView* _sender, const GdkEventButton* event, gpointer self);
static gboolean _gmpc_metadata_browser_browser_artist_key_press_event_gtk_widget_key_press_event (GtkTreeView* _sender, const GdkEventKey* event, gpointer self);
static void gmpc_metadata_browser_browser_artist_changed (GmpcMetadataBrowser* self, GtkTreeSelection* sel);
static void _gmpc_metadata_browser_browser_artist_changed_gtk_tree_selection_changed (GtkTreeSelection* _sender, gpointer self);
static void _gmpc_metadata_browser_browser_album_entry_changed_gtk_editable_changed (GtkEntry* _sender, gpointer self);
static gboolean _gmpc_metadata_browser_visible_func_album_gtk_tree_model_filter_visible_func (GtkTreeModel* model, GtkTreeIter* iter, gpointer self);
static gboolean _gmpc_metadata_browser_album_browser_button_release_event_gtk_widget_button_release_event (GtkTreeView* _sender, const GdkEventButton* event, gpointer self);
static gboolean _gmpc_metadata_browser_browser_album_key_press_event_gtk_widget_key_press_event (GtkTreeView* _sender, const GdkEventKey* event, gpointer self);
static void gmpc_metadata_browser_browser_album_changed (GmpcMetadataBrowser* self, GtkTreeSelection* album_sel);
static void _gmpc_metadata_browser_browser_album_changed_gtk_tree_selection_changed (GtkTreeSelection* _sender, gpointer self);
static gboolean _gmpc_metadata_browser_song_browser_button_release_event_gtk_widget_button_release_event (GtkTreeView* _sender, const GdkEventButton* event, gpointer self);
static void gmpc_metadata_browser_browser_songs_changed (GmpcMetadataBrowser* self, GtkTreeSelection* song_sel);
static void _gmpc_metadata_browser_browser_songs_changed_gtk_tree_selection_changed (GtkTreeSelection* _sender, gpointer self);
static void _gmpc_metadata_browser_browser_bg_style_changed_gtk_widget_style_set (GtkScrolledWindow* _sender, GtkStyle* previous_style, gpointer self);
static gboolean _gmpc_metadata_browser_browser_button_release_event_gtk_widget_button_release_event (GtkWidget* _sender, const GdkEventButton* event, gpointer self);
static void gmpc_metadata_browser_reload_browsers (GmpcMetadataBrowser* self);
static void gmpc_metadata_browser_browser_init (GmpcMetadataBrowser* self);
static void gmpc_metadata_browser_metadata_box_clear (GmpcMetadataBrowser* self);
static void gmpc_metadata_browser_metadata_box_update (GmpcMetadataBrowser* self);
static void gmpc_metadata_browser_play_song (GmpcMetadataBrowser* self, GtkButton* button);
static void gmpc_metadata_browser_add_song (GmpcMetadataBrowser* self, GtkButton* button);
static void gmpc_metadata_browser_replace_song (GmpcMetadataBrowser* self, GtkButton* button);
static void gmpc_metadata_browser_add_selected_song (GmpcMetadataBrowser* self, GtkButton* button);
static void gmpc_metadata_browser_replace_selected_song (GmpcMetadataBrowser* self, GtkButton* button);
static void gmpc_metadata_browser_add_entry (GmpcMetadataBrowser* self, GtkTable* table, const char* entry_label, const char* value, GtkWidget* extra, gint* i);
static void gmpc_metadata_browser_artist_button_clicked (GmpcMetadataBrowser* self, GtkButton* button);
static void gmpc_metadata_browser_album_button_clicked (GmpcMetadataBrowser* self, GtkButton* button);
static GtkHBox* gmpc_metadata_browser_history_buttons (GmpcMetadataBrowser* self);
static void gmpc_metadata_browser_metadata_find_query (GmpcMetadataBrowser* self, GtkButton* button);
static void _gmpc_metadata_browser_metadata_find_query_gtk_button_clicked (GtkButton* _sender, gpointer self);
static void _gmpc_metadata_browser_artist_button_clicked_gtk_button_clicked (GtkButton* _sender, gpointer self);
static void _gmpc_metadata_browser_album_button_clicked_gtk_button_clicked (GtkButton* _sender, gpointer self);
static void gmpc_metadata_browser_metadata_button_open_file_browser_path (GmpcMetadataBrowser* self, GtkButton* button);
static void _gmpc_metadata_browser_metadata_button_open_file_browser_path_gtk_button_clicked (GtkButton* _sender, gpointer self);
static void _gmpc_metadata_browser_play_song_gtk_button_clicked (GtkButton* _sender, gpointer self);
static void _gmpc_metadata_browser_add_song_gtk_button_clicked (GtkButton* _sender, gpointer self);
static void _gmpc_metadata_browser_replace_song_gtk_button_clicked (GtkButton* _sender, gpointer self);
static void gmpc_metadata_browser_album_song_tree_row_activated (GmpcMetadataBrowser* self, GtkTreeView* tree, const GtkTreePath* path, GtkTreeViewColumn* column);
static void gmpc_metadata_browser_album_song_browser_play_clicked (GmpcMetadataBrowser* self, GtkImageMenuItem* item);
static void gmpc_metadata_browser_album_song_browser_add_clicked (GmpcMetadataBrowser* self, GtkImageMenuItem* item);
static void gmpc_metadata_browser_album_song_browser_replace_clicked (GmpcMetadataBrowser* self, GtkImageMenuItem* item);
static void _gmpc_metadata_browser_album_song_browser_play_clicked_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self);
static void _gmpc_metadata_browser_album_song_browser_add_clicked_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self);
static void _gmpc_metadata_browser_album_song_browser_replace_clicked_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self);
static gboolean gmpc_metadata_browser_album_song_tree_button_press_event (GmpcMetadataBrowser* self, GmpcMpdDataTreeview* tree, const GdkEventButton* event);
static void _gmpc_metadata_browser_add_selected_song_gtk_button_clicked (GtkButton* _sender, gpointer self);
static void _gmpc_metadata_browser_replace_selected_song_gtk_button_clicked (GtkButton* _sender, gpointer self);
static gboolean _gmpc_metadata_browser_album_song_tree_button_press_event_gtk_widget_button_release_event (GmpcMpdDataTreeview* _sender, const GdkEventButton* event, gpointer self);
static void _gmpc_metadata_browser_album_song_tree_row_activated_gtk_tree_view_row_activated (GmpcMpdDataTreeview* _sender, const GtkTreePath* path, GtkTreeViewColumn* column, gpointer self);
static void gmpc_metadata_browser_metadata_box_show_album (GmpcMetadataBrowser* self, const char* artist, const char* album);
static void gmpc_metadata_browser_metadata_box_show_artist (GmpcMetadataBrowser* self, const char* artist);
static gboolean gmpc_metadata_browser_metadata_box_update_real (GmpcMetadataBrowser* self);
static gboolean _gmpc_metadata_browser_metadata_box_update_real_gsource_func (gpointer self);
static void gmpc_metadata_browser_history_add (GmpcMetadataBrowser* self, const GmpcMetadataBrowserHitem* hi);
static void gmpc_metadata_browser_real_browser_add (GmpcPluginBrowserIface* base, GtkWidget* category_tree);
static void gmpc_metadata_browser_real_browser_selected (GmpcPluginBrowserIface* base, GtkContainer* container);
static void gmpc_metadata_browser_real_browser_unselected (GmpcPluginBrowserIface* base, GtkContainer* container);
static void gmpc_metadata_browser_history_clear (GmpcMetadataBrowser* self);
static void gmpc_metadata_browser_con_changed (GmpcMetadataBrowser* self, GmpcConnection* conn, MpdObj* server, gint connect);
static void gmpc_metadata_browser_show_hitem (GmpcMetadataBrowser* self, const GmpcMetadataBrowserHitem* hi);
static void gmpc_metadata_browser_status_changed (GmpcMetadataBrowser* self, GmpcConnection* conn, MpdObj* server, ChangedStatusType what);
void gmpc_metadata_browser_set_album (GmpcMetadataBrowser* self, const char* artist, const char* album);
void gmpc_metadata_browser_set_song (GmpcMetadataBrowser* self, const mpd_Song* song);
static void gmpc_metadata_browser_history_show_list_clicked (GmpcMetadataBrowser* self, GtkMenuItem* item);
static void _gmpc_metadata_browser_history_show_list_clicked_gtk_menu_item_activate (GtkMenuItem* _sender, gpointer self);
static void gmpc_metadata_browser_history_show_list (GmpcMetadataBrowser* self);
static void _gmpc_metadata_browser_history_next_gtk_button_clicked (GtkButton* _sender, gpointer self);
static void _gmpc_metadata_browser_history_show_list_gtk_button_clicked (GtkButton* _sender, gpointer self);
static void _gmpc_metadata_browser_history_previous_gtk_button_clicked (GtkButton* _sender, gpointer self);
static void _lambda0_ (GtkToggleButton* source, GmpcMetadataBrowser* self);
static void __lambda0__gtk_toggle_button_toggled (GtkToggleButton* _sender, gpointer self);
static void _lambda1_ (GtkToggleButton* source, GmpcMetadataBrowser* self);
static void __lambda1__gtk_toggle_button_toggled (GtkToggleButton* _sender, gpointer self);
static void _lambda2_ (GtkToggleButton* source, GmpcMetadataBrowser* self);
static void __lambda2__gtk_toggle_button_toggled (GtkToggleButton* _sender, gpointer self);
static void _lambda3_ (GtkToggleButton* source, GmpcMetadataBrowser* self);
static void __lambda3__gtk_toggle_button_toggled (GtkToggleButton* _sender, gpointer self);
static void _lambda4_ (GtkToggleButton* source, GmpcMetadataBrowser* self);
static void __lambda4__gtk_toggle_button_toggled (GtkToggleButton* _sender, gpointer self);
static void _lambda5_ (GtkToggleButton* source, GmpcMetadataBrowser* self);
static void __lambda5__gtk_toggle_button_toggled (GtkToggleButton* _sender, gpointer self);
static void _lambda6_ (GtkToggleButton* source, GmpcMetadataBrowser* self);
static void __lambda6__gtk_toggle_button_toggled (GtkToggleButton* _sender, gpointer self);
static void gmpc_metadata_browser_real_preferences_pane_construct (GmpcPluginPreferencesIface* base, GtkContainer* container);
static void gmpc_metadata_browser_real_preferences_pane_destroy (GmpcPluginPreferencesIface* base, GtkContainer* container);
static void _gmpc_metadata_browser_con_changed_gmpc_connection_connection_changed (GmpcConnection* _sender, MpdObj* server, gint connect, gpointer self);
static void _gmpc_metadata_browser_status_changed_gmpc_connection_status_changed (GmpcConnection* _sender, MpdObj* server, ChangedStatusType what, gpointer self);
static GObject * gmpc_metadata_browser_constructor (GType type, guint n_construct_properties, GObjectConstructParam * construct_properties);
static void gmpc_metadata_browser_finalize (GObject* obj);
static void _vala_array_destroy (gpointer array, gint array_length, GDestroyNotify destroy_func);
static void _vala_array_free (gpointer array, gint array_length, GDestroyNotify destroy_func);
static gint _vala_array_length (gpointer array);
static int _vala_strcmp0 (const char * str1, const char * str2);

static const gint GMPC_NOW_PLAYING_version[] = {0, 0, 0};
static const gint GMPC_METADATA_BROWSER_version[] = {0, 0, 0};


static gpointer _mpd_songDup0 (gpointer self) {
	return self ? mpd_songDup (self) : NULL;
}


GmpcWidgetSimilarSongs* gmpc_widget_similar_songs_construct (GType object_type, const mpd_Song* song) {
	GmpcWidgetSimilarSongs * self;
	mpd_Song* _tmp0_;
	GtkLabel* label;
	char* _tmp1_;
	g_return_val_if_fail (song != NULL, NULL);
	self = g_object_newv (object_type, 0, NULL);
	self->priv->song = (_tmp0_ = _mpd_songDup0 (song), _mpd_freeSong0 (self->priv->song), _tmp0_);
	label = g_object_ref_sink ((GtkLabel*) gtk_label_new (_ ("Similar songs")));
	gtk_label_set_markup (label, _tmp1_ = g_strdup_printf ("<b>%s</b>", _ ("Similar songs")));
	_g_free0 (_tmp1_);
	gtk_expander_set_label_widget ((GtkExpander*) self, (GtkWidget*) label);
	gtk_widget_show ((GtkWidget*) label);
	_g_object_unref0 (label);
	return self;
}


GmpcWidgetSimilarSongs* gmpc_widget_similar_songs_new (const mpd_Song* song) {
	return gmpc_widget_similar_songs_construct (GMPC_WIDGET_TYPE_SIMILAR_SONGS, song);
}


static gpointer _g_object_ref0 (gpointer self) {
	return self ? g_object_ref (self) : NULL;
}


static gpointer _gtk_tree_path_copy0 (gpointer self) {
	return self ? gtk_tree_path_copy (self) : NULL;
}


static void _g_list_free_gtk_tree_path_free (GList* self) {
	g_list_foreach (self, (GFunc) gtk_tree_path_free, NULL);
	g_list_free (self);
}


static void gmpc_widget_similar_songs_add_clicked (GmpcWidgetSimilarSongs* self, GtkImageMenuItem* item) {
	GtkTreeView* tree;
	GtkTreeSelection* sel;
	GtkTreeModel* model;
	GtkTreeIter iter = {0};
	GtkTreeModel* _tmp2_;
	GList* _tmp1_;
	GtkTreeModel* _tmp0_;
	GList* list;
	g_return_if_fail (self != NULL);
	g_return_if_fail (item != NULL);
	tree = _g_object_ref0 (GTK_TREE_VIEW (self->priv->pchild));
	sel = _g_object_ref0 (gtk_tree_view_get_selection (tree));
	model = NULL;
	_tmp0_ = NULL;
	list = (_tmp1_ = gtk_tree_selection_get_selected_rows (sel, &_tmp0_), model = (_tmp2_ = _g_object_ref0 (_tmp0_), _g_object_unref0 (model), _tmp2_), _tmp1_);
	{
		GList* path_collection;
		GList* path_it;
		path_collection = list;
		for (path_it = path_collection; path_it != NULL; path_it = path_it->next) {
			GtkTreePath* path;
			path = _gtk_tree_path_copy0 ((const GtkTreePath*) path_it->data);
			{
				if (gtk_tree_model_get_iter (model, &iter, path)) {
					const mpd_Song* song;
					song = NULL;
					gtk_tree_model_get (model, &iter, 0, &song, -1, -1);
					if (song != NULL) {
						mpd_playlist_queue_add (connection, song->file);
					}
				}
				_gtk_tree_path_free0 (path);
			}
		}
	}
	mpd_playlist_queue_commit (connection);
	_g_object_unref0 (tree);
	_g_object_unref0 (sel);
	_g_object_unref0 (model);
	__g_list_free_gtk_tree_path_free0 (list);
}


static void gmpc_widget_similar_songs_play_clicked (GmpcWidgetSimilarSongs* self, GtkImageMenuItem* item) {
	GtkTreeView* tree;
	GtkTreeSelection* sel;
	GtkTreeModel* model;
	GtkTreeIter iter = {0};
	GtkTreeModel* _tmp2_;
	GList* _tmp1_;
	GtkTreeModel* _tmp0_;
	GList* list;
	g_return_if_fail (self != NULL);
	g_return_if_fail (item != NULL);
	tree = _g_object_ref0 (GTK_TREE_VIEW (self->priv->pchild));
	sel = _g_object_ref0 (gtk_tree_view_get_selection (tree));
	model = NULL;
	_tmp0_ = NULL;
	list = (_tmp1_ = gtk_tree_selection_get_selected_rows (sel, &_tmp0_), model = (_tmp2_ = _g_object_ref0 (_tmp0_), _g_object_unref0 (model), _tmp2_), _tmp1_);
	if (list != NULL) {
		GtkTreePath* path;
		path = _gtk_tree_path_copy0 ((const GtkTreePath*) list->data);
		if (gtk_tree_model_get_iter (model, &iter, path)) {
			const mpd_Song* song;
			song = NULL;
			gtk_tree_model_get (model, &iter, 0, &song, -1, -1);
			if (song != NULL) {
				play_path (song->file);
			}
		}
		_gtk_tree_path_free0 (path);
	}
	_g_object_unref0 (tree);
	_g_object_unref0 (sel);
	_g_object_unref0 (model);
	__g_list_free_gtk_tree_path_free0 (list);
}


static void gmpc_widget_similar_songs_replace_clicked (GmpcWidgetSimilarSongs* self, GtkImageMenuItem* item) {
	gboolean found;
	GtkTreeView* tree;
	GtkTreeSelection* sel;
	GtkTreeModel* model;
	GtkTreeIter iter = {0};
	GtkTreeModel* _tmp2_;
	GList* _tmp1_;
	GtkTreeModel* _tmp0_;
	GList* list;
	g_return_if_fail (self != NULL);
	g_return_if_fail (item != NULL);
	found = FALSE;
	tree = _g_object_ref0 (GTK_TREE_VIEW (self->priv->pchild));
	sel = _g_object_ref0 (gtk_tree_view_get_selection (tree));
	model = NULL;
	_tmp0_ = NULL;
	list = (_tmp1_ = gtk_tree_selection_get_selected_rows (sel, &_tmp0_), model = (_tmp2_ = _g_object_ref0 (_tmp0_), _g_object_unref0 (model), _tmp2_), _tmp1_);
	{
		GList* path_collection;
		GList* path_it;
		path_collection = list;
		for (path_it = path_collection; path_it != NULL; path_it = path_it->next) {
			GtkTreePath* path;
			path = _gtk_tree_path_copy0 ((const GtkTreePath*) path_it->data);
			{
				if (gtk_tree_model_get_iter (model, &iter, path)) {
					const mpd_Song* song;
					song = NULL;
					gtk_tree_model_get (model, &iter, 0, &song, -1, -1);
					if (song != NULL) {
						mpd_playlist_queue_add (connection, song->file);
						found = TRUE;
					}
				}
				_gtk_tree_path_free0 (path);
			}
		}
	}
	if (found) {
		mpd_playlist_clear (connection);
		mpd_playlist_queue_commit (connection);
		mpd_player_play (connection);
	}
	gmpc_widget_similar_songs_play_clicked (self, item);
	_g_object_unref0 (tree);
	_g_object_unref0 (sel);
	_g_object_unref0 (model);
	__g_list_free_gtk_tree_path_free0 (list);
}


static void gmpc_widget_similar_songs_tree_row_activated (GmpcWidgetSimilarSongs* self, GmpcMpdDataTreeview* tree, const GtkTreePath* path, GtkTreeViewColumn* column) {
	GtkTreeModel* model;
	GtkTreeIter iter = {0};
	g_return_if_fail (self != NULL);
	g_return_if_fail (tree != NULL);
	g_return_if_fail (path != NULL);
	g_return_if_fail (column != NULL);
	model = _g_object_ref0 (gtk_tree_view_get_model ((GtkTreeView*) tree));
	if (gtk_tree_model_get_iter (model, &iter, path)) {
		const mpd_Song* song;
		song = NULL;
		gtk_tree_model_get (model, &iter, 0, &song, -1, -1);
		if (song != NULL) {
			play_path (song->file);
		}
	}
	_g_object_unref0 (model);
}


static void _gmpc_widget_similar_songs_play_clicked_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self) {
	gmpc_widget_similar_songs_play_clicked (self, _sender);
}


static void _gmpc_widget_similar_songs_add_clicked_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self) {
	gmpc_widget_similar_songs_add_clicked (self, _sender);
}


static void _gmpc_widget_similar_songs_replace_clicked_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self) {
	gmpc_widget_similar_songs_replace_clicked (self, _sender);
}


static gboolean gmpc_widget_similar_songs_tree_right_menu (GmpcWidgetSimilarSongs* self, GmpcMpdDataTreeview* tree, const GdkEventButton* event) {
	gboolean result;
	g_return_val_if_fail (self != NULL, FALSE);
	g_return_val_if_fail (tree != NULL, FALSE);
	if ((*event).button == 3) {
		GtkMenu* menu;
		GtkImageMenuItem* item;
		GtkImageMenuItem* _tmp0_;
		GtkImageMenuItem* _tmp1_;
		GtkImage* _tmp2_;
		menu = g_object_ref_sink ((GtkMenu*) gtk_menu_new ());
		item = g_object_ref_sink ((GtkImageMenuItem*) gtk_image_menu_item_new_from_stock ("gtk-media-play", NULL));
		g_signal_connect_object ((GtkMenuItem*) item, "activate", (GCallback) _gmpc_widget_similar_songs_play_clicked_gtk_menu_item_activate, self, 0);
		gtk_menu_shell_append ((GtkMenuShell*) menu, (GtkWidget*) ((GtkMenuItem*) item));
		item = (_tmp0_ = g_object_ref_sink ((GtkImageMenuItem*) gtk_image_menu_item_new_from_stock ("gtk-add", NULL)), _g_object_unref0 (item), _tmp0_);
		g_signal_connect_object ((GtkMenuItem*) item, "activate", (GCallback) _gmpc_widget_similar_songs_add_clicked_gtk_menu_item_activate, self, 0);
		gtk_menu_shell_append ((GtkMenuShell*) menu, (GtkWidget*) ((GtkMenuItem*) item));
		item = (_tmp1_ = g_object_ref_sink ((GtkImageMenuItem*) gtk_image_menu_item_new_with_mnemonic (_ ("_Replace"))), _g_object_unref0 (item), _tmp1_);
		gtk_image_menu_item_set_image (item, (GtkWidget*) (_tmp2_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_stock ("gtk-redo", GTK_ICON_SIZE_MENU))));
		_g_object_unref0 (_tmp2_);
		g_signal_connect_object ((GtkMenuItem*) item, "activate", (GCallback) _gmpc_widget_similar_songs_replace_clicked_gtk_menu_item_activate, self, 0);
		gtk_menu_shell_append ((GtkMenuShell*) menu, (GtkWidget*) ((GtkMenuItem*) item));
		gtk_menu_popup (menu, NULL, NULL, NULL, NULL, (*event).button, (*event).time);
		gtk_widget_show_all ((GtkWidget*) menu);
		result = TRUE;
		_g_object_unref0 (menu);
		_g_object_unref0 (item);
		return result;
	}
	result = FALSE;
	return result;
}


static gboolean _gmpc_widget_similar_songs_tree_right_menu_gtk_widget_button_release_event (GmpcMpdDataTreeview* _sender, const GdkEventButton* event, gpointer self) {
	return gmpc_widget_similar_songs_tree_right_menu (self, _sender, event);
}


static void _gmpc_widget_similar_songs_tree_row_activated_gtk_tree_view_row_activated (GmpcMpdDataTreeview* _sender, const GtkTreePath* path, GtkTreeViewColumn* column, gpointer self) {
	gmpc_widget_similar_songs_tree_row_activated (self, _sender, path, column);
}


static gboolean gmpc_widget_similar_songs_update_sim_song (GmpcWidgetSimilarSongs* self) {
	gboolean result;
	MetaData* _tmp9_;
	g_return_val_if_fail (self != NULL, FALSE);
	if (self->priv->current == NULL) {
		GtkWidget* _tmp0_;
		self->priv->current = meta_data_get_text_list (self->priv->copy);
		self->priv->pchild = (_tmp0_ = (GtkWidget*) g_object_ref_sink ((GtkProgressBar*) gtk_progress_bar_new ()), _g_object_unref0 (self->priv->pchild), _tmp0_);
		gtk_container_add ((GtkContainer*) self, self->priv->pchild);
		gtk_widget_show_all ((GtkWidget*) self);
	}
	gtk_progress_bar_pulse (GTK_PROGRESS_BAR (self->priv->pchild));
	if (self->priv->current != NULL) {
		char* entry;
		entry = g_strdup ((const char*) self->priv->current->data);
		if (entry != NULL) {
			char** _tmp2_;
			gint split_size;
			gint split_length1;
			char** _tmp1_;
			char** split;
			split = (_tmp2_ = _tmp1_ = g_strsplit (entry, "::", 2), split_length1 = _vala_array_length (_tmp1_), split_size = split_length1, _tmp2_);
			if (split_length1 == 2) {
				char** _tmp4_;
				gint art_split_size;
				gint art_split_length1;
				char** _tmp3_;
				char** art_split;
				MpdData* data;
				mpd_database_search_start (connection, FALSE);
				art_split = (_tmp4_ = _tmp3_ = g_strsplit (split[0], " ", 0), art_split_length1 = _vala_array_length (_tmp3_), art_split_size = art_split_length1, _tmp4_);
				{
					char** artist_collection;
					int artist_collection_length1;
					int artist_it;
					artist_collection = art_split;
					artist_collection_length1 = art_split_length1;
					for (artist_it = 0; artist_it < art_split_length1; artist_it = artist_it + 1) {
						char* artist;
						artist = g_strdup (artist_collection[artist_it]);
						{
							mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ARTIST, artist);
							_g_free0 (artist);
						}
					}
				}
				mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_TITLE, split[1]);
				data = mpd_database_search_commit (connection);
				if (data != NULL) {
					MpdData* _tmp5_;
					self->priv->item = mpd_data_concatenate (self->priv->item, (_tmp5_ = data, data = NULL, _tmp5_));
				}
				art_split = (_vala_array_free (art_split, art_split_length1, (GDestroyNotify) g_free), NULL);
				_mpd_data_free0 (data);
			}
			split = (_vala_array_free (split, split_length1, (GDestroyNotify) g_free), NULL);
		}
		self->priv->current = self->priv->current->next;
		if (self->priv->current != NULL) {
			result = TRUE;
			_g_free0 (entry);
			return result;
		}
		_g_free0 (entry);
	}
	gtk_object_destroy ((GtkObject*) self->priv->pchild);
	if (self->priv->item != NULL) {
		GmpcMpdDataModel* model;
		MpdData* _tmp6_;
		GmpcMpdDataTreeview* tree;
		GtkWidget* _tmp7_;
		model = gmpc_mpddata_model_new ();
		gmpc_mpddata_model_set_mpd_data (model, (_tmp6_ = self->priv->item, self->priv->item = NULL, _tmp6_));
		tree = g_object_ref_sink (gmpc_mpddata_treeview_new ("similar-song", TRUE, (GtkTreeModel*) model));
		gmpc_mpddata_treeview_enable_click_fix (tree);
		g_signal_connect_object ((GtkWidget*) tree, "button-release-event", (GCallback) _gmpc_widget_similar_songs_tree_right_menu_gtk_widget_button_release_event, self, 0);
		g_signal_connect_object ((GtkTreeView*) tree, "row-activated", (GCallback) _gmpc_widget_similar_songs_tree_row_activated_gtk_tree_view_row_activated, self, 0);
		gtk_container_add ((GtkContainer*) self, (GtkWidget*) tree);
		self->priv->pchild = (_tmp7_ = _g_object_ref0 ((GtkWidget*) tree), _g_object_unref0 (self->priv->pchild), _tmp7_);
		_g_object_unref0 (model);
		_g_object_unref0 (tree);
	} else {
		GtkLabel* label;
		GtkWidget* _tmp8_;
		label = g_object_ref_sink ((GtkLabel*) gtk_label_new (_ ("Unavailable")));
		gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.0f);
		gtk_container_add ((GtkContainer*) self, (GtkWidget*) label);
		self->priv->pchild = (_tmp8_ = _g_object_ref0 ((GtkWidget*) label), _g_object_unref0 (self->priv->pchild), _tmp8_);
		_g_object_unref0 (label);
	}
	self->priv->copy = (_tmp9_ = NULL, _meta_data_free0 (self->priv->copy), _tmp9_);
	self->priv->idle_add = (guint) 0;
	gtk_widget_show_all ((GtkWidget*) self);
	result = FALSE;
	return result;
}


static gboolean _gmpc_widget_similar_songs_update_sim_song_gsource_func (gpointer self) {
	return gmpc_widget_similar_songs_update_sim_song (self);
}


static void gmpc_widget_similar_songs_metadata_changed (GmpcWidgetSimilarSongs* self, GmpcMetaWatcher* gmw2, const mpd_Song* song, MetaDataType type, MetaDataResult _result_, const MetaData* met) {
	g_return_if_fail (self != NULL);
	g_return_if_fail (gmw2 != NULL);
	g_return_if_fail (song != NULL);
	if (g_utf8_collate (self->priv->song->artist, song->artist) != 0) {
		return;
	}
	if (type != META_SONG_SIMILAR) {
		return;
	}
	if (self->priv->pchild != NULL) {
		gtk_object_destroy ((GtkObject*) self->priv->pchild);
	}
	if (_result_ == META_DATA_FETCHING) {
		GtkLabel* label;
		GtkWidget* _tmp0_;
		label = g_object_ref_sink ((GtkLabel*) gtk_label_new (_ ("Fetching .. ")));
		gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.0f);
		gtk_container_add ((GtkContainer*) self, (GtkWidget*) label);
		self->priv->pchild = (_tmp0_ = _g_object_ref0 ((GtkWidget*) label), _g_object_unref0 (self->priv->pchild), _tmp0_);
		_g_object_unref0 (label);
	} else {
		if (_result_ == META_DATA_UNAVAILABLE) {
			GtkLabel* label;
			GtkWidget* _tmp1_;
			label = g_object_ref_sink ((GtkLabel*) gtk_label_new (_ ("Unavailable")));
			gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.0f);
			gtk_container_add ((GtkContainer*) self, (GtkWidget*) label);
			self->priv->pchild = (_tmp1_ = _g_object_ref0 ((GtkWidget*) label), _g_object_unref0 (self->priv->pchild), _tmp1_);
			_g_object_unref0 (label);
		} else {
			if (meta_data_is_text_list (met)) {
				MetaData* _tmp2_;
				self->priv->copy = (_tmp2_ = meta_data_dup_steal (met), _meta_data_free0 (self->priv->copy), _tmp2_);
				self->priv->idle_add = g_idle_add_full (G_PRIORITY_DEFAULT_IDLE, _gmpc_widget_similar_songs_update_sim_song_gsource_func, g_object_ref (self), g_object_unref);
				return;
			} else {
				GtkLabel* label;
				GtkWidget* _tmp3_;
				label = g_object_ref_sink ((GtkLabel*) gtk_label_new (_ ("Unavailable")));
				gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.0f);
				gtk_container_add ((GtkContainer*) self, (GtkWidget*) label);
				self->priv->pchild = (_tmp3_ = _g_object_ref0 ((GtkWidget*) label), _g_object_unref0 (self->priv->pchild), _tmp3_);
				_g_object_unref0 (label);
			}
		}
	}
	gtk_widget_show_all ((GtkWidget*) self);
}


static void _gmpc_widget_similar_songs_metadata_changed_gmpc_meta_watcher_data_changed (GmpcMetaWatcher* _sender, const mpd_Song* song, MetaDataType type, MetaDataResult _result_, const MetaData* met, gpointer self) {
	gmpc_widget_similar_songs_metadata_changed (self, _sender, song, type, _result_, met);
}


static void gmpc_widget_similar_songs_update (GmpcWidgetSimilarSongs* self) {
	MetaData* item;
	MetaData* _tmp2_;
	MetaDataResult _tmp1_;
	MetaData* _tmp0_;
	MetaDataResult gm_result;
	g_return_if_fail (self != NULL);
	item = NULL;
	g_signal_connect_object (gmw, "data-changed", (GCallback) _gmpc_widget_similar_songs_metadata_changed_gmpc_meta_watcher_data_changed, self, 0);
	_tmp0_ = NULL;
	gm_result = (_tmp1_ = gmpc_meta_watcher_get_meta_path (gmw, self->priv->song, META_SONG_SIMILAR, &_tmp0_), item = (_tmp2_ = _tmp0_, _meta_data_free0 (item), _tmp2_), _tmp1_);
	gmpc_widget_similar_songs_metadata_changed (self, gmw, self->priv->song, META_SONG_SIMILAR, gm_result, item);
	_meta_data_free0 (item);
}


static void gmpc_widget_similar_songs_real_activate (GtkExpander* base) {
	GmpcWidgetSimilarSongs * self;
	self = (GmpcWidgetSimilarSongs*) base;
	if (!gtk_expander_get_expanded ((GtkExpander*) self)) {
		gtk_expander_set_expanded ((GtkExpander*) self, TRUE);
		if (!self->priv->filled) {
			gmpc_widget_similar_songs_update (self);
			self->priv->filled = TRUE;
		}
	} else {
		gtk_expander_set_expanded ((GtkExpander*) self, FALSE);
	}
}


static void gmpc_widget_similar_songs_class_init (GmpcWidgetSimilarSongsClass * klass) {
	gmpc_widget_similar_songs_parent_class = g_type_class_peek_parent (klass);
	g_type_class_add_private (klass, sizeof (GmpcWidgetSimilarSongsPrivate));
	GTK_EXPANDER_CLASS (klass)->activate = gmpc_widget_similar_songs_real_activate;
	G_OBJECT_CLASS (klass)->finalize = gmpc_widget_similar_songs_finalize;
}


static void gmpc_widget_similar_songs_instance_init (GmpcWidgetSimilarSongs * self) {
	self->priv = GMPC_WIDGET_SIMILAR_SONGS_GET_PRIVATE (self);
	self->priv->song = NULL;
	self->priv->filled = FALSE;
	self->priv->pchild = NULL;
	self->priv->idle_add = (guint) 0;
	self->priv->copy = NULL;
	self->priv->item = NULL;
	self->priv->current = NULL;
}


static void gmpc_widget_similar_songs_finalize (GObject* obj) {
	GmpcWidgetSimilarSongs * self;
	self = GMPC_WIDGET_SIMILAR_SONGS (obj);
	{
		if (self->priv->idle_add > 0) {
			g_source_remove (self->priv->idle_add);
			self->priv->idle_add = (guint) 0;
		}
	}
	_mpd_freeSong0 (self->priv->song);
	_g_object_unref0 (self->priv->pchild);
	_meta_data_free0 (self->priv->copy);
	_mpd_data_free0 (self->priv->item);
	G_OBJECT_CLASS (gmpc_widget_similar_songs_parent_class)->finalize (obj);
}


GType gmpc_widget_similar_songs_get_type (void) {
	static GType gmpc_widget_similar_songs_type_id = 0;
	if (gmpc_widget_similar_songs_type_id == 0) {
		static const GTypeInfo g_define_type_info = { sizeof (GmpcWidgetSimilarSongsClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) gmpc_widget_similar_songs_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (GmpcWidgetSimilarSongs), 0, (GInstanceInitFunc) gmpc_widget_similar_songs_instance_init, NULL };
		gmpc_widget_similar_songs_type_id = g_type_register_static (GTK_TYPE_EXPANDER, "GmpcWidgetSimilarSongs", &g_define_type_info, 0);
	}
	return gmpc_widget_similar_songs_type_id;
}


static void _g_list_free_g_object_unref (GList* self) {
	g_list_foreach (self, (GFunc) g_object_unref, NULL);
	g_list_free (self);
}


static void gmpc_widget_similar_artist_metadata_changed (GmpcWidgetSimilarArtist* self, GmpcMetaWatcher* gmw2, const mpd_Song* song, MetaDataType type, MetaDataResult _result_, const MetaData* met) {
	GList* child_list;
	gboolean _tmp0_;
	gboolean _tmp1_;
	g_return_if_fail (self != NULL);
	g_return_if_fail (gmw2 != NULL);
	g_return_if_fail (song != NULL);
	if (type != META_ARTIST_SIMILAR) {
		return;
	}
	if (g_utf8_collate (self->priv->song->artist, song->artist) != 0) {
		return;
	}
	child_list = gtk_container_get_children ((GtkContainer*) self);
	{
		GList* child_collection;
		GList* child_it;
		child_collection = child_list;
		for (child_it = child_collection; child_it != NULL; child_it = child_it->next) {
			GtkWidget* child;
			child = _g_object_ref0 ((GtkWidget*) child_it->data);
			{
				gtk_object_destroy ((GtkObject*) child);
				_g_object_unref0 (child);
			}
		}
	}
	_tmp0_ = FALSE;
	_tmp1_ = FALSE;
	if (_result_ == META_DATA_UNAVAILABLE) {
		_tmp1_ = TRUE;
	} else {
		_tmp1_ = meta_data_is_empty (met);
	}
	if (_tmp1_) {
		_tmp0_ = TRUE;
	} else {
		_tmp0_ = !meta_data_is_text_list (met);
	}
	if (_tmp0_) {
		GtkLabel* label;
		label = g_object_ref_sink ((GtkLabel*) gtk_label_new (_ ("Unavailable")));
		gtk_table_attach ((GtkTable*) self, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) 0, (guint) 1, GTK_SHRINK, GTK_SHRINK, (guint) 0, (guint) 0);
		_g_object_unref0 (label);
	} else {
		if (_result_ == META_DATA_FETCHING) {
			GtkLabel* label;
			label = g_object_ref_sink ((GtkLabel*) gtk_label_new (_ ("Fetching")));
			gtk_table_attach ((GtkTable*) self, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) 0, (guint) 1, GTK_SHRINK, GTK_SHRINK, (guint) 0, (guint) 0);
			_g_object_unref0 (label);
		} else {
			GList* in_db_list;
			GList* list;
			gint items;
			gint i;
			guint llength;
			gint columns;
			in_db_list = NULL;
			list = g_list_copy (meta_data_get_text_list (met));
			list = g_list_sort (list, (GCompareFunc) g_utf8_collate);
			items = 30;
			i = 0;
			if (list != NULL) {
				GList* liter;
				MpdData* data;
				gint q;
				liter = NULL;
				mpd_database_search_field_start (connection, MPD_TAG_ITEM_ARTIST);
				data = mpd_database_search_commit (connection);
				q = 0;
				if (data != NULL) {
					const MpdData* iter;
					char* artist;
					data = misc_sort_mpddata_by_album_disc_track (data);
					iter = mpd_data_get_first (data);
					liter = g_list_first (list);
					artist = g_strdup ("");
					if (g_utf8_validate (iter->tag, -1, NULL) == FALSE) {
						g_error ("gmpc-metadata-browser2.vala:355: Failed to validate");
					}
					if (iter->tag != NULL) {
						char* _tmp2_;
						artist = (_tmp2_ = g_utf8_casefold (iter->tag, -1), _g_free0 (artist), _tmp2_);
					}
					{
						gboolean _tmp3_;
						_tmp3_ = TRUE;
						while (TRUE) {
							char* _tmp6_;
							gint _tmp7_;
							gint res;
							if (!_tmp3_) {
								gboolean _tmp4_;
								gboolean _tmp5_;
								_tmp4_ = FALSE;
								_tmp5_ = FALSE;
								if (iter != NULL) {
									_tmp5_ = liter != NULL;
								} else {
									_tmp5_ = FALSE;
								}
								if (_tmp5_) {
									_tmp4_ = i < items;
								} else {
									_tmp4_ = FALSE;
								}
								if (!_tmp4_) {
									break;
								}
							}
							_tmp3_ = FALSE;
							res = (_tmp7_ = g_utf8_collate (_tmp6_ = g_utf8_casefold ((const char*) liter->data, -1), artist), _g_free0 (_tmp6_), _tmp7_);
							q++;
							if (res == 0) {
								char* d;
								in_db_list = g_list_prepend (in_db_list, gmpc_widget_similar_artist_new_artist_button (self, iter->tag, TRUE));
								i++;
								d = g_strdup ((const char*) liter->data);
								liter = liter->next;
								list = g_list_remove (list, d);
								iter = mpd_data_get_next_real (iter, FALSE);
								if (iter != NULL) {
									char* _tmp8_;
									artist = (_tmp8_ = g_utf8_casefold (iter->tag, -1), _g_free0 (artist), _tmp8_);
								}
								_g_free0 (d);
							} else {
								if (res > 0) {
									iter = mpd_data_get_next_real (iter, FALSE);
									if (iter != NULL) {
										char* _tmp9_;
										artist = (_tmp9_ = g_utf8_casefold (iter->tag, -1), _g_free0 (artist), _tmp9_);
									}
								} else {
									liter = liter->next;
								}
							}
						}
					}
					_g_free0 (artist);
				}
				liter = g_list_first (list);
				while (TRUE) {
					gboolean _tmp10_;
					char* artist;
					_tmp10_ = FALSE;
					if (liter != NULL) {
						_tmp10_ = i < items;
					} else {
						_tmp10_ = FALSE;
					}
					if (!_tmp10_) {
						break;
					}
					artist = g_strdup ((const char*) liter->data);
					in_db_list = g_list_prepend (in_db_list, gmpc_widget_similar_artist_new_artist_button (self, artist, FALSE));
					i++;
					liter = liter->next;
					_g_free0 (artist);
				}
				_mpd_data_free0 (data);
			}
			in_db_list = g_list_reverse (in_db_list);
			i = 0;
			gtk_widget_hide ((GtkWidget*) self);
			llength = g_list_length (in_db_list);
			columns = 3;
			gtk_table_resize ((GtkTable*) self, (llength / columns) + 1, (guint) columns);
			{
				GList* item_collection;
				GList* item_it;
				item_collection = in_db_list;
				for (item_it = item_collection; item_it != NULL; item_it = item_it->next) {
					GtkWidget* item;
					item = _g_object_ref0 ((GtkWidget*) item_it->data);
					{
						gtk_table_attach ((GtkTable*) self, item, (guint) (i % columns), (guint) ((i % columns) + 1), (guint) (i / columns), (guint) ((i / columns) + 1), GTK_EXPAND | GTK_FILL, GTK_SHRINK, (guint) 0, (guint) 0);
						i++;
						_g_object_unref0 (item);
					}
				}
			}
			__g_list_free_g_object_unref0 (in_db_list);
			_g_list_free0 (list);
		}
	}
	gtk_widget_show_all ((GtkWidget*) self);
	_g_list_free0 (child_list);
}


static void gmpc_widget_similar_artist_artist_button_clicked (GmpcWidgetSimilarArtist* self, GtkButton* button) {
	const char* artist;
	g_return_if_fail (self != NULL);
	g_return_if_fail (button != NULL);
	artist = (const char*) g_object_get_data ((GObject*) button, "artist");
	gmpc_metadata_browser_set_artist (self->priv->browser, artist);
}


static gboolean _misc_header_expose_event_gtk_widget_expose_event (GtkWidget* _sender, const GdkEventExpose* event, gpointer self) {
	return misc_header_expose_event (_sender, event);
}


static void _gmpc_widget_similar_artist_artist_button_clicked_gtk_button_clicked (GtkButton* _sender, gpointer self) {
	gmpc_widget_similar_artist_artist_button_clicked (self, _sender);
}


GtkWidget* gmpc_widget_similar_artist_new_artist_button (GmpcWidgetSimilarArtist* self, const char* artist, gboolean in_db) {
	GtkWidget* result;
	GtkHBox* hbox;
	GtkEventBox* event;
	GmpcMetaImage* image;
	mpd_Song* song;
	char* _tmp0_;
	GtkLabel* label;
	g_return_val_if_fail (self != NULL, NULL);
	g_return_val_if_fail (artist != NULL, NULL);
	hbox = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 6));
	gtk_container_set_border_width ((GtkContainer*) hbox, (guint) 4);
	event = g_object_ref_sink ((GtkEventBox*) gtk_event_box_new ());
	g_object_set ((GtkWidget*) event, "app-paintable", TRUE, NULL);
	gtk_event_box_set_visible_window (event, TRUE);
	g_signal_connect ((GtkWidget*) event, "expose-event", (GCallback) _misc_header_expose_event_gtk_widget_expose_event, NULL);
	gtk_widget_set_size_request ((GtkWidget*) event, 200, 60);
	image = g_object_ref_sink (gmpc_metaimage_new_size (META_ARTIST_ART, 48));
	song = mpd_newSong ();
	song->artist = (_tmp0_ = g_strdup (artist), _g_free0 (song->artist), _tmp0_);
	gmpc_metaimage_set_squared (image, TRUE);
	gmpc_metaimage_update_cover_from_song_delayed (image, song);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) image, FALSE, FALSE, (guint) 0);
	label = g_object_ref_sink ((GtkLabel*) gtk_label_new (artist));
	gtk_widget_set_tooltip_text ((GtkWidget*) label, artist);
	gtk_label_set_selectable (label, TRUE);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
	gtk_label_set_ellipsize (label, PANGO_ELLIPSIZE_END);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) label, TRUE, TRUE, (guint) 0);
	if (in_db) {
		GtkButton* find;
		GtkImage* _tmp1_;
		find = g_object_ref_sink ((GtkButton*) gtk_button_new ());
		gtk_container_add ((GtkContainer*) find, (GtkWidget*) (_tmp1_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_stock ("gtk-find", GTK_ICON_SIZE_MENU))));
		_g_object_unref0 (_tmp1_);
		gtk_button_set_relief (find, GTK_RELIEF_NONE);
		gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) find, FALSE, FALSE, (guint) 0);
		g_object_set_data_full ((GObject*) find, "artist", (void*) g_strdup_printf ("%s", artist), (GDestroyNotify) g_free);
		g_signal_connect_object (find, "clicked", (GCallback) _gmpc_widget_similar_artist_artist_button_clicked_gtk_button_clicked, self, 0);
		_g_object_unref0 (find);
	}
	gtk_container_add ((GtkContainer*) event, (GtkWidget*) hbox);
	result = (GtkWidget*) event;
	_g_object_unref0 (hbox);
	_g_object_unref0 (image);
	_mpd_freeSong0 (song);
	_g_object_unref0 (label);
	return result;
}


static void _gmpc_widget_similar_artist_metadata_changed_gmpc_meta_watcher_data_changed (GmpcMetaWatcher* _sender, const mpd_Song* song, MetaDataType type, MetaDataResult _result_, const MetaData* met, gpointer self) {
	gmpc_widget_similar_artist_metadata_changed (self, _sender, song, type, _result_, met);
}


GmpcWidgetSimilarArtist* gmpc_widget_similar_artist_construct (GType object_type, GmpcMetadataBrowser* browser, MpdObj* server, const mpd_Song* song) {
	GmpcWidgetSimilarArtist * self;
	MetaData* item;
	GmpcMetadataBrowser* _tmp0_;
	mpd_Song* _tmp1_;
	MetaData* _tmp4_;
	MetaDataResult _tmp3_;
	MetaData* _tmp2_;
	MetaDataResult gm_result;
	g_return_val_if_fail (browser != NULL, NULL);
	g_return_val_if_fail (server != NULL, NULL);
	g_return_val_if_fail (song != NULL, NULL);
	self = g_object_newv (object_type, 0, NULL);
	item = NULL;
	self->priv->browser = (_tmp0_ = _g_object_ref0 (browser), _g_object_unref0 (self->priv->browser), _tmp0_);
	self->priv->song = (_tmp1_ = _mpd_songDup0 (song), _mpd_freeSong0 (self->priv->song), _tmp1_);
	gtk_table_set_homogeneous ((GtkTable*) self, TRUE);
	gtk_table_set_row_spacings ((GtkTable*) self, (guint) 6);
	gtk_table_set_col_spacings ((GtkTable*) self, (guint) 6);
	g_signal_connect_object (gmw, "data-changed", (GCallback) _gmpc_widget_similar_artist_metadata_changed_gmpc_meta_watcher_data_changed, self, 0);
	_tmp2_ = NULL;
	gm_result = (_tmp3_ = gmpc_meta_watcher_get_meta_path (gmw, song, META_ARTIST_SIMILAR, &_tmp2_), item = (_tmp4_ = _tmp2_, _meta_data_free0 (item), _tmp4_), _tmp3_);
	if (gm_result == META_DATA_AVAILABLE) {
		gmpc_widget_similar_artist_metadata_changed (self, gmw, self->priv->song, META_ARTIST_SIMILAR, gm_result, item);
	}
	_meta_data_free0 (item);
	return self;
}


GmpcWidgetSimilarArtist* gmpc_widget_similar_artist_new (GmpcMetadataBrowser* browser, MpdObj* server, const mpd_Song* song) {
	return gmpc_widget_similar_artist_construct (GMPC_WIDGET_TYPE_SIMILAR_ARTIST, browser, server, song);
}


static void gmpc_widget_similar_artist_class_init (GmpcWidgetSimilarArtistClass * klass) {
	gmpc_widget_similar_artist_parent_class = g_type_class_peek_parent (klass);
	g_type_class_add_private (klass, sizeof (GmpcWidgetSimilarArtistPrivate));
	G_OBJECT_CLASS (klass)->finalize = gmpc_widget_similar_artist_finalize;
}


static void gmpc_widget_similar_artist_instance_init (GmpcWidgetSimilarArtist * self) {
	self->priv = GMPC_WIDGET_SIMILAR_ARTIST_GET_PRIVATE (self);
	self->priv->song = NULL;
	self->priv->browser = NULL;
}


static void gmpc_widget_similar_artist_finalize (GObject* obj) {
	GmpcWidgetSimilarArtist * self;
	self = GMPC_WIDGET_SIMILAR_ARTIST (obj);
	_mpd_freeSong0 (self->priv->song);
	_g_object_unref0 (self->priv->browser);
	G_OBJECT_CLASS (gmpc_widget_similar_artist_parent_class)->finalize (obj);
}


GType gmpc_widget_similar_artist_get_type (void) {
	static GType gmpc_widget_similar_artist_type_id = 0;
	if (gmpc_widget_similar_artist_type_id == 0) {
		static const GTypeInfo g_define_type_info = { sizeof (GmpcWidgetSimilarArtistClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) gmpc_widget_similar_artist_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (GmpcWidgetSimilarArtist), 0, (GInstanceInitFunc) gmpc_widget_similar_artist_instance_init, NULL };
		gmpc_widget_similar_artist_type_id = g_type_register_static (GTK_TYPE_TABLE, "GmpcWidgetSimilarArtist", &g_define_type_info, 0);
	}
	return gmpc_widget_similar_artist_type_id;
}


static void gmpc_widget_more_expand (GmpcWidgetMore* self, GtkButton* but) {
	g_return_if_fail (self != NULL);
	g_return_if_fail (but != NULL);
	if (self->priv->expand_state == 0) {
		GtkImage* _tmp0_;
		gtk_button_set_label (but, _ ("(less)"));
		gtk_button_set_image (but, (GtkWidget*) (_tmp0_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_stock ("gtk-remove", GTK_ICON_SIZE_MENU))));
		_g_object_unref0 (_tmp0_);
		gtk_widget_set_size_request ((GtkWidget*) self->priv->ali, -1, -1);
		self->priv->expand_state = 1;
	} else {
		GtkImage* _tmp1_;
		gtk_button_set_label (but, _ ("(more)"));
		gtk_button_set_image (but, (GtkWidget*) (_tmp1_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_stock ("gtk-add", GTK_ICON_SIZE_MENU))));
		_g_object_unref0 (_tmp1_);
		gtk_widget_set_size_request ((GtkWidget*) self->priv->ali, -1, self->priv->max_height);
		self->priv->expand_state = 0;
	}
}


static void gmpc_widget_more_size_changed (GmpcWidgetMore* self, GtkWidget* child, const GdkRectangle* alloc) {
	g_return_if_fail (self != NULL);
	g_return_if_fail (child != NULL);
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
	GdkColor _tmp0_;
	GdkColor _tmp1_;
	GdkColor _tmp2_;
	GdkColor _tmp3_;
	g_return_if_fail (self != NULL);
	g_return_if_fail (frame != NULL);
	gtk_widget_modify_bg (self->priv->pchild, GTK_STATE_NORMAL, (_tmp0_ = gtk_widget_get_style ((GtkWidget*) gtk_widget_get_parent ((GtkWidget*) self))->mid[GTK_STATE_NORMAL], &_tmp0_));
	gtk_widget_modify_base (self->priv->pchild, GTK_STATE_NORMAL, (_tmp1_ = gtk_widget_get_style ((GtkWidget*) gtk_widget_get_parent ((GtkWidget*) self))->mid[GTK_STATE_NORMAL], &_tmp1_));
	gtk_widget_modify_bg ((GtkWidget*) self->priv->eventbox, GTK_STATE_NORMAL, (_tmp2_ = gtk_widget_get_style ((GtkWidget*) gtk_widget_get_parent ((GtkWidget*) self))->dark[GTK_STATE_NORMAL], &_tmp2_));
	gtk_widget_modify_base ((GtkWidget*) self->priv->eventbox, GTK_STATE_NORMAL, (_tmp3_ = gtk_widget_get_style ((GtkWidget*) gtk_widget_get_parent ((GtkWidget*) self))->dark[GTK_STATE_NORMAL], &_tmp3_));
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


GmpcWidgetMore* gmpc_widget_more_construct (GType object_type, const char* unique_id, const char* markup, GtkWidget* child) {
	GmpcWidgetMore * self;
	char* _tmp0_;
	GtkWidget* _tmp1_;
	GtkAlignment* _tmp2_;
	GtkEventBox* _tmp3_;
	GtkHBox* hbox;
	GtkLabel* label;
	const char* _tmp4_;
	GtkButton* _tmp5_;
	const char* _tmp6_;
	GtkImage* _tmp7_;
	g_return_val_if_fail (unique_id != NULL, NULL);
	g_return_val_if_fail (markup != NULL, NULL);
	g_return_val_if_fail (child != NULL, NULL);
	self = g_object_newv (object_type, 0, NULL);
	self->priv->unique_id = (_tmp0_ = g_strdup (unique_id), _g_free0 (self->priv->unique_id), _tmp0_);
	gtk_frame_set_shadow_type ((GtkFrame*) self, GTK_SHADOW_NONE);
	self->priv->pchild = (_tmp1_ = _g_object_ref0 (child), _g_object_unref0 (self->priv->pchild), _tmp1_);
	self->priv->ali = (_tmp2_ = g_object_ref_sink ((GtkAlignment*) gtk_alignment_new (0.f, 0.f, 1.f, 0.f)), _g_object_unref0 (self->priv->ali), _tmp2_);
	gtk_alignment_set_padding (self->priv->ali, (guint) 1, (guint) 1, (guint) 1, (guint) 1);
	self->priv->eventbox = (_tmp3_ = g_object_ref_sink ((GtkEventBox*) gtk_event_box_new ()), _g_object_unref0 (self->priv->eventbox), _tmp3_);
	gtk_event_box_set_visible_window (self->priv->eventbox, TRUE);
	gtk_container_add ((GtkContainer*) self, (GtkWidget*) self->priv->eventbox);
	gtk_container_add ((GtkContainer*) self->priv->eventbox, (GtkWidget*) self->priv->ali);
	if (cfg_get_single_value_as_int_with_default (config, "MoreWidget", unique_id, 0) == 1) {
		self->priv->expand_state = 1;
		gtk_widget_set_size_request ((GtkWidget*) self->priv->ali, -1, -1);
	} else {
		gtk_widget_set_size_request ((GtkWidget*) self->priv->ali, -1, self->priv->max_height);
	}
	gtk_container_add ((GtkContainer*) self->priv->ali, child);
	g_signal_connect_object ((GtkWidget*) self, "style-set", (GCallback) _gmpc_widget_more_bg_style_changed_gtk_widget_style_set, self, 0);
	hbox = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 6));
	label = g_object_ref_sink ((GtkLabel*) gtk_label_new (""));
	gtk_label_set_selectable (label, TRUE);
	gtk_label_set_markup (label, markup);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) label, FALSE, FALSE, (guint) 0);
	_tmp4_ = NULL;
	if (self->priv->expand_state == 0) {
		_tmp4_ = _ ("(more)");
	} else {
		_tmp4_ = _ ("(less)");
	}
	self->priv->expand_button = (_tmp5_ = g_object_ref_sink ((GtkButton*) gtk_button_new_with_label (_tmp4_)), _g_object_unref0 (self->priv->expand_button), _tmp5_);
	_tmp6_ = NULL;
	if (self->priv->expand_state == 0) {
		_tmp6_ = "gtk-add";
	} else {
		_tmp6_ = "gtk-remove";
	}
	gtk_button_set_image (self->priv->expand_button, (GtkWidget*) (_tmp7_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_stock (_tmp6_, GTK_ICON_SIZE_MENU))));
	_g_object_unref0 (_tmp7_);
	gtk_button_set_relief (self->priv->expand_button, GTK_RELIEF_NONE);
	g_signal_connect_object (self->priv->expand_button, "clicked", (GCallback) _gmpc_widget_more_expand_gtk_button_clicked, self, 0);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) self->priv->expand_button, FALSE, FALSE, (guint) 0);
	gtk_frame_set_label_widget ((GtkFrame*) self, (GtkWidget*) hbox);
	g_signal_connect_object (child, "size-allocate", (GCallback) _gmpc_widget_more_size_changed_gtk_widget_size_allocate, self, 0);
	_g_object_unref0 (hbox);
	_g_object_unref0 (label);
	return self;
}


GmpcWidgetMore* gmpc_widget_more_new (const char* unique_id, const char* markup, GtkWidget* child) {
	return gmpc_widget_more_construct (GMPC_WIDGET_TYPE_MORE, unique_id, markup, child);
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
	self->priv->unique_id = NULL;
}


static void gmpc_widget_more_finalize (GObject* obj) {
	GmpcWidgetMore * self;
	self = GMPC_WIDGET_MORE (obj);
	{
		if (self->priv->unique_id != NULL) {
			cfg_set_single_value_as_int (config, "MoreWidget", self->priv->unique_id, self->priv->expand_state);
		}
	}
	_g_object_unref0 (self->priv->ali);
	_g_object_unref0 (self->priv->expand_button);
	_g_object_unref0 (self->priv->eventbox);
	_g_object_unref0 (self->priv->pchild);
	_g_free0 (self->priv->unique_id);
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


static gint* gmpc_now_playing_real_get_version (GmpcPluginBase* base, int* result_length1) {
	GmpcNowPlaying * self;
	gint* result;
	gint* _tmp0_;
	self = (GmpcNowPlaying*) base;
	result = (_tmp0_ = GMPC_NOW_PLAYING_version, *result_length1 = G_N_ELEMENTS (GMPC_NOW_PLAYING_version), _tmp0_);
	return result;
}


static const char* gmpc_now_playing_real_get_name (GmpcPluginBase* base) {
	GmpcNowPlaying * self;
	const char* result;
	self = (GmpcNowPlaying*) base;
	result = N_ ("Now Playing");
	return result;
}


static void gmpc_now_playing_real_save_yourself (GmpcPluginBase* base) {
	GmpcNowPlaying * self;
	self = (GmpcNowPlaying*) base;
	if (self->priv->paned != NULL) {
		GtkScrolledWindow* _tmp0_;
		gtk_object_destroy ((GtkObject*) self->priv->paned);
		self->priv->paned = (_tmp0_ = NULL, _g_object_unref0 (self->priv->paned), _tmp0_);
	}
	if (self->priv->np_ref != NULL) {
		GtkTreePath* path;
		path = gtk_tree_row_reference_get_path (self->priv->np_ref);
		if (path != NULL) {
			gint* _tmp1_;
			gint indices_size;
			gint indices_length1;
			gint* indices;
			indices = (_tmp1_ = gtk_tree_path_get_indices (path), indices_length1 = -1, indices_size = indices_length1, _tmp1_);
			cfg_set_single_value_as_int (config, gmpc_plugin_base_get_name ((GmpcPluginBase*) self), "position", indices[0]);
		}
		_gtk_tree_path_free0 (path);
	}
}


static void gmpc_now_playing_status_changed (GmpcNowPlaying* self, GmpcConnection* conn, MpdObj* server, ChangedStatusType what) {
	gboolean _tmp0_;
	gboolean _tmp1_;
	gboolean _tmp2_;
	g_return_if_fail (self != NULL);
	g_return_if_fail (conn != NULL);
	g_return_if_fail (server != NULL);
	if (self->priv->paned == NULL) {
		return;
	}
	_tmp0_ = FALSE;
	_tmp1_ = FALSE;
	_tmp2_ = FALSE;
	if ((what & MPD_CST_SONGID) == MPD_CST_SONGID) {
		_tmp2_ = TRUE;
	} else {
		_tmp2_ = (what & MPD_CST_PLAYLIST) == MPD_CST_PLAYLIST;
	}
	if (_tmp2_) {
		_tmp1_ = TRUE;
	} else {
		_tmp1_ = (what & MPD_CST_STATE) == MPD_CST_STATE;
	}
	if (_tmp1_) {
		_tmp0_ = self->priv->selected;
	} else {
		_tmp0_ = FALSE;
	}
	if (_tmp0_) {
		gmpc_now_playing_update (self);
	}
}


static void gmpc_now_playing_real_browser_add (GmpcPluginBrowserIface* base, GtkWidget* category_tree) {
	GmpcNowPlaying * self;
	GtkTreeView* tree;
	GtkListStore* store;
	GtkTreeModel* model;
	GtkTreeIter iter = {0};
	GtkTreeRowReference* _tmp1_;
	GtkTreePath* _tmp0_;
	self = (GmpcNowPlaying*) base;
	g_return_if_fail (category_tree != NULL);
	tree = _g_object_ref0 (GTK_TREE_VIEW (category_tree));
	store = _g_object_ref0 (GTK_LIST_STORE (gtk_tree_view_get_model (tree)));
	model = _g_object_ref0 (gtk_tree_view_get_model (tree));
	playlist3_insert_browser (&iter, cfg_get_single_value_as_int_with_default (config, gmpc_plugin_base_get_name ((GmpcPluginBase*) self), "position", 0));
	gtk_list_store_set (store, &iter, 0, ((GmpcPluginBase*) self)->id, 1, _ (gmpc_plugin_base_get_name ((GmpcPluginBase*) self)), 3, "media-audiofile", -1);
	self->priv->np_ref = (_tmp1_ = gtk_tree_row_reference_new (model, _tmp0_ = gtk_tree_model_get_path (model, &iter)), _gtk_tree_row_reference_free0 (self->priv->np_ref), _tmp1_);
	_gtk_tree_path_free0 (_tmp0_);
	_g_object_unref0 (tree);
	_g_object_unref0 (store);
	_g_object_unref0 (model);
}


static void gmpc_now_playing_real_browser_selected (GmpcPluginBrowserIface* base, GtkContainer* container) {
	GmpcNowPlaying * self;
	self = (GmpcNowPlaying*) base;
	g_return_if_fail (container != NULL);
	self->priv->selected = TRUE;
	gmpc_now_playing_browser_init (self);
	gtk_container_add (container, (GtkWidget*) self->priv->paned);
	gmpc_now_playing_update (self);
}


static void gmpc_now_playing_real_browser_unselected (GmpcPluginBrowserIface* base, GtkContainer* container) {
	GmpcNowPlaying * self;
	self = (GmpcNowPlaying*) base;
	g_return_if_fail (container != NULL);
	self->priv->selected = FALSE;
	gtk_container_remove (container, (GtkWidget*) self->priv->paned);
}


static void gmpc_now_playing_browser_bg_style_changed (GmpcNowPlaying* self, GtkScrolledWindow* bg, GtkStyle* style) {
	GdkColor _tmp0_;
	g_return_if_fail (self != NULL);
	g_return_if_fail (bg != NULL);
	gtk_widget_modify_bg ((GtkWidget*) self->priv->container, GTK_STATE_NORMAL, (_tmp0_ = gtk_widget_get_style ((GtkWidget*) self->priv->paned)->base[GTK_STATE_NORMAL], &_tmp0_));
}


static gboolean gmpc_now_playing_browser_key_release_event (GmpcNowPlaying* self, const GdkEventKey* event) {
	gboolean result;
	GtkAdjustment* adj;
	double incr;
	g_return_val_if_fail (self != NULL, FALSE);
	adj = _g_object_ref0 (gtk_scrolled_window_get_vadjustment (self->priv->paned));
	incr = (double) 20;
	g_object_get ((GObject*) adj, "step-increment", &incr, NULL);
	if ((*event).keyval == 0xff55) {
		gtk_adjustment_set_value (adj, gtk_adjustment_get_value (adj) - incr);
		result = TRUE;
		_g_object_unref0 (adj);
		return result;
	} else {
		if ((*event).keyval == 0xff56) {
			gtk_adjustment_set_value (adj, gtk_adjustment_get_value (adj) + incr);
			result = TRUE;
			_g_object_unref0 (adj);
			return result;
		}
	}
	result = FALSE;
	_g_object_unref0 (adj);
	return result;
}


static void _gmpc_now_playing_browser_bg_style_changed_gtk_widget_style_set (GtkScrolledWindow* _sender, GtkStyle* previous_style, gpointer self) {
	gmpc_now_playing_browser_bg_style_changed (self, _sender, previous_style);
}


static gboolean _gmpc_now_playing_browser_key_release_event_gtk_widget_key_release_event (GtkScrolledWindow* _sender, const GdkEventKey* event, gpointer self) {
	return gmpc_now_playing_browser_key_release_event (self, event);
}


static void gmpc_now_playing_browser_init (GmpcNowPlaying* self) {
	g_return_if_fail (self != NULL);
	if (self->priv->paned == NULL) {
		GtkScrolledWindow* _tmp0_;
		GtkEventBox* _tmp1_;
		self->priv->paned = (_tmp0_ = g_object_ref_sink ((GtkScrolledWindow*) gtk_scrolled_window_new (NULL, NULL)), _g_object_unref0 (self->priv->paned), _tmp0_);
		gtk_scrolled_window_set_policy (self->priv->paned, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
		gtk_scrolled_window_set_shadow_type (self->priv->paned, GTK_SHADOW_NONE);
		self->priv->container = (_tmp1_ = g_object_ref_sink ((GtkEventBox*) gtk_event_box_new ()), _g_object_unref0 (self->priv->container), _tmp1_);
		gtk_event_box_set_visible_window (self->priv->container, TRUE);
		g_signal_connect_object ((GtkWidget*) self->priv->paned, "style-set", (GCallback) _gmpc_now_playing_browser_bg_style_changed_gtk_widget_style_set, self, 0);
		gtk_scrolled_window_add_with_viewport (self->priv->paned, (GtkWidget*) self->priv->container);
		g_object_set ((GObject*) gtk_scrolled_window_get_vadjustment (self->priv->paned), "step-increment", 20.0, NULL);
		g_signal_connect_object ((GtkWidget*) self->priv->paned, "key-release-event", (GCallback) _gmpc_now_playing_browser_key_release_event_gtk_widget_key_release_event, self, 0);
	}
}


static void gmpc_now_playing_update (GmpcNowPlaying* self) {
	GError * _inner_error_;
	mpd_Song* song;
	gboolean _tmp0_;
	g_return_if_fail (self != NULL);
	_inner_error_ = NULL;
	if (self->priv->paned == NULL) {
		return;
	}
	song = _mpd_songDup0 (mpd_playlist_get_current_song (connection));
	_tmp0_ = FALSE;
	if (song != NULL) {
		_tmp0_ = mpd_player_get_state (connection) != MPD_STATUS_STATE_STOP;
	} else {
		_tmp0_ = FALSE;
	}
	if (_tmp0_) {
		char* checksum;
		checksum = mpd_song_checksum (song);
		if (_vala_strcmp0 (checksum, self->priv->song_checksum) != 0) {
			GList* list;
			GtkWidget* view;
			char* _tmp1_;
			list = gtk_container_get_children ((GtkContainer*) self->priv->container);
			{
				GList* child_collection;
				GList* child_it;
				child_collection = list;
				for (child_it = child_collection; child_it != NULL; child_it = child_it->next) {
					GtkWidget* child;
					child = _g_object_ref0 ((GtkWidget*) child_it->data);
					{
						gtk_object_destroy ((GtkObject*) child);
						_g_object_unref0 (child);
					}
				}
			}
			view = gmpc_metadata_browser_metadata_box_show_song (self->priv->browser, song, FALSE);
			gtk_container_add ((GtkContainer*) self->priv->container, view);
			self->priv->song_checksum = (_tmp1_ = g_strdup (checksum), _g_free0 (self->priv->song_checksum), _tmp1_);
			_g_list_free0 (list);
			_g_object_unref0 (view);
		}
		_g_free0 (checksum);
	} else {
		char* _tmp2_;
		GList* list;
		GtkIconTheme* it;
		GtkIconInfo* info;
		char* path;
		GtkImage* image;
		GtkHBox* hbox;
		GtkLabel* label;
		char* _tmp5_;
		GtkAlignment* ali;
		self->priv->song_checksum = (_tmp2_ = NULL, _g_free0 (self->priv->song_checksum), _tmp2_);
		list = gtk_container_get_children ((GtkContainer*) self->priv->container);
		{
			GList* child_collection;
			GList* child_it;
			child_collection = list;
			for (child_it = child_collection; child_it != NULL; child_it = child_it->next) {
				GtkWidget* child;
				child = _g_object_ref0 ((GtkWidget*) child_it->data);
				{
					gtk_object_destroy ((GtkObject*) child);
					_g_object_unref0 (child);
				}
			}
		}
		it = _g_object_ref0 (gtk_icon_theme_get_default ());
		info = gtk_icon_theme_lookup_icon (it, "gmpc", 150, 0);
		path = g_strdup (gtk_icon_info_get_filename (info));
		image = NULL;
		if (path != NULL) {
			{
				GdkPixbuf* pb;
				GtkImage* _tmp3_;
				pb = gdk_pixbuf_new_from_file_at_scale (path, 150, 150, TRUE, &_inner_error_);
				if (_inner_error_ != NULL) {
					goto __catch0_g_error;
					goto __finally0;
				}
				image = (_tmp3_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_pixbuf (pb)), _g_object_unref0 (image), _tmp3_);
				_g_object_unref0 (pb);
			}
			goto __finally0;
			__catch0_g_error:
			{
				GError * e;
				e = _inner_error_;
				_inner_error_ = NULL;
				{
					_g_error_free0 (e);
				}
			}
			__finally0:
			if (_inner_error_ != NULL) {
				_g_list_free0 (list);
				_g_object_unref0 (it);
				_gtk_icon_info_free0 (info);
				_g_free0 (path);
				_g_object_unref0 (image);
				_mpd_freeSong0 (song);
				g_critical ("file %s: line %d: uncaught error: %s", __FILE__, __LINE__, _inner_error_->message);
				g_clear_error (&_inner_error_);
				return;
			}
		}
		if (image == NULL) {
			GtkImage* _tmp4_;
			image = (_tmp4_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_icon_name ("gmpc", GTK_ICON_SIZE_DIALOG)), _g_object_unref0 (image), _tmp4_);
		}
		hbox = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 6));
		label = g_object_ref_sink ((GtkLabel*) gtk_label_new (_ ("Gnome Music Player Client")));
		gtk_label_set_selectable (label, TRUE);
		gtk_label_set_markup (label, _tmp5_ = g_strdup_printf ("<span size='%i' weight='bold'>%s</span>", 28 * PANGO_SCALE, _ ("Gnome Music Player Client")));
		_g_free0 (_tmp5_);
		gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) image, FALSE, FALSE, (guint) 0);
		gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) label, FALSE, FALSE, (guint) 0);
		ali = g_object_ref_sink ((GtkAlignment*) gtk_alignment_new (0.5f, 0.5f, 0.0f, 0.0f));
		gtk_container_add ((GtkContainer*) ali, (GtkWidget*) hbox);
		gtk_container_add ((GtkContainer*) self->priv->container, (GtkWidget*) ali);
		_g_list_free0 (list);
		_g_object_unref0 (it);
		_gtk_icon_info_free0 (info);
		_g_free0 (path);
		_g_object_unref0 (image);
		_g_object_unref0 (hbox);
		_g_object_unref0 (label);
		_g_object_unref0 (ali);
	}
	gtk_widget_show_all ((GtkWidget*) self->priv->paned);
	_mpd_freeSong0 (song);
}


static void gmpc_now_playing_select_now_playing_browser (GmpcNowPlaying* self, GtkImageMenuItem* item) {
	GtkTreeView* tree;
	GtkTreeSelection* sel;
	GtkTreePath* path;
	g_return_if_fail (self != NULL);
	g_return_if_fail (item != NULL);
	tree = playlist3_get_category_tree_view ();
	sel = _g_object_ref0 (gtk_tree_view_get_selection (tree));
	path = gtk_tree_row_reference_get_path (self->priv->np_ref);
	if (path != NULL) {
		gtk_tree_selection_select_path (sel, path);
	}
	_g_object_unref0 (sel);
	_gtk_tree_path_free0 (path);
}


static void _gmpc_now_playing_select_now_playing_browser_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self) {
	gmpc_now_playing_select_now_playing_browser (self, _sender);
}


static gint gmpc_now_playing_real_browser_add_go_menu (GmpcPluginBrowserIface* base, GtkMenu* menu) {
	GmpcNowPlaying * self;
	gint result;
	self = (GmpcNowPlaying*) base;
	g_return_val_if_fail (menu != NULL, 0);
	if (gmpc_plugin_base_get_enabled ((GmpcPluginBase*) self)) {
		GtkImageMenuItem* item;
		GtkImage* _tmp0_;
		item = g_object_ref_sink ((GtkImageMenuItem*) gtk_image_menu_item_new_with_mnemonic (_ ("Now Playing")));
		gtk_image_menu_item_set_image (item, (GtkWidget*) (_tmp0_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_icon_name ("media-audiofile", GTK_ICON_SIZE_MENU))));
		_g_object_unref0 (_tmp0_);
		g_signal_connect_object ((GtkMenuItem*) item, "activate", (GCallback) _gmpc_now_playing_select_now_playing_browser_gtk_menu_item_activate, self, 0);
		gtk_widget_add_accelerator ((GtkWidget*) item, "activate", gtk_menu_get_accel_group (menu), (guint) 0x069, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
		gtk_menu_shell_append ((GtkMenuShell*) menu, (GtkWidget*) ((GtkMenuItem*) item));
		result = 1;
		_g_object_unref0 (item);
		return result;
	}
	result = 0;
	return result;
}


GmpcNowPlaying* gmpc_now_playing_construct (GType object_type) {
	GmpcNowPlaying * self;
	self = g_object_newv (object_type, 0, NULL);
	return self;
}


GmpcNowPlaying* gmpc_now_playing_new (void) {
	return gmpc_now_playing_construct (GMPC_TYPE_NOW_PLAYING);
}


static void _gmpc_now_playing_status_changed_gmpc_connection_status_changed (GmpcConnection* _sender, MpdObj* server, ChangedStatusType what, gpointer self) {
	gmpc_now_playing_status_changed (self, _sender, server, what);
}


static GObject * gmpc_now_playing_constructor (GType type, guint n_construct_properties, GObjectConstructParam * construct_properties) {
	GObject * obj;
	GObjectClass * parent_class;
	GmpcNowPlaying * self;
	parent_class = G_OBJECT_CLASS (gmpc_now_playing_parent_class);
	obj = parent_class->constructor (type, n_construct_properties, construct_properties);
	self = GMPC_NOW_PLAYING (obj);
	{
		GmpcMetadataBrowser* _tmp0_;
		((GmpcPluginBase*) self)->plugin_type = 2 | 8;
		g_signal_connect_object (gmpcconn, "status-changed", (GCallback) _gmpc_now_playing_status_changed_gmpc_connection_status_changed, self, 0);
		self->priv->browser = (_tmp0_ = gmpc_metadata_browser_new (), _g_object_unref0 (self->priv->browser), _tmp0_);
	}
	return obj;
}


static void gmpc_now_playing_class_init (GmpcNowPlayingClass * klass) {
	gmpc_now_playing_parent_class = g_type_class_peek_parent (klass);
	g_type_class_add_private (klass, sizeof (GmpcNowPlayingPrivate));
	GMPC_PLUGIN_BASE_CLASS (klass)->get_version = gmpc_now_playing_real_get_version;
	GMPC_PLUGIN_BASE_CLASS (klass)->get_name = gmpc_now_playing_real_get_name;
	GMPC_PLUGIN_BASE_CLASS (klass)->save_yourself = gmpc_now_playing_real_save_yourself;
	G_OBJECT_CLASS (klass)->constructor = gmpc_now_playing_constructor;
	G_OBJECT_CLASS (klass)->finalize = gmpc_now_playing_finalize;
}


static void gmpc_now_playing_gmpc_plugin_browser_iface_interface_init (GmpcPluginBrowserIfaceIface * iface) {
	gmpc_now_playing_gmpc_plugin_browser_iface_parent_iface = g_type_interface_peek_parent (iface);
	iface->browser_add = gmpc_now_playing_real_browser_add;
	iface->browser_selected = gmpc_now_playing_real_browser_selected;
	iface->browser_unselected = gmpc_now_playing_real_browser_unselected;
	iface->browser_add_go_menu = gmpc_now_playing_real_browser_add_go_menu;
}


static void gmpc_now_playing_instance_init (GmpcNowPlaying * self) {
	self->priv = GMPC_NOW_PLAYING_GET_PRIVATE (self);
	self->priv->np_ref = NULL;
	self->priv->browser = NULL;
	self->priv->paned = NULL;
	self->priv->container = NULL;
	self->priv->selected = FALSE;
	self->priv->song_checksum = NULL;
}


static void gmpc_now_playing_finalize (GObject* obj) {
	GmpcNowPlaying * self;
	self = GMPC_NOW_PLAYING (obj);
	_gtk_tree_row_reference_free0 (self->priv->np_ref);
	_g_object_unref0 (self->priv->browser);
	_g_object_unref0 (self->priv->paned);
	_g_object_unref0 (self->priv->container);
	_g_free0 (self->priv->song_checksum);
	G_OBJECT_CLASS (gmpc_now_playing_parent_class)->finalize (obj);
}


GType gmpc_now_playing_get_type (void) {
	static GType gmpc_now_playing_type_id = 0;
	if (gmpc_now_playing_type_id == 0) {
		static const GTypeInfo g_define_type_info = { sizeof (GmpcNowPlayingClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) gmpc_now_playing_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (GmpcNowPlaying), 0, (GInstanceInitFunc) gmpc_now_playing_instance_init, NULL };
		static const GInterfaceInfo gmpc_plugin_browser_iface_info = { (GInterfaceInitFunc) gmpc_now_playing_gmpc_plugin_browser_iface_interface_init, (GInterfaceFinalizeFunc) NULL, NULL};
		gmpc_now_playing_type_id = g_type_register_static (GMPC_PLUGIN_TYPE_BASE, "GmpcNowPlaying", &g_define_type_info, 0);
		g_type_add_interface_static (gmpc_now_playing_type_id, GMPC_PLUGIN_TYPE_BROWSER_IFACE, &gmpc_plugin_browser_iface_info);
	}
	return gmpc_now_playing_type_id;
}



static GType gmpc_metadata_browser_hitem_type_get_type (void) {
	static GType gmpc_metadata_browser_hitem_type_type_id = 0;
	if (G_UNLIKELY (gmpc_metadata_browser_hitem_type_type_id == 0)) {
		static const GEnumValue values[] = {{GMPC_METADATA_BROWSER_HITEM_TYPE_CLEAR, "GMPC_METADATA_BROWSER_HITEM_TYPE_CLEAR", "clear"}, {GMPC_METADATA_BROWSER_HITEM_TYPE_ARTIST, "GMPC_METADATA_BROWSER_HITEM_TYPE_ARTIST", "artist"}, {GMPC_METADATA_BROWSER_HITEM_TYPE_ALBUM, "GMPC_METADATA_BROWSER_HITEM_TYPE_ALBUM", "album"}, {GMPC_METADATA_BROWSER_HITEM_TYPE_SONG, "GMPC_METADATA_BROWSER_HITEM_TYPE_SONG", "song"}, {0, NULL, NULL}};
		gmpc_metadata_browser_hitem_type_type_id = g_enum_register_static ("GmpcMetadataBrowserHitemType", values);
	}
	return gmpc_metadata_browser_hitem_type_type_id;
}


static void _g_list_free_gmpc_metadata_browser_hitem_free (GList* self) {
	g_list_foreach (self, (GFunc) gmpc_metadata_browser_hitem_free, NULL);
	g_list_free (self);
}


static gint* gmpc_metadata_browser_real_get_version (GmpcPluginBase* base, int* result_length1) {
	GmpcMetadataBrowser * self;
	gint* result;
	gint* _tmp0_;
	self = (GmpcMetadataBrowser*) base;
	result = (_tmp0_ = GMPC_METADATA_BROWSER_version, *result_length1 = G_N_ELEMENTS (GMPC_METADATA_BROWSER_version), _tmp0_);
	return result;
}


static const char* gmpc_metadata_browser_real_get_name (GmpcPluginBase* base) {
	GmpcMetadataBrowser * self;
	const char* result;
	self = (GmpcMetadataBrowser*) base;
	result = N_ ("Metadata Browser");
	return result;
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
			gint* _tmp0_;
			gint indices_size;
			gint indices_length1;
			gint* indices;
			indices = (_tmp0_ = gtk_tree_path_get_indices (path), indices_length1 = -1, indices_size = indices_length1, _tmp0_);
			cfg_set_single_value_as_int (config, gmpc_plugin_base_get_name ((GmpcPluginBase*) self), "position", indices[0]);
		}
		_gtk_tree_path_free0 (path);
	}
}


static void gmpc_metadata_browser_select_metadata_browser (GmpcMetadataBrowser* self, GtkImageMenuItem* item) {
	g_return_if_fail (self != NULL);
	g_return_if_fail (item != NULL);
	gmpc_metadata_browser_select_browser (self, NULL);
}


static void _gmpc_metadata_browser_select_metadata_browser_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self) {
	gmpc_metadata_browser_select_metadata_browser (self, _sender);
}


static gint gmpc_metadata_browser_real_browser_add_go_menu (GmpcPluginBrowserIface* base, GtkMenu* menu) {
	GmpcMetadataBrowser * self;
	gint result;
	self = (GmpcMetadataBrowser*) base;
	g_return_val_if_fail (menu != NULL, 0);
	if (gmpc_plugin_base_get_enabled ((GmpcPluginBase*) self)) {
		GtkImageMenuItem* item;
		GtkImage* _tmp0_;
		item = g_object_ref_sink ((GtkImageMenuItem*) gtk_image_menu_item_new_with_mnemonic (_ (gmpc_plugin_base_get_name ((GmpcPluginBase*) self))));
		gtk_image_menu_item_set_image (item, (GtkWidget*) (_tmp0_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_stock ("gtk-info", GTK_ICON_SIZE_MENU))));
		_g_object_unref0 (_tmp0_);
		g_signal_connect_object ((GtkMenuItem*) item, "activate", (GCallback) _gmpc_metadata_browser_select_metadata_browser_gtk_menu_item_activate, self, 0);
		gtk_widget_add_accelerator ((GtkWidget*) item, "activate", gtk_menu_get_accel_group (menu), (guint) 0xffc1, 0, GTK_ACCEL_VISIBLE);
		gtk_menu_shell_append ((GtkMenuShell*) menu, (GtkWidget*) ((GtkMenuItem*) item));
		result = 1;
		_g_object_unref0 (item);
		return result;
	}
	result = 0;
	return result;
}


static void gmpc_metadata_browser_browser_bg_style_changed (GmpcMetadataBrowser* self, GtkScrolledWindow* bg, GtkStyle* style) {
	GdkColor _tmp0_;
	g_return_if_fail (self != NULL);
	g_return_if_fail (bg != NULL);
	gtk_widget_modify_bg ((GtkWidget*) self->priv->metadata_box, GTK_STATE_NORMAL, (_tmp0_ = gtk_widget_get_style ((GtkWidget*) self->priv->metadata_sw)->base[GTK_STATE_NORMAL], &_tmp0_));
}


static gboolean gmpc_metadata_browser_browser_button_press_event (GmpcMetadataBrowser* self, GtkTreeView* tree, const GdkEventButton* event) {
	gboolean result;
	GtkTreePath* path;
	GtkTreePath* _tmp2_;
	gboolean _tmp1_;
	GtkTreePath* _tmp0_;
	g_return_val_if_fail (self != NULL, FALSE);
	g_return_val_if_fail (tree != NULL, FALSE);
	path = NULL;
	if ((*event).button != 1) {
		result = FALSE;
		_gtk_tree_path_free0 (path);
		return result;
	}
	_tmp0_ = NULL;
	if ((_tmp1_ = gtk_tree_view_get_path_at_pos (tree, (gint) (*event).x, (gint) (*event).y, &_tmp0_, NULL, NULL, NULL), path = (_tmp2_ = _tmp0_, _gtk_tree_path_free0 (path), _tmp2_), _tmp1_)) {
		if (gtk_tree_selection_path_is_selected (gtk_tree_view_get_selection (tree), path)) {
			gtk_tree_selection_unselect_path (gtk_tree_view_get_selection (tree), path);
			result = TRUE;
			_gtk_tree_path_free0 (path);
			return result;
		}
	}
	result = FALSE;
	_gtk_tree_path_free0 (path);
	return result;
}


static void gmpc_metadata_browser_browser_artist_entry_changed (GmpcMetadataBrowser* self, GtkEntry* entry) {
	char* text;
	g_return_if_fail (self != NULL);
	g_return_if_fail (entry != NULL);
	text = g_strdup (gtk_entry_get_text (entry));
	if (strlen (text) > 0) {
		gtk_widget_show ((GtkWidget*) entry);
	} else {
		gtk_widget_hide ((GtkWidget*) entry);
		gtk_widget_grab_focus ((GtkWidget*) self->priv->tree_artist);
	}
	gtk_tree_model_filter_refilter (self->priv->model_filter_artist);
	_g_free0 (text);
}


static void gmpc_metadata_browser_artist_add_clicked (GmpcMetadataBrowser* self, GtkImageMenuItem* item) {
	char* artist;
	g_return_if_fail (self != NULL);
	g_return_if_fail (item != NULL);
	artist = gmpc_metadata_browser_browser_get_selected_artist (self);
	if (artist != NULL) {
		MpdData* data;
		mpd_database_search_start (connection, TRUE);
		mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ARTIST, artist);
		data = mpd_database_search_commit (connection);
		data = misc_sort_mpddata_by_album_disc_track (data);
		if (data != NULL) {
			data = mpd_data_get_first (data);
			{
				gboolean _tmp0_;
				_tmp0_ = TRUE;
				while (TRUE) {
					if (!_tmp0_) {
						if (!(data != NULL)) {
							break;
						}
					}
					_tmp0_ = FALSE;
					mpd_playlist_queue_add (connection, data->song->file);
					data = mpd_data_get_next (data);
				}
			}
			mpd_playlist_queue_commit (connection);
		}
		_mpd_data_free0 (data);
	}
	_g_free0 (artist);
}


static void gmpc_metadata_browser_artist_replace_clicked (GmpcMetadataBrowser* self, GtkImageMenuItem* item) {
	g_return_if_fail (self != NULL);
	g_return_if_fail (item != NULL);
	mpd_playlist_clear (connection);
	gmpc_metadata_browser_artist_add_clicked (self, item);
	mpd_player_play (connection);
}


static void _gmpc_metadata_browser_artist_add_clicked_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self) {
	gmpc_metadata_browser_artist_add_clicked (self, _sender);
}


static void _gmpc_metadata_browser_artist_replace_clicked_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self) {
	gmpc_metadata_browser_artist_replace_clicked (self, _sender);
}


static gboolean gmpc_metadata_browser_artist_browser_button_release_event (GmpcMetadataBrowser* self, GtkTreeView* tree, const GdkEventButton* event) {
	gboolean result;
	g_return_val_if_fail (self != NULL, FALSE);
	g_return_val_if_fail (tree != NULL, FALSE);
	if ((*event).button == 3) {
		if (gtk_tree_selection_count_selected_rows (gtk_tree_view_get_selection (tree)) > 0) {
			GtkMenu* menu;
			GtkImageMenuItem* item;
			GtkImageMenuItem* _tmp0_;
			GtkImage* _tmp1_;
			menu = g_object_ref_sink ((GtkMenu*) gtk_menu_new ());
			item = g_object_ref_sink ((GtkImageMenuItem*) gtk_image_menu_item_new_from_stock ("gtk-add", NULL));
			g_signal_connect_object ((GtkMenuItem*) item, "activate", (GCallback) _gmpc_metadata_browser_artist_add_clicked_gtk_menu_item_activate, self, 0);
			gtk_menu_shell_append ((GtkMenuShell*) menu, (GtkWidget*) ((GtkMenuItem*) item));
			item = (_tmp0_ = g_object_ref_sink ((GtkImageMenuItem*) gtk_image_menu_item_new_with_mnemonic (_ ("_Replace"))), _g_object_unref0 (item), _tmp0_);
			gtk_image_menu_item_set_image (item, (GtkWidget*) (_tmp1_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_stock ("gtk-redo", GTK_ICON_SIZE_MENU))));
			_g_object_unref0 (_tmp1_);
			g_signal_connect_object ((GtkMenuItem*) item, "activate", (GCallback) _gmpc_metadata_browser_artist_replace_clicked_gtk_menu_item_activate, self, 0);
			gtk_menu_shell_append ((GtkMenuShell*) menu, (GtkWidget*) ((GtkMenuItem*) item));
			gtk_menu_popup (menu, NULL, NULL, NULL, NULL, (*event).button, (*event).time);
			gtk_widget_show_all ((GtkWidget*) menu);
			result = TRUE;
			_g_object_unref0 (menu);
			_g_object_unref0 (item);
			return result;
		}
	}
	result = FALSE;
	return result;
}


static gboolean gmpc_metadata_browser_visible_func_artist (GmpcMetadataBrowser* self, GtkTreeModel* model, GtkTreeIter* iter) {
	gboolean result;
	char* text;
	char* str;
	gboolean visible;
	gboolean _tmp0_;
	g_return_val_if_fail (self != NULL, FALSE);
	g_return_val_if_fail (model != NULL, FALSE);
	text = g_strdup (gtk_entry_get_text (self->priv->artist_filter_entry));
	str = NULL;
	visible = FALSE;
	if (g_utf8_get_char (g_utf8_offset_to_pointer (text, 0)) == '\0') {
		result = TRUE;
		_g_free0 (text);
		_g_free0 (str);
		return result;
	}
	gtk_tree_model_get (model, iter, 7, &str, -1, -1);
	_tmp0_ = FALSE;
	if (str != NULL) {
		char* _tmp4_;
		char* _tmp3_;
		char* _tmp2_;
		char* _tmp1_;
		_tmp0_ = strstr (_tmp2_ = g_utf8_normalize (_tmp1_ = g_utf8_casefold (str, -1), -1, G_NORMALIZE_DEFAULT), _tmp4_ = g_utf8_normalize (_tmp3_ = g_utf8_casefold (text, -1), -1, G_NORMALIZE_DEFAULT)) != NULL;
		_g_free0 (_tmp4_);
		_g_free0 (_tmp3_);
		_g_free0 (_tmp2_);
		_g_free0 (_tmp1_);
	} else {
		_tmp0_ = FALSE;
	}
	if (_tmp0_) {
		visible = TRUE;
	}
	result = visible;
	_g_free0 (text);
	_g_free0 (str);
	return result;
}


static gboolean gmpc_metadata_browser_browser_artist_key_press_event (GmpcMetadataBrowser* self, GtkTreeView* widget, const GdkEventKey* event) {
	gboolean result;
	gunichar uc;
	g_return_val_if_fail (self != NULL, FALSE);
	g_return_val_if_fail (widget != NULL, FALSE);
	uc = (gunichar) gdk_keyval_to_unicode ((*event).keyval);
	if (uc > 0) {
		char* outbuf;
		gint i;
		outbuf = g_strdup ("       ");
		i = g_unichar_to_utf8 (uc, outbuf);
		((gchar*) outbuf)[i] = '\0';
		gtk_entry_set_text (self->priv->artist_filter_entry, outbuf);
		gtk_widget_grab_focus ((GtkWidget*) self->priv->artist_filter_entry);
		gtk_editable_set_position ((GtkEditable*) self->priv->artist_filter_entry, 1);
		result = TRUE;
		_g_free0 (outbuf);
		return result;
	}
	result = FALSE;
	return result;
}


static glong string_get_length (const char* self) {
	glong result;
	g_return_val_if_fail (self != NULL, 0L);
	result = g_utf8_strlen (self, -1);
	return result;
}


static void gmpc_metadata_browser_album_add_clicked (GmpcMetadataBrowser* self, GtkImageMenuItem* item) {
	char* artist;
	g_return_if_fail (self != NULL);
	g_return_if_fail (item != NULL);
	artist = gmpc_metadata_browser_browser_get_selected_artist (self);
	if (artist != NULL) {
		char* albumartist;
		char* album;
		MpdData* data;
		albumartist = NULL;
		album = gmpc_metadata_browser_browser_get_selected_album (self);
		if (album != NULL) {
			MpdData* ydata;
			mpd_database_search_field_start (connection, MPD_TAG_ITEM_ALBUM_ARTIST);
			mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ALBUM, album);
			ydata = mpd_database_search_commit (connection);
			if (ydata != NULL) {
				if (string_get_length (ydata->tag) > 0) {
					char* _tmp0_;
					albumartist = (_tmp0_ = g_strdup (ydata->tag), _g_free0 (albumartist), _tmp0_);
				}
			}
			_mpd_data_free0 (ydata);
		}
		mpd_database_search_start (connection, TRUE);
		if (albumartist != NULL) {
			mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ALBUM_ARTIST, albumartist);
		} else {
			mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ARTIST, artist);
		}
		if (album != NULL) {
			mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ALBUM, album);
		}
		data = mpd_database_search_commit (connection);
		data = misc_sort_mpddata_by_album_disc_track (data);
		if (data != NULL) {
			{
				gboolean _tmp1_;
				_tmp1_ = TRUE;
				while (TRUE) {
					if (!_tmp1_) {
						if (!(data != NULL)) {
							break;
						}
					}
					_tmp1_ = FALSE;
					mpd_playlist_queue_add (connection, data->song->file);
					data = mpd_data_get_next (data);
				}
			}
			mpd_playlist_queue_commit (connection);
		}
		_g_free0 (albumartist);
		_g_free0 (album);
		_mpd_data_free0 (data);
	}
	_g_free0 (artist);
}


static void gmpc_metadata_browser_album_replace_clicked (GmpcMetadataBrowser* self, GtkImageMenuItem* item) {
	g_return_if_fail (self != NULL);
	g_return_if_fail (item != NULL);
	mpd_playlist_clear (connection);
	gmpc_metadata_browser_album_add_clicked (self, item);
	mpd_player_play (connection);
}


static void _gmpc_metadata_browser_album_add_clicked_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self) {
	gmpc_metadata_browser_album_add_clicked (self, _sender);
}


static void _gmpc_metadata_browser_album_replace_clicked_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self) {
	gmpc_metadata_browser_album_replace_clicked (self, _sender);
}


static gboolean gmpc_metadata_browser_album_browser_button_release_event (GmpcMetadataBrowser* self, GtkTreeView* tree, const GdkEventButton* event) {
	gboolean result;
	g_return_val_if_fail (self != NULL, FALSE);
	g_return_val_if_fail (tree != NULL, FALSE);
	if ((*event).button == 3) {
		if (gtk_tree_selection_count_selected_rows (gtk_tree_view_get_selection (tree)) > 0) {
			GtkMenu* menu;
			GtkImageMenuItem* item;
			GtkImageMenuItem* _tmp0_;
			GtkImage* _tmp1_;
			menu = g_object_ref_sink ((GtkMenu*) gtk_menu_new ());
			item = g_object_ref_sink ((GtkImageMenuItem*) gtk_image_menu_item_new_from_stock ("gtk-add", NULL));
			g_signal_connect_object ((GtkMenuItem*) item, "activate", (GCallback) _gmpc_metadata_browser_album_add_clicked_gtk_menu_item_activate, self, 0);
			gtk_menu_shell_append ((GtkMenuShell*) menu, (GtkWidget*) ((GtkMenuItem*) item));
			item = (_tmp0_ = g_object_ref_sink ((GtkImageMenuItem*) gtk_image_menu_item_new_with_mnemonic (_ ("_Replace"))), _g_object_unref0 (item), _tmp0_);
			gtk_image_menu_item_set_image (item, (GtkWidget*) (_tmp1_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_stock ("gtk-redo", GTK_ICON_SIZE_MENU))));
			_g_object_unref0 (_tmp1_);
			g_signal_connect_object ((GtkMenuItem*) item, "activate", (GCallback) _gmpc_metadata_browser_album_replace_clicked_gtk_menu_item_activate, self, 0);
			gtk_menu_shell_append ((GtkMenuShell*) menu, (GtkWidget*) ((GtkMenuItem*) item));
			gtk_menu_popup (menu, NULL, NULL, NULL, NULL, (*event).button, (*event).time);
			gtk_widget_show_all ((GtkWidget*) menu);
			result = TRUE;
			_g_object_unref0 (menu);
			_g_object_unref0 (item);
			return result;
		}
	}
	result = FALSE;
	return result;
}


static gboolean gmpc_metadata_browser_visible_func_album (GmpcMetadataBrowser* self, GtkTreeModel* model, GtkTreeIter* iter) {
	gboolean result;
	char* text;
	char* str;
	gboolean visible;
	gboolean _tmp0_;
	g_return_val_if_fail (self != NULL, FALSE);
	g_return_val_if_fail (model != NULL, FALSE);
	text = g_strdup (gtk_entry_get_text (self->priv->album_filter_entry));
	str = NULL;
	visible = FALSE;
	if (g_utf8_get_char (g_utf8_offset_to_pointer (text, 0)) == '\0') {
		result = TRUE;
		_g_free0 (text);
		_g_free0 (str);
		return result;
	}
	gtk_tree_model_get (model, iter, 6, &str, -1, -1);
	_tmp0_ = FALSE;
	if (str != NULL) {
		char* _tmp4_;
		char* _tmp3_;
		char* _tmp2_;
		char* _tmp1_;
		_tmp0_ = strstr (_tmp2_ = g_utf8_normalize (_tmp1_ = g_utf8_casefold (str, -1), -1, G_NORMALIZE_DEFAULT), _tmp4_ = g_utf8_normalize (_tmp3_ = g_utf8_casefold (text, -1), -1, G_NORMALIZE_DEFAULT)) != NULL;
		_g_free0 (_tmp4_);
		_g_free0 (_tmp3_);
		_g_free0 (_tmp2_);
		_g_free0 (_tmp1_);
	} else {
		_tmp0_ = FALSE;
	}
	if (_tmp0_) {
		visible = TRUE;
	}
	result = visible;
	_g_free0 (text);
	_g_free0 (str);
	return result;
}


static gboolean gmpc_metadata_browser_browser_album_key_press_event (GmpcMetadataBrowser* self, GtkTreeView* widget, const GdkEventKey* event) {
	gboolean result;
	gunichar uc;
	g_return_val_if_fail (self != NULL, FALSE);
	g_return_val_if_fail (widget != NULL, FALSE);
	uc = (gunichar) gdk_keyval_to_unicode ((*event).keyval);
	if (uc > 0) {
		char* outbuf;
		gint i;
		outbuf = g_strdup ("       ");
		i = g_unichar_to_utf8 (uc, outbuf);
		((gchar*) outbuf)[i] = '\0';
		gtk_entry_set_text (self->priv->album_filter_entry, outbuf);
		gtk_widget_grab_focus ((GtkWidget*) self->priv->album_filter_entry);
		gtk_editable_set_position ((GtkEditable*) self->priv->album_filter_entry, 1);
		result = TRUE;
		_g_free0 (outbuf);
		return result;
	}
	result = FALSE;
	return result;
}


static void gmpc_metadata_browser_browser_album_entry_changed (GmpcMetadataBrowser* self, GtkEntry* entry) {
	char* text;
	g_return_if_fail (self != NULL);
	g_return_if_fail (entry != NULL);
	text = g_strdup (gtk_entry_get_text (entry));
	if (strlen (text) > 0) {
		gtk_widget_show ((GtkWidget*) entry);
	} else {
		gtk_widget_hide ((GtkWidget*) entry);
		gtk_widget_grab_focus ((GtkWidget*) self->priv->tree_album);
	}
	gtk_tree_model_filter_refilter (self->priv->model_filter_album);
	_g_free0 (text);
}


static void gmpc_metadata_browser_song_add_clicked (GmpcMetadataBrowser* self, GtkImageMenuItem* item) {
	mpd_Song* song;
	g_return_if_fail (self != NULL);
	g_return_if_fail (item != NULL);
	song = gmpc_metadata_browser_browser_get_selected_song (self);
	if (song != NULL) {
		mpd_playlist_add (connection, song->file);
	}
	_mpd_freeSong0 (song);
}


static void gmpc_metadata_browser_song_replace_clicked (GmpcMetadataBrowser* self, GtkImageMenuItem* item) {
	g_return_if_fail (self != NULL);
	g_return_if_fail (item != NULL);
	mpd_playlist_clear (connection);
	gmpc_metadata_browser_song_add_clicked (self, item);
	mpd_player_play (connection);
}


static void _gmpc_metadata_browser_song_add_clicked_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self) {
	gmpc_metadata_browser_song_add_clicked (self, _sender);
}


static void _gmpc_metadata_browser_song_replace_clicked_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self) {
	gmpc_metadata_browser_song_replace_clicked (self, _sender);
}


static gboolean gmpc_metadata_browser_song_browser_button_release_event (GmpcMetadataBrowser* self, GtkTreeView* tree, const GdkEventButton* event) {
	gboolean result;
	g_return_val_if_fail (self != NULL, FALSE);
	g_return_val_if_fail (tree != NULL, FALSE);
	if ((*event).button == 3) {
		if (gtk_tree_selection_count_selected_rows (gtk_tree_view_get_selection (tree)) > 0) {
			GtkMenu* menu;
			GtkImageMenuItem* item;
			GtkImageMenuItem* _tmp0_;
			GtkImage* _tmp1_;
			menu = g_object_ref_sink ((GtkMenu*) gtk_menu_new ());
			item = g_object_ref_sink ((GtkImageMenuItem*) gtk_image_menu_item_new_from_stock ("gtk-add", NULL));
			g_signal_connect_object ((GtkMenuItem*) item, "activate", (GCallback) _gmpc_metadata_browser_song_add_clicked_gtk_menu_item_activate, self, 0);
			gtk_menu_shell_append ((GtkMenuShell*) menu, (GtkWidget*) ((GtkMenuItem*) item));
			item = (_tmp0_ = g_object_ref_sink ((GtkImageMenuItem*) gtk_image_menu_item_new_with_mnemonic (_ ("_Replace"))), _g_object_unref0 (item), _tmp0_);
			gtk_image_menu_item_set_image (item, (GtkWidget*) (_tmp1_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_stock ("gtk-redo", GTK_ICON_SIZE_MENU))));
			_g_object_unref0 (_tmp1_);
			g_signal_connect_object ((GtkMenuItem*) item, "activate", (GCallback) _gmpc_metadata_browser_song_replace_clicked_gtk_menu_item_activate, self, 0);
			gtk_menu_shell_append ((GtkMenuShell*) menu, (GtkWidget*) ((GtkMenuItem*) item));
			gtk_menu_popup (menu, NULL, NULL, NULL, NULL, (*event).button, (*event).time);
			gtk_widget_show_all ((GtkWidget*) menu);
			result = TRUE;
			_g_object_unref0 (menu);
			_g_object_unref0 (item);
			return result;
		}
	}
	result = FALSE;
	return result;
}


static gboolean gmpc_metadata_browser_browser_button_release_event (GmpcMetadataBrowser* self, GtkWidget* widget, const GdkEventButton* event) {
	gboolean result;
	g_return_val_if_fail (self != NULL, FALSE);
	g_return_val_if_fail (widget != NULL, FALSE);
	if ((*event).button == 8) {
		gmpc_metadata_browser_history_previous (self);
		result = TRUE;
		return result;
	} else {
		if ((*event).button == 9) {
			gmpc_metadata_browser_history_next (self);
			result = TRUE;
			return result;
		}
	}
	result = FALSE;
	return result;
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


static gboolean _gmpc_metadata_browser_artist_browser_button_release_event_gtk_widget_button_release_event (GtkTreeView* _sender, const GdkEventButton* event, gpointer self) {
	return gmpc_metadata_browser_artist_browser_button_release_event (self, _sender, event);
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


static gboolean _gmpc_metadata_browser_album_browser_button_release_event_gtk_widget_button_release_event (GtkTreeView* _sender, const GdkEventButton* event, gpointer self) {
	return gmpc_metadata_browser_album_browser_button_release_event (self, _sender, event);
}


static gboolean _gmpc_metadata_browser_browser_album_key_press_event_gtk_widget_key_press_event (GtkTreeView* _sender, const GdkEventKey* event, gpointer self) {
	return gmpc_metadata_browser_browser_album_key_press_event (self, _sender, event);
}


static void _gmpc_metadata_browser_browser_album_changed_gtk_tree_selection_changed (GtkTreeSelection* _sender, gpointer self) {
	gmpc_metadata_browser_browser_album_changed (self, _sender);
}


static gboolean _gmpc_metadata_browser_song_browser_button_release_event_gtk_widget_button_release_event (GtkTreeView* _sender, const GdkEventButton* event, gpointer self) {
	return gmpc_metadata_browser_song_browser_button_release_event (self, _sender, event);
}


static void _gmpc_metadata_browser_browser_songs_changed_gtk_tree_selection_changed (GtkTreeSelection* _sender, gpointer self) {
	gmpc_metadata_browser_browser_songs_changed (self, _sender);
}


static void _gmpc_metadata_browser_browser_bg_style_changed_gtk_widget_style_set (GtkScrolledWindow* _sender, GtkStyle* previous_style, gpointer self) {
	gmpc_metadata_browser_browser_bg_style_changed (self, _sender, previous_style);
}


static gboolean _gmpc_metadata_browser_browser_button_release_event_gtk_widget_button_release_event (GtkWidget* _sender, const GdkEventButton* event, gpointer self) {
	return gmpc_metadata_browser_browser_button_release_event (self, _sender, event);
}


static void gmpc_metadata_browser_browser_init (GmpcMetadataBrowser* self) {
	g_return_if_fail (self != NULL);
	if (self->priv->paned == NULL) {
		GtkPaned* _tmp0_;
		GtkBox* _tmp1_;
		GtkVBox* box;
		GtkEntry* _tmp2_;
		GtkScrolledWindow* sw;
		GmpcMpdDataModel* _tmp3_;
		GtkTreeModelFilter* _tmp4_;
		GtkTreeView* _tmp5_;
		GmpcMpdDataTreeviewTooltip* _tmp6_;
		GtkTreeViewColumn* column;
		GtkCellRendererPixbuf* prenderer;
		GtkCellRendererText* trenderer;
		GtkVBox* _tmp7_;
		GtkEntry* _tmp8_;
		GtkScrolledWindow* _tmp9_;
		GmpcMpdDataModel* _tmp10_;
		GtkTreeModelFilter* _tmp11_;
		GtkTreeView* _tmp12_;
		GmpcMpdDataTreeviewTooltip* _tmp13_;
		GtkTreeViewColumn* _tmp14_;
		GtkCellRendererPixbuf* _tmp15_;
		GtkTreeViewColumn* _tmp16_;
		GtkCellRendererText* _tmp17_;
		GtkTreeViewColumn* _tmp18_;
		GtkCellRendererText* _tmp19_;
		GtkScrolledWindow* _tmp20_;
		GmpcMpdDataModel* _tmp21_;
		GtkTreeView* _tmp22_;
		GtkTreeViewColumn* _tmp23_;
		GtkCellRendererPixbuf* _tmp24_;
		GtkCellRendererText* _tmp25_;
		GtkTreeViewColumn* _tmp26_;
		GtkCellRendererText* _tmp27_;
		GtkScrolledWindow* _tmp28_;
		GtkEventBox* _tmp29_;
		self->priv->paned = (_tmp0_ = (GtkPaned*) g_object_ref_sink ((GtkHPaned*) gtk_hpaned_new ()), _g_object_unref0 (self->priv->paned), _tmp0_);
		gmpc_paned_size_group_add_paned (paned_size_group, self->priv->paned);
		self->priv->browser_box = (_tmp1_ = (GtkBox*) g_object_ref_sink ((GtkVBox*) gtk_vbox_new (TRUE, 6)), _g_object_unref0 (self->priv->browser_box), _tmp1_);
		gtk_paned_add1 (self->priv->paned, (GtkWidget*) self->priv->browser_box);
		box = g_object_ref_sink ((GtkVBox*) gtk_vbox_new (FALSE, 6));
		gtk_box_pack_start (self->priv->browser_box, (GtkWidget*) box, TRUE, TRUE, (guint) 0);
		self->priv->artist_filter_entry = (_tmp2_ = g_object_ref_sink ((GtkEntry*) gtk_entry_new ()), _g_object_unref0 (self->priv->artist_filter_entry), _tmp2_);
		gtk_widget_set_no_show_all ((GtkWidget*) self->priv->artist_filter_entry, TRUE);
		g_signal_connect_object ((GtkEditable*) self->priv->artist_filter_entry, "changed", (GCallback) _gmpc_metadata_browser_browser_artist_entry_changed_gtk_editable_changed, self, 0);
		gtk_box_pack_start ((GtkBox*) box, (GtkWidget*) self->priv->artist_filter_entry, FALSE, FALSE, (guint) 0);
		sw = g_object_ref_sink ((GtkScrolledWindow*) gtk_scrolled_window_new (NULL, NULL));
		gtk_scrolled_window_set_policy (sw, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
		gtk_scrolled_window_set_shadow_type (sw, GTK_SHADOW_ETCHED_IN);
		gtk_box_pack_start ((GtkBox*) box, (GtkWidget*) sw, TRUE, TRUE, (guint) 0);
		self->priv->model_artist = (_tmp3_ = gmpc_mpddata_model_new (), _g_object_unref0 (self->priv->model_artist), _tmp3_);
		self->priv->model_filter_artist = (_tmp4_ = (GtkTreeModelFilter*) gtk_tree_model_filter_new ((GtkTreeModel*) self->priv->model_artist, NULL), _g_object_unref0 (self->priv->model_filter_artist), _tmp4_);
		gtk_tree_model_filter_set_visible_func (self->priv->model_filter_artist, _gmpc_metadata_browser_visible_func_artist_gtk_tree_model_filter_visible_func, g_object_ref (self), g_object_unref);
		self->priv->tree_artist = (_tmp5_ = g_object_ref_sink ((GtkTreeView*) gtk_tree_view_new_with_model ((GtkTreeModel*) self->priv->model_filter_artist)), _g_object_unref0 (self->priv->tree_artist), _tmp5_);
		_tmp6_ = g_object_ref_sink (gmpc_mpd_data_treeview_tooltip_new (self->priv->tree_artist, META_ARTIST_ART));
		_g_object_unref0 (_tmp6_);
		g_signal_connect_object ((GtkWidget*) self->priv->tree_artist, "button-press-event", (GCallback) _gmpc_metadata_browser_browser_button_press_event_gtk_widget_button_press_event, self, 0);
		g_signal_connect_object ((GtkWidget*) self->priv->tree_artist, "button-release-event", (GCallback) _gmpc_metadata_browser_artist_browser_button_release_event_gtk_widget_button_release_event, self, 0);
		g_signal_connect_object ((GtkWidget*) self->priv->tree_artist, "key-press-event", (GCallback) _gmpc_metadata_browser_browser_artist_key_press_event_gtk_widget_key_press_event, self, 0);
		gtk_container_add ((GtkContainer*) sw, (GtkWidget*) self->priv->tree_artist);
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
		gtk_tree_view_column_set_sizing (column, GTK_TREE_VIEW_COLUMN_FIXED);
		gtk_tree_view_set_fixed_height_mode (self->priv->tree_artist, TRUE);
		box = (_tmp7_ = g_object_ref_sink ((GtkVBox*) gtk_vbox_new (FALSE, 6)), _g_object_unref0 (box), _tmp7_);
		gtk_box_pack_start (self->priv->browser_box, (GtkWidget*) box, TRUE, TRUE, (guint) 0);
		self->priv->album_filter_entry = (_tmp8_ = g_object_ref_sink ((GtkEntry*) gtk_entry_new ()), _g_object_unref0 (self->priv->album_filter_entry), _tmp8_);
		gtk_widget_set_no_show_all ((GtkWidget*) self->priv->album_filter_entry, TRUE);
		g_signal_connect_object ((GtkEditable*) self->priv->album_filter_entry, "changed", (GCallback) _gmpc_metadata_browser_browser_album_entry_changed_gtk_editable_changed, self, 0);
		gtk_box_pack_start ((GtkBox*) box, (GtkWidget*) self->priv->album_filter_entry, FALSE, FALSE, (guint) 0);
		sw = (_tmp9_ = g_object_ref_sink ((GtkScrolledWindow*) gtk_scrolled_window_new (NULL, NULL)), _g_object_unref0 (sw), _tmp9_);
		gtk_scrolled_window_set_policy (sw, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
		gtk_scrolled_window_set_shadow_type (sw, GTK_SHADOW_ETCHED_IN);
		gtk_box_pack_start ((GtkBox*) box, (GtkWidget*) sw, TRUE, TRUE, (guint) 0);
		self->priv->model_albums = (_tmp10_ = gmpc_mpddata_model_new (), _g_object_unref0 (self->priv->model_albums), _tmp10_);
		self->priv->model_filter_album = (_tmp11_ = (GtkTreeModelFilter*) gtk_tree_model_filter_new ((GtkTreeModel*) self->priv->model_albums, NULL), _g_object_unref0 (self->priv->model_filter_album), _tmp11_);
		gtk_tree_model_filter_set_visible_func (self->priv->model_filter_album, _gmpc_metadata_browser_visible_func_album_gtk_tree_model_filter_visible_func, g_object_ref (self), g_object_unref);
		self->priv->tree_album = (_tmp12_ = g_object_ref_sink ((GtkTreeView*) gtk_tree_view_new_with_model ((GtkTreeModel*) self->priv->model_filter_album)), _g_object_unref0 (self->priv->tree_album), _tmp12_);
		_tmp13_ = g_object_ref_sink (gmpc_mpd_data_treeview_tooltip_new (self->priv->tree_album, META_ALBUM_ART));
		_g_object_unref0 (_tmp13_);
		g_signal_connect_object ((GtkWidget*) self->priv->tree_album, "button-press-event", (GCallback) _gmpc_metadata_browser_browser_button_press_event_gtk_widget_button_press_event, self, 0);
		g_signal_connect_object ((GtkWidget*) self->priv->tree_album, "button-release-event", (GCallback) _gmpc_metadata_browser_album_browser_button_release_event_gtk_widget_button_release_event, self, 0);
		g_signal_connect_object ((GtkWidget*) self->priv->tree_album, "key-press-event", (GCallback) _gmpc_metadata_browser_browser_album_key_press_event_gtk_widget_key_press_event, self, 0);
		gtk_container_add ((GtkContainer*) sw, (GtkWidget*) self->priv->tree_album);
		column = (_tmp14_ = g_object_ref_sink (gtk_tree_view_column_new ()), _g_object_unref0 (column), _tmp14_);
		prenderer = (_tmp15_ = g_object_ref_sink ((GtkCellRendererPixbuf*) gtk_cell_renderer_pixbuf_new ()), _g_object_unref0 (prenderer), _tmp15_);
		g_object_set ((GObject*) prenderer, "height", self->priv->model_albums->icon_size, NULL);
		gtk_cell_layout_pack_start ((GtkCellLayout*) column, (GtkCellRenderer*) prenderer, FALSE);
		gtk_cell_layout_add_attribute ((GtkCellLayout*) column, (GtkCellRenderer*) prenderer, "pixbuf", 27);
		gtk_tree_view_append_column (self->priv->tree_album, column);
		column = (_tmp16_ = g_object_ref_sink (gtk_tree_view_column_new ()), _g_object_unref0 (column), _tmp16_);
		trenderer = (_tmp17_ = g_object_ref_sink ((GtkCellRendererText*) gtk_cell_renderer_text_new ()), _g_object_unref0 (trenderer), _tmp17_);
		gtk_cell_layout_pack_start ((GtkCellLayout*) column, (GtkCellRenderer*) trenderer, TRUE);
		gtk_cell_layout_add_attribute ((GtkCellLayout*) column, (GtkCellRenderer*) trenderer, "text", 14);
		gtk_tree_view_append_column (self->priv->tree_album, column);
		gtk_tree_view_column_set_title (column, _ ("Year"));
		column = (_tmp18_ = g_object_ref_sink (gtk_tree_view_column_new ()), _g_object_unref0 (column), _tmp18_);
		trenderer = (_tmp19_ = g_object_ref_sink ((GtkCellRendererText*) gtk_cell_renderer_text_new ()), _g_object_unref0 (trenderer), _tmp19_);
		gtk_cell_layout_pack_start ((GtkCellLayout*) column, (GtkCellRenderer*) trenderer, TRUE);
		gtk_cell_layout_add_attribute ((GtkCellLayout*) column, (GtkCellRenderer*) trenderer, "text", 6);
		gtk_tree_view_append_column (self->priv->tree_album, column);
		gtk_tree_view_set_search_column (self->priv->tree_album, 6);
		gtk_tree_view_column_set_title (column, _ ("Album"));
		g_signal_connect_object (gtk_tree_view_get_selection (self->priv->tree_album), "changed", (GCallback) _gmpc_metadata_browser_browser_album_changed_gtk_tree_selection_changed, self, 0);
		sw = (_tmp20_ = g_object_ref_sink ((GtkScrolledWindow*) gtk_scrolled_window_new (NULL, NULL)), _g_object_unref0 (sw), _tmp20_);
		gtk_scrolled_window_set_policy (sw, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
		gtk_scrolled_window_set_shadow_type (sw, GTK_SHADOW_ETCHED_IN);
		gtk_box_pack_start (self->priv->browser_box, (GtkWidget*) sw, TRUE, TRUE, (guint) 0);
		self->priv->model_songs = (_tmp21_ = gmpc_mpddata_model_new (), _g_object_unref0 (self->priv->model_songs), _tmp21_);
		self->priv->tree_songs = (_tmp22_ = g_object_ref_sink ((GtkTreeView*) gtk_tree_view_new_with_model ((GtkTreeModel*) self->priv->model_songs)), _g_object_unref0 (self->priv->tree_songs), _tmp22_);
		g_signal_connect_object ((GtkWidget*) self->priv->tree_songs, "button-press-event", (GCallback) _gmpc_metadata_browser_browser_button_press_event_gtk_widget_button_press_event, self, 0);
		g_signal_connect_object ((GtkWidget*) self->priv->tree_songs, "button-release-event", (GCallback) _gmpc_metadata_browser_song_browser_button_release_event_gtk_widget_button_release_event, self, 0);
		gtk_container_add ((GtkContainer*) sw, (GtkWidget*) self->priv->tree_songs);
		column = (_tmp23_ = g_object_ref_sink (gtk_tree_view_column_new ()), _g_object_unref0 (column), _tmp23_);
		prenderer = (_tmp24_ = g_object_ref_sink ((GtkCellRendererPixbuf*) gtk_cell_renderer_pixbuf_new ()), _g_object_unref0 (prenderer), _tmp24_);
		gtk_cell_layout_pack_start ((GtkCellLayout*) column, (GtkCellRenderer*) prenderer, FALSE);
		gtk_cell_layout_add_attribute ((GtkCellLayout*) column, (GtkCellRenderer*) prenderer, "icon-name", 23);
		trenderer = (_tmp25_ = g_object_ref_sink ((GtkCellRendererText*) gtk_cell_renderer_text_new ()), _g_object_unref0 (trenderer), _tmp25_);
		gtk_cell_layout_pack_start ((GtkCellLayout*) column, (GtkCellRenderer*) trenderer, FALSE);
		gtk_cell_layout_add_attribute ((GtkCellLayout*) column, (GtkCellRenderer*) trenderer, "text", 10);
		gtk_tree_view_column_set_title (column, _ ("Track"));
		gtk_tree_view_append_column (self->priv->tree_songs, column);
		column = (_tmp26_ = g_object_ref_sink (gtk_tree_view_column_new ()), _g_object_unref0 (column), _tmp26_);
		trenderer = (_tmp27_ = g_object_ref_sink ((GtkCellRendererText*) gtk_cell_renderer_text_new ()), _g_object_unref0 (trenderer), _tmp27_);
		gtk_cell_layout_pack_start ((GtkCellLayout*) column, (GtkCellRenderer*) trenderer, TRUE);
		gtk_cell_layout_add_attribute ((GtkCellLayout*) column, (GtkCellRenderer*) trenderer, "text", 7);
		gtk_tree_view_append_column (self->priv->tree_songs, column);
		gtk_tree_view_set_search_column (self->priv->tree_songs, 7);
		gtk_tree_view_column_set_title (column, _ ("Songs"));
		g_signal_connect_object (gtk_tree_view_get_selection (self->priv->tree_songs), "changed", (GCallback) _gmpc_metadata_browser_browser_songs_changed_gtk_tree_selection_changed, self, 0);
		self->priv->metadata_sw = (_tmp28_ = g_object_ref_sink ((GtkScrolledWindow*) gtk_scrolled_window_new (NULL, NULL)), _g_object_unref0 (self->priv->metadata_sw), _tmp28_);
		gtk_scrolled_window_set_policy (self->priv->metadata_sw, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
		g_signal_connect_object ((GtkWidget*) self->priv->metadata_sw, "style-set", (GCallback) _gmpc_metadata_browser_browser_bg_style_changed_gtk_widget_style_set, self, 0);
		self->priv->metadata_box = (_tmp29_ = g_object_ref_sink ((GtkEventBox*) gtk_event_box_new ()), _g_object_unref0 (self->priv->metadata_box), _tmp29_);
		gtk_event_box_set_visible_window (self->priv->metadata_box, TRUE);
		gtk_scrolled_window_add_with_viewport (self->priv->metadata_sw, (GtkWidget*) self->priv->metadata_box);
		gtk_paned_add2 (self->priv->paned, (GtkWidget*) self->priv->metadata_sw);
		g_signal_connect_object ((GtkWidget*) self->priv->paned, "button-release-event", (GCallback) _gmpc_metadata_browser_browser_button_release_event_gtk_widget_button_release_event, self, 0);
		gmpc_metadata_browser_reload_browsers (self);
		_g_object_unref0 (box);
		_g_object_unref0 (sw);
		_g_object_unref0 (column);
		_g_object_unref0 (prenderer);
		_g_object_unref0 (trenderer);
	}
	gtk_widget_show_all ((GtkWidget*) self->priv->paned);
}


static void gmpc_metadata_browser_reload_browsers (GmpcMetadataBrowser* self) {
	MpdData* data;
	MpdData* _tmp0_;
	g_return_if_fail (self != NULL);
	if (self->priv->paned == NULL) {
		return;
	}
	gmpc_mpddata_model_set_mpd_data (self->priv->model_songs, NULL);
	gmpc_mpddata_model_set_mpd_data (self->priv->model_albums, NULL);
	gmpc_mpddata_model_set_mpd_data (self->priv->model_artist, NULL);
	gtk_entry_set_text (self->priv->artist_filter_entry, "");
	gtk_entry_set_text (self->priv->album_filter_entry, "");
	mpd_database_search_field_start (connection, MPD_TAG_ITEM_ARTIST);
	data = mpd_database_search_commit (connection);
	data = misc_sort_mpddata_by_album_disc_track (data);
	gmpc_mpddata_model_set_mpd_data (self->priv->model_artist, (_tmp0_ = data, data = NULL, _tmp0_));
	_mpd_data_free0 (data);
}


static char* gmpc_metadata_browser_browser_get_selected_artist (GmpcMetadataBrowser* self) {
	char* result;
	GtkTreeIter iter = {0};
	GtkTreeSelection* sel;
	GtkTreeModel* model;
	GtkTreeModel* _tmp2_;
	gboolean _tmp1_;
	GtkTreeModel* _tmp0_;
	g_return_val_if_fail (self != NULL, NULL);
	sel = _g_object_ref0 (gtk_tree_view_get_selection (self->priv->tree_artist));
	model = NULL;
	_tmp0_ = NULL;
	if ((_tmp1_ = gtk_tree_selection_get_selected (sel, &_tmp0_, &iter), model = (_tmp2_ = _g_object_ref0 (_tmp0_), _g_object_unref0 (model), _tmp2_), _tmp1_)) {
		char* artist;
		artist = NULL;
		gtk_tree_model_get (model, &iter, 7, &artist, -1, -1);
		result = artist;
		_g_object_unref0 (sel);
		_g_object_unref0 (model);
		return result;
	}
	result = NULL;
	_g_object_unref0 (sel);
	_g_object_unref0 (model);
	return result;
}


static char* gmpc_metadata_browser_browser_get_selected_album (GmpcMetadataBrowser* self) {
	char* result;
	GtkTreeIter iter = {0};
	GtkTreeSelection* sel;
	GtkTreeModel* model;
	GtkTreeModel* _tmp2_;
	gboolean _tmp1_;
	GtkTreeModel* _tmp0_;
	g_return_val_if_fail (self != NULL, NULL);
	sel = _g_object_ref0 (gtk_tree_view_get_selection (self->priv->tree_album));
	model = NULL;
	_tmp0_ = NULL;
	if ((_tmp1_ = gtk_tree_selection_get_selected (sel, &_tmp0_, &iter), model = (_tmp2_ = _g_object_ref0 (_tmp0_), _g_object_unref0 (model), _tmp2_), _tmp1_)) {
		char* album;
		album = NULL;
		gtk_tree_model_get (model, &iter, 6, &album, -1, -1);
		result = album;
		_g_object_unref0 (sel);
		_g_object_unref0 (model);
		return result;
	}
	result = NULL;
	_g_object_unref0 (sel);
	_g_object_unref0 (model);
	return result;
}


static mpd_Song* gmpc_metadata_browser_browser_get_selected_song (GmpcMetadataBrowser* self) {
	mpd_Song* result;
	GtkTreeIter iter = {0};
	GtkTreeSelection* sel;
	GtkTreeModel* model;
	GtkTreeModel* _tmp2_;
	gboolean _tmp1_;
	GtkTreeModel* _tmp0_;
	g_return_val_if_fail (self != NULL, NULL);
	sel = _g_object_ref0 (gtk_tree_view_get_selection (self->priv->tree_songs));
	model = NULL;
	_tmp0_ = NULL;
	if ((_tmp1_ = gtk_tree_selection_get_selected (sel, &_tmp0_, &iter), model = (_tmp2_ = _g_object_ref0 (_tmp0_), _g_object_unref0 (model), _tmp2_), _tmp1_)) {
		const mpd_Song* songs;
		songs = NULL;
		gtk_tree_model_get ((GtkTreeModel*) self->priv->model_songs, &iter, 0, &songs, -1, -1);
		result = _mpd_songDup0 (songs);
		_g_object_unref0 (sel);
		_g_object_unref0 (model);
		return result;
	}
	result = NULL;
	_g_object_unref0 (sel);
	_g_object_unref0 (model);
	return result;
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
		MpdData* list;
		const MpdData* iter;
		MpdData* _tmp5_;
		MpdData* _tmp6_;
		MpdData* _tmp7_;
		mpd_database_search_field_start (connection, MPD_TAG_ITEM_ALBUM);
		mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ARTIST, artist);
		data = mpd_database_search_commit (connection);
		data = misc_sort_mpddata_by_album_disc_track (data);
		gmpc_mpddata_model_set_request_artist (self->priv->model_albums, artist);
		list = NULL;
		iter = mpd_data_get_first (data);
		if (iter != NULL) {
			{
				gboolean _tmp0_;
				_tmp0_ = TRUE;
				while (TRUE) {
					mpd_Song* _tmp1_;
					char* _tmp2_;
					char* _tmp3_;
					MpdData* ydata;
					if (!_tmp0_) {
						if (!(iter != NULL)) {
							break;
						}
					}
					_tmp0_ = FALSE;
					list = mpd_new_data_struct_append (list);
					list->type = MPD_DATA_TYPE_SONG;
					list->song = (_tmp1_ = mpd_newSong (), _mpd_freeSong0 (list->song), _tmp1_);
					list->song->artist = (_tmp2_ = g_strdup (artist), _g_free0 (list->song->artist), _tmp2_);
					list->song->album = (_tmp3_ = g_strdup (iter->tag), _g_free0 (list->song->album), _tmp3_);
					mpd_database_search_field_start (connection, MPD_TAG_ITEM_DATE);
					mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ARTIST, artist);
					mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ALBUM, iter->tag);
					ydata = mpd_database_search_commit (connection);
					if (ydata != NULL) {
						char* _tmp4_;
						list->song->date = (_tmp4_ = g_strdup (ydata->tag), _g_free0 (list->song->date), _tmp4_);
					}
					iter = mpd_data_get_next_real (iter, FALSE);
					_mpd_data_free0 (ydata);
				}
			}
		}
		list = misc_sort_mpddata_by_album_disc_track (list);
		gmpc_mpddata_model_set_mpd_data (self->priv->model_albums, (_tmp5_ = list, list = NULL, _tmp5_));
		mpd_database_search_start (connection, TRUE);
		mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ARTIST, artist);
		data = (_tmp6_ = mpd_database_search_commit (connection), _mpd_data_free0 (data), _tmp6_);
		data = misc_sort_mpddata_by_album_disc_track (data);
		gmpc_mpddata_model_set_mpd_data (self->priv->model_songs, (_tmp7_ = data, data = NULL, _tmp7_));
		_mpd_data_free0 (data);
		_mpd_data_free0 (list);
	}
	gmpc_metadata_browser_metadata_box_update (self);
	_g_free0 (artist);
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
		char* albumartist;
		MpdData* data;
		MpdData* _tmp1_;
		album = gmpc_metadata_browser_browser_get_selected_album (self);
		albumartist = NULL;
		if (album != NULL) {
			MpdData* ydata;
			mpd_database_search_field_start (connection, MPD_TAG_ITEM_ALBUM_ARTIST);
			mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ALBUM, album);
			ydata = mpd_database_search_commit (connection);
			if (ydata != NULL) {
				if (string_get_length (ydata->tag) > 0) {
					char* _tmp0_;
					albumartist = (_tmp0_ = g_strdup (ydata->tag), _g_free0 (albumartist), _tmp0_);
				}
			}
			_mpd_data_free0 (ydata);
		}
		mpd_database_search_start (connection, TRUE);
		if (albumartist != NULL) {
			mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ALBUM_ARTIST, albumartist);
		} else {
			mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ARTIST, artist);
		}
		if (album != NULL) {
			mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ALBUM, album);
		}
		data = mpd_database_search_commit (connection);
		data = misc_sort_mpddata_by_album_disc_track (data);
		gmpc_mpddata_model_set_mpd_data (self->priv->model_songs, (_tmp1_ = data, data = NULL, _tmp1_));
		_g_free0 (album);
		_g_free0 (albumartist);
		_mpd_data_free0 (data);
	}
	gmpc_metadata_browser_metadata_box_update (self);
	_g_free0 (artist);
}


static void gmpc_metadata_browser_browser_songs_changed (GmpcMetadataBrowser* self, GtkTreeSelection* song_sel) {
	g_return_if_fail (self != NULL);
	g_return_if_fail (song_sel != NULL);
	gmpc_metadata_browser_metadata_box_clear (self);
	gmpc_metadata_browser_metadata_box_update (self);
}


static void gmpc_metadata_browser_play_song (GmpcMetadataBrowser* self, GtkButton* button) {
	mpd_Song* song;
	g_return_if_fail (self != NULL);
	g_return_if_fail (button != NULL);
	song = _mpd_songDup0 ((const mpd_Song*) g_object_get_data ((GObject*) button, "song"));
	if (song != NULL) {
		play_path (song->file);
	}
	_mpd_freeSong0 (song);
}


static void gmpc_metadata_browser_add_song (GmpcMetadataBrowser* self, GtkButton* button) {
	mpd_Song* song;
	g_return_if_fail (self != NULL);
	g_return_if_fail (button != NULL);
	song = _mpd_songDup0 ((const mpd_Song*) g_object_get_data ((GObject*) button, "song"));
	if (song != NULL) {
		mpd_playlist_add (connection, song->file);
		_mpd_freeSong0 (song);
		return;
	}
	_mpd_freeSong0 (song);
}


static void gmpc_metadata_browser_replace_song (GmpcMetadataBrowser* self, GtkButton* button) {
	mpd_Song* song;
	g_return_if_fail (self != NULL);
	g_return_if_fail (button != NULL);
	song = _mpd_songDup0 ((const mpd_Song*) g_object_get_data ((GObject*) button, "song"));
	if (song != NULL) {
		mpd_playlist_clear (connection);
		mpd_playlist_add (connection, song->file);
		mpd_player_play (connection);
		_mpd_freeSong0 (song);
		return;
	}
	_mpd_freeSong0 (song);
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
		_g_free0 (artist);
		_g_free0 (album);
		_mpd_freeSong0 (song);
		return;
	}
	if (artist != NULL) {
		char* albumartist;
		MpdData* data;
		albumartist = NULL;
		if (album != NULL) {
			MpdData* ydata;
			mpd_database_search_field_start (connection, MPD_TAG_ITEM_ALBUM_ARTIST);
			mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ALBUM, album);
			ydata = mpd_database_search_commit (connection);
			if (ydata != NULL) {
				if (string_get_length (ydata->tag) > 0) {
					char* _tmp0_;
					albumartist = (_tmp0_ = g_strdup (ydata->tag), _g_free0 (albumartist), _tmp0_);
				}
			}
			_mpd_data_free0 (ydata);
		}
		mpd_database_search_start (connection, TRUE);
		if (albumartist != NULL) {
			mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ALBUM_ARTIST, albumartist);
		} else {
			mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ARTIST, artist);
		}
		if (album != NULL) {
			mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ALBUM, album);
		}
		data = mpd_database_search_commit (connection);
		if (data != NULL) {
			data = misc_sort_mpddata_by_album_disc_track (data);
			while (TRUE) {
				if (!(data != NULL)) {
					break;
				}
				mpd_playlist_queue_add (connection, data->song->file);
				data = mpd_data_get_next (data);
			}
			mpd_playlist_queue_commit (connection);
		}
		_g_free0 (albumartist);
		_mpd_data_free0 (data);
	}
	_g_free0 (artist);
	_g_free0 (album);
	_mpd_freeSong0 (song);
}


static void gmpc_metadata_browser_replace_selected_song (GmpcMetadataBrowser* self, GtkButton* button) {
	g_return_if_fail (self != NULL);
	g_return_if_fail (button != NULL);
	mpd_playlist_clear (connection);
	gmpc_metadata_browser_add_selected_song (self, button);
	mpd_player_play (connection);
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
			GtkWidget* child;
			child = _g_object_ref0 ((GtkWidget*) child_it->data);
			{
				gtk_object_destroy ((GtkObject*) child);
				_g_object_unref0 (child);
			}
		}
	}
	_g_list_free0 (list);
}


static void gmpc_metadata_browser_add_entry (GmpcMetadataBrowser* self, GtkTable* table, const char* entry_label, const char* value, GtkWidget* extra, gint* i) {
	gint j;
	gboolean _tmp0_;
	GtkLabel* label;
	char* _tmp1_;
	g_return_if_fail (self != NULL);
	g_return_if_fail (table != NULL);
	g_return_if_fail (entry_label != NULL);
	j = 0;
	_tmp0_ = FALSE;
	if (value == NULL) {
		_tmp0_ = extra == NULL;
	} else {
		_tmp0_ = FALSE;
	}
	if (_tmp0_) {
		return;
	}
	label = g_object_ref_sink ((GtkLabel*) gtk_label_new (""));
	gtk_label_set_selectable (label, TRUE);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.0f);
	gtk_label_set_markup (label, _tmp1_ = g_markup_printf_escaped ("<b>%s:</b>", entry_label));
	_g_free0 (_tmp1_);
	gtk_table_attach (table, (GtkWidget*) label, (guint) j, (guint) (j + 1), (guint) (*i), (guint) ((*i) + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	j++;
	if (value != NULL) {
		GtkLabel* pt_label;
		pt_label = g_object_ref_sink ((GtkLabel*) gtk_label_new (value));
		gtk_label_set_selectable (pt_label, TRUE);
		gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.0f);
		gtk_label_set_line_wrap (pt_label, TRUE);
		gtk_table_attach (table, (GtkWidget*) pt_label, (guint) j, (guint) (j + 1), (guint) (*i), (guint) ((*i) + 1), GTK_EXPAND | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		j++;
		_g_object_unref0 (pt_label);
	}
	if (extra != NULL) {
		gtk_table_attach (table, extra, (guint) j, (guint) (j + 1), (guint) (*i), (guint) ((*i) + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		j++;
	}
	(*i)++;
	_g_object_unref0 (label);
}


static void gmpc_metadata_browser_artist_button_clicked (GmpcMetadataBrowser* self, GtkButton* button) {
	char* artist;
	g_return_if_fail (self != NULL);
	g_return_if_fail (button != NULL);
	artist = g_strdup ((const char*) g_object_get_data ((GObject*) button, "artist"));
	info2_fill_artist_view (artist);
	_g_free0 (artist);
}


static void gmpc_metadata_browser_album_button_clicked (GmpcMetadataBrowser* self, GtkButton* button) {
	char* artist;
	char* album;
	g_return_if_fail (self != NULL);
	g_return_if_fail (button != NULL);
	artist = g_strdup ((const char*) g_object_get_data ((GObject*) button, "artist"));
	album = g_strdup ((const char*) g_object_get_data ((GObject*) button, "album"));
	info2_fill_album_view (artist, album);
	_g_free0 (artist);
	_g_free0 (album);
}


static void _gmpc_metadata_browser_metadata_find_query_gtk_button_clicked (GtkButton* _sender, gpointer self) {
	gmpc_metadata_browser_metadata_find_query (self, _sender);
}


static void _gmpc_metadata_browser_artist_button_clicked_gtk_button_clicked (GtkButton* _sender, gpointer self) {
	gmpc_metadata_browser_artist_button_clicked (self, _sender);
}


static void _gmpc_metadata_browser_album_button_clicked_gtk_button_clicked (GtkButton* _sender, gpointer self) {
	gmpc_metadata_browser_album_button_clicked (self, _sender);
}


static void _gmpc_metadata_browser_metadata_button_open_file_browser_path_gtk_button_clicked (GtkButton* _sender, gpointer self) {
	gmpc_metadata_browser_metadata_button_open_file_browser_path (self, _sender);
}


static void _gmpc_metadata_browser_play_song_gtk_button_clicked (GtkButton* _sender, gpointer self) {
	gmpc_metadata_browser_play_song (self, _sender);
}


static void _gmpc_metadata_browser_add_song_gtk_button_clicked (GtkButton* _sender, gpointer self) {
	gmpc_metadata_browser_add_song (self, _sender);
}


static void _gmpc_metadata_browser_replace_song_gtk_button_clicked (GtkButton* _sender, gpointer self) {
	gmpc_metadata_browser_replace_song (self, _sender);
}


GtkWidget* gmpc_metadata_browser_metadata_box_show_song (GmpcMetadataBrowser* self, const mpd_Song* song, gboolean show_controls) {
	GtkWidget* result;
	GtkVBox* vbox;
	GtkHBox* box;
	GtkLabel* label;
	GtkHBox* hbox;
	GtkAlignment* ali;
	GmpcMetaImage* artist_image;
	GtkTable* info_box;
	gint i;
	GmpcFavoritesButton* fav_button;
	GtkAlignment* _tmp8_;
	g_return_val_if_fail (self != NULL, NULL);
	g_return_val_if_fail (song != NULL, NULL);
	vbox = g_object_ref_sink ((GtkVBox*) gtk_vbox_new (FALSE, 6));
	gtk_container_set_border_width ((GtkContainer*) vbox, (guint) 8);
	box = gmpc_metadata_browser_history_buttons (self);
	label = g_object_ref_sink ((GtkLabel*) gtk_label_new (""));
	gtk_label_set_selectable (label, TRUE);
	if (song->title != NULL) {
		char* _tmp0_;
		gtk_label_set_markup (label, _tmp0_ = g_markup_printf_escaped ("<span size='xx-large' weight='bold'>%s</span>", song->title));
		_g_free0 (_tmp0_);
	} else {
		char* _tmp1_;
		gtk_label_set_markup (label, _tmp1_ = g_markup_printf_escaped ("<span size='xx-large' weight='bold'>%s</span>", _ ("Unknown")));
		_g_free0 (_tmp1_);
	}
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
	gtk_box_pack_start ((GtkBox*) box, (GtkWidget*) label, TRUE, TRUE, (guint) 0);
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) box, FALSE, FALSE, (guint) 0);
	hbox = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 6));
	ali = g_object_ref_sink ((GtkAlignment*) gtk_alignment_new (0.f, 0.f, 0.f, 0.f));
	artist_image = g_object_ref_sink (gmpc_metaimage_new_size (META_ALBUM_ART, 250));
	gmpc_metaimage_set_squared (artist_image, FALSE);
	gmpc_metaimage_update_cover_from_song (artist_image, song);
	gtk_container_add ((GtkContainer*) ali, (GtkWidget*) artist_image);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) ali, FALSE, FALSE, (guint) 0);
	info_box = g_object_ref_sink ((GtkTable*) gtk_table_new ((guint) 4, (guint) 2, FALSE));
	gtk_table_set_row_spacings (info_box, (guint) 3);
	gtk_table_set_col_spacings (info_box, (guint) 8);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) info_box, FALSE, FALSE, (guint) 0);
	i = 0;
	if (song->title != NULL) {
		GtkButton* button;
		GtkImage* _tmp2_;
		button = g_object_ref_sink ((GtkButton*) gtk_button_new ());
		gtk_container_add ((GtkContainer*) button, (GtkWidget*) (_tmp2_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_stock ("gtk-find", GTK_ICON_SIZE_MENU))));
		_g_object_unref0 (_tmp2_);
		gtk_button_set_relief (button, GTK_RELIEF_NONE);
		g_object_set_data_full ((GObject*) button, "query", (void*) g_strdup_printf ("title=(%s)", song->title), (GDestroyNotify) g_free);
		g_signal_connect_object (button, "clicked", (GCallback) _gmpc_metadata_browser_metadata_find_query_gtk_button_clicked, self, 0);
		gtk_widget_set_tooltip_text ((GtkWidget*) button, _ ("Search songs with similar title"));
		gmpc_metadata_browser_add_entry (self, info_box, _ ("Title"), song->title, (GtkWidget*) button, &i);
		_g_object_unref0 (button);
	}
	if (song->artist != NULL) {
		GtkButton* button;
		GtkButton* _tmp3_;
		GtkImage* _tmp4_;
		button = NULL;
		button = (_tmp3_ = g_object_ref_sink ((GtkButton*) gtk_button_new ()), _g_object_unref0 (button), _tmp3_);
		gtk_container_add ((GtkContainer*) button, (GtkWidget*) (_tmp4_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_icon_name ("media-artist", GTK_ICON_SIZE_MENU))));
		_g_object_unref0 (_tmp4_);
		gtk_button_set_relief (button, GTK_RELIEF_NONE);
		g_object_set_data_full ((GObject*) button, "artist", (void*) g_strdup_printf ("%s", song->artist), (GDestroyNotify) g_free);
		g_signal_connect_object (button, "clicked", (GCallback) _gmpc_metadata_browser_artist_button_clicked_gtk_button_clicked, self, 0);
		gmpc_metadata_browser_add_entry (self, info_box, _ ("Artist"), song->artist, (GtkWidget*) button, &i);
		_g_object_unref0 (button);
	}
	gmpc_metadata_browser_add_entry (self, info_box, _ ("Album artist"), song->albumartist, NULL, &i);
	if (song->album != NULL) {
		GtkButton* button;
		button = NULL;
		if (song->artist != NULL) {
			GtkButton* _tmp5_;
			GtkImage* _tmp6_;
			button = (_tmp5_ = g_object_ref_sink ((GtkButton*) gtk_button_new ()), _g_object_unref0 (button), _tmp5_);
			gtk_container_add ((GtkContainer*) button, (GtkWidget*) (_tmp6_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_icon_name ("media-album", GTK_ICON_SIZE_MENU))));
			_g_object_unref0 (_tmp6_);
			gtk_button_set_relief (button, GTK_RELIEF_NONE);
			g_object_set_data_full ((GObject*) button, "artist", (void*) g_strdup_printf ("%s", song->artist), (GDestroyNotify) g_free);
			g_object_set_data_full ((GObject*) button, "album", (void*) g_strdup_printf ("%s", song->album), (GDestroyNotify) g_free);
			g_signal_connect_object (button, "clicked", (GCallback) _gmpc_metadata_browser_album_button_clicked_gtk_button_clicked, self, 0);
		}
		gmpc_metadata_browser_add_entry (self, info_box, _ ("Album"), song->album, (GtkWidget*) button, &i);
		_g_object_unref0 (button);
	}
	gmpc_metadata_browser_add_entry (self, info_box, _ ("Track"), song->track, NULL, &i);
	gmpc_metadata_browser_add_entry (self, info_box, _ ("Date"), song->date, NULL, &i);
	gmpc_metadata_browser_add_entry (self, info_box, _ ("Performer"), song->performer, NULL, &i);
	gmpc_metadata_browser_add_entry (self, info_box, _ ("Disc"), song->disc, NULL, &i);
	gmpc_metadata_browser_add_entry (self, info_box, _ ("Genre"), song->genre, NULL, &i);
	if (song->file != NULL) {
		GtkButton* dbutton;
		GtkImage* _tmp7_;
		dbutton = g_object_ref_sink ((GtkButton*) gtk_button_new ());
		gtk_button_set_relief (dbutton, GTK_RELIEF_NONE);
		gtk_container_add ((GtkContainer*) dbutton, (GtkWidget*) (_tmp7_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_stock ("gtk-open", GTK_ICON_SIZE_MENU))));
		_g_object_unref0 (_tmp7_);
		g_object_set_data_full ((GObject*) dbutton, "path", (void*) g_path_get_dirname (song->file), (GDestroyNotify) g_free);
		g_signal_connect_object (dbutton, "clicked", (GCallback) _gmpc_metadata_browser_metadata_button_open_file_browser_path_gtk_button_clicked, self, 0);
		gtk_widget_set_tooltip_text ((GtkWidget*) dbutton, _ ("Open path to song in file browser"));
		gmpc_metadata_browser_add_entry (self, info_box, _ ("Path"), song->file, (GtkWidget*) dbutton, &i);
		_g_object_unref0 (dbutton);
	}
	fav_button = g_object_ref_sink (gmpc_favorites_button_new ());
	gmpc_favorites_button_set_song (fav_button, song);
	ali = (_tmp8_ = g_object_ref_sink ((GtkAlignment*) gtk_alignment_new (0.0f, 0.5f, 0.f, 0.f)), _g_object_unref0 (ali), _tmp8_);
	gtk_container_add ((GtkContainer*) ali, (GtkWidget*) fav_button);
	gmpc_metadata_browser_add_entry (self, info_box, _ ("Favored"), NULL, (GtkWidget*) ali, &i);
	if (mpd_sticker_supported (connection)) {
		GmpcRating* rating_button;
		GtkAlignment* _tmp9_;
		rating_button = g_object_ref_sink (gmpc_rating_new (connection, song));
		ali = (_tmp9_ = g_object_ref_sink ((GtkAlignment*) gtk_alignment_new (0.0f, 0.5f, 0.f, 0.f)), _g_object_unref0 (ali), _tmp9_);
		gtk_container_add ((GtkContainer*) ali, (GtkWidget*) rating_button);
		gmpc_metadata_browser_add_entry (self, info_box, _ ("Rating"), NULL, (GtkWidget*) ali, &i);
		_g_object_unref0 (rating_button);
	}
	gmpc_metadata_browser_add_entry (self, info_box, _ ("Comment"), song->comment, NULL, &i);
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) hbox, FALSE, FALSE, (guint) 0);
	if (show_controls) {
		GtkButton* button;
		GtkHBox* _tmp10_;
		GtkButton* _tmp11_;
		GtkButton* _tmp12_;
		GtkImage* _tmp13_;
		button = g_object_ref_sink ((GtkButton*) gtk_button_new_from_stock ("gtk-media-play"));
		gtk_button_set_relief (button, GTK_RELIEF_NONE);
		g_object_set_data_full ((GObject*) button, "song", mpd_songDup (song), (GDestroyNotify) mpd_freeSong);
		g_signal_connect_object (button, "clicked", (GCallback) _gmpc_metadata_browser_play_song_gtk_button_clicked, self, 0);
		hbox = (_tmp10_ = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 6)), _g_object_unref0 (hbox), _tmp10_);
		gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) button, FALSE, FALSE, (guint) 0);
		button = (_tmp11_ = g_object_ref_sink ((GtkButton*) gtk_button_new_from_stock ("gtk-add")), _g_object_unref0 (button), _tmp11_);
		gtk_button_set_relief (button, GTK_RELIEF_NONE);
		g_object_set_data_full ((GObject*) button, "song", mpd_songDup (song), (GDestroyNotify) mpd_freeSong);
		g_signal_connect_object (button, "clicked", (GCallback) _gmpc_metadata_browser_add_song_gtk_button_clicked, self, 0);
		gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) button, FALSE, FALSE, (guint) 0);
		button = (_tmp12_ = g_object_ref_sink ((GtkButton*) gtk_button_new_with_mnemonic (_ ("_Replace"))), _g_object_unref0 (button), _tmp12_);
		gtk_button_set_image (button, (GtkWidget*) (_tmp13_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_stock ("gtk-redo", GTK_ICON_SIZE_BUTTON))));
		_g_object_unref0 (_tmp13_);
		gtk_button_set_relief (button, GTK_RELIEF_NONE);
		g_object_set_data_full ((GObject*) button, "song", mpd_songDup (song), (GDestroyNotify) mpd_freeSong);
		g_signal_connect_object (button, "clicked", (GCallback) _gmpc_metadata_browser_replace_song_gtk_button_clicked, self, 0);
		gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) button, FALSE, FALSE, (guint) 0);
		gtk_table_attach (info_box, (GtkWidget*) hbox, (guint) 0, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		i++;
		_g_object_unref0 (button);
	}
	if (cfg_get_single_value_as_int_with_default (config, "MetaData", "show-lyrics", 1) == 1) {
		GmpcMetaTextView* text_view;
		char* _tmp14_;
		GmpcWidgetMore* _tmp15_;
		GmpcWidgetMore* frame;
		text_view = g_object_ref_sink (gmpc_meta_text_view_new (META_SONG_TXT));
		gtk_text_view_set_left_margin ((GtkTextView*) text_view, 8);
		frame = (_tmp15_ = g_object_ref_sink (gmpc_widget_more_new ("lyrics-view", _tmp14_ = g_markup_printf_escaped ("<b>%s:</b>", _ ("Lyrics")), (GtkWidget*) text_view)), _g_free0 (_tmp14_), _tmp15_);
		gmpc_meta_text_view_query_text_from_song (text_view, song);
		gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) frame, FALSE, FALSE, (guint) 0);
		_g_object_unref0 (text_view);
		_g_object_unref0 (frame);
	}
	if (cfg_get_single_value_as_int_with_default (config, "MetaData", "show-guitar-tabs", 1) == 1) {
		GmpcMetaTextView* text_view;
		char* _tmp16_;
		GmpcWidgetMore* _tmp17_;
		GmpcWidgetMore* frame;
		text_view = g_object_ref_sink (gmpc_meta_text_view_new (META_SONG_GUITAR_TAB));
		text_view->use_monospace = TRUE;
		gtk_text_view_set_left_margin ((GtkTextView*) text_view, 8);
		frame = (_tmp17_ = g_object_ref_sink (gmpc_widget_more_new ("guitar-tabs-view", _tmp16_ = g_markup_printf_escaped ("<b>%s:</b>", _ ("Guitar Tabs")), (GtkWidget*) text_view)), _g_free0 (_tmp16_), _tmp17_);
		gmpc_meta_text_view_query_text_from_song (text_view, song);
		gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) frame, FALSE, FALSE, (guint) 0);
		_g_object_unref0 (text_view);
		_g_object_unref0 (frame);
	}
	if (cfg_get_single_value_as_int_with_default (config, "MetaData", "show-similar-songs", 1) == 1) {
		GmpcWidgetSimilarSongs* similar_songs;
		similar_songs = g_object_ref_sink (gmpc_widget_similar_songs_new (song));
		gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) similar_songs, FALSE, FALSE, (guint) 0);
		_g_object_unref0 (similar_songs);
	}
	if (cfg_get_single_value_as_int_with_default (config, "MetaData", "show-web-links", 1) == 1) {
		GmpcSongLinks* song_links;
		song_links = g_object_ref_sink (gmpc_song_links_new (GMPC_SONG_LINKS_TYPE_SONG, song));
		gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) song_links, FALSE, FALSE, (guint) 0);
		_g_object_unref0 (song_links);
	}
	result = (GtkWidget*) vbox;
	_g_object_unref0 (box);
	_g_object_unref0 (label);
	_g_object_unref0 (hbox);
	_g_object_unref0 (ali);
	_g_object_unref0 (artist_image);
	_g_object_unref0 (info_box);
	_g_object_unref0 (fav_button);
	return result;
}


static void gmpc_metadata_browser_metadata_button_open_file_browser_path (GmpcMetadataBrowser* self, GtkButton* button) {
	char* path;
	g_return_if_fail (self != NULL);
	g_return_if_fail (button != NULL);
	path = g_strdup ((const char*) g_object_get_data ((GObject*) button, "path"));
	if (path != NULL) {
		pl3_file_browser_open_path (path);
	}
	_g_free0 (path);
}


static void gmpc_metadata_browser_metadata_find_query (GmpcMetadataBrowser* self, GtkButton* button) {
	char* path;
	g_return_if_fail (self != NULL);
	g_return_if_fail (button != NULL);
	path = g_strdup ((const char*) g_object_get_data ((GObject*) button, "query"));
	if (path != NULL) {
		pl3_find2_ec_database (NULL, path);
	}
	_g_free0 (path);
}


static void gmpc_metadata_browser_album_song_tree_row_activated (GmpcMetadataBrowser* self, GtkTreeView* tree, const GtkTreePath* path, GtkTreeViewColumn* column) {
	GtkTreeIter iter = {0};
	GtkTreeModel* model;
	g_return_if_fail (self != NULL);
	g_return_if_fail (tree != NULL);
	g_return_if_fail (path != NULL);
	g_return_if_fail (column != NULL);
	model = _g_object_ref0 (gtk_tree_view_get_model (tree));
	if (gtk_tree_model_get_iter (model, &iter, path)) {
		const mpd_Song* song;
		song = NULL;
		gtk_tree_model_get (model, &iter, 0, &song, -1, -1);
		if (song != NULL) {
			play_path (song->file);
		}
	}
	_g_object_unref0 (model);
}


static void gmpc_metadata_browser_album_song_browser_play_clicked (GmpcMetadataBrowser* self, GtkImageMenuItem* item) {
	GtkTreeView* tree;
	g_return_if_fail (self != NULL);
	g_return_if_fail (item != NULL);
	tree = _g_object_ref0 (GTK_TREE_VIEW (g_object_get_data ((GObject*) item, "tree")));
	if (tree != NULL) {
		GtkTreeIter iter = {0};
		GtkTreeModel* model;
		GtkTreeSelection* sel;
		GtkTreeModel* _tmp2_;
		GList* _tmp1_;
		GtkTreeModel* _tmp0_;
		GList* list;
		model = _g_object_ref0 (gtk_tree_view_get_model (tree));
		sel = _g_object_ref0 (gtk_tree_view_get_selection (tree));
		_tmp0_ = NULL;
		list = (_tmp1_ = gtk_tree_selection_get_selected_rows (sel, &_tmp0_), model = (_tmp2_ = _g_object_ref0 (_tmp0_), _g_object_unref0 (model), _tmp2_), _tmp1_);
		{
			GList* path_collection;
			GList* path_it;
			path_collection = list;
			for (path_it = path_collection; path_it != NULL; path_it = path_it->next) {
				GtkTreePath* path;
				path = _gtk_tree_path_copy0 ((const GtkTreePath*) path_it->data);
				{
					if (gtk_tree_model_get_iter (model, &iter, path)) {
						const mpd_Song* song;
						song = NULL;
						gtk_tree_model_get (model, &iter, 0, &song, -1, -1);
						if (song != NULL) {
							play_path (song->file);
							_gtk_tree_path_free0 (path);
							_g_object_unref0 (model);
							_g_object_unref0 (sel);
							__g_list_free_gtk_tree_path_free0 (list);
							_g_object_unref0 (tree);
							return;
						}
					}
					_gtk_tree_path_free0 (path);
				}
			}
		}
		_g_object_unref0 (model);
		_g_object_unref0 (sel);
		__g_list_free_gtk_tree_path_free0 (list);
	}
	_g_object_unref0 (tree);
}


static void gmpc_metadata_browser_album_song_browser_add_clicked (GmpcMetadataBrowser* self, GtkImageMenuItem* item) {
	GtkTreeView* tree;
	g_return_if_fail (self != NULL);
	g_return_if_fail (item != NULL);
	tree = _g_object_ref0 (GTK_TREE_VIEW (g_object_get_data ((GObject*) item, "tree")));
	if (tree != NULL) {
		GtkTreeIter iter = {0};
		GtkTreeModel* model;
		GtkTreeSelection* sel;
		GtkTreeModel* _tmp2_;
		GList* _tmp1_;
		GtkTreeModel* _tmp0_;
		GList* list;
		model = _g_object_ref0 (gtk_tree_view_get_model (tree));
		sel = _g_object_ref0 (gtk_tree_view_get_selection (tree));
		_tmp0_ = NULL;
		list = (_tmp1_ = gtk_tree_selection_get_selected_rows (sel, &_tmp0_), model = (_tmp2_ = _g_object_ref0 (_tmp0_), _g_object_unref0 (model), _tmp2_), _tmp1_);
		{
			GList* path_collection;
			GList* path_it;
			path_collection = list;
			for (path_it = path_collection; path_it != NULL; path_it = path_it->next) {
				GtkTreePath* path;
				path = _gtk_tree_path_copy0 ((const GtkTreePath*) path_it->data);
				{
					if (gtk_tree_model_get_iter (model, &iter, path)) {
						const mpd_Song* song;
						song = NULL;
						gtk_tree_model_get (model, &iter, 0, &song, -1, -1);
						if (song != NULL) {
							mpd_playlist_queue_add (connection, song->file);
						}
					}
					_gtk_tree_path_free0 (path);
				}
			}
		}
		mpd_playlist_queue_commit (connection);
		_g_object_unref0 (model);
		_g_object_unref0 (sel);
		__g_list_free_gtk_tree_path_free0 (list);
	}
	_g_object_unref0 (tree);
}


static void gmpc_metadata_browser_album_song_browser_replace_clicked (GmpcMetadataBrowser* self, GtkImageMenuItem* item) {
	g_return_if_fail (self != NULL);
	g_return_if_fail (item != NULL);
	mpd_playlist_clear (connection);
	gmpc_metadata_browser_album_song_browser_add_clicked (self, item);
	mpd_player_play (connection);
}


static void _gmpc_metadata_browser_album_song_browser_play_clicked_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self) {
	gmpc_metadata_browser_album_song_browser_play_clicked (self, _sender);
}


static void _gmpc_metadata_browser_album_song_browser_add_clicked_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self) {
	gmpc_metadata_browser_album_song_browser_add_clicked (self, _sender);
}


static void _gmpc_metadata_browser_album_song_browser_replace_clicked_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self) {
	gmpc_metadata_browser_album_song_browser_replace_clicked (self, _sender);
}


static gboolean gmpc_metadata_browser_album_song_tree_button_press_event (GmpcMetadataBrowser* self, GmpcMpdDataTreeview* tree, const GdkEventButton* event) {
	gboolean result;
	g_return_val_if_fail (self != NULL, FALSE);
	g_return_val_if_fail (tree != NULL, FALSE);
	if ((*event).button == 3) {
		GtkMenu* menu;
		GtkImageMenuItem* item;
		GtkImageMenuItem* _tmp0_;
		GtkImage* _tmp1_;
		menu = g_object_ref_sink ((GtkMenu*) gtk_menu_new ());
		if (gtk_tree_selection_count_selected_rows (gtk_tree_view_get_selection ((GtkTreeView*) tree)) == 1) {
			GtkImageMenuItem* item;
			item = g_object_ref_sink ((GtkImageMenuItem*) gtk_image_menu_item_new_from_stock ("gtk-media-play", NULL));
			g_signal_connect_object ((GtkMenuItem*) item, "activate", (GCallback) _gmpc_metadata_browser_album_song_browser_play_clicked_gtk_menu_item_activate, self, 0);
			g_object_set_data ((GObject*) item, "tree", (void*) tree);
			gtk_menu_shell_append ((GtkMenuShell*) menu, (GtkWidget*) ((GtkMenuItem*) item));
			_g_object_unref0 (item);
		}
		item = g_object_ref_sink ((GtkImageMenuItem*) gtk_image_menu_item_new_from_stock ("gtk-add", NULL));
		g_signal_connect_object ((GtkMenuItem*) item, "activate", (GCallback) _gmpc_metadata_browser_album_song_browser_add_clicked_gtk_menu_item_activate, self, 0);
		g_object_set_data ((GObject*) item, "tree", (void*) tree);
		gtk_menu_shell_append ((GtkMenuShell*) menu, (GtkWidget*) ((GtkMenuItem*) item));
		item = (_tmp0_ = g_object_ref_sink ((GtkImageMenuItem*) gtk_image_menu_item_new_with_mnemonic (_ ("_Replace"))), _g_object_unref0 (item), _tmp0_);
		gtk_image_menu_item_set_image (item, (GtkWidget*) (_tmp1_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_stock ("gtk-redo", GTK_ICON_SIZE_MENU))));
		_g_object_unref0 (_tmp1_);
		g_object_set_data ((GObject*) item, "tree", (void*) tree);
		g_signal_connect_object ((GtkMenuItem*) item, "activate", (GCallback) _gmpc_metadata_browser_album_song_browser_replace_clicked_gtk_menu_item_activate, self, 0);
		gtk_menu_shell_append ((GtkMenuShell*) menu, (GtkWidget*) ((GtkMenuItem*) item));
		if (gtk_tree_selection_count_selected_rows (gtk_tree_view_get_selection ((GtkTreeView*) tree)) == 1) {
			GtkTreeModel* model;
			GtkTreeModel* _tmp4_;
			GList* _tmp3_;
			GtkTreeModel* _tmp2_;
			GList* list;
			model = NULL;
			_tmp2_ = NULL;
			list = (_tmp3_ = gtk_tree_selection_get_selected_rows (gtk_tree_view_get_selection ((GtkTreeView*) tree), &_tmp2_), model = (_tmp4_ = _g_object_ref0 (_tmp2_), _g_object_unref0 (model), _tmp4_), _tmp3_);
			if (list != NULL) {
				const GtkTreePath* path;
				GtkTreeIter iter = {0};
				const mpd_Song* song;
				path = (const GtkTreePath*) list->data;
				song = NULL;
				if (gtk_tree_model_get_iter (model, &iter, path)) {
					gtk_tree_model_get (model, &iter, 0, &song, -1);
					submenu_for_song ((GtkWidget*) menu, song);
				}
			}
			_g_object_unref0 (model);
			__g_list_free_gtk_tree_path_free0 (list);
		}
		gmpc_mpddata_treeview_right_mouse_intergration (tree, menu);
		gtk_menu_popup (menu, NULL, NULL, NULL, NULL, (*event).button, (*event).time);
		gtk_widget_show_all ((GtkWidget*) menu);
		result = TRUE;
		_g_object_unref0 (menu);
		_g_object_unref0 (item);
		return result;
	}
	result = FALSE;
	return result;
}


static void _gmpc_metadata_browser_add_selected_song_gtk_button_clicked (GtkButton* _sender, gpointer self) {
	gmpc_metadata_browser_add_selected_song (self, _sender);
}


static void _gmpc_metadata_browser_replace_selected_song_gtk_button_clicked (GtkButton* _sender, gpointer self) {
	gmpc_metadata_browser_replace_selected_song (self, _sender);
}


static gboolean _gmpc_metadata_browser_album_song_tree_button_press_event_gtk_widget_button_release_event (GmpcMpdDataTreeview* _sender, const GdkEventButton* event, gpointer self) {
	return gmpc_metadata_browser_album_song_tree_button_press_event (self, _sender, event);
}


static void _gmpc_metadata_browser_album_song_tree_row_activated_gtk_tree_view_row_activated (GmpcMpdDataTreeview* _sender, const GtkTreePath* path, GtkTreeViewColumn* column, gpointer self) {
	gmpc_metadata_browser_album_song_tree_row_activated (self, _sender, path, column);
}


static void gmpc_metadata_browser_metadata_box_show_album (GmpcMetadataBrowser* self, const char* artist, const char* album) {
	GtkVBox* vbox;
	GtkHBox* box;
	GtkLabel* label;
	const char* _tmp0_;
	const char* _tmp1_;
	char* _tmp2_;
	GtkHBox* hbox;
	GtkAlignment* ali;
	GmpcMetaImage* artist_image;
	mpd_Song* song;
	char* _tmp3_;
	char* _tmp4_;
	GtkTable* info_box;
	gint i;
	GmpcStatsLabel* pt_label;
	GmpcStatsLabel* _tmp7_;
	GmpcStatsLabel* _tmp8_;
	GmpcStatsLabel* _tmp9_;
	GtkHBox* _tmp10_;
	GtkButton* button;
	GtkButton* _tmp11_;
	GtkImage* _tmp12_;
	GtkLabel* _tmp15_;
	char* _tmp16_;
	GtkScrolledWindow* sw;
	GmpcMpdDataTreeview* song_tree;
	g_return_if_fail (self != NULL);
	g_return_if_fail (artist != NULL);
	g_return_if_fail (album != NULL);
	vbox = g_object_ref_sink ((GtkVBox*) gtk_vbox_new (FALSE, 6));
	gtk_container_set_border_width ((GtkContainer*) vbox, (guint) 8);
	box = gmpc_metadata_browser_history_buttons (self);
	label = g_object_ref_sink ((GtkLabel*) gtk_label_new (""));
	gtk_label_set_selectable (label, TRUE);
	_tmp0_ = NULL;
	if (artist != NULL) {
		_tmp0_ = artist;
	} else {
		_tmp0_ = _ ("Unknown");
	}
	_tmp1_ = NULL;
	if (album != NULL) {
		_tmp1_ = album;
	} else {
		_tmp1_ = _ ("Unknown");
	}
	gtk_label_set_markup (label, _tmp2_ = g_markup_printf_escaped ("<span size='xx-large' weight='bold'>%s - %s</span>", _tmp0_, _tmp1_));
	_g_free0 (_tmp2_);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
	gtk_box_pack_start ((GtkBox*) box, (GtkWidget*) label, TRUE, TRUE, (guint) 0);
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) box, FALSE, FALSE, (guint) 0);
	hbox = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 6));
	ali = g_object_ref_sink ((GtkAlignment*) gtk_alignment_new (0.f, 0.f, 0.f, 0.f));
	artist_image = g_object_ref_sink (gmpc_metaimage_new_size (META_ALBUM_ART, 250));
	gmpc_metaimage_set_squared (artist_image, FALSE);
	song = mpd_newSong ();
	song->artist = (_tmp3_ = g_strdup (artist), _g_free0 (song->artist), _tmp3_);
	song->album = (_tmp4_ = g_strdup (album), _g_free0 (song->album), _tmp4_);
	gmpc_metaimage_update_cover_from_song (artist_image, song);
	gtk_container_add ((GtkContainer*) ali, (GtkWidget*) artist_image);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) ali, FALSE, FALSE, (guint) 0);
	info_box = g_object_ref_sink ((GtkTable*) gtk_table_new ((guint) 4, (guint) 2, FALSE));
	gtk_table_set_row_spacings (info_box, (guint) 3);
	gtk_table_set_col_spacings (info_box, (guint) 8);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) info_box, FALSE, FALSE, (guint) 0);
	i = 0;
	if (song->artist != NULL) {
		GtkButton* button;
		GtkButton* _tmp5_;
		GtkImage* _tmp6_;
		button = NULL;
		button = (_tmp5_ = g_object_ref_sink ((GtkButton*) gtk_button_new ()), _g_object_unref0 (button), _tmp5_);
		gtk_container_add ((GtkContainer*) button, (GtkWidget*) (_tmp6_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_icon_name ("media-artist", GTK_ICON_SIZE_MENU))));
		_g_object_unref0 (_tmp6_);
		gtk_button_set_relief (button, GTK_RELIEF_NONE);
		g_object_set_data_full ((GObject*) button, "artist", (void*) g_strdup_printf ("%s", song->artist), (GDestroyNotify) g_free);
		g_signal_connect_object (button, "clicked", (GCallback) _gmpc_metadata_browser_artist_button_clicked_gtk_button_clicked, self, 0);
		gmpc_metadata_browser_add_entry (self, info_box, _ ("Artist"), song->artist, (GtkWidget*) button, &i);
		_g_object_unref0 (button);
	}
	pt_label = g_object_ref_sink (gmpc_stats_label_new (ALBUM_GENRES_SONGS, song));
	gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
	gtk_label_set_line_wrap ((GtkLabel*) pt_label, TRUE);
	gmpc_metadata_browser_add_entry (self, info_box, _ ("Genres"), NULL, (GtkWidget*) pt_label, &i);
	pt_label = (_tmp7_ = g_object_ref_sink (gmpc_stats_label_new (ALBUM_DATES_SONGS, song)), _g_object_unref0 (pt_label), _tmp7_);
	gtk_label_set_line_wrap ((GtkLabel*) pt_label, TRUE);
	gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
	gmpc_metadata_browser_add_entry (self, info_box, _ ("Dates"), NULL, (GtkWidget*) pt_label, &i);
	pt_label = (_tmp8_ = g_object_ref_sink (gmpc_stats_label_new (ALBUM_NUM_SONGS, song)), _g_object_unref0 (pt_label), _tmp8_);
	gtk_label_set_line_wrap ((GtkLabel*) pt_label, TRUE);
	gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
	gmpc_metadata_browser_add_entry (self, info_box, _ ("Songs"), NULL, (GtkWidget*) pt_label, &i);
	pt_label = (_tmp9_ = g_object_ref_sink (gmpc_stats_label_new (ALBUM_PLAYTIME_SONGS, song)), _g_object_unref0 (pt_label), _tmp9_);
	gtk_label_set_line_wrap ((GtkLabel*) pt_label, TRUE);
	gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
	gmpc_metadata_browser_add_entry (self, info_box, _ ("Playtime"), NULL, (GtkWidget*) pt_label, &i);
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) hbox, FALSE, FALSE, (guint) 0);
	hbox = (_tmp10_ = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 6)), _g_object_unref0 (hbox), _tmp10_);
	button = g_object_ref_sink ((GtkButton*) gtk_button_new_from_stock ("gtk-add"));
	gtk_button_set_relief (button, GTK_RELIEF_NONE);
	g_signal_connect_object (button, "clicked", (GCallback) _gmpc_metadata_browser_add_selected_song_gtk_button_clicked, self, 0);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) button, FALSE, FALSE, (guint) 0);
	button = (_tmp11_ = g_object_ref_sink ((GtkButton*) gtk_button_new_with_mnemonic (_ ("_Replace"))), _g_object_unref0 (button), _tmp11_);
	gtk_button_set_image (button, (GtkWidget*) (_tmp12_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_stock ("gtk-redo", GTK_ICON_SIZE_BUTTON))));
	_g_object_unref0 (_tmp12_);
	gtk_button_set_relief (button, GTK_RELIEF_NONE);
	g_signal_connect_object (button, "clicked", (GCallback) _gmpc_metadata_browser_replace_selected_song_gtk_button_clicked, self, 0);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) button, FALSE, FALSE, (guint) 0);
	gtk_table_attach (info_box, (GtkWidget*) hbox, (guint) 0, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	i++;
	if (cfg_get_single_value_as_int_with_default (config, "MetaData", "show-album-information", 1) == 1) {
		GmpcMetaTextView* text_view;
		char* _tmp13_;
		GmpcWidgetMore* _tmp14_;
		GmpcWidgetMore* frame;
		text_view = g_object_ref_sink (gmpc_meta_text_view_new (META_ALBUM_TXT));
		gtk_text_view_set_left_margin ((GtkTextView*) text_view, 8);
		frame = (_tmp14_ = g_object_ref_sink (gmpc_widget_more_new ("album-information-view", _tmp13_ = g_markup_printf_escaped ("<b>%s:</b>", _ ("Album information")), (GtkWidget*) text_view)), _g_free0 (_tmp13_), _tmp14_);
		gmpc_meta_text_view_query_text_from_song (text_view, song);
		gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) frame, FALSE, FALSE, (guint) 0);
		_g_object_unref0 (text_view);
		_g_object_unref0 (frame);
	}
	label = (_tmp15_ = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), _g_object_unref0 (label), _tmp15_);
	gtk_label_set_selectable (label, TRUE);
	gtk_label_set_markup (label, _tmp16_ = g_strdup_printf ("<b>%s</b>", _ ("Songs")));
	_g_free0 (_tmp16_);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) label, FALSE, FALSE, (guint) 0);
	sw = g_object_ref_sink ((GtkScrolledWindow*) gtk_scrolled_window_new (NULL, NULL));
	gtk_scrolled_window_set_policy (sw, GTK_POLICY_AUTOMATIC, GTK_POLICY_NEVER);
	gtk_scrolled_window_set_shadow_type (sw, GTK_SHADOW_ETCHED_IN);
	song_tree = g_object_ref_sink (gmpc_mpddata_treeview_new ("album-songs", TRUE, (GtkTreeModel*) self->priv->model_songs));
	gmpc_mpddata_treeview_enable_click_fix (song_tree);
	g_signal_connect_object ((GtkWidget*) song_tree, "button-release-event", (GCallback) _gmpc_metadata_browser_album_song_tree_button_press_event_gtk_widget_button_release_event, self, 0);
	g_signal_connect_object ((GtkTreeView*) song_tree, "row-activated", (GCallback) _gmpc_metadata_browser_album_song_tree_row_activated_gtk_tree_view_row_activated, self, 0);
	gtk_container_add ((GtkContainer*) sw, (GtkWidget*) song_tree);
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) sw, FALSE, FALSE, (guint) 0);
	if (cfg_get_single_value_as_int_with_default (config, "MetaData", "show-web-links", 1) == 1) {
		GmpcSongLinks* song_links;
		song_links = g_object_ref_sink (gmpc_song_links_new (GMPC_SONG_LINKS_TYPE_ALBUM, song));
		gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) song_links, FALSE, FALSE, (guint) 0);
		_g_object_unref0 (song_links);
	}
	gtk_container_add ((GtkContainer*) self->priv->metadata_box, (GtkWidget*) vbox);
	gtk_widget_show_all ((GtkWidget*) self->priv->metadata_sw);
	_g_object_unref0 (vbox);
	_g_object_unref0 (box);
	_g_object_unref0 (label);
	_g_object_unref0 (hbox);
	_g_object_unref0 (ali);
	_g_object_unref0 (artist_image);
	_mpd_freeSong0 (song);
	_g_object_unref0 (info_box);
	_g_object_unref0 (pt_label);
	_g_object_unref0 (button);
	_g_object_unref0 (sw);
	_g_object_unref0 (song_tree);
}


static void gmpc_metadata_browser_metadata_box_show_artist (GmpcMetadataBrowser* self, const char* artist) {
	GtkVBox* vbox;
	GtkHBox* box;
	GtkLabel* label;
	const char* _tmp0_;
	char* _tmp1_;
	mpd_Song* song;
	char* _tmp2_;
	GtkHBox* hbox;
	GtkAlignment* ali;
	GmpcMetaImage* artist_image;
	GtkTable* info_box;
	gint i;
	GmpcStatsLabel* pt_label;
	GmpcStatsLabel* _tmp3_;
	GmpcStatsLabel* _tmp4_;
	GmpcStatsLabel* _tmp5_;
	GtkHBox* _tmp6_;
	GtkButton* button;
	GtkButton* _tmp7_;
	GtkImage* _tmp8_;
	g_return_if_fail (self != NULL);
	g_return_if_fail (artist != NULL);
	vbox = g_object_ref_sink ((GtkVBox*) gtk_vbox_new (FALSE, 6));
	gtk_container_set_border_width ((GtkContainer*) vbox, (guint) 8);
	box = gmpc_metadata_browser_history_buttons (self);
	label = g_object_ref_sink ((GtkLabel*) gtk_label_new (""));
	gtk_label_set_selectable (label, TRUE);
	_tmp0_ = NULL;
	if (artist != NULL) {
		_tmp0_ = artist;
	} else {
		_tmp0_ = _ ("Unknown");
	}
	gtk_label_set_markup (label, _tmp1_ = g_markup_printf_escaped ("<span size='xx-large' weight='bold'>%s</span>", _tmp0_));
	_g_free0 (_tmp1_);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
	gtk_box_pack_start ((GtkBox*) box, (GtkWidget*) label, TRUE, TRUE, (guint) 0);
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) box, FALSE, FALSE, (guint) 0);
	song = mpd_newSong ();
	song->artist = (_tmp2_ = g_strdup (artist), _g_free0 (song->artist), _tmp2_);
	hbox = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 6));
	ali = g_object_ref_sink ((GtkAlignment*) gtk_alignment_new (0.f, 0.f, 0.f, 0.f));
	artist_image = g_object_ref_sink (gmpc_metaimage_new_size (META_ARTIST_ART, 250));
	gmpc_metaimage_set_squared (artist_image, FALSE);
	gmpc_metaimage_update_cover_from_song (artist_image, song);
	gtk_container_add ((GtkContainer*) ali, (GtkWidget*) artist_image);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) ali, FALSE, FALSE, (guint) 0);
	info_box = g_object_ref_sink ((GtkTable*) gtk_table_new ((guint) 4, (guint) 2, FALSE));
	gtk_table_set_row_spacings (info_box, (guint) 3);
	gtk_table_set_col_spacings (info_box, (guint) 8);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) info_box, FALSE, FALSE, (guint) 0);
	i = 0;
	pt_label = g_object_ref_sink (gmpc_stats_label_new (ARTIST_GENRES_SONGS, song));
	gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
	gtk_label_set_line_wrap ((GtkLabel*) pt_label, TRUE);
	gmpc_metadata_browser_add_entry (self, info_box, _ ("Genres"), NULL, (GtkWidget*) pt_label, &i);
	pt_label = (_tmp3_ = g_object_ref_sink (gmpc_stats_label_new (ARTIST_DATES_SONGS, song)), _g_object_unref0 (pt_label), _tmp3_);
	gtk_label_set_line_wrap ((GtkLabel*) pt_label, TRUE);
	gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
	gmpc_metadata_browser_add_entry (self, info_box, _ ("Dates"), NULL, (GtkWidget*) pt_label, &i);
	pt_label = (_tmp4_ = g_object_ref_sink (gmpc_stats_label_new (ARTIST_NUM_SONGS, song)), _g_object_unref0 (pt_label), _tmp4_);
	gtk_label_set_line_wrap ((GtkLabel*) pt_label, TRUE);
	gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
	gmpc_metadata_browser_add_entry (self, info_box, _ ("Songs"), NULL, (GtkWidget*) pt_label, &i);
	pt_label = (_tmp5_ = g_object_ref_sink (gmpc_stats_label_new (ARTIST_PLAYTIME_SONGS, song)), _g_object_unref0 (pt_label), _tmp5_);
	gtk_label_set_line_wrap ((GtkLabel*) pt_label, TRUE);
	gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
	gmpc_metadata_browser_add_entry (self, info_box, _ ("Playtime"), NULL, (GtkWidget*) pt_label, &i);
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) hbox, FALSE, FALSE, (guint) 0);
	hbox = (_tmp6_ = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 6)), _g_object_unref0 (hbox), _tmp6_);
	button = g_object_ref_sink ((GtkButton*) gtk_button_new_from_stock ("gtk-add"));
	gtk_button_set_relief (button, GTK_RELIEF_NONE);
	g_signal_connect_object (button, "clicked", (GCallback) _gmpc_metadata_browser_add_selected_song_gtk_button_clicked, self, 0);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) button, FALSE, FALSE, (guint) 0);
	button = (_tmp7_ = g_object_ref_sink ((GtkButton*) gtk_button_new_with_mnemonic (_ ("_Replace"))), _g_object_unref0 (button), _tmp7_);
	gtk_button_set_image (button, (GtkWidget*) (_tmp8_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_stock ("gtk-redo", GTK_ICON_SIZE_BUTTON))));
	_g_object_unref0 (_tmp8_);
	gtk_button_set_relief (button, GTK_RELIEF_NONE);
	g_signal_connect_object (button, "clicked", (GCallback) _gmpc_metadata_browser_replace_selected_song_gtk_button_clicked, self, 0);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) button, FALSE, FALSE, (guint) 0);
	gtk_table_attach (info_box, (GtkWidget*) hbox, (guint) 0, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	i++;
	if (cfg_get_single_value_as_int_with_default (config, "MetaData", "show-artist-information", 1) == 1) {
		GmpcMetaTextView* text_view;
		char* _tmp9_;
		GmpcWidgetMore* _tmp10_;
		GmpcWidgetMore* frame;
		text_view = g_object_ref_sink (gmpc_meta_text_view_new (META_ARTIST_TXT));
		gtk_text_view_set_left_margin ((GtkTextView*) text_view, 8);
		frame = (_tmp10_ = g_object_ref_sink (gmpc_widget_more_new ("artist-information-view", _tmp9_ = g_markup_printf_escaped ("<b>%s:</b>", _ ("Artist information")), (GtkWidget*) text_view)), _g_free0 (_tmp9_), _tmp10_);
		gmpc_meta_text_view_query_text_from_song (text_view, song);
		gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) frame, FALSE, FALSE, (guint) 0);
		_g_object_unref0 (text_view);
		_g_object_unref0 (frame);
	}
	if (cfg_get_single_value_as_int_with_default (config, "MetaData", "show-similar-artist", 1) == 1) {
		GtkLabel* _tmp11_;
		char* _tmp12_;
		GtkAlignment* _tmp13_;
		GmpcWidgetSimilarArtist* similar_artist;
		label = (_tmp11_ = g_object_ref_sink ((GtkLabel*) gtk_label_new (_ ("Similar artist"))), _g_object_unref0 (label), _tmp11_);
		gtk_label_set_selectable (label, TRUE);
		gtk_label_set_markup (label, _tmp12_ = g_strdup_printf ("<span weight='bold'>%s</span>", _ ("Similar artist")));
		_g_free0 (_tmp12_);
		gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.0f);
		gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) label, FALSE, FALSE, (guint) 0);
		ali = (_tmp13_ = g_object_ref_sink ((GtkAlignment*) gtk_alignment_new (0.0f, 0.0f, 0.0f, 0.0f)), _g_object_unref0 (ali), _tmp13_);
		similar_artist = g_object_ref_sink (gmpc_widget_similar_artist_new (self, connection, song));
		gtk_container_add ((GtkContainer*) ali, (GtkWidget*) similar_artist);
		gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) ali, FALSE, FALSE, (guint) 0);
		_g_object_unref0 (similar_artist);
	}
	if (cfg_get_single_value_as_int_with_default (config, "MetaData", "show-web-links", 1) == 1) {
		GmpcSongLinks* song_links;
		song_links = g_object_ref_sink (gmpc_song_links_new (GMPC_SONG_LINKS_TYPE_ARTIST, song));
		gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) song_links, FALSE, FALSE, (guint) 0);
		_g_object_unref0 (song_links);
	}
	gtk_container_add ((GtkContainer*) self->priv->metadata_box, (GtkWidget*) vbox);
	gtk_widget_show_all ((GtkWidget*) self->priv->metadata_box);
	_g_object_unref0 (vbox);
	_g_object_unref0 (box);
	_g_object_unref0 (label);
	_mpd_freeSong0 (song);
	_g_object_unref0 (hbox);
	_g_object_unref0 (ali);
	_g_object_unref0 (artist_image);
	_g_object_unref0 (info_box);
	_g_object_unref0 (pt_label);
	_g_object_unref0 (button);
}


static gboolean _gmpc_metadata_browser_metadata_box_update_real_gsource_func (gpointer self) {
	return gmpc_metadata_browser_metadata_box_update_real (self);
}


static void gmpc_metadata_browser_metadata_box_update (GmpcMetadataBrowser* self) {
	g_return_if_fail (self != NULL);
	if (self->priv->update_timeout > 0) {
		g_source_remove (self->priv->update_timeout);
	}
	self->priv->update_timeout = g_idle_add_full (G_PRIORITY_DEFAULT_IDLE, _gmpc_metadata_browser_metadata_box_update_real_gsource_func, g_object_ref (self), g_object_unref);
}


static gboolean gmpc_metadata_browser_metadata_box_update_real (GmpcMetadataBrowser* self) {
	gboolean result;
	char* artist;
	char* album;
	mpd_Song* song;
	g_return_val_if_fail (self != NULL, FALSE);
	if (self->priv->block_update > 0) {
		self->priv->update_timeout = (guint) 0;
		result = FALSE;
		return result;
	}
	artist = gmpc_metadata_browser_browser_get_selected_artist (self);
	album = gmpc_metadata_browser_browser_get_selected_album (self);
	song = gmpc_metadata_browser_browser_get_selected_song (self);
	if (song != NULL) {
		GmpcMetadataBrowserHitem _tmp0_ = {0};
		GmpcMetadataBrowserHitem item;
		mpd_Song* _tmp1_;
		GtkWidget* view;
		item = (memset (&_tmp0_, 0, sizeof (GmpcMetadataBrowserHitem)), _tmp0_);
		item.song = (_tmp1_ = _mpd_songDup0 (song), _mpd_freeSong0 (item.song), _tmp1_);
		item.type = GMPC_METADATA_BROWSER_HITEM_TYPE_SONG;
		gmpc_metadata_browser_history_add (self, &item);
		view = gmpc_metadata_browser_metadata_box_show_song (self, song, TRUE);
		gtk_container_add ((GtkContainer*) self->priv->metadata_box, view);
		gtk_widget_show_all ((GtkWidget*) self->priv->metadata_box);
		gmpc_metadata_browser_hitem_destroy (&item);
		_g_object_unref0 (view);
	} else {
		gboolean _tmp2_;
		_tmp2_ = FALSE;
		if (album != NULL) {
			_tmp2_ = artist != NULL;
		} else {
			_tmp2_ = FALSE;
		}
		if (_tmp2_) {
			GmpcMetadataBrowserHitem _tmp3_ = {0};
			GmpcMetadataBrowserHitem item;
			mpd_Song* _tmp4_;
			char* _tmp5_;
			char* _tmp6_;
			item = (memset (&_tmp3_, 0, sizeof (GmpcMetadataBrowserHitem)), _tmp3_);
			item.song = (_tmp4_ = mpd_newSong (), _mpd_freeSong0 (item.song), _tmp4_);
			item.song->artist = (_tmp5_ = g_strdup (artist), _g_free0 (item.song->artist), _tmp5_);
			item.song->album = (_tmp6_ = g_strdup (album), _g_free0 (item.song->album), _tmp6_);
			item.type = GMPC_METADATA_BROWSER_HITEM_TYPE_ALBUM;
			gmpc_metadata_browser_history_add (self, &item);
			gmpc_metadata_browser_metadata_box_show_album (self, artist, album);
			gmpc_metadata_browser_hitem_destroy (&item);
		} else {
			if (artist != NULL) {
				GmpcMetadataBrowserHitem _tmp7_ = {0};
				GmpcMetadataBrowserHitem item;
				mpd_Song* _tmp8_;
				char* _tmp9_;
				item = (memset (&_tmp7_, 0, sizeof (GmpcMetadataBrowserHitem)), _tmp7_);
				item.song = (_tmp8_ = mpd_newSong (), _mpd_freeSong0 (item.song), _tmp8_);
				item.song->artist = (_tmp9_ = g_strdup (artist), _g_free0 (item.song->artist), _tmp9_);
				item.type = GMPC_METADATA_BROWSER_HITEM_TYPE_ARTIST;
				gmpc_metadata_browser_history_add (self, &item);
				gmpc_metadata_browser_metadata_box_show_artist (self, artist);
				gmpc_metadata_browser_hitem_destroy (&item);
			}
		}
	}
	self->priv->update_timeout = (guint) 0;
	result = FALSE;
	_g_free0 (artist);
	_g_free0 (album);
	_mpd_freeSong0 (song);
	return result;
}


static void gmpc_metadata_browser_real_browser_add (GmpcPluginBrowserIface* base, GtkWidget* category_tree) {
	GmpcMetadataBrowser * self;
	GtkTreeView* tree;
	GtkListStore* store;
	GtkTreeModel* model;
	GtkTreeIter iter = {0};
	GtkTreeRowReference* _tmp1_;
	GtkTreePath* _tmp0_;
	self = (GmpcMetadataBrowser*) base;
	g_return_if_fail (category_tree != NULL);
	tree = _g_object_ref0 (GTK_TREE_VIEW (category_tree));
	store = _g_object_ref0 (GTK_LIST_STORE (gtk_tree_view_get_model (tree)));
	model = _g_object_ref0 (gtk_tree_view_get_model (tree));
	playlist3_insert_browser (&iter, cfg_get_single_value_as_int_with_default (config, gmpc_plugin_base_get_name ((GmpcPluginBase*) self), "position", 100));
	gtk_list_store_set (store, &iter, 0, ((GmpcPluginBase*) self)->id, 1, _ (gmpc_plugin_base_get_name ((GmpcPluginBase*) self)), 3, "gtk-info", -1);
	self->priv->rref = (_tmp1_ = gtk_tree_row_reference_new (model, _tmp0_ = gtk_tree_model_get_path (model, &iter)), _gtk_tree_row_reference_free0 (self->priv->rref), _tmp1_);
	_gtk_tree_path_free0 (_tmp0_);
	_g_object_unref0 (tree);
	_g_object_unref0 (store);
	_g_object_unref0 (model);
}


static void gmpc_metadata_browser_real_browser_selected (GmpcPluginBrowserIface* base, GtkContainer* container) {
	GmpcMetadataBrowser * self;
	char* artist;
	char* _tmp0_;
	self = (GmpcMetadataBrowser*) base;
	g_return_if_fail (container != NULL);
	artist = NULL;
	self->priv->selected = TRUE;
	gmpc_metadata_browser_browser_init (self);
	gtk_container_add (container, (GtkWidget*) self->priv->paned);
	artist = (_tmp0_ = gmpc_metadata_browser_browser_get_selected_artist (self), _g_free0 (artist), _tmp0_);
	if (artist == NULL) {
		gmpc_metadata_browser_metadata_box_clear (self);
		gmpc_metadata_browser_metadata_box_update (self);
	}
	_g_free0 (artist);
}


static void gmpc_metadata_browser_real_browser_unselected (GmpcPluginBrowserIface* base, GtkContainer* container) {
	GmpcMetadataBrowser * self;
	self = (GmpcMetadataBrowser*) base;
	g_return_if_fail (container != NULL);
	self->priv->selected = FALSE;
	gtk_container_remove (container, (GtkWidget*) self->priv->paned);
}


static void gmpc_metadata_browser_con_changed (GmpcMetadataBrowser* self, GmpcConnection* conn, MpdObj* server, gint connect) {
	g_return_if_fail (self != NULL);
	g_return_if_fail (conn != NULL);
	g_return_if_fail (server != NULL);
	if (self->priv->paned == NULL) {
		return;
	}
	gmpc_metadata_browser_history_clear (self);
	gmpc_metadata_browser_reload_browsers (self);
	gmpc_metadata_browser_metadata_box_clear (self);
	gmpc_metadata_browser_metadata_box_update (self);
}


static void gmpc_metadata_browser_status_changed (GmpcMetadataBrowser* self, GmpcConnection* conn, MpdObj* server, ChangedStatusType what) {
	g_return_if_fail (self != NULL);
	g_return_if_fail (conn != NULL);
	g_return_if_fail (server != NULL);
	if (self->priv->paned == NULL) {
		return;
	}
	if ((what & MPD_CST_DATABASE) != 0) {
		gmpc_metadata_browser_reload_browsers (self);
		if (self->priv->current != NULL) {
			gmpc_metadata_browser_show_hitem (self, (GmpcMetadataBrowserHitem*) self->priv->current->data);
		}
	}
}


static void gmpc_metadata_browser_show_hitem (GmpcMetadataBrowser* self, const GmpcMetadataBrowserHitem* hi) {
	g_return_if_fail (self != NULL);
	switch ((*hi).type) {
		case GMPC_METADATA_BROWSER_HITEM_TYPE_ARTIST:
		{
			gmpc_metadata_browser_set_artist (self, (*hi).song->artist);
			break;
		}
		case GMPC_METADATA_BROWSER_HITEM_TYPE_ALBUM:
		{
			gmpc_metadata_browser_set_album (self, (*hi).song->artist, (*hi).song->album);
			break;
		}
		case GMPC_METADATA_BROWSER_HITEM_TYPE_SONG:
		{
			gmpc_metadata_browser_set_song (self, (*hi).song);
			break;
		}
		default:
		{
			gmpc_metadata_browser_metadata_box_clear (self);
			break;
		}
	}
}


static void gmpc_metadata_browser_history_previous (GmpcMetadataBrowser* self) {
	gboolean _tmp0_;
	g_return_if_fail (self != NULL);
	_tmp0_ = FALSE;
	if (self->priv->history == NULL) {
		_tmp0_ = TRUE;
	} else {
		_tmp0_ = self->priv->current == NULL;
	}
	if (_tmp0_) {
		return;
	}
	if (self->priv->current->next == NULL) {
		return;
	}
	self->priv->current = self->priv->current->next;
	if (self->priv->current != NULL) {
		gmpc_metadata_browser_show_hitem (self, (GmpcMetadataBrowserHitem*) self->priv->current->data);
	} else {
		gmpc_metadata_browser_metadata_box_clear (self);
	}
}


static void gmpc_metadata_browser_history_next (GmpcMetadataBrowser* self) {
	gboolean _tmp0_;
	g_return_if_fail (self != NULL);
	_tmp0_ = FALSE;
	if (self->priv->history == NULL) {
		_tmp0_ = TRUE;
	} else {
		_tmp0_ = self->priv->current == NULL;
	}
	if (_tmp0_) {
		return;
	}
	if (self->priv->current->prev == NULL) {
		return;
	}
	self->priv->current = self->priv->current->prev;
	if (self->priv->current != NULL) {
		gmpc_metadata_browser_show_hitem (self, (GmpcMetadataBrowserHitem*) self->priv->current->data);
	} else {
		gmpc_metadata_browser_metadata_box_clear (self);
	}
}


static void gmpc_metadata_browser_history_show_list_clicked (GmpcMetadataBrowser* self, GtkMenuItem* item) {
	GList* a;
	g_return_if_fail (self != NULL);
	g_return_if_fail (item != NULL);
	a = (GList*) g_object_get_data ((GObject*) item, "current");
	if (a != NULL) {
		self->priv->current = a;
		gmpc_metadata_browser_show_hitem (self, (GmpcMetadataBrowserHitem*) self->priv->current->data);
	}
}


static gpointer _gmpc_metadata_browser_hitem_dup0 (gpointer self) {
	return self ? gmpc_metadata_browser_hitem_dup (self) : NULL;
}


static void _gmpc_metadata_browser_history_show_list_clicked_gtk_menu_item_activate (GtkMenuItem* _sender, gpointer self) {
	gmpc_metadata_browser_history_show_list_clicked (self, _sender);
}


static void gmpc_metadata_browser_history_show_list (GmpcMetadataBrowser* self) {
	GtkMenu* menu;
	GList* iter;
	g_return_if_fail (self != NULL);
	menu = g_object_ref_sink ((GtkMenu*) gtk_menu_new ());
	iter = g_list_last (self->priv->history);
	while (TRUE) {
		GmpcMetadataBrowserHitem* i;
		char* label;
		GtkCheckMenuItem* item;
		gboolean _tmp4_;
		if (!(iter != NULL)) {
			break;
		}
		i = _gmpc_metadata_browser_hitem_dup0 ((GmpcMetadataBrowserHitem*) iter->data);
		label = g_strdup ("");
		if ((*i).type == GMPC_METADATA_BROWSER_HITEM_TYPE_ARTIST) {
			char* _tmp0_;
			label = (_tmp0_ = g_strdup ((*i).song->artist), _g_free0 (label), _tmp0_);
		} else {
			if ((*i).type == GMPC_METADATA_BROWSER_HITEM_TYPE_ALBUM) {
				char* _tmp1_;
				label = (_tmp1_ = g_strdup_printf ("%s - %s", (*i).song->artist, (*i).song->album), _g_free0 (label), _tmp1_);
			} else {
				if ((*i).type == GMPC_METADATA_BROWSER_HITEM_TYPE_SONG) {
					if ((*i).song->title != NULL) {
						char* _tmp2_;
						label = (_tmp2_ = g_strdup ((*i).song->title), _g_free0 (label), _tmp2_);
					} else {
						char* _tmp3_;
						label = (_tmp3_ = g_strdup (_ ("Unknown")), _g_free0 (label), _tmp3_);
					}
				}
			}
		}
		item = g_object_ref_sink ((GtkCheckMenuItem*) gtk_check_menu_item_new_with_label (label));
		gtk_check_menu_item_set_draw_as_radio (item, TRUE);
		_tmp4_ = FALSE;
		if (self->priv->current != NULL) {
			_tmp4_ = self->priv->current == iter;
		} else {
			_tmp4_ = FALSE;
		}
		if (_tmp4_) {
			gtk_check_menu_item_set_active (item, TRUE);
		}
		g_signal_connect_object ((GtkMenuItem*) item, "activate", (GCallback) _gmpc_metadata_browser_history_show_list_clicked_gtk_menu_item_activate, self, 0);
		g_object_set_data ((GObject*) item, "current", (void*) iter);
		gtk_menu_shell_append ((GtkMenuShell*) menu, (GtkWidget*) ((GtkMenuItem*) item));
		iter = iter->prev;
		_gmpc_metadata_browser_hitem_free0 (i);
		_g_free0 (label);
		_g_object_unref0 (item);
	}
	gtk_widget_show_all ((GtkWidget*) menu);
	gtk_menu_popup (menu, NULL, NULL, NULL, NULL, (guint) 0, gtk_get_current_event_time ());
	_g_object_unref0 (menu);
}


static void _gmpc_metadata_browser_history_next_gtk_button_clicked (GtkButton* _sender, gpointer self) {
	gmpc_metadata_browser_history_next (self);
}


static void _gmpc_metadata_browser_history_show_list_gtk_button_clicked (GtkButton* _sender, gpointer self) {
	gmpc_metadata_browser_history_show_list (self);
}


static void _gmpc_metadata_browser_history_previous_gtk_button_clicked (GtkButton* _sender, gpointer self) {
	gmpc_metadata_browser_history_previous (self);
}


static GtkHBox* gmpc_metadata_browser_history_buttons (GmpcMetadataBrowser* self) {
	GtkHBox* result;
	GtkHBox* box;
	gboolean _tmp0_;
	GtkButton* next_but;
	gboolean _tmp1_;
	gboolean _tmp2_;
	GtkButton* back_but;
	gboolean _tmp4_;
	g_return_val_if_fail (self != NULL, NULL);
	box = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 0));
	_tmp0_ = FALSE;
	if (self->priv->history == NULL) {
		_tmp0_ = self->priv->current == NULL;
	} else {
		_tmp0_ = FALSE;
	}
	if (_tmp0_) {
		result = box;
		return result;
	}
	next_but = g_object_ref_sink ((GtkButton*) gtk_button_new_from_stock ("gtk-go-forward"));
	_tmp1_ = FALSE;
	if (self->priv->current == NULL) {
		_tmp1_ = TRUE;
	} else {
		_tmp1_ = self->priv->current->prev == NULL;
	}
	if (_tmp1_) {
		g_object_set ((GtkWidget*) next_but, "sensitive", FALSE, NULL);
	}
	g_signal_connect_object (next_but, "clicked", (GCallback) _gmpc_metadata_browser_history_next_gtk_button_clicked, self, 0);
	gtk_box_pack_end ((GtkBox*) box, (GtkWidget*) next_but, FALSE, FALSE, (guint) 0);
	_tmp2_ = FALSE;
	if (self->priv->current != NULL) {
		gboolean _tmp3_;
		_tmp3_ = FALSE;
		if (self->priv->current->next != NULL) {
			_tmp3_ = TRUE;
		} else {
			_tmp3_ = self->priv->current->prev != NULL;
		}
		_tmp2_ = _tmp3_;
	} else {
		_tmp2_ = FALSE;
	}
	if (_tmp2_) {
		GtkButton* dd_but;
		dd_but = g_object_ref_sink ((GtkButton*) gtk_button_new_with_label ("L"));
		g_signal_connect_object (dd_but, "clicked", (GCallback) _gmpc_metadata_browser_history_show_list_gtk_button_clicked, self, 0);
		gtk_box_pack_end ((GtkBox*) box, (GtkWidget*) dd_but, FALSE, FALSE, (guint) 0);
		_g_object_unref0 (dd_but);
	}
	back_but = g_object_ref_sink ((GtkButton*) gtk_button_new_from_stock ("gtk-go-back"));
	_tmp4_ = FALSE;
	if (self->priv->current == NULL) {
		_tmp4_ = TRUE;
	} else {
		_tmp4_ = self->priv->current->next == NULL;
	}
	if (_tmp4_) {
		g_object_set ((GtkWidget*) back_but, "sensitive", FALSE, NULL);
	}
	g_signal_connect_object (back_but, "clicked", (GCallback) _gmpc_metadata_browser_history_previous_gtk_button_clicked, self, 0);
	gtk_box_pack_end ((GtkBox*) box, (GtkWidget*) back_but, FALSE, FALSE, (guint) 0);
	result = box;
	_g_object_unref0 (next_but);
	_g_object_unref0 (back_but);
	return result;
}


static void gmpc_metadata_browser_history_add (GmpcMetadataBrowser* self, const GmpcMetadataBrowserHitem* hi) {
	g_return_if_fail (self != NULL);
	if (self->priv->history != NULL) {
		GmpcMetadataBrowserHitem a;
		a = *((GmpcMetadataBrowserHitem*) self->priv->current->data);
		if (a.type == (*hi).type) {
			char* _tmp1_;
			char* _tmp0_;
			gboolean _tmp2_;
			if ((_tmp2_ = _vala_strcmp0 (_tmp0_ = mpd_song_checksum (a.song), _tmp1_ = mpd_song_checksum ((*hi).song)) == 0, _g_free0 (_tmp1_), _g_free0 (_tmp0_), _tmp2_)) {
				return;
			}
		}
	}
	self->priv->history = g_list_prepend (self->priv->history, _gmpc_metadata_browser_hitem_dup0 (hi));
	if (g_list_length (self->priv->history) > 25) {
		GList* a;
		a = g_list_last (self->priv->history);
		self->priv->history = g_list_remove (self->priv->history, (GmpcMetadataBrowserHitem*) a->data);
	}
	self->priv->current = self->priv->history;
}


static void gmpc_metadata_browser_history_clear (GmpcMetadataBrowser* self) {
	GList* _tmp0_;
	g_return_if_fail (self != NULL);
	self->priv->current = NULL;
	self->priv->history = (_tmp0_ = NULL, __g_list_free_gmpc_metadata_browser_hitem_free0 (self->priv->history), _tmp0_);
}


void gmpc_metadata_browser_set_artist (GmpcMetadataBrowser* self, const char* artist) {
	GtkTreeIter iter = {0};
	g_return_if_fail (self != NULL);
	g_return_if_fail (artist != NULL);
	if (!gmpc_plugin_base_get_enabled ((GmpcPluginBase*) self)) {
		return;
	}
	self->priv->block_update++;
	gtk_tree_selection_unselect_all (gtk_tree_view_get_selection (self->priv->tree_artist));
	gtk_tree_selection_unselect_all (gtk_tree_view_get_selection (self->priv->tree_album));
	gtk_entry_set_text (self->priv->artist_filter_entry, "");
	if (gtk_tree_model_get_iter_first ((GtkTreeModel*) self->priv->model_filter_artist, &iter)) {
		{
			gboolean _tmp0_;
			_tmp0_ = TRUE;
			while (TRUE) {
				char* lartist;
				gboolean _tmp1_;
				if (!_tmp0_) {
					if (!gtk_tree_model_iter_next ((GtkTreeModel*) self->priv->model_filter_artist, &iter)) {
						break;
					}
				}
				_tmp0_ = FALSE;
				lartist = NULL;
				gtk_tree_model_get ((GtkTreeModel*) self->priv->model_filter_artist, &iter, 7, &lartist, -1, -1);
				_tmp1_ = FALSE;
				if (lartist != NULL) {
					_tmp1_ = g_utf8_collate (lartist, artist) == 0;
				} else {
					_tmp1_ = FALSE;
				}
				if (_tmp1_) {
					GtkTreePath* _tmp2_;
					gtk_tree_selection_select_iter (gtk_tree_view_get_selection (self->priv->tree_artist), &iter);
					gtk_tree_view_scroll_to_cell (self->priv->tree_artist, _tmp2_ = gtk_tree_model_get_path ((GtkTreeModel*) self->priv->model_filter_artist, &iter), NULL, TRUE, 0.5f, 0.f);
					_gtk_tree_path_free0 (_tmp2_);
					self->priv->block_update--;
					gmpc_metadata_browser_metadata_box_clear (self);
					gmpc_metadata_browser_metadata_box_update (self);
					_g_free0 (lartist);
					return;
				}
				_g_free0 (lartist);
			}
		}
	}
	self->priv->block_update--;
	gmpc_metadata_browser_metadata_box_clear (self);
	gmpc_metadata_browser_metadata_box_update (self);
}


void gmpc_metadata_browser_set_album (GmpcMetadataBrowser* self, const char* artist, const char* album) {
	GtkTreeIter iter = {0};
	g_return_if_fail (self != NULL);
	g_return_if_fail (artist != NULL);
	g_return_if_fail (album != NULL);
	if (!gmpc_plugin_base_get_enabled ((GmpcPluginBase*) self)) {
		return;
	}
	self->priv->block_update++;
	gmpc_metadata_browser_set_artist (self, artist);
	gtk_entry_set_text (self->priv->album_filter_entry, "");
	if (gtk_tree_model_get_iter_first ((GtkTreeModel*) self->priv->model_filter_album, &iter)) {
		{
			gboolean _tmp0_;
			_tmp0_ = TRUE;
			while (TRUE) {
				char* lalbum;
				gboolean _tmp1_;
				if (!_tmp0_) {
					if (!gtk_tree_model_iter_next ((GtkTreeModel*) self->priv->model_filter_album, &iter)) {
						break;
					}
				}
				_tmp0_ = FALSE;
				lalbum = NULL;
				gtk_tree_model_get ((GtkTreeModel*) self->priv->model_filter_album, &iter, 6, &lalbum, -1, -1);
				_tmp1_ = FALSE;
				if (lalbum != NULL) {
					_tmp1_ = g_utf8_collate (lalbum, album) == 0;
				} else {
					_tmp1_ = FALSE;
				}
				if (_tmp1_) {
					GtkTreePath* _tmp2_;
					gtk_tree_selection_select_iter (gtk_tree_view_get_selection (self->priv->tree_album), &iter);
					gtk_tree_view_scroll_to_cell (self->priv->tree_album, _tmp2_ = gtk_tree_model_get_path ((GtkTreeModel*) self->priv->model_filter_album, &iter), NULL, TRUE, 0.5f, 0.f);
					_gtk_tree_path_free0 (_tmp2_);
					gtk_tree_selection_unselect_all (gtk_tree_view_get_selection (self->priv->tree_songs));
					self->priv->block_update--;
					gmpc_metadata_browser_metadata_box_update (self);
					_g_free0 (lalbum);
					return;
				}
				_g_free0 (lalbum);
			}
		}
	}
	gtk_tree_selection_unselect_all (gtk_tree_view_get_selection (self->priv->tree_songs));
	self->priv->block_update--;
	gmpc_metadata_browser_metadata_box_clear (self);
	gmpc_metadata_browser_metadata_box_update (self);
}


void gmpc_metadata_browser_set_song (GmpcMetadataBrowser* self, const mpd_Song* song) {
	GtkTreeIter iter = {0};
	GmpcMetadataBrowserHitem _tmp3_ = {0};
	GmpcMetadataBrowserHitem item;
	mpd_Song* _tmp4_;
	GtkWidget* view;
	g_return_if_fail (self != NULL);
	g_return_if_fail (song != NULL);
	if (!gmpc_plugin_base_get_enabled ((GmpcPluginBase*) self)) {
		return;
	}
	self->priv->block_update++;
	if (song->artist != NULL) {
		gmpc_metadata_browser_set_artist (self, song->artist);
		if (song->album != NULL) {
			gmpc_metadata_browser_set_album (self, song->artist, song->album);
		}
	}
	if (gtk_tree_model_get_iter_first ((GtkTreeModel*) self->priv->model_songs, &iter)) {
		{
			gboolean _tmp0_;
			_tmp0_ = TRUE;
			while (TRUE) {
				char* ltitle;
				gboolean _tmp1_;
				if (!_tmp0_) {
					if (!gtk_tree_model_iter_next ((GtkTreeModel*) self->priv->model_songs, &iter)) {
						break;
					}
				}
				_tmp0_ = FALSE;
				ltitle = NULL;
				gtk_tree_model_get ((GtkTreeModel*) self->priv->model_songs, &iter, 7, &ltitle, -1, -1);
				_tmp1_ = FALSE;
				if (ltitle != NULL) {
					_tmp1_ = g_utf8_collate (ltitle, song->title) == 0;
				} else {
					_tmp1_ = FALSE;
				}
				if (_tmp1_) {
					GtkTreePath* _tmp2_;
					gtk_tree_selection_select_iter (gtk_tree_view_get_selection (self->priv->tree_songs), &iter);
					gtk_tree_view_scroll_to_cell (self->priv->tree_songs, _tmp2_ = gtk_tree_model_get_path ((GtkTreeModel*) self->priv->model_songs, &iter), NULL, TRUE, 0.5f, 0.f);
					_gtk_tree_path_free0 (_tmp2_);
					self->priv->block_update--;
					gmpc_metadata_browser_metadata_box_update (self);
					_g_free0 (ltitle);
					return;
				}
				_g_free0 (ltitle);
			}
		}
	}
	self->priv->block_update--;
	gmpc_metadata_browser_metadata_box_clear (self);
	if (self->priv->update_timeout > 0) {
		g_source_remove (self->priv->update_timeout);
		self->priv->update_timeout = (guint) 0;
	}
	item = (memset (&_tmp3_, 0, sizeof (GmpcMetadataBrowserHitem)), _tmp3_);
	item.song = (_tmp4_ = _mpd_songDup0 (song), _mpd_freeSong0 (item.song), _tmp4_);
	item.type = GMPC_METADATA_BROWSER_HITEM_TYPE_SONG;
	gmpc_metadata_browser_history_add (self, &item);
	view = gmpc_metadata_browser_metadata_box_show_song (self, song, TRUE);
	gtk_container_add ((GtkContainer*) self->priv->metadata_box, view);
	gtk_widget_show_all ((GtkWidget*) self->priv->metadata_box);
	gmpc_metadata_browser_hitem_destroy (&item);
	_g_object_unref0 (view);
}


void gmpc_metadata_browser_select_browser (GmpcMetadataBrowser* self, GtkTreeView* tree) {
	g_return_if_fail (self != NULL);
	if (self->priv->rref != NULL) {
		GtkTreeView* category_tree;
		GtkTreeSelection* sel;
		GtkTreePath* path;
		category_tree = playlist3_get_category_tree_view ();
		sel = _g_object_ref0 (gtk_tree_view_get_selection (category_tree));
		path = gtk_tree_row_reference_get_path (self->priv->rref);
		if (path != NULL) {
			gtk_tree_selection_select_path (sel, path);
		}
		_g_object_unref0 (sel);
		_gtk_tree_path_free0 (path);
	}
}


static void _lambda0_ (GtkToggleButton* source, GmpcMetadataBrowser* self) {
	g_return_if_fail (source != NULL);
	cfg_set_single_value_as_int (config, "MetaData", "show-artist-information", (gint) gtk_toggle_button_get_active (source));
}


static void __lambda0__gtk_toggle_button_toggled (GtkToggleButton* _sender, gpointer self) {
	_lambda0_ (_sender, self);
}


static void _lambda1_ (GtkToggleButton* source, GmpcMetadataBrowser* self) {
	g_return_if_fail (source != NULL);
	cfg_set_single_value_as_int (config, "MetaData", "show-album-information", (gint) gtk_toggle_button_get_active (source));
}


static void __lambda1__gtk_toggle_button_toggled (GtkToggleButton* _sender, gpointer self) {
	_lambda1_ (_sender, self);
}


static void _lambda2_ (GtkToggleButton* source, GmpcMetadataBrowser* self) {
	g_return_if_fail (source != NULL);
	cfg_set_single_value_as_int (config, "MetaData", "show-similar-artist", (gint) gtk_toggle_button_get_active (source));
}


static void __lambda2__gtk_toggle_button_toggled (GtkToggleButton* _sender, gpointer self) {
	_lambda2_ (_sender, self);
}


static void _lambda3_ (GtkToggleButton* source, GmpcMetadataBrowser* self) {
	g_return_if_fail (source != NULL);
	cfg_set_single_value_as_int (config, "MetaData", "show-lyrics", (gint) gtk_toggle_button_get_active (source));
}


static void __lambda3__gtk_toggle_button_toggled (GtkToggleButton* _sender, gpointer self) {
	_lambda3_ (_sender, self);
}


static void _lambda4_ (GtkToggleButton* source, GmpcMetadataBrowser* self) {
	g_return_if_fail (source != NULL);
	cfg_set_single_value_as_int (config, "MetaData", "show-guitar-tabs", (gint) gtk_toggle_button_get_active (source));
}


static void __lambda4__gtk_toggle_button_toggled (GtkToggleButton* _sender, gpointer self) {
	_lambda4_ (_sender, self);
}


static void _lambda5_ (GtkToggleButton* source, GmpcMetadataBrowser* self) {
	g_return_if_fail (source != NULL);
	cfg_set_single_value_as_int (config, "MetaData", "show-similar-songs", (gint) gtk_toggle_button_get_active (source));
}


static void __lambda5__gtk_toggle_button_toggled (GtkToggleButton* _sender, gpointer self) {
	_lambda5_ (_sender, self);
}


static void _lambda6_ (GtkToggleButton* source, GmpcMetadataBrowser* self) {
	g_return_if_fail (source != NULL);
	cfg_set_single_value_as_int (config, "MetaData", "show-web-links", (gint) gtk_toggle_button_get_active (source));
}


static void __lambda6__gtk_toggle_button_toggled (GtkToggleButton* _sender, gpointer self) {
	_lambda6_ (_sender, self);
}


static void gmpc_metadata_browser_real_preferences_pane_construct (GmpcPluginPreferencesIface* base, GtkContainer* container) {
	GmpcMetadataBrowser * self;
	GtkVBox* box;
	GtkLabel* label;
	GtkCheckButton* chk;
	GtkCheckButton* _tmp0_;
	GtkCheckButton* _tmp1_;
	GtkCheckButton* _tmp2_;
	GtkCheckButton* _tmp3_;
	GtkCheckButton* _tmp4_;
	GtkCheckButton* _tmp5_;
	self = (GmpcMetadataBrowser*) base;
	g_return_if_fail (container != NULL);
	box = g_object_ref_sink ((GtkVBox*) gtk_vbox_new (FALSE, 6));
	label = g_object_ref_sink ((GtkLabel*) gtk_label_new (_ ("Enable/disable metadata options")));
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
	gtk_box_pack_start ((GtkBox*) box, (GtkWidget*) label, FALSE, FALSE, (guint) 0);
	chk = g_object_ref_sink ((GtkCheckButton*) gtk_check_button_new_with_label (_ ("Artist information")));
	gtk_toggle_button_set_active ((GtkToggleButton*) chk, cfg_get_single_value_as_int_with_default (config, "MetaData", "show-artist-information", 1) == 1);
	gtk_box_pack_start ((GtkBox*) box, (GtkWidget*) chk, FALSE, FALSE, (guint) 0);
	g_signal_connect_object ((GtkToggleButton*) chk, "toggled", (GCallback) __lambda0__gtk_toggle_button_toggled, self, 0);
	chk = (_tmp0_ = g_object_ref_sink ((GtkCheckButton*) gtk_check_button_new_with_label (_ ("Album information"))), _g_object_unref0 (chk), _tmp0_);
	gtk_toggle_button_set_active ((GtkToggleButton*) chk, cfg_get_single_value_as_int_with_default (config, "MetaData", "show-album-information", 1) == 1);
	gtk_box_pack_start ((GtkBox*) box, (GtkWidget*) chk, FALSE, FALSE, (guint) 0);
	g_signal_connect_object ((GtkToggleButton*) chk, "toggled", (GCallback) __lambda1__gtk_toggle_button_toggled, self, 0);
	chk = (_tmp1_ = g_object_ref_sink ((GtkCheckButton*) gtk_check_button_new_with_label (_ ("Similar Artist"))), _g_object_unref0 (chk), _tmp1_);
	gtk_toggle_button_set_active ((GtkToggleButton*) chk, cfg_get_single_value_as_int_with_default (config, "MetaData", "show-similar-artist", 1) == 1);
	gtk_box_pack_start ((GtkBox*) box, (GtkWidget*) chk, FALSE, FALSE, (guint) 0);
	g_signal_connect_object ((GtkToggleButton*) chk, "toggled", (GCallback) __lambda2__gtk_toggle_button_toggled, self, 0);
	chk = (_tmp2_ = g_object_ref_sink ((GtkCheckButton*) gtk_check_button_new_with_label (_ ("Lyrics"))), _g_object_unref0 (chk), _tmp2_);
	gtk_toggle_button_set_active ((GtkToggleButton*) chk, cfg_get_single_value_as_int_with_default (config, "MetaData", "show-lyrics", 1) == 1);
	gtk_box_pack_start ((GtkBox*) box, (GtkWidget*) chk, FALSE, FALSE, (guint) 0);
	g_signal_connect_object ((GtkToggleButton*) chk, "toggled", (GCallback) __lambda3__gtk_toggle_button_toggled, self, 0);
	chk = (_tmp3_ = g_object_ref_sink ((GtkCheckButton*) gtk_check_button_new_with_label (_ ("Guitar Tabs"))), _g_object_unref0 (chk), _tmp3_);
	gtk_toggle_button_set_active ((GtkToggleButton*) chk, cfg_get_single_value_as_int_with_default (config, "MetaData", "show-guitar-tabs", 1) == 1);
	gtk_box_pack_start ((GtkBox*) box, (GtkWidget*) chk, FALSE, FALSE, (guint) 0);
	g_signal_connect_object ((GtkToggleButton*) chk, "toggled", (GCallback) __lambda4__gtk_toggle_button_toggled, self, 0);
	chk = (_tmp4_ = g_object_ref_sink ((GtkCheckButton*) gtk_check_button_new_with_label (_ ("Similar Songs"))), _g_object_unref0 (chk), _tmp4_);
	gtk_toggle_button_set_active ((GtkToggleButton*) chk, cfg_get_single_value_as_int_with_default (config, "MetaData", "show-similar-songs", 1) == 1);
	gtk_box_pack_start ((GtkBox*) box, (GtkWidget*) chk, FALSE, FALSE, (guint) 0);
	g_signal_connect_object ((GtkToggleButton*) chk, "toggled", (GCallback) __lambda5__gtk_toggle_button_toggled, self, 0);
	chk = (_tmp5_ = g_object_ref_sink ((GtkCheckButton*) gtk_check_button_new_with_label (_ ("Web links"))), _g_object_unref0 (chk), _tmp5_);
	gtk_toggle_button_set_active ((GtkToggleButton*) chk, cfg_get_single_value_as_int_with_default (config, "MetaData", "show-web-links", 1) == 1);
	gtk_box_pack_start ((GtkBox*) box, (GtkWidget*) chk, FALSE, FALSE, (guint) 0);
	g_signal_connect_object ((GtkToggleButton*) chk, "toggled", (GCallback) __lambda6__gtk_toggle_button_toggled, self, 0);
	gtk_container_add (container, (GtkWidget*) box);
	gtk_widget_show_all ((GtkWidget*) box);
	_g_object_unref0 (box);
	_g_object_unref0 (label);
	_g_object_unref0 (chk);
}


static void gmpc_metadata_browser_real_preferences_pane_destroy (GmpcPluginPreferencesIface* base, GtkContainer* container) {
	GmpcMetadataBrowser * self;
	self = (GmpcMetadataBrowser*) base;
	g_return_if_fail (container != NULL);
	{
		GList* child_collection;
		GList* child_it;
		child_collection = gtk_container_get_children (container);
		for (child_it = child_collection; child_it != NULL; child_it = child_it->next) {
			GtkWidget* child;
			child = _g_object_ref0 ((GtkWidget*) child_it->data);
			{
				gtk_container_remove (container, child);
				_g_object_unref0 (child);
			}
		}
		_g_list_free0 (child_collection);
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
	GObjectClass * parent_class;
	GmpcMetadataBrowser * self;
	parent_class = G_OBJECT_CLASS (gmpc_metadata_browser_parent_class);
	obj = parent_class->constructor (type, n_construct_properties, construct_properties);
	self = GMPC_METADATA_BROWSER (obj);
	{
		((GmpcPluginBase*) self)->plugin_type = 2 | 8;
		g_signal_connect_object (gmpcconn, "connection-changed", (GCallback) _gmpc_metadata_browser_con_changed_gmpc_connection_connection_changed, self, 0);
		g_signal_connect_object (gmpcconn, "status-changed", (GCallback) _gmpc_metadata_browser_status_changed_gmpc_connection_status_changed, self, 0);
	}
	return obj;
}


static void gmpc_metadata_browser_hitem_copy (const GmpcMetadataBrowserHitem* self, GmpcMetadataBrowserHitem* dest) {
	dest->type = self->type;
	dest->song = _mpd_songDup0 (self->song);
}


static void gmpc_metadata_browser_hitem_destroy (GmpcMetadataBrowserHitem* self) {
	_mpd_freeSong0 (self->song);
}


static GmpcMetadataBrowserHitem* gmpc_metadata_browser_hitem_dup (const GmpcMetadataBrowserHitem* self) {
	GmpcMetadataBrowserHitem* dup;
	dup = g_new0 (GmpcMetadataBrowserHitem, 1);
	gmpc_metadata_browser_hitem_copy (self, dup);
	return dup;
}


static void gmpc_metadata_browser_hitem_free (GmpcMetadataBrowserHitem* self) {
	gmpc_metadata_browser_hitem_destroy (self);
	g_free (self);
}


static GType gmpc_metadata_browser_hitem_get_type (void) {
	static GType gmpc_metadata_browser_hitem_type_id = 0;
	if (gmpc_metadata_browser_hitem_type_id == 0) {
		gmpc_metadata_browser_hitem_type_id = g_boxed_type_register_static ("GmpcMetadataBrowserHitem", (GBoxedCopyFunc) gmpc_metadata_browser_hitem_dup, (GBoxedFreeFunc) gmpc_metadata_browser_hitem_free);
	}
	return gmpc_metadata_browser_hitem_type_id;
}


static void gmpc_metadata_browser_class_init (GmpcMetadataBrowserClass * klass) {
	gmpc_metadata_browser_parent_class = g_type_class_peek_parent (klass);
	g_type_class_add_private (klass, sizeof (GmpcMetadataBrowserPrivate));
	GMPC_PLUGIN_BASE_CLASS (klass)->get_version = gmpc_metadata_browser_real_get_version;
	GMPC_PLUGIN_BASE_CLASS (klass)->get_name = gmpc_metadata_browser_real_get_name;
	GMPC_PLUGIN_BASE_CLASS (klass)->save_yourself = gmpc_metadata_browser_real_save_yourself;
	G_OBJECT_CLASS (klass)->constructor = gmpc_metadata_browser_constructor;
	G_OBJECT_CLASS (klass)->finalize = gmpc_metadata_browser_finalize;
}


static void gmpc_metadata_browser_gmpc_plugin_browser_iface_interface_init (GmpcPluginBrowserIfaceIface * iface) {
	gmpc_metadata_browser_gmpc_plugin_browser_iface_parent_iface = g_type_interface_peek_parent (iface);
	iface->browser_add_go_menu = gmpc_metadata_browser_real_browser_add_go_menu;
	iface->browser_add = gmpc_metadata_browser_real_browser_add;
	iface->browser_selected = gmpc_metadata_browser_real_browser_selected;
	iface->browser_unselected = gmpc_metadata_browser_real_browser_unselected;
}


static void gmpc_metadata_browser_gmpc_plugin_preferences_iface_interface_init (GmpcPluginPreferencesIfaceIface * iface) {
	gmpc_metadata_browser_gmpc_plugin_preferences_iface_parent_iface = g_type_interface_peek_parent (iface);
	iface->preferences_pane_construct = gmpc_metadata_browser_real_preferences_pane_construct;
	iface->preferences_pane_destroy = gmpc_metadata_browser_real_preferences_pane_destroy;
}


static void gmpc_metadata_browser_instance_init (GmpcMetadataBrowser * self) {
	self->priv = GMPC_METADATA_BROWSER_GET_PRIVATE (self);
	self->priv->block_update = 0;
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
	self->priv->update_timeout = (guint) 0;
	self->priv->selected = FALSE;
	self->priv->history = NULL;
	self->priv->current = NULL;
}


static void gmpc_metadata_browser_finalize (GObject* obj) {
	GmpcMetadataBrowser * self;
	self = GMPC_METADATA_BROWSER (obj);
	_gtk_tree_row_reference_free0 (self->priv->rref);
	_g_object_unref0 (self->priv->paned);
	_g_object_unref0 (self->priv->browser_box);
	_g_object_unref0 (self->priv->tree_artist);
	_g_object_unref0 (self->priv->model_artist);
	_g_object_unref0 (self->priv->model_filter_artist);
	_g_object_unref0 (self->priv->artist_filter_entry);
	_g_object_unref0 (self->priv->tree_album);
	_g_object_unref0 (self->priv->model_albums);
	_g_object_unref0 (self->priv->model_filter_album);
	_g_object_unref0 (self->priv->album_filter_entry);
	_g_object_unref0 (self->priv->tree_songs);
	_g_object_unref0 (self->priv->model_songs);
	_g_object_unref0 (self->priv->metadata_sw);
	_g_object_unref0 (self->priv->metadata_box);
	__g_list_free_gmpc_metadata_browser_hitem_free0 (self->priv->history);
	G_OBJECT_CLASS (gmpc_metadata_browser_parent_class)->finalize (obj);
}


GType gmpc_metadata_browser_get_type (void) {
	static GType gmpc_metadata_browser_type_id = 0;
	if (gmpc_metadata_browser_type_id == 0) {
		static const GTypeInfo g_define_type_info = { sizeof (GmpcMetadataBrowserClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) gmpc_metadata_browser_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (GmpcMetadataBrowser), 0, (GInstanceInitFunc) gmpc_metadata_browser_instance_init, NULL };
		static const GInterfaceInfo gmpc_plugin_browser_iface_info = { (GInterfaceInitFunc) gmpc_metadata_browser_gmpc_plugin_browser_iface_interface_init, (GInterfaceFinalizeFunc) NULL, NULL};
		static const GInterfaceInfo gmpc_plugin_preferences_iface_info = { (GInterfaceInitFunc) gmpc_metadata_browser_gmpc_plugin_preferences_iface_interface_init, (GInterfaceFinalizeFunc) NULL, NULL};
		gmpc_metadata_browser_type_id = g_type_register_static (GMPC_PLUGIN_TYPE_BASE, "GmpcMetadataBrowser", &g_define_type_info, 0);
		g_type_add_interface_static (gmpc_metadata_browser_type_id, GMPC_PLUGIN_TYPE_BROWSER_IFACE, &gmpc_plugin_browser_iface_info);
		g_type_add_interface_static (gmpc_metadata_browser_type_id, GMPC_PLUGIN_TYPE_PREFERENCES_IFACE, &gmpc_plugin_preferences_iface_info);
	}
	return gmpc_metadata_browser_type_id;
}


static void _vala_array_destroy (gpointer array, gint array_length, GDestroyNotify destroy_func) {
	if ((array != NULL) && (destroy_func != NULL)) {
		int i;
		for (i = 0; i < array_length; i = i + 1) {
			if (((gpointer*) array)[i] != NULL) {
				destroy_func (((gpointer*) array)[i]);
			}
		}
	}
}


static void _vala_array_free (gpointer array, gint array_length, GDestroyNotify destroy_func) {
	_vala_array_destroy (array, array_length, destroy_func);
	g_free (array);
}


static gint _vala_array_length (gpointer array) {
	int length;
	length = 0;
	if (array) {
		while (((gpointer*) array)[length]) {
			length++;
		}
	}
	return length;
}


static int _vala_strcmp0 (const char * str1, const char * str2) {
	if (str1 == NULL) {
		return -(str1 != str2);
	}
	if (str2 == NULL) {
		return str1 != str2;
	}
	return strcmp (str1, str2);
}




