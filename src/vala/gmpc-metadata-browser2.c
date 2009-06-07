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
#include <main.h>
#include <gdk/gdk.h>
#include <gmpc-mpddata-model.h>
#include <gmpc-meta-watcher.h>
#include <gmpc-plugin.h>
#include <misc.h>
#include <gmpc-metaimage.h>
#include <pango/pango.h>
#include <plugin.h>
#include <config1.h>
#include <gmpc-connection.h>
#include <float.h>
#include <math.h>
#include <gdk-pixbuf/gdk-pixdata.h>
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
typedef struct _GmpcMetadataBrowserPrivate GmpcMetadataBrowserPrivate;

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
};

/**
 * Now playing uses the MetaDataBrowser plugin to "plot" the view */
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
};



#define use_transition_mb TRUE
#define some_unique_name_mb VERSION
GType gmpc_widget_similar_songs_get_type (void);
#define GMPC_WIDGET_SIMILAR_SONGS_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), GMPC_WIDGET_TYPE_SIMILAR_SONGS, GmpcWidgetSimilarSongsPrivate))
enum  {
	GMPC_WIDGET_SIMILAR_SONGS_DUMMY_PROPERTY
};
static GmpcWidgetSimilarSongs* gmpc_widget_similar_songs_new (const mpd_Song* song);
static GmpcWidgetSimilarSongs* gmpc_widget_similar_songs_construct (GType object_type, const mpd_Song* song);
static GmpcWidgetSimilarSongs* gmpc_widget_similar_songs_new (const mpd_Song* song);
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
static void gmpc_widget_similar_songs_metadata_changed (GmpcWidgetSimilarSongs* self, GmpcMetaWatcher* gmw, const mpd_Song* song, MetaDataType type, MetaDataResult _result_, const MetaData* met);
static void _gmpc_widget_similar_songs_metadata_changed_gmpc_meta_watcher_data_changed (GmpcMetaWatcher* _sender, const mpd_Song* song, MetaDataType type, MetaDataResult _result_, const MetaData* met, gpointer self);
static void gmpc_widget_similar_songs_update (GmpcWidgetSimilarSongs* self);
static void gmpc_widget_similar_songs_real_activate (GtkExpander* base);
static gpointer gmpc_widget_similar_songs_parent_class = NULL;
static void gmpc_widget_similar_songs_finalize (GObject* obj);
GType gmpc_widget_similar_artist_get_type (void);
GType gmpc_metadata_browser_get_type (void);
#define GMPC_WIDGET_SIMILAR_ARTIST_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), GMPC_WIDGET_TYPE_SIMILAR_ARTIST, GmpcWidgetSimilarArtistPrivate))
enum  {
	GMPC_WIDGET_SIMILAR_ARTIST_DUMMY_PROPERTY
};
GtkWidget* gmpc_widget_similar_artist_new_artist_button (GmpcWidgetSimilarArtist* self, const char* artist, gboolean in_db);
static void _g_list_free_g_object_unref (GList* self);
static void gmpc_widget_similar_artist_metadata_changed (GmpcWidgetSimilarArtist* self, GmpcMetaWatcher* gmw, const mpd_Song* song, MetaDataType type, MetaDataResult _result_, const MetaData* met);
void gmpc_metadata_browser_set_artist (GmpcMetadataBrowser* self, const char* artist);
static void gmpc_widget_similar_artist_artist_button_clicked (GmpcWidgetSimilarArtist* self, GtkButton* button);
static gboolean _misc_header_expose_event_gtk_widget_expose_event (GtkEventBox* _sender, const GdkEventExpose* event, gpointer self);
static void _gmpc_widget_similar_artist_artist_button_clicked_gtk_button_clicked (GtkButton* _sender, gpointer self);
static void _gmpc_widget_similar_artist_metadata_changed_gmpc_meta_watcher_data_changed (GmpcMetaWatcher* _sender, const mpd_Song* song, MetaDataType type, MetaDataResult _result_, const MetaData* met, gpointer self);
static GmpcWidgetSimilarArtist* gmpc_widget_similar_artist_new (GmpcMetadataBrowser* browser, MpdObj* server, const mpd_Song* song);
static GmpcWidgetSimilarArtist* gmpc_widget_similar_artist_construct (GType object_type, GmpcMetadataBrowser* browser, MpdObj* server, const mpd_Song* song);
static GmpcWidgetSimilarArtist* gmpc_widget_similar_artist_new (GmpcMetadataBrowser* browser, MpdObj* server, const mpd_Song* song);
static gpointer gmpc_widget_similar_artist_parent_class = NULL;
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
static GmpcWidgetMore* gmpc_widget_more_new (const char* markup, GtkWidget* child);
static GmpcWidgetMore* gmpc_widget_more_construct (GType object_type, const char* markup, GtkWidget* child);
static GmpcWidgetMore* gmpc_widget_more_new (const char* markup, GtkWidget* child);
static gpointer gmpc_widget_more_parent_class = NULL;
static void gmpc_widget_more_finalize (GObject* obj);
GType gmpc_now_playing_get_type (void);
#define GMPC_NOW_PLAYING_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), GMPC_TYPE_NOW_PLAYING, GmpcNowPlayingPrivate))
enum  {
	GMPC_NOW_PLAYING_DUMMY_PROPERTY
};
static gint gmpc_now_playing_real_get_version (GmpcPluginBase* base, int* result_length1);
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
GtkWidget* gmpc_metadata_browser_metadata_box_show_song (GmpcMetadataBrowser* self, const mpd_Song* song);
static void gmpc_now_playing_select_now_playing_browser (GmpcNowPlaying* self, GtkImageMenuItem* item);
static void _gmpc_now_playing_select_now_playing_browser_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self);
static gint gmpc_now_playing_real_browser_add_go_menu (GmpcPluginBrowserIface* base, GtkMenu* menu);
GmpcNowPlaying* gmpc_now_playing_new (void);
GmpcNowPlaying* gmpc_now_playing_construct (GType object_type);
GmpcNowPlaying* gmpc_now_playing_new (void);
static void _gmpc_now_playing_status_changed_gmpc_connection_status_changed (GmpcConnection* _sender, MpdObj* server, ChangedStatusType what, gpointer self);
GmpcMetadataBrowser* gmpc_metadata_browser_new (void);
GmpcMetadataBrowser* gmpc_metadata_browser_construct (GType object_type);
static GObject * gmpc_now_playing_constructor (GType type, guint n_construct_properties, GObjectConstructParam * construct_properties);
static gpointer gmpc_now_playing_parent_class = NULL;
static GmpcPluginBrowserIfaceIface* gmpc_now_playing_gmpc_plugin_browser_iface_parent_iface = NULL;
static void gmpc_now_playing_finalize (GObject* obj);
#define GMPC_METADATA_BROWSER_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), GMPC_TYPE_METADATA_BROWSER, GmpcMetadataBrowserPrivate))
enum  {
	GMPC_METADATA_BROWSER_DUMMY_PROPERTY
};
static gint gmpc_metadata_browser_real_get_version (GmpcPluginBase* base, int* result_length1);
static const char* gmpc_metadata_browser_real_get_name (GmpcPluginBase* base);
static void gmpc_metadata_browser_real_save_yourself (GmpcPluginBase* base);
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
static void gmpc_metadata_browser_reload_browsers (GmpcMetadataBrowser* self);
static void gmpc_metadata_browser_browser_init (GmpcMetadataBrowser* self);
static void gmpc_metadata_browser_metadata_box_clear (GmpcMetadataBrowser* self);
static void gmpc_metadata_browser_metadata_box_update (GmpcMetadataBrowser* self);
static void gmpc_metadata_browser_play_song (GmpcMetadataBrowser* self, GtkButton* button);
static void gmpc_metadata_browser_add_song (GmpcMetadataBrowser* self, GtkButton* button);
static void gmpc_metadata_browser_replace_song (GmpcMetadataBrowser* self, GtkButton* button);
static void gmpc_metadata_browser_add_selected_song (GmpcMetadataBrowser* self, GtkButton* button);
static void gmpc_metadata_browser_replace_selected_song (GmpcMetadataBrowser* self, GtkButton* button);
static void gmpc_metadata_browser_metadata_find_query (GmpcMetadataBrowser* self, GtkButton* button);
static void _gmpc_metadata_browser_metadata_find_query_gtk_button_clicked (GtkButton* _sender, gpointer self);
static void gmpc_metadata_browser_metadata_button_open_file_browser_path (GmpcMetadataBrowser* self, GtkButton* button);
static void _gmpc_metadata_browser_metadata_button_open_file_browser_path_gtk_button_clicked (GtkButton* _sender, gpointer self);
static void _gmpc_metadata_browser_play_song_gtk_button_clicked (GtkButton* _sender, gpointer self);
static void _gmpc_metadata_browser_add_song_gtk_button_clicked (GtkButton* _sender, gpointer self);
static void _gmpc_metadata_browser_replace_song_gtk_button_clicked (GtkButton* _sender, gpointer self);
static void gmpc_metadata_browser_album_song_tree_row_activated (GmpcMetadataBrowser* self, GtkTreeView* tree, const GtkTreePath* path, GtkTreeViewColumn* column);
static void gmpc_metadata_browser_album_song_browser_add_clicked (GmpcMetadataBrowser* self, GtkImageMenuItem* item);
static void gmpc_metadata_browser_album_song_browser_replace_clicked (GmpcMetadataBrowser* self, GtkImageMenuItem* item);
static void _gmpc_metadata_browser_album_song_browser_add_clicked_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self);
static void _gmpc_metadata_browser_album_song_browser_replace_clicked_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self);
static gboolean gmpc_metadata_browser_album_song_tree_button_press_event (GmpcMetadataBrowser* self, GtkTreeView* tree, const GdkEventButton* event);
static void _gmpc_metadata_browser_add_selected_song_gtk_button_clicked (GtkButton* _sender, gpointer self);
static void _gmpc_metadata_browser_replace_selected_song_gtk_button_clicked (GtkButton* _sender, gpointer self);
static gboolean _gmpc_metadata_browser_album_song_tree_button_press_event_gtk_widget_button_press_event (GmpcMpdDataTreeview* _sender, const GdkEventButton* event, gpointer self);
static void _gmpc_metadata_browser_album_song_tree_row_activated_gtk_tree_view_row_activated (GmpcMpdDataTreeview* _sender, const GtkTreePath* path, GtkTreeViewColumn* column, gpointer self);
static void gmpc_metadata_browser_metadata_box_show_album (GmpcMetadataBrowser* self, const char* artist, const char* album);
static void gmpc_metadata_browser_metadata_box_show_artist (GmpcMetadataBrowser* self, const char* artist);
static gboolean gmpc_metadata_browser_metadata_box_update_real (GmpcMetadataBrowser* self);
static gboolean _gmpc_metadata_browser_metadata_box_update_real_gsource_func (gpointer self);
static void gmpc_metadata_browser_real_browser_add (GmpcPluginBrowserIface* base, GtkWidget* category_tree);
static void gmpc_metadata_browser_real_browser_selected (GmpcPluginBrowserIface* base, GtkContainer* container);
static void gmpc_metadata_browser_real_browser_unselected (GmpcPluginBrowserIface* base, GtkContainer* container);
static void gmpc_metadata_browser_con_changed (GmpcMetadataBrowser* self, GmpcConnection* conn, MpdObj* server, gint connect);
static void gmpc_metadata_browser_status_changed (GmpcMetadataBrowser* self, GmpcConnection* conn, MpdObj* server, ChangedStatusType what);
void gmpc_metadata_browser_set_album (GmpcMetadataBrowser* self, const char* artist, const char* album);
void gmpc_metadata_browser_set_song (GmpcMetadataBrowser* self, const mpd_Song* song);
void gmpc_metadata_browser_select_browser (GmpcMetadataBrowser* self, GtkTreeView* tree);
GmpcMetadataBrowser* gmpc_metadata_browser_new (void);
static void _gmpc_metadata_browser_con_changed_gmpc_connection_connection_changed (GmpcConnection* _sender, MpdObj* server, gint connect, gpointer self);
static void _gmpc_metadata_browser_status_changed_gmpc_connection_status_changed (GmpcConnection* _sender, MpdObj* server, ChangedStatusType what, gpointer self);
static GObject * gmpc_metadata_browser_constructor (GType type, guint n_construct_properties, GObjectConstructParam * construct_properties);
static gpointer gmpc_metadata_browser_parent_class = NULL;
static GmpcPluginBrowserIfaceIface* gmpc_metadata_browser_gmpc_plugin_browser_iface_parent_iface = NULL;
static void gmpc_metadata_browser_finalize (GObject* obj);
static void _vala_array_destroy (gpointer array, gint array_length, GDestroyNotify destroy_func);
static void _vala_array_free (gpointer array, gint array_length, GDestroyNotify destroy_func);
static gint _vala_array_length (gpointer array);
static int _vala_strcmp0 (const char * str1, const char * str2);

static const gint GMPC_NOW_PLAYING_version[] = {0, 0, 0};
static const gint GMPC_METADATA_BROWSER_version[] = {0, 0, 0};


static GmpcWidgetSimilarSongs* gmpc_widget_similar_songs_construct (GType object_type, const mpd_Song* song) {
	GmpcWidgetSimilarSongs * self;
	mpd_Song* _tmp1_;
	const mpd_Song* _tmp0_;
	GtkLabel* label;
	char* _tmp2_;
	g_return_val_if_fail (song != NULL, NULL);
	self = g_object_newv (object_type, 0, NULL);
	_tmp1_ = NULL;
	_tmp0_ = NULL;
	self->priv->song = (_tmp1_ = (_tmp0_ = song, (_tmp0_ == NULL) ? NULL : mpd_songDup (_tmp0_)), (self->priv->song == NULL) ? NULL : (self->priv->song = (mpd_freeSong (self->priv->song), NULL)), _tmp1_);
	label = g_object_ref_sink ((GtkLabel*) gtk_label_new (_ ("Similar songs")));
	_tmp2_ = NULL;
	gtk_label_set_markup (label, _tmp2_ = g_strdup_printf ("<b>%s</b>", _ ("Similar songs")));
	_tmp2_ = (g_free (_tmp2_), NULL);
	gtk_expander_set_label_widget ((GtkExpander*) self, (GtkWidget*) label);
	gtk_widget_show ((GtkWidget*) label);
	(label == NULL) ? NULL : (label = (g_object_unref (label), NULL));
	return self;
}


static GmpcWidgetSimilarSongs* gmpc_widget_similar_songs_new (const mpd_Song* song) {
	return gmpc_widget_similar_songs_construct (GMPC_WIDGET_TYPE_SIMILAR_SONGS, song);
}


static void _g_list_free_gtk_tree_path_free (GList* self) {
	g_list_foreach (self, (GFunc) gtk_tree_path_free, NULL);
	g_list_free (self);
}


static void gmpc_widget_similar_songs_add_clicked (GmpcWidgetSimilarSongs* self, GtkImageMenuItem* item) {
	GtkTreeView* _tmp0_;
	GtkTreeView* tree;
	GtkTreeSelection* _tmp1_;
	GtkTreeSelection* sel;
	GtkTreeModel* model;
	GtkTreeIter iter = {0};
	GtkTreeModel* _tmp5_;
	GtkTreeModel* _tmp4_;
	GList* _tmp3_;
	GtkTreeModel* _tmp2_;
	GList* list;
	g_return_if_fail (self != NULL);
	g_return_if_fail (item != NULL);
	_tmp0_ = NULL;
	tree = (_tmp0_ = GTK_TREE_VIEW (self->priv->pchild), (_tmp0_ == NULL) ? NULL : g_object_ref (_tmp0_));
	_tmp1_ = NULL;
	sel = (_tmp1_ = gtk_tree_view_get_selection (tree), (_tmp1_ == NULL) ? NULL : g_object_ref (_tmp1_));
	model = NULL;
	_tmp5_ = NULL;
	_tmp4_ = NULL;
	_tmp3_ = NULL;
	_tmp2_ = NULL;
	list = (_tmp3_ = gtk_tree_selection_get_selected_rows (sel, &_tmp2_), model = (_tmp4_ = (_tmp5_ = _tmp2_, (_tmp5_ == NULL) ? NULL : g_object_ref (_tmp5_)), (model == NULL) ? NULL : (model = (g_object_unref (model), NULL)), _tmp4_), _tmp3_);
	{
		GList* path_collection;
		GList* path_it;
		path_collection = list;
		for (path_it = path_collection; path_it != NULL; path_it = path_it->next) {
			const GtkTreePath* _tmp6_;
			GtkTreePath* path;
			_tmp6_ = NULL;
			path = (_tmp6_ = (const GtkTreePath*) path_it->data, (_tmp6_ == NULL) ? NULL : gtk_tree_path_copy (_tmp6_));
			{
				if (gtk_tree_model_get_iter (model, &iter, path)) {
					const mpd_Song* song;
					song = NULL;
					gtk_tree_model_get (model, &iter, 0, &song, -1, -1);
					if (song != NULL) {
						mpd_playlist_queue_add (connection, song->file);
					}
				}
				(path == NULL) ? NULL : (path = (gtk_tree_path_free (path), NULL));
			}
		}
	}
	mpd_playlist_queue_commit (connection);
	(tree == NULL) ? NULL : (tree = (g_object_unref (tree), NULL));
	(sel == NULL) ? NULL : (sel = (g_object_unref (sel), NULL));
	(model == NULL) ? NULL : (model = (g_object_unref (model), NULL));
	(list == NULL) ? NULL : (list = (_g_list_free_gtk_tree_path_free (list), NULL));
}


static void gmpc_widget_similar_songs_play_clicked (GmpcWidgetSimilarSongs* self, GtkImageMenuItem* item) {
	GtkTreeView* _tmp0_;
	GtkTreeView* tree;
	GtkTreeSelection* _tmp1_;
	GtkTreeSelection* sel;
	GtkTreeModel* model;
	GtkTreeIter iter = {0};
	GtkTreeModel* _tmp5_;
	GtkTreeModel* _tmp4_;
	GList* _tmp3_;
	GtkTreeModel* _tmp2_;
	GList* list;
	g_return_if_fail (self != NULL);
	g_return_if_fail (item != NULL);
	_tmp0_ = NULL;
	tree = (_tmp0_ = GTK_TREE_VIEW (self->priv->pchild), (_tmp0_ == NULL) ? NULL : g_object_ref (_tmp0_));
	_tmp1_ = NULL;
	sel = (_tmp1_ = gtk_tree_view_get_selection (tree), (_tmp1_ == NULL) ? NULL : g_object_ref (_tmp1_));
	model = NULL;
	_tmp5_ = NULL;
	_tmp4_ = NULL;
	_tmp3_ = NULL;
	_tmp2_ = NULL;
	list = (_tmp3_ = gtk_tree_selection_get_selected_rows (sel, &_tmp2_), model = (_tmp4_ = (_tmp5_ = _tmp2_, (_tmp5_ == NULL) ? NULL : g_object_ref (_tmp5_)), (model == NULL) ? NULL : (model = (g_object_unref (model), NULL)), _tmp4_), _tmp3_);
	if (list != NULL) {
		const GtkTreePath* _tmp6_;
		GtkTreePath* path;
		_tmp6_ = NULL;
		path = (_tmp6_ = (const GtkTreePath*) list->data, (_tmp6_ == NULL) ? NULL : gtk_tree_path_copy (_tmp6_));
		if (gtk_tree_model_get_iter (model, &iter, path)) {
			const mpd_Song* song;
			song = NULL;
			gtk_tree_model_get (model, &iter, 0, &song, -1, -1);
			if (song != NULL) {
				play_path (song->file);
			}
		}
		(path == NULL) ? NULL : (path = (gtk_tree_path_free (path), NULL));
	}
	(tree == NULL) ? NULL : (tree = (g_object_unref (tree), NULL));
	(sel == NULL) ? NULL : (sel = (g_object_unref (sel), NULL));
	(model == NULL) ? NULL : (model = (g_object_unref (model), NULL));
	(list == NULL) ? NULL : (list = (_g_list_free_gtk_tree_path_free (list), NULL));
}


static void gmpc_widget_similar_songs_replace_clicked (GmpcWidgetSimilarSongs* self, GtkImageMenuItem* item) {
	gboolean found;
	GtkTreeView* _tmp0_;
	GtkTreeView* tree;
	GtkTreeSelection* _tmp1_;
	GtkTreeSelection* sel;
	GtkTreeModel* model;
	GtkTreeIter iter = {0};
	GtkTreeModel* _tmp5_;
	GtkTreeModel* _tmp4_;
	GList* _tmp3_;
	GtkTreeModel* _tmp2_;
	GList* list;
	g_return_if_fail (self != NULL);
	g_return_if_fail (item != NULL);
	found = FALSE;
	_tmp0_ = NULL;
	tree = (_tmp0_ = GTK_TREE_VIEW (self->priv->pchild), (_tmp0_ == NULL) ? NULL : g_object_ref (_tmp0_));
	_tmp1_ = NULL;
	sel = (_tmp1_ = gtk_tree_view_get_selection (tree), (_tmp1_ == NULL) ? NULL : g_object_ref (_tmp1_));
	model = NULL;
	_tmp5_ = NULL;
	_tmp4_ = NULL;
	_tmp3_ = NULL;
	_tmp2_ = NULL;
	list = (_tmp3_ = gtk_tree_selection_get_selected_rows (sel, &_tmp2_), model = (_tmp4_ = (_tmp5_ = _tmp2_, (_tmp5_ == NULL) ? NULL : g_object_ref (_tmp5_)), (model == NULL) ? NULL : (model = (g_object_unref (model), NULL)), _tmp4_), _tmp3_);
	{
		GList* path_collection;
		GList* path_it;
		path_collection = list;
		for (path_it = path_collection; path_it != NULL; path_it = path_it->next) {
			const GtkTreePath* _tmp6_;
			GtkTreePath* path;
			_tmp6_ = NULL;
			path = (_tmp6_ = (const GtkTreePath*) path_it->data, (_tmp6_ == NULL) ? NULL : gtk_tree_path_copy (_tmp6_));
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
				(path == NULL) ? NULL : (path = (gtk_tree_path_free (path), NULL));
			}
		}
	}
	if (found) {
		mpd_playlist_clear (connection);
		mpd_playlist_queue_commit (connection);
		mpd_player_play (connection);
	}
	gmpc_widget_similar_songs_play_clicked (self, item);
	(tree == NULL) ? NULL : (tree = (g_object_unref (tree), NULL));
	(sel == NULL) ? NULL : (sel = (g_object_unref (sel), NULL));
	(model == NULL) ? NULL : (model = (g_object_unref (model), NULL));
	(list == NULL) ? NULL : (list = (_g_list_free_gtk_tree_path_free (list), NULL));
}


static void gmpc_widget_similar_songs_tree_row_activated (GmpcWidgetSimilarSongs* self, GmpcMpdDataTreeview* tree, const GtkTreePath* path, GtkTreeViewColumn* column) {
	GtkTreeModel* _tmp0_;
	GtkTreeModel* model;
	GtkTreeIter iter = {0};
	g_return_if_fail (self != NULL);
	g_return_if_fail (tree != NULL);
	g_return_if_fail (path != NULL);
	g_return_if_fail (column != NULL);
	_tmp0_ = NULL;
	model = (_tmp0_ = gtk_tree_view_get_model ((GtkTreeView*) tree), (_tmp0_ == NULL) ? NULL : g_object_ref (_tmp0_));
	if (gtk_tree_model_get_iter (model, &iter, path)) {
		const mpd_Song* song;
		song = NULL;
		gtk_tree_model_get (model, &iter, 0, &song, -1, -1);
		if (song != NULL) {
			play_path (song->file);
		}
	}
	(model == NULL) ? NULL : (model = (g_object_unref (model), NULL));
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
	g_return_val_if_fail (self != NULL, FALSE);
	g_return_val_if_fail (tree != NULL, FALSE);
	if ((*event).button == 3) {
		GtkMenu* menu;
		GtkImageMenuItem* item;
		GtkImageMenuItem* _tmp0_;
		GtkImageMenuItem* _tmp1_;
		GtkImage* _tmp2_;
		gboolean _tmp3_;
		menu = g_object_ref_sink ((GtkMenu*) gtk_menu_new ());
		item = g_object_ref_sink ((GtkImageMenuItem*) gtk_image_menu_item_new_from_stock ("gtk-media-play", NULL));
		g_signal_connect_object ((GtkMenuItem*) item, "activate", (GCallback) _gmpc_widget_similar_songs_play_clicked_gtk_menu_item_activate, self, 0);
		gtk_menu_shell_append ((GtkMenuShell*) menu, (GtkWidget*) ((GtkMenuItem*) item));
		_tmp0_ = NULL;
		item = (_tmp0_ = g_object_ref_sink ((GtkImageMenuItem*) gtk_image_menu_item_new_from_stock ("gtk-add", NULL)), (item == NULL) ? NULL : (item = (g_object_unref (item), NULL)), _tmp0_);
		g_signal_connect_object ((GtkMenuItem*) item, "activate", (GCallback) _gmpc_widget_similar_songs_add_clicked_gtk_menu_item_activate, self, 0);
		gtk_menu_shell_append ((GtkMenuShell*) menu, (GtkWidget*) ((GtkMenuItem*) item));
		_tmp1_ = NULL;
		item = (_tmp1_ = g_object_ref_sink ((GtkImageMenuItem*) gtk_image_menu_item_new_with_mnemonic ("_Replace")), (item == NULL) ? NULL : (item = (g_object_unref (item), NULL)), _tmp1_);
		_tmp2_ = NULL;
		gtk_image_menu_item_set_image (item, (GtkWidget*) (_tmp2_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_stock ("gtk-redo", GTK_ICON_SIZE_MENU))));
		(_tmp2_ == NULL) ? NULL : (_tmp2_ = (g_object_unref (_tmp2_), NULL));
		g_signal_connect_object ((GtkMenuItem*) item, "activate", (GCallback) _gmpc_widget_similar_songs_replace_clicked_gtk_menu_item_activate, self, 0);
		gtk_menu_shell_append ((GtkMenuShell*) menu, (GtkWidget*) ((GtkMenuItem*) item));
		gtk_menu_popup (menu, NULL, NULL, NULL, NULL, (*event).button, (*event).time);
		gtk_widget_show_all ((GtkWidget*) menu);
		return (_tmp3_ = TRUE, (menu == NULL) ? NULL : (menu = (g_object_unref (menu), NULL)), (item == NULL) ? NULL : (item = (g_object_unref (item), NULL)), _tmp3_);
	}
	return FALSE;
}


static gboolean _gmpc_widget_similar_songs_tree_right_menu_gtk_widget_button_release_event (GmpcMpdDataTreeview* _sender, const GdkEventButton* event, gpointer self) {
	return gmpc_widget_similar_songs_tree_right_menu (self, _sender, event);
}


static void _gmpc_widget_similar_songs_tree_row_activated_gtk_tree_view_row_activated (GmpcMpdDataTreeview* _sender, const GtkTreePath* path, GtkTreeViewColumn* column, gpointer self) {
	gmpc_widget_similar_songs_tree_row_activated (self, _sender, path, column);
}


static gboolean gmpc_widget_similar_songs_update_sim_song (GmpcWidgetSimilarSongs* self) {
	MetaData* _tmp14_;
	g_return_val_if_fail (self != NULL, FALSE);
	if (self->priv->current == NULL) {
		GtkWidget* _tmp0_;
		self->priv->current = meta_data_get_text_list (self->priv->copy);
		_tmp0_ = NULL;
		self->priv->pchild = (_tmp0_ = (GtkWidget*) g_object_ref_sink ((GtkProgressBar*) gtk_progress_bar_new ()), (self->priv->pchild == NULL) ? NULL : (self->priv->pchild = (g_object_unref (self->priv->pchild), NULL)), _tmp0_);
		gtk_container_add ((GtkContainer*) self, self->priv->pchild);
		gtk_widget_show_all ((GtkWidget*) self);
	}
	gtk_progress_bar_pulse (GTK_PROGRESS_BAR (self->priv->pchild));
	if (self->priv->current != NULL) {
		const char* _tmp1_;
		char* entry;
		_tmp1_ = NULL;
		entry = (_tmp1_ = (const char*) self->priv->current->data, (_tmp1_ == NULL) ? NULL : g_strdup (_tmp1_));
		if (entry != NULL) {
			char** _tmp3_;
			gint split_size;
			gint split_length1;
			char** _tmp2_;
			char** split;
			char** _tmp5_;
			gint art_split_size;
			gint art_split_length1;
			char** _tmp4_;
			char** art_split;
			MpdData* data;
			_tmp3_ = NULL;
			_tmp2_ = NULL;
			split = (_tmp3_ = _tmp2_ = g_strsplit (entry, "::", 2), split_length1 = _vala_array_length (_tmp2_), split_size = split_length1, _tmp3_);
			mpd_database_search_start (connection, FALSE);
			_tmp5_ = NULL;
			_tmp4_ = NULL;
			art_split = (_tmp5_ = _tmp4_ = g_strsplit (split[0], " ", 0), art_split_length1 = _vala_array_length (_tmp4_), art_split_size = art_split_length1, _tmp5_);
			{
				char** artist_collection;
				int artist_collection_length1;
				int artist_it;
				artist_collection = art_split;
				artist_collection_length1 = art_split_length1;
				for (artist_it = 0; artist_it < art_split_length1; artist_it = artist_it + 1) {
					const char* _tmp6_;
					char* artist;
					_tmp6_ = NULL;
					artist = (_tmp6_ = artist_collection[artist_it], (_tmp6_ == NULL) ? NULL : g_strdup (_tmp6_));
					{
						mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ARTIST, artist);
						artist = (g_free (artist), NULL);
					}
				}
			}
			mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_TITLE, split[1]);
			data = mpd_database_search_commit (connection);
			if (data != NULL) {
				MpdData* _tmp7_;
				_tmp7_ = NULL;
				self->priv->item = mpd_data_concatenate (self->priv->item, (_tmp7_ = data, data = NULL, _tmp7_));
			}
			split = (_vala_array_free (split, split_length1, (GDestroyNotify) g_free), NULL);
			art_split = (_vala_array_free (art_split, art_split_length1, (GDestroyNotify) g_free), NULL);
			(data == NULL) ? NULL : (data = (mpd_data_free (data), NULL));
		}
		self->priv->current = self->priv->current->next;
		if (self->priv->current != NULL) {
			gboolean _tmp8_;
			return (_tmp8_ = TRUE, entry = (g_free (entry), NULL), _tmp8_);
		}
		entry = (g_free (entry), NULL);
	}
	gtk_object_destroy ((GtkObject*) self->priv->pchild);
	if (self->priv->item != NULL) {
		GmpcMpdDataModel* model;
		MpdData* _tmp9_;
		GmpcMpdDataTreeview* tree;
		GtkWidget* _tmp11_;
		GtkWidget* _tmp10_;
		model = gmpc_mpddata_model_new ();
		_tmp9_ = NULL;
		gmpc_mpddata_model_set_mpd_data (model, (_tmp9_ = self->priv->item, self->priv->item = NULL, _tmp9_));
		tree = g_object_ref_sink (gmpc_mpddata_treeview_new ("similar-song", TRUE, (GtkTreeModel*) model));
		gmpc_mpddata_treeview_enable_click_fix (tree);
		g_signal_connect_object ((GtkWidget*) tree, "button-release-event", (GCallback) _gmpc_widget_similar_songs_tree_right_menu_gtk_widget_button_release_event, self, 0);
		g_signal_connect_object ((GtkTreeView*) tree, "row-activated", (GCallback) _gmpc_widget_similar_songs_tree_row_activated_gtk_tree_view_row_activated, self, 0);
		gtk_container_add ((GtkContainer*) self, (GtkWidget*) tree);
		_tmp11_ = NULL;
		_tmp10_ = NULL;
		self->priv->pchild = (_tmp11_ = (_tmp10_ = (GtkWidget*) tree, (_tmp10_ == NULL) ? NULL : g_object_ref (_tmp10_)), (self->priv->pchild == NULL) ? NULL : (self->priv->pchild = (g_object_unref (self->priv->pchild), NULL)), _tmp11_);
		(model == NULL) ? NULL : (model = (g_object_unref (model), NULL));
		(tree == NULL) ? NULL : (tree = (g_object_unref (tree), NULL));
	} else {
		GtkLabel* label;
		GtkWidget* _tmp13_;
		GtkWidget* _tmp12_;
		label = g_object_ref_sink ((GtkLabel*) gtk_label_new (_ ("Unavailable")));
		gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.0f);
		gtk_container_add ((GtkContainer*) self, (GtkWidget*) label);
		_tmp13_ = NULL;
		_tmp12_ = NULL;
		self->priv->pchild = (_tmp13_ = (_tmp12_ = (GtkWidget*) label, (_tmp12_ == NULL) ? NULL : g_object_ref (_tmp12_)), (self->priv->pchild == NULL) ? NULL : (self->priv->pchild = (g_object_unref (self->priv->pchild), NULL)), _tmp13_);
		(label == NULL) ? NULL : (label = (g_object_unref (label), NULL));
	}
	_tmp14_ = NULL;
	self->priv->copy = (_tmp14_ = NULL, (self->priv->copy == NULL) ? NULL : (self->priv->copy = (meta_data_free (self->priv->copy), NULL)), _tmp14_);
	self->priv->idle_add = (guint) 0;
	gtk_widget_show_all ((GtkWidget*) self);
	return FALSE;
}


static gboolean _gmpc_widget_similar_songs_update_sim_song_gsource_func (gpointer self) {
	return gmpc_widget_similar_songs_update_sim_song (self);
}


static void gmpc_widget_similar_songs_metadata_changed (GmpcWidgetSimilarSongs* self, GmpcMetaWatcher* gmw, const mpd_Song* song, MetaDataType type, MetaDataResult _result_, const MetaData* met) {
	g_return_if_fail (self != NULL);
	g_return_if_fail (gmw != NULL);
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
		GtkWidget* _tmp1_;
		GtkWidget* _tmp0_;
		label = g_object_ref_sink ((GtkLabel*) gtk_label_new (_ ("Fetching .. ")));
		gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.0f);
		gtk_container_add ((GtkContainer*) self, (GtkWidget*) label);
		_tmp1_ = NULL;
		_tmp0_ = NULL;
		self->priv->pchild = (_tmp1_ = (_tmp0_ = (GtkWidget*) label, (_tmp0_ == NULL) ? NULL : g_object_ref (_tmp0_)), (self->priv->pchild == NULL) ? NULL : (self->priv->pchild = (g_object_unref (self->priv->pchild), NULL)), _tmp1_);
		(label == NULL) ? NULL : (label = (g_object_unref (label), NULL));
	} else {
		if (_result_ == META_DATA_UNAVAILABLE) {
			GtkLabel* label;
			GtkWidget* _tmp3_;
			GtkWidget* _tmp2_;
			label = g_object_ref_sink ((GtkLabel*) gtk_label_new (_ ("Unavailable")));
			gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.0f);
			gtk_container_add ((GtkContainer*) self, (GtkWidget*) label);
			_tmp3_ = NULL;
			_tmp2_ = NULL;
			self->priv->pchild = (_tmp3_ = (_tmp2_ = (GtkWidget*) label, (_tmp2_ == NULL) ? NULL : g_object_ref (_tmp2_)), (self->priv->pchild == NULL) ? NULL : (self->priv->pchild = (g_object_unref (self->priv->pchild), NULL)), _tmp3_);
			(label == NULL) ? NULL : (label = (g_object_unref (label), NULL));
		} else {
			if (meta_data_is_text_list (met)) {
				MetaData* _tmp4_;
				_tmp4_ = NULL;
				self->priv->copy = (_tmp4_ = meta_data_dup_steal (met), (self->priv->copy == NULL) ? NULL : (self->priv->copy = (meta_data_free (self->priv->copy), NULL)), _tmp4_);
				self->priv->idle_add = g_idle_add (_gmpc_widget_similar_songs_update_sim_song_gsource_func, self);
				return;
			} else {
				GtkLabel* label;
				GtkWidget* _tmp6_;
				GtkWidget* _tmp5_;
				label = g_object_ref_sink ((GtkLabel*) gtk_label_new (_ ("Unavailable")));
				gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.0f);
				gtk_container_add ((GtkContainer*) self, (GtkWidget*) label);
				_tmp6_ = NULL;
				_tmp5_ = NULL;
				self->priv->pchild = (_tmp6_ = (_tmp5_ = (GtkWidget*) label, (_tmp5_ == NULL) ? NULL : g_object_ref (_tmp5_)), (self->priv->pchild == NULL) ? NULL : (self->priv->pchild = (g_object_unref (self->priv->pchild), NULL)), _tmp6_);
				(label == NULL) ? NULL : (label = (g_object_unref (label), NULL));
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
	_tmp2_ = NULL;
	_tmp0_ = NULL;
	gm_result = (_tmp1_ = gmpc_meta_watcher_get_meta_path (gmw, self->priv->song, META_SONG_SIMILAR, &_tmp0_), item = (_tmp2_ = _tmp0_, (item == NULL) ? NULL : (item = (meta_data_free (item), NULL)), _tmp2_), _tmp1_);
	gmpc_widget_similar_songs_metadata_changed (self, gmw, self->priv->song, META_SONG_SIMILAR, gm_result, item);
	(item == NULL) ? NULL : (item = (meta_data_free (item), NULL));
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
	(self->priv->song == NULL) ? NULL : (self->priv->song = (mpd_freeSong (self->priv->song), NULL));
	(self->priv->pchild == NULL) ? NULL : (self->priv->pchild = (g_object_unref (self->priv->pchild), NULL));
	(self->priv->copy == NULL) ? NULL : (self->priv->copy = (meta_data_free (self->priv->copy), NULL));
	(self->priv->item == NULL) ? NULL : (self->priv->item = (mpd_data_free (self->priv->item), NULL));
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


static glong string_get_length (const char* self) {
	g_return_val_if_fail (self != NULL, 0L);
	return g_utf8_strlen (self, -1);
}


static void _g_list_free_g_object_unref (GList* self) {
	g_list_foreach (self, (GFunc) g_object_unref, NULL);
	g_list_free (self);
}


static void gmpc_widget_similar_artist_metadata_changed (GmpcWidgetSimilarArtist* self, GmpcMetaWatcher* gmw, const mpd_Song* song, MetaDataType type, MetaDataResult _result_, const MetaData* met) {
	GError * _inner_error_;
	GList* child_list;
	gboolean _tmp1_;
	gboolean _tmp2_;
	g_return_if_fail (self != NULL);
	g_return_if_fail (gmw != NULL);
	g_return_if_fail (song != NULL);
	g_return_if_fail (met != NULL);
	_inner_error_ = NULL;
	if (g_utf8_collate (self->priv->song->artist, song->artist) != 0) {
		return;
	}
	if (type != META_ARTIST_SIMILAR) {
		return;
	}
	/* clear widgets */
	child_list = gtk_container_get_children ((GtkContainer*) self);
	{
		GList* child_collection;
		GList* child_it;
		child_collection = child_list;
		for (child_it = child_collection; child_it != NULL; child_it = child_it->next) {
			GtkWidget* _tmp0_;
			GtkWidget* child;
			_tmp0_ = NULL;
			child = (_tmp0_ = (GtkWidget*) child_it->data, (_tmp0_ == NULL) ? NULL : g_object_ref (_tmp0_));
			{
				gtk_object_destroy ((GtkObject*) child);
				(child == NULL) ? NULL : (child = (g_object_unref (child), NULL));
			}
		}
	}
	_tmp1_ = FALSE;
	_tmp2_ = FALSE;
	if (_result_ == META_DATA_UNAVAILABLE) {
		_tmp2_ = TRUE;
	} else {
		_tmp2_ = meta_data_is_empty (met);
	}
	if (_tmp2_) {
		_tmp1_ = TRUE;
	} else {
		_tmp1_ = !meta_data_is_text_list (met);
	}
	if (_tmp1_) {
		GtkLabel* label;
		label = g_object_ref_sink ((GtkLabel*) gtk_label_new (_ ("Unavailable")));
		gtk_table_attach ((GtkTable*) self, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) 0, (guint) 1, GTK_SHRINK, GTK_SHRINK, (guint) 0, (guint) 0);
		(label == NULL) ? NULL : (label = (g_object_unref (label), NULL));
	} else {
		if (_result_ == META_DATA_FETCHING) {
			GtkLabel* label;
			label = g_object_ref_sink ((GtkLabel*) gtk_label_new (_ ("Fetching")));
			gtk_table_attach ((GtkTable*) self, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) 0, (guint) 1, GTK_SHRINK, GTK_SHRINK, (guint) 0, (guint) 0);
			(label == NULL) ? NULL : (label = (g_object_unref (label), NULL));
		} else {
			GList* in_db_list;
			GList* list;
			gint i;
			in_db_list = NULL;
			list = g_list_copy (meta_data_get_text_list (met));
			if (list != NULL) {
				MpdData* data;
				const MpdData* iter;
				mpd_database_search_field_start (connection, MPD_TAG_ITEM_ARTIST);
				data = mpd_database_search_commit (connection);
				iter = mpd_data_get_first (data);
				while (TRUE) {
					gboolean _tmp3_;
					gboolean _tmp4_;
					_tmp3_ = FALSE;
					if (iter != NULL) {
						_tmp3_ = list != NULL;
					} else {
						_tmp3_ = FALSE;
					}
					if (!_tmp3_) {
						break;
					}
					_tmp4_ = FALSE;
					if (iter->tag != NULL) {
						_tmp4_ = string_get_length (iter->tag) > 0;
					} else {
						_tmp4_ = FALSE;
					}
					if (_tmp4_) {
						GList* liter;
						char* artist;
						liter = g_list_first (list);
						artist = g_regex_escape_string (iter->tag, -1);
						{
							GRegex* reg;
							gboolean _tmp5_;
							reg = g_regex_new (artist, G_REGEX_CASELESS, 0, &_inner_error_);
							if (_inner_error_ != NULL) {
								goto __catch0_g_error;
								goto __finally0;
							}
							_tmp5_ = FALSE;
							do {
								if (_tmp5_) {
									gboolean _tmp6_;
									_tmp6_ = FALSE;
									if (liter != NULL) {
										_tmp6_ = (liter = liter->next) != NULL;
									} else {
										_tmp6_ = FALSE;
									}
									if (!_tmp6_) {
										break;
									}
								}
								_tmp5_ = TRUE;
								if (g_regex_match (reg, (const char*) liter->data, 0, NULL)) {
									in_db_list = g_list_prepend (in_db_list, gmpc_widget_similar_artist_new_artist_button (self, iter->tag, TRUE));
									list = g_list_remove (list, (const char*) liter->data);
									liter = NULL;
								}
							} while (TRUE);
							(reg == NULL) ? NULL : (reg = (g_regex_unref (reg), NULL));
						}
						goto __finally0;
						__catch0_g_error:
						{
							GError * E;
							E = _inner_error_;
							_inner_error_ = NULL;
							{
								g_assert_not_reached ();
								(E == NULL) ? NULL : (E = (g_error_free (E), NULL));
							}
						}
						__finally0:
						if (_inner_error_ != NULL) {
							artist = (g_free (artist), NULL);
							(data == NULL) ? NULL : (data = (mpd_data_free (data), NULL));
							(in_db_list == NULL) ? NULL : (in_db_list = (_g_list_free_g_object_unref (in_db_list), NULL));
							(list == NULL) ? NULL : (list = (g_list_free (list), NULL));
							(child_list == NULL) ? NULL : (child_list = (g_list_free (child_list), NULL));
							g_critical ("file %s: line %d: uncaught error: %s", __FILE__, __LINE__, _inner_error_->message);
							g_clear_error (&_inner_error_);
							return;
						}
						artist = (g_free (artist), NULL);
					}
					iter = mpd_data_get_next_real (iter, FALSE);
				}
				(data == NULL) ? NULL : (data = (mpd_data_free (data), NULL));
			}
			{
				GList* artist_collection;
				GList* artist_it;
				artist_collection = list;
				for (artist_it = artist_collection; artist_it != NULL; artist_it = artist_it->next) {
					const char* _tmp7_;
					char* artist;
					_tmp7_ = NULL;
					artist = (_tmp7_ = (const char*) artist_it->data, (_tmp7_ == NULL) ? NULL : g_strdup (_tmp7_));
					{
						in_db_list = g_list_prepend (in_db_list, gmpc_widget_similar_artist_new_artist_button (self, artist, FALSE));
						artist = (g_free (artist), NULL);
					}
				}
			}
			in_db_list = g_list_reverse (in_db_list);
			i = 0;
			gtk_widget_hide ((GtkWidget*) self);
			{
				GList* item_collection;
				GList* item_it;
				item_collection = in_db_list;
				for (item_it = item_collection; item_it != NULL; item_it = item_it->next) {
					GtkWidget* _tmp9_;
					GtkWidget* item;
					_tmp9_ = NULL;
					item = (_tmp9_ = (GtkWidget*) item_it->data, (_tmp9_ == NULL) ? NULL : g_object_ref (_tmp9_));
					{
						if (i < 50) {
							gtk_table_attach ((GtkTable*) self, item, (guint) (i % 4), (guint) ((i % 4) + 1), (guint) (i / 4), (guint) ((i / 4) + 1), GTK_EXPAND | GTK_FILL, GTK_SHRINK, (guint) 0, (guint) 0);
						} else {
							GObject* _tmp8_;
							_tmp8_ = NULL;
							_tmp8_ = g_object_ref_sink ((GObject*) item);
							(_tmp8_ == NULL) ? NULL : (_tmp8_ = (g_object_unref (_tmp8_), NULL));
							gtk_object_destroy ((GtkObject*) item);
						}
						i++;
						(item == NULL) ? NULL : (item = (g_object_unref (item), NULL));
					}
				}
			}
			(in_db_list == NULL) ? NULL : (in_db_list = (_g_list_free_g_object_unref (in_db_list), NULL));
			(list == NULL) ? NULL : (list = (g_list_free (list), NULL));
		}
	}
	gtk_widget_show_all ((GtkWidget*) self);
	(child_list == NULL) ? NULL : (child_list = (g_list_free (child_list), NULL));
}


static void gmpc_widget_similar_artist_artist_button_clicked (GmpcWidgetSimilarArtist* self, GtkButton* button) {
	const char* artist;
	g_return_if_fail (self != NULL);
	g_return_if_fail (button != NULL);
	artist = (const char*) g_object_get_data ((GObject*) button, "artist");
	gmpc_metadata_browser_set_artist (self->priv->browser, artist);
}


static gboolean _misc_header_expose_event_gtk_widget_expose_event (GtkEventBox* _sender, const GdkEventExpose* event, gpointer self) {
	return misc_header_expose_event (_sender, event);
}


static void _gmpc_widget_similar_artist_artist_button_clicked_gtk_button_clicked (GtkButton* _sender, gpointer self) {
	gmpc_widget_similar_artist_artist_button_clicked (self, _sender);
}


GtkWidget* gmpc_widget_similar_artist_new_artist_button (GmpcWidgetSimilarArtist* self, const char* artist, gboolean in_db) {
	GtkHBox* hbox;
	GtkEventBox* event;
	GmpcMetaImage* image;
	mpd_Song* song;
	char* _tmp1_;
	const char* _tmp0_;
	GtkLabel* label;
	GtkWidget* _tmp2_;
	g_return_val_if_fail (self != NULL, NULL);
	g_return_val_if_fail (artist != NULL, NULL);
	hbox = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 6));
	gtk_container_set_border_width ((GtkContainer*) hbox, (guint) 6);
	event = g_object_ref_sink ((GtkEventBox*) gtk_event_box_new ());
	g_object_set ((GtkWidget*) event, "app-paintable", TRUE, NULL);
	g_signal_connect ((GtkWidget*) event, "expose-event", (GCallback) _misc_header_expose_event_gtk_widget_expose_event, NULL);
	image = g_object_ref_sink (gmpc_metaimage_new_size (META_ARTIST_ART, 48));
	song = mpd_newSong ();
	_tmp1_ = NULL;
	_tmp0_ = NULL;
	song->artist = (_tmp1_ = (_tmp0_ = artist, (_tmp0_ == NULL) ? NULL : g_strdup (_tmp0_)), song->artist = (g_free (song->artist), NULL), _tmp1_);
	gmpc_metaimage_update_cover_from_song_delayed (image, song);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) image, FALSE, FALSE, (guint) 0);
	label = g_object_ref_sink ((GtkLabel*) gtk_label_new (artist));
	gtk_label_set_selectable (label, TRUE);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
	gtk_label_set_ellipsize (label, PANGO_ELLIPSIZE_END);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) label, TRUE, TRUE, (guint) 0);
	if (in_db) {
		GtkButton* find;
		find = g_object_ref_sink ((GtkButton*) gtk_button_new_from_stock ("gtk-find"));
		gtk_button_set_relief (find, GTK_RELIEF_NONE);
		gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) find, FALSE, FALSE, (guint) 0);
		g_object_set_data_full ((GObject*) find, "artist", (void*) g_strdup_printf ("%s", artist), (GDestroyNotify) g_free);
		g_signal_connect_object (find, "clicked", (GCallback) _gmpc_widget_similar_artist_artist_button_clicked_gtk_button_clicked, self, 0);
		(find == NULL) ? NULL : (find = (g_object_unref (find), NULL));
	}
	gtk_container_add ((GtkContainer*) event, (GtkWidget*) hbox);
	gtk_widget_set_size_request ((GtkWidget*) event, 180, 60);
	_tmp2_ = NULL;
	return (_tmp2_ = (GtkWidget*) event, (hbox == NULL) ? NULL : (hbox = (g_object_unref (hbox), NULL)), (image == NULL) ? NULL : (image = (g_object_unref (image), NULL)), (song == NULL) ? NULL : (song = (mpd_freeSong (song), NULL)), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp2_);
}


static void _gmpc_widget_similar_artist_metadata_changed_gmpc_meta_watcher_data_changed (GmpcMetaWatcher* _sender, const mpd_Song* song, MetaDataType type, MetaDataResult _result_, const MetaData* met, gpointer self) {
	gmpc_widget_similar_artist_metadata_changed (self, _sender, song, type, _result_, met);
}


static GmpcWidgetSimilarArtist* gmpc_widget_similar_artist_construct (GType object_type, GmpcMetadataBrowser* browser, MpdObj* server, const mpd_Song* song) {
	GmpcWidgetSimilarArtist * self;
	MetaData* item;
	GmpcMetadataBrowser* _tmp1_;
	GmpcMetadataBrowser* _tmp0_;
	mpd_Song* _tmp3_;
	const mpd_Song* _tmp2_;
	MetaData* _tmp6_;
	MetaDataResult _tmp5_;
	MetaData* _tmp4_;
	MetaDataResult gm_result;
	g_return_val_if_fail (browser != NULL, NULL);
	g_return_val_if_fail (server != NULL, NULL);
	g_return_val_if_fail (song != NULL, NULL);
	self = g_object_newv (object_type, 0, NULL);
	item = NULL;
	_tmp1_ = NULL;
	_tmp0_ = NULL;
	self->priv->browser = (_tmp1_ = (_tmp0_ = browser, (_tmp0_ == NULL) ? NULL : g_object_ref (_tmp0_)), (self->priv->browser == NULL) ? NULL : (self->priv->browser = (g_object_unref (self->priv->browser), NULL)), _tmp1_);
	_tmp3_ = NULL;
	_tmp2_ = NULL;
	self->priv->song = (_tmp3_ = (_tmp2_ = song, (_tmp2_ == NULL) ? NULL : mpd_songDup (_tmp2_)), (self->priv->song == NULL) ? NULL : (self->priv->song = (mpd_freeSong (self->priv->song), NULL)), _tmp3_);
	gtk_table_set_homogeneous ((GtkTable*) self, TRUE);
	gtk_table_set_row_spacings ((GtkTable*) self, (guint) 6);
	gtk_table_set_col_spacings ((GtkTable*) self, (guint) 6);
	g_signal_connect_object (gmw, "data-changed", (GCallback) _gmpc_widget_similar_artist_metadata_changed_gmpc_meta_watcher_data_changed, self, 0);
	_tmp6_ = NULL;
	_tmp4_ = NULL;
	gm_result = (_tmp5_ = gmpc_meta_watcher_get_meta_path (gmw, song, META_ARTIST_SIMILAR, &_tmp4_), item = (_tmp6_ = _tmp4_, (item == NULL) ? NULL : (item = (meta_data_free (item), NULL)), _tmp6_), _tmp5_);
	if (gm_result == META_DATA_AVAILABLE) {
		gmpc_widget_similar_artist_metadata_changed (self, gmw, self->priv->song, META_ARTIST_SIMILAR, gm_result, item);
	}
	(item == NULL) ? NULL : (item = (meta_data_free (item), NULL));
	return self;
}


static GmpcWidgetSimilarArtist* gmpc_widget_similar_artist_new (GmpcMetadataBrowser* browser, MpdObj* server, const mpd_Song* song) {
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
	(self->priv->song == NULL) ? NULL : (self->priv->song = (mpd_freeSong (self->priv->song), NULL));
	(self->priv->browser == NULL) ? NULL : (self->priv->browser = (g_object_unref (self->priv->browser), NULL));
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
	GdkColor _tmp0_ = {0};
	GdkColor _tmp1_ = {0};
	GdkColor _tmp2_ = {0};
	GdkColor _tmp3_ = {0};
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


static GmpcWidgetMore* gmpc_widget_more_construct (GType object_type, const char* markup, GtkWidget* child) {
	GmpcWidgetMore * self;
	GtkWidget* _tmp1_;
	GtkWidget* _tmp0_;
	GtkAlignment* _tmp2_;
	GtkEventBox* _tmp3_;
	GtkHBox* hbox;
	GtkLabel* label;
	GtkButton* _tmp4_;
	g_return_val_if_fail (markup != NULL, NULL);
	g_return_val_if_fail (child != NULL, NULL);
	self = g_object_newv (object_type, 0, NULL);
	gtk_frame_set_shadow_type ((GtkFrame*) self, GTK_SHADOW_NONE);
	_tmp1_ = NULL;
	_tmp0_ = NULL;
	self->priv->pchild = (_tmp1_ = (_tmp0_ = child, (_tmp0_ == NULL) ? NULL : g_object_ref (_tmp0_)), (self->priv->pchild == NULL) ? NULL : (self->priv->pchild = (g_object_unref (self->priv->pchild), NULL)), _tmp1_);
	_tmp2_ = NULL;
	self->priv->ali = (_tmp2_ = g_object_ref_sink ((GtkAlignment*) gtk_alignment_new (0.f, 0.f, 1.f, 0.f)), (self->priv->ali == NULL) ? NULL : (self->priv->ali = (g_object_unref (self->priv->ali), NULL)), _tmp2_);
	gtk_alignment_set_padding (self->priv->ali, (guint) 1, (guint) 1, (guint) 1, (guint) 1);
	_tmp3_ = NULL;
	self->priv->eventbox = (_tmp3_ = g_object_ref_sink ((GtkEventBox*) gtk_event_box_new ()), (self->priv->eventbox == NULL) ? NULL : (self->priv->eventbox = (g_object_unref (self->priv->eventbox), NULL)), _tmp3_);
	gtk_event_box_set_visible_window (self->priv->eventbox, TRUE);
	gtk_container_add ((GtkContainer*) self, (GtkWidget*) self->priv->eventbox);
	gtk_container_add ((GtkContainer*) self->priv->eventbox, (GtkWidget*) self->priv->ali);
	gtk_widget_set_size_request ((GtkWidget*) self->priv->ali, -1, self->priv->max_height);
	gtk_container_add ((GtkContainer*) self->priv->ali, child);
	g_signal_connect_object ((GtkWidget*) self, "style-set", (GCallback) _gmpc_widget_more_bg_style_changed_gtk_widget_style_set, self, 0);
	hbox = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 6));
	label = g_object_ref_sink ((GtkLabel*) gtk_label_new (""));
	gtk_label_set_selectable (label, TRUE);
	gtk_label_set_markup (label, markup);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) label, FALSE, FALSE, (guint) 0);
	_tmp4_ = NULL;
	self->priv->expand_button = (_tmp4_ = g_object_ref_sink ((GtkButton*) gtk_button_new_with_label (_ ("(more)"))), (self->priv->expand_button == NULL) ? NULL : (self->priv->expand_button = (g_object_unref (self->priv->expand_button), NULL)), _tmp4_);
	gtk_button_set_relief (self->priv->expand_button, GTK_RELIEF_NONE);
	g_signal_connect_object (self->priv->expand_button, "clicked", (GCallback) _gmpc_widget_more_expand_gtk_button_clicked, self, 0);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) self->priv->expand_button, FALSE, FALSE, (guint) 0);
	gtk_frame_set_label_widget ((GtkFrame*) self, (GtkWidget*) hbox);
	g_signal_connect_object (child, "size-allocate", (GCallback) _gmpc_widget_more_size_changed_gtk_widget_size_allocate, self, 0);
	(hbox == NULL) ? NULL : (hbox = (g_object_unref (hbox), NULL));
	(label == NULL) ? NULL : (label = (g_object_unref (label), NULL));
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


static gint gmpc_now_playing_real_get_version (GmpcPluginBase* base, int* result_length1) {
	GmpcNowPlaying * self;
	gint* _tmp0_;
	self = (GmpcNowPlaying*) base;
	_tmp0_ = NULL;
	return (_tmp0_ = GMPC_NOW_PLAYING_version, *result_length1 = G_N_ELEMENTS (GMPC_NOW_PLAYING_version), _tmp0_);
}


static const char* gmpc_now_playing_real_get_name (GmpcPluginBase* base) {
	GmpcNowPlaying * self;
	self = (GmpcNowPlaying*) base;
	return N_ ("Now Playing");
}


static void gmpc_now_playing_real_save_yourself (GmpcPluginBase* base) {
	GmpcNowPlaying * self;
	self = (GmpcNowPlaying*) base;
	if (self->priv->paned != NULL) {
		GtkScrolledWindow* _tmp0_;
		gtk_object_destroy ((GtkObject*) self->priv->paned);
		_tmp0_ = NULL;
		self->priv->paned = (_tmp0_ = NULL, (self->priv->paned == NULL) ? NULL : (self->priv->paned = (g_object_unref (self->priv->paned), NULL)), _tmp0_);
	}
	if (self->priv->np_ref != NULL) {
		GtkTreePath* path;
		path = gtk_tree_row_reference_get_path (self->priv->np_ref);
		if (path != NULL) {
			gint* _tmp1_;
			gint indices_size;
			gint indices_length1;
			gint* indices;
			_tmp1_ = NULL;
			indices = (_tmp1_ = gtk_tree_path_get_indices (path), indices_length1 = -1, indices_size = indices_length1, _tmp1_);
			cfg_set_single_value_as_int (config, gmpc_plugin_base_get_name ((GmpcPluginBase*) self), "position", indices[0]);
		}
		(path == NULL) ? NULL : (path = (gtk_tree_path_free (path), NULL));
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


/** 
     * Browser Interface bindings
     */
static void gmpc_now_playing_real_browser_add (GmpcPluginBrowserIface* base, GtkWidget* category_tree) {
	GmpcNowPlaying * self;
	GtkTreeView* _tmp0_;
	GtkTreeView* tree;
	GtkListStore* _tmp1_;
	GtkListStore* store;
	GtkTreeModel* _tmp2_;
	GtkTreeModel* model;
	GtkTreeIter iter = {0};
	GtkTreeRowReference* _tmp4_;
	GtkTreePath* _tmp3_;
	self = (GmpcNowPlaying*) base;
	g_return_if_fail (category_tree != NULL);
	_tmp0_ = NULL;
	tree = (_tmp0_ = GTK_TREE_VIEW (category_tree), (_tmp0_ == NULL) ? NULL : g_object_ref (_tmp0_));
	_tmp1_ = NULL;
	store = (_tmp1_ = GTK_LIST_STORE (gtk_tree_view_get_model (tree)), (_tmp1_ == NULL) ? NULL : g_object_ref (_tmp1_));
	_tmp2_ = NULL;
	model = (_tmp2_ = gtk_tree_view_get_model (tree), (_tmp2_ == NULL) ? NULL : g_object_ref (_tmp2_));
	playlist3_insert_browser (&iter, cfg_get_single_value_as_int_with_default (config, gmpc_plugin_base_get_name ((GmpcPluginBase*) self), "position", 0));
	gtk_list_store_set (store, &iter, 0, ((GmpcPluginBase*) self)->id, 1, _ (gmpc_plugin_base_get_name ((GmpcPluginBase*) self)), 3, "media-audiofile", -1);
	/* Create a row reference */
	_tmp4_ = NULL;
	_tmp3_ = NULL;
	self->priv->np_ref = (_tmp4_ = gtk_tree_row_reference_new (model, _tmp3_ = gtk_tree_model_get_path (model, &iter)), (self->priv->np_ref == NULL) ? NULL : (self->priv->np_ref = (gtk_tree_row_reference_free (self->priv->np_ref), NULL)), _tmp4_);
	(_tmp3_ == NULL) ? NULL : (_tmp3_ = (gtk_tree_path_free (_tmp3_), NULL));
	(tree == NULL) ? NULL : (tree = (g_object_unref (tree), NULL));
	(store == NULL) ? NULL : (store = (g_object_unref (store), NULL));
	(model == NULL) ? NULL : (model = (g_object_unref (model), NULL));
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
	GdkColor _tmp0_ = {0};
	g_return_if_fail (self != NULL);
	g_return_if_fail (bg != NULL);
	gtk_widget_modify_bg ((GtkWidget*) self->priv->container, GTK_STATE_NORMAL, (_tmp0_ = gtk_widget_get_style ((GtkWidget*) self->priv->paned)->base[GTK_STATE_NORMAL], &_tmp0_));
}


/* Handle buttons presses, f.e. for scrolling */
static gboolean gmpc_now_playing_browser_key_release_event (GmpcNowPlaying* self, const GdkEventKey* event) {
	GtkAdjustment* _tmp0_;
	GtkAdjustment* adj;
	double incr;
	gboolean _tmp3_;
	g_return_val_if_fail (self != NULL, FALSE);
	_tmp0_ = NULL;
	adj = (_tmp0_ = gtk_scrolled_window_get_vadjustment (self->priv->paned), (_tmp0_ == NULL) ? NULL : g_object_ref (_tmp0_));
	incr = (double) 20;
	g_object_get ((GObject*) adj, "step-increment", &incr, NULL);
	if ((*event).keyval == 0xff55) {
		gboolean _tmp1_;
		/* GDK_Page_Up*/
		gtk_adjustment_set_value (adj, gtk_adjustment_get_value (adj) - incr);
		return (_tmp1_ = TRUE, (adj == NULL) ? NULL : (adj = (g_object_unref (adj), NULL)), _tmp1_);
	} else {
		if ((*event).keyval == 0xff56) {
			gboolean _tmp2_;
			/* GDK_Page_Down*/
			gtk_adjustment_set_value (adj, gtk_adjustment_get_value (adj) + incr);
			return (_tmp2_ = TRUE, (adj == NULL) ? NULL : (adj = (g_object_unref (adj), NULL)), _tmp2_);
		}
	}
	return (_tmp3_ = FALSE, (adj == NULL) ? NULL : (adj = (g_object_unref (adj), NULL)), _tmp3_);
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
		_tmp0_ = NULL;
		self->priv->paned = (_tmp0_ = g_object_ref_sink ((GtkScrolledWindow*) gtk_scrolled_window_new (NULL, NULL)), (self->priv->paned == NULL) ? NULL : (self->priv->paned = (g_object_unref (self->priv->paned), NULL)), _tmp0_);
		gtk_scrolled_window_set_policy (self->priv->paned, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
		gtk_scrolled_window_set_shadow_type (self->priv->paned, GTK_SHADOW_NONE);
		_tmp1_ = NULL;
		self->priv->container = (_tmp1_ = g_object_ref_sink ((GtkEventBox*) gtk_event_box_new ()), (self->priv->container == NULL) ? NULL : (self->priv->container = (g_object_unref (self->priv->container), NULL)), _tmp1_);
		gtk_event_box_set_visible_window (self->priv->container, TRUE);
		g_signal_connect_object ((GtkWidget*) self->priv->paned, "style-set", (GCallback) _gmpc_now_playing_browser_bg_style_changed_gtk_widget_style_set, self, 0);
		gtk_scrolled_window_add_with_viewport (self->priv->paned, (GtkWidget*) self->priv->container);
		g_object_set ((GObject*) gtk_scrolled_window_get_vadjustment (self->priv->paned), "step-increment", 20.0, NULL);
		gtk_container_set_focus_vadjustment ((GtkContainer*) self->priv->container, gtk_scrolled_window_get_vadjustment (self->priv->paned));
		/* Bind keys */
		g_signal_connect_object ((GtkWidget*) self->priv->paned, "key-release-event", (GCallback) _gmpc_now_playing_browser_key_release_event_gtk_widget_key_release_event, self, 0);
	}
}


static void gmpc_now_playing_update (GmpcNowPlaying* self) {
	GError * _inner_error_;
	const mpd_Song* _tmp0_;
	mpd_Song* song;
	gboolean _tmp1_;
	g_return_if_fail (self != NULL);
	_inner_error_ = NULL;
	if (self->priv->paned == NULL) {
		return;
	}
	_tmp0_ = NULL;
	song = (_tmp0_ = mpd_playlist_get_current_song (connection), (_tmp0_ == NULL) ? NULL : mpd_songDup (_tmp0_));
	_tmp1_ = FALSE;
	if (song != NULL) {
		_tmp1_ = mpd_player_get_state (connection) != MPD_STATUS_STATE_STOP;
	} else {
		_tmp1_ = FALSE;
	}
	if (_tmp1_) {
		char* checksum;
		checksum = mpd_song_checksum (song);
		if (_vala_strcmp0 (checksum, self->priv->song_checksum) != 0) {
			GList* list;
			GtkWidget* view;
			char* _tmp4_;
			const char* _tmp3_;
			/* Clear */
			list = gtk_container_get_children ((GtkContainer*) self->priv->container);
			{
				GList* child_collection;
				GList* child_it;
				child_collection = list;
				for (child_it = child_collection; child_it != NULL; child_it = child_it->next) {
					GtkWidget* _tmp2_;
					GtkWidget* child;
					_tmp2_ = NULL;
					child = (_tmp2_ = (GtkWidget*) child_it->data, (_tmp2_ == NULL) ? NULL : g_object_ref (_tmp2_));
					{
						gtk_object_destroy ((GtkObject*) child);
						(child == NULL) ? NULL : (child = (g_object_unref (child), NULL));
					}
				}
			}
			view = gmpc_metadata_browser_metadata_box_show_song (self->priv->browser, song);
			gtk_container_add ((GtkContainer*) self->priv->container, view);
			_tmp4_ = NULL;
			_tmp3_ = NULL;
			self->priv->song_checksum = (_tmp4_ = (_tmp3_ = checksum, (_tmp3_ == NULL) ? NULL : g_strdup (_tmp3_)), self->priv->song_checksum = (g_free (self->priv->song_checksum), NULL), _tmp4_);
			(list == NULL) ? NULL : (list = (g_list_free (list), NULL));
			(view == NULL) ? NULL : (view = (g_object_unref (view), NULL));
		}
		checksum = (g_free (checksum), NULL);
	} else {
		char* _tmp5_;
		GList* list;
		GtkIconTheme* _tmp7_;
		GtkIconTheme* it;
		GtkIconInfo* info;
		const char* _tmp8_;
		char* path;
		GtkImage* image;
		GtkHBox* hbox;
		GtkLabel* label;
		char* _tmp11_;
		GtkAlignment* ali;
		_tmp5_ = NULL;
		self->priv->song_checksum = (_tmp5_ = NULL, self->priv->song_checksum = (g_free (self->priv->song_checksum), NULL), _tmp5_);
		/* Clear */
		list = gtk_container_get_children ((GtkContainer*) self->priv->container);
		{
			GList* child_collection;
			GList* child_it;
			child_collection = list;
			for (child_it = child_collection; child_it != NULL; child_it = child_it->next) {
				GtkWidget* _tmp6_;
				GtkWidget* child;
				_tmp6_ = NULL;
				child = (_tmp6_ = (GtkWidget*) child_it->data, (_tmp6_ == NULL) ? NULL : g_object_ref (_tmp6_));
				{
					gtk_object_destroy ((GtkObject*) child);
					(child == NULL) ? NULL : (child = (g_object_unref (child), NULL));
				}
			}
		}
		_tmp7_ = NULL;
		it = (_tmp7_ = gtk_icon_theme_get_default (), (_tmp7_ == NULL) ? NULL : g_object_ref (_tmp7_));
		info = gtk_icon_theme_lookup_icon (it, "gmpc", 150, 0);
		_tmp8_ = NULL;
		path = (_tmp8_ = gtk_icon_info_get_filename (info), (_tmp8_ == NULL) ? NULL : g_strdup (_tmp8_));
		image = NULL;
		if (path != NULL) {
			{
				GdkPixbuf* pb;
				GtkImage* _tmp9_;
				pb = gdk_pixbuf_new_from_file_at_scale (path, 150, 150, TRUE, &_inner_error_);
				if (_inner_error_ != NULL) {
					goto __catch1_g_error;
					goto __finally1;
				}
				_tmp9_ = NULL;
				image = (_tmp9_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_pixbuf (pb)), (image == NULL) ? NULL : (image = (g_object_unref (image), NULL)), _tmp9_);
				(pb == NULL) ? NULL : (pb = (g_object_unref (pb), NULL));
			}
			goto __finally1;
			__catch1_g_error:
			{
				GError * e;
				e = _inner_error_;
				_inner_error_ = NULL;
				{
					(e == NULL) ? NULL : (e = (g_error_free (e), NULL));
				}
			}
			__finally1:
			if (_inner_error_ != NULL) {
				(list == NULL) ? NULL : (list = (g_list_free (list), NULL));
				(it == NULL) ? NULL : (it = (g_object_unref (it), NULL));
				path = (g_free (path), NULL);
				(image == NULL) ? NULL : (image = (g_object_unref (image), NULL));
				(song == NULL) ? NULL : (song = (mpd_freeSong (song), NULL));
				g_critical ("file %s: line %d: uncaught error: %s", __FILE__, __LINE__, _inner_error_->message);
				g_clear_error (&_inner_error_);
				return;
			}
		}
		if (image == NULL) {
			GtkImage* _tmp10_;
			_tmp10_ = NULL;
			image = (_tmp10_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_icon_name ("gmpc", GTK_ICON_SIZE_DIALOG)), (image == NULL) ? NULL : (image = (g_object_unref (image), NULL)), _tmp10_);
		}
		hbox = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 6));
		label = g_object_ref_sink ((GtkLabel*) gtk_label_new (_ ("Gnome Music Player Client")));
		gtk_label_set_selectable (label, TRUE);
		_tmp11_ = NULL;
		gtk_label_set_markup (label, _tmp11_ = g_strdup_printf ("<span size='%i' weight='bold'>%s</span>", 28 * PANGO_SCALE, _ ("Gnome Music Player Client")));
		_tmp11_ = (g_free (_tmp11_), NULL);
		gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) image, FALSE, FALSE, (guint) 0);
		gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) label, FALSE, FALSE, (guint) 0);
		ali = g_object_ref_sink ((GtkAlignment*) gtk_alignment_new (0.5f, 0.5f, 0.0f, 0.0f));
		gtk_container_add ((GtkContainer*) ali, (GtkWidget*) hbox);
		gtk_container_add ((GtkContainer*) self->priv->container, (GtkWidget*) ali);
		(list == NULL) ? NULL : (list = (g_list_free (list), NULL));
		(it == NULL) ? NULL : (it = (g_object_unref (it), NULL));
		path = (g_free (path), NULL);
		(image == NULL) ? NULL : (image = (g_object_unref (image), NULL));
		(hbox == NULL) ? NULL : (hbox = (g_object_unref (hbox), NULL));
		(label == NULL) ? NULL : (label = (g_object_unref (label), NULL));
		(ali == NULL) ? NULL : (ali = (g_object_unref (ali), NULL));
	}
	gtk_widget_show_all ((GtkWidget*) self->priv->paned);
	(song == NULL) ? NULL : (song = (mpd_freeSong (song), NULL));
}


static void gmpc_now_playing_select_now_playing_browser (GmpcNowPlaying* self, GtkImageMenuItem* item) {
	GtkTreeView* tree;
	GtkTreeSelection* _tmp0_;
	GtkTreeSelection* sel;
	GtkTreePath* path;
	g_return_if_fail (self != NULL);
	g_return_if_fail (item != NULL);
	tree = playlist3_get_category_tree_view ();
	_tmp0_ = NULL;
	sel = (_tmp0_ = gtk_tree_view_get_selection (tree), (_tmp0_ == NULL) ? NULL : g_object_ref (_tmp0_));
	path = gtk_tree_row_reference_get_path (self->priv->np_ref);
	gtk_tree_selection_select_path (sel, path);
	(sel == NULL) ? NULL : (sel = (g_object_unref (sel), NULL));
	(path == NULL) ? NULL : (path = (gtk_tree_path_free (path), NULL));
}


static void _gmpc_now_playing_select_now_playing_browser_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self) {
	gmpc_now_playing_select_now_playing_browser (self, _sender);
}


static gint gmpc_now_playing_real_browser_add_go_menu (GmpcPluginBrowserIface* base, GtkMenu* menu) {
	GmpcNowPlaying * self;
	GtkImageMenuItem* item;
	GtkImage* _tmp0_;
	gint _tmp1_;
	self = (GmpcNowPlaying*) base;
	g_return_val_if_fail (menu != NULL, 0);
	item = g_object_ref_sink ((GtkImageMenuItem*) gtk_image_menu_item_new_with_mnemonic (_ ("Now Playing")));
	_tmp0_ = NULL;
	gtk_image_menu_item_set_image (item, (GtkWidget*) (_tmp0_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_stock ("gtk-info", GTK_ICON_SIZE_MENU))));
	(_tmp0_ == NULL) ? NULL : (_tmp0_ = (g_object_unref (_tmp0_), NULL));
	g_signal_connect_object ((GtkMenuItem*) item, "activate", (GCallback) _gmpc_now_playing_select_now_playing_browser_gtk_menu_item_activate, self, 0);
	gtk_widget_add_accelerator ((GtkWidget*) item, "activate", gtk_menu_get_accel_group (menu), (guint) 0x069, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_menu_shell_append ((GtkMenuShell*) menu, (GtkWidget*) ((GtkMenuItem*) item));
	return (_tmp1_ = 1, (item == NULL) ? NULL : (item = (g_object_unref (item), NULL)), _tmp1_);
}


/**
 * Now playing uses the MetaDataBrowser plugin to "plot" the view */
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
	GmpcNowPlayingClass * klass;
	GObjectClass * parent_class;
	GmpcNowPlaying * self;
	klass = GMPC_NOW_PLAYING_CLASS (g_type_class_peek (GMPC_TYPE_NOW_PLAYING));
	parent_class = G_OBJECT_CLASS (g_type_class_peek_parent (klass));
	obj = parent_class->constructor (type, n_construct_properties, construct_properties);
	self = GMPC_NOW_PLAYING (obj);
	{
		GmpcMetadataBrowser* _tmp0_;
		/* Set the plugin as an internal one and of type pl_browser */
		((GmpcPluginBase*) self)->plugin_type = 2 | 8;
		/*    gmpcconn.connection_changed += con_changed;*/
		g_signal_connect_object (gmpcconn, "status-changed", (GCallback) _gmpc_now_playing_status_changed_gmpc_connection_status_changed, self, 0);
		_tmp0_ = NULL;
		self->priv->browser = (_tmp0_ = gmpc_metadata_browser_new (), (self->priv->browser == NULL) ? NULL : (self->priv->browser = (g_object_unref (self->priv->browser), NULL)), _tmp0_);
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
	(self->priv->np_ref == NULL) ? NULL : (self->priv->np_ref = (gtk_tree_row_reference_free (self->priv->np_ref), NULL));
	(self->priv->browser == NULL) ? NULL : (self->priv->browser = (g_object_unref (self->priv->browser), NULL));
	(self->priv->paned == NULL) ? NULL : (self->priv->paned = (g_object_unref (self->priv->paned), NULL));
	(self->priv->container == NULL) ? NULL : (self->priv->container = (g_object_unref (self->priv->container), NULL));
	self->priv->song_checksum = (g_free (self->priv->song_checksum), NULL);
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


static gint gmpc_metadata_browser_real_get_version (GmpcPluginBase* base, int* result_length1) {
	GmpcMetadataBrowser * self;
	gint* _tmp0_;
	self = (GmpcMetadataBrowser*) base;
	_tmp0_ = NULL;
	return (_tmp0_ = GMPC_METADATA_BROWSER_version, *result_length1 = G_N_ELEMENTS (GMPC_METADATA_BROWSER_version), _tmp0_);
}


static const char* gmpc_metadata_browser_real_get_name (GmpcPluginBase* base) {
	GmpcMetadataBrowser * self;
	self = (GmpcMetadataBrowser*) base;
	return N_ ("Metadata Browser");
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
			_tmp0_ = NULL;
			indices = (_tmp0_ = gtk_tree_path_get_indices (path), indices_length1 = -1, indices_size = indices_length1, _tmp0_);
			cfg_set_single_value_as_int (config, gmpc_plugin_base_get_name ((GmpcPluginBase*) self), "position", indices[0]);
		}
		(path == NULL) ? NULL : (path = (gtk_tree_path_free (path), NULL));
	}
}


/**
     * This builds the browser
     */
static void gmpc_metadata_browser_browser_bg_style_changed (GmpcMetadataBrowser* self, GtkScrolledWindow* bg, GtkStyle* style) {
	GdkColor _tmp0_ = {0};
	g_return_if_fail (self != NULL);
	g_return_if_fail (bg != NULL);
	gtk_widget_modify_bg ((GtkWidget*) self->priv->metadata_box, GTK_STATE_NORMAL, (_tmp0_ = gtk_widget_get_style ((GtkWidget*) self->priv->metadata_sw)->base[GTK_STATE_NORMAL], &_tmp0_));
}


/* This hack makes clicking a selected row again, unselect it */
static gboolean gmpc_metadata_browser_browser_button_press_event (GmpcMetadataBrowser* self, GtkTreeView* tree, const GdkEventButton* event) {
	GtkTreePath* path;
	GtkTreePath* _tmp3_;
	gboolean _tmp2_;
	GtkTreePath* _tmp1_;
	gboolean _tmp5_;
	g_return_val_if_fail (self != NULL, FALSE);
	g_return_val_if_fail (tree != NULL, FALSE);
	path = NULL;
	if ((*event).button != 1) {
		gboolean _tmp0_;
		return (_tmp0_ = FALSE, (path == NULL) ? NULL : (path = (gtk_tree_path_free (path), NULL)), _tmp0_);
	}
	_tmp3_ = NULL;
	_tmp1_ = NULL;
	if ((_tmp2_ = gtk_tree_view_get_path_at_pos (tree, (gint) (*event).x, (gint) (*event).y, &_tmp1_, NULL, NULL, NULL), path = (_tmp3_ = _tmp1_, (path == NULL) ? NULL : (path = (gtk_tree_path_free (path), NULL)), _tmp3_), _tmp2_)) {
		if (gtk_tree_selection_path_is_selected (gtk_tree_view_get_selection (tree), path)) {
			gboolean _tmp4_;
			gtk_tree_selection_unselect_path (gtk_tree_view_get_selection (tree), path);
			return (_tmp4_ = TRUE, (path == NULL) ? NULL : (path = (gtk_tree_path_free (path), NULL)), _tmp4_);
		}
	}
	return (_tmp5_ = FALSE, (path == NULL) ? NULL : (path = (gtk_tree_path_free (path), NULL)), _tmp5_);
}


/**
     * Artist tree view functions */
static void gmpc_metadata_browser_browser_artist_entry_changed (GmpcMetadataBrowser* self, GtkEntry* entry) {
	const char* _tmp0_;
	char* text;
	g_return_if_fail (self != NULL);
	g_return_if_fail (entry != NULL);
	_tmp0_ = NULL;
	text = (_tmp0_ = gtk_entry_get_text (entry), (_tmp0_ == NULL) ? NULL : g_strdup (_tmp0_));
	if (strlen (text) > 0) {
		gtk_widget_show ((GtkWidget*) entry);
	} else {
		gtk_widget_hide ((GtkWidget*) entry);
		gtk_widget_grab_focus ((GtkWidget*) self->priv->tree_artist);
	}
	gtk_tree_model_filter_refilter (self->priv->model_filter_artist);
	text = (g_free (text), NULL);
}


static void gmpc_metadata_browser_artist_add_clicked (GmpcMetadataBrowser* self, GtkImageMenuItem* item) {
	char* artist;
	g_return_if_fail (self != NULL);
	g_return_if_fail (item != NULL);
	artist = gmpc_metadata_browser_browser_get_selected_artist (self);
	if (artist != NULL) {
		MpdData* data;
		mpd_database_search_field_start (connection, MPD_TAG_ITEM_FILENAME);
		mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ARTIST, artist);
		data = mpd_database_search_commit (connection);
		if (data != NULL) {
			do {
				mpd_playlist_queue_add (connection, data->tag);
				data = mpd_data_get_next (data);
			} while (data != NULL);
			mpd_playlist_queue_commit (connection);
		}
		(data == NULL) ? NULL : (data = (mpd_data_free (data), NULL));
	}
	artist = (g_free (artist), NULL);
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


/* Handle right mouse click */
static gboolean gmpc_metadata_browser_artist_browser_button_release_event (GmpcMetadataBrowser* self, GtkTreeView* tree, const GdkEventButton* event) {
	g_return_val_if_fail (self != NULL, FALSE);
	g_return_val_if_fail (tree != NULL, FALSE);
	if ((*event).button == 3) {
		if (gtk_tree_selection_count_selected_rows (gtk_tree_view_get_selection (tree)) > 0) {
			GtkMenu* menu;
			GtkImageMenuItem* item;
			GtkImageMenuItem* _tmp0_;
			GtkImage* _tmp1_;
			gboolean _tmp2_;
			menu = g_object_ref_sink ((GtkMenu*) gtk_menu_new ());
			item = g_object_ref_sink ((GtkImageMenuItem*) gtk_image_menu_item_new_from_stock ("gtk-add", NULL));
			g_signal_connect_object ((GtkMenuItem*) item, "activate", (GCallback) _gmpc_metadata_browser_artist_add_clicked_gtk_menu_item_activate, self, 0);
			gtk_menu_shell_append ((GtkMenuShell*) menu, (GtkWidget*) ((GtkMenuItem*) item));
			_tmp0_ = NULL;
			item = (_tmp0_ = g_object_ref_sink ((GtkImageMenuItem*) gtk_image_menu_item_new_with_mnemonic ("_Replace")), (item == NULL) ? NULL : (item = (g_object_unref (item), NULL)), _tmp0_);
			_tmp1_ = NULL;
			gtk_image_menu_item_set_image (item, (GtkWidget*) (_tmp1_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_stock ("gtk-redo", GTK_ICON_SIZE_MENU))));
			(_tmp1_ == NULL) ? NULL : (_tmp1_ = (g_object_unref (_tmp1_), NULL));
			g_signal_connect_object ((GtkMenuItem*) item, "activate", (GCallback) _gmpc_metadata_browser_artist_replace_clicked_gtk_menu_item_activate, self, 0);
			gtk_menu_shell_append ((GtkMenuShell*) menu, (GtkWidget*) ((GtkMenuItem*) item));
			gtk_menu_popup (menu, NULL, NULL, NULL, NULL, (*event).button, (*event).time);
			gtk_widget_show_all ((GtkWidget*) menu);
			return (_tmp2_ = TRUE, (menu == NULL) ? NULL : (menu = (g_object_unref (menu), NULL)), (item == NULL) ? NULL : (item = (g_object_unref (item), NULL)), _tmp2_);
		}
	}
	return FALSE;
}


static gboolean gmpc_metadata_browser_visible_func_artist (GmpcMetadataBrowser* self, GtkTreeModel* model, GtkTreeIter* iter) {
	const char* _tmp0_;
	char* text;
	char* str;
	gboolean visible;
	gboolean _tmp2_;
	gboolean _tmp7_;
	g_return_val_if_fail (self != NULL, FALSE);
	g_return_val_if_fail (model != NULL, FALSE);
	_tmp0_ = NULL;
	text = (_tmp0_ = gtk_entry_get_text (self->priv->artist_filter_entry), (_tmp0_ == NULL) ? NULL : g_strdup (_tmp0_));
	/* Visible if row is non-empty and first column is "HI" */
	str = NULL;
	visible = FALSE;
	if (g_utf8_get_char (g_utf8_offset_to_pointer (text, 0)) == '\0') {
		gboolean _tmp1_;
		return (_tmp1_ = TRUE, text = (g_free (text), NULL), str = (g_free (str), NULL), _tmp1_);
	}
	gtk_tree_model_get (model, &(*iter), 7, &str, -1, -1);
	_tmp2_ = FALSE;
	if (str != NULL) {
		char* _tmp6_;
		char* _tmp5_;
		char* _tmp4_;
		char* _tmp3_;
		_tmp6_ = NULL;
		_tmp5_ = NULL;
		_tmp4_ = NULL;
		_tmp3_ = NULL;
		_tmp2_ = strstr (_tmp4_ = g_utf8_normalize (_tmp3_ = g_utf8_casefold (str, -1), -1, G_NORMALIZE_DEFAULT), _tmp6_ = g_utf8_normalize (_tmp5_ = g_utf8_casefold (text, -1), -1, G_NORMALIZE_DEFAULT)) != NULL;
		_tmp6_ = (g_free (_tmp6_), NULL);
		_tmp5_ = (g_free (_tmp5_), NULL);
		_tmp4_ = (g_free (_tmp4_), NULL);
		_tmp3_ = (g_free (_tmp3_), NULL);
	} else {
		_tmp2_ = FALSE;
	}
	if (_tmp2_) {
		visible = TRUE;
	}
	return (_tmp7_ = visible, text = (g_free (text), NULL), str = (g_free (str), NULL), _tmp7_);
}


static gboolean gmpc_metadata_browser_browser_artist_key_press_event (GmpcMetadataBrowser* self, GtkTreeView* widget, const GdkEventKey* event) {
	gunichar uc;
	g_return_val_if_fail (self != NULL, FALSE);
	g_return_val_if_fail (widget != NULL, FALSE);
	uc = (gunichar) gdk_keyval_to_unicode ((*event).keyval);
	if (uc > 0) {
		char* outbuf;
		gint i;
		gboolean _tmp0_;
		outbuf = g_strdup ("       ");
		i = g_unichar_to_utf8 (uc, outbuf);
		((gchar*) outbuf)[i] = '\0';
		gtk_entry_set_text (self->priv->artist_filter_entry, outbuf);
		gtk_widget_grab_focus ((GtkWidget*) self->priv->artist_filter_entry);
		gtk_editable_set_position ((GtkEditable*) self->priv->artist_filter_entry, 1);
		return (_tmp0_ = TRUE, outbuf = (g_free (outbuf), NULL), _tmp0_);
	}
	return FALSE;
}


/** 
      * Album tree view
      */
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
					char* _tmp1_;
					const char* _tmp0_;
					_tmp1_ = NULL;
					_tmp0_ = NULL;
					albumartist = (_tmp1_ = (_tmp0_ = ydata->tag, (_tmp0_ == NULL) ? NULL : g_strdup (_tmp0_)), albumartist = (g_free (albumartist), NULL), _tmp1_);
				}
			}
			(ydata == NULL) ? NULL : (ydata = (mpd_data_free (ydata), NULL));
		}
		/* Fill in the first browser */
		mpd_database_search_field_start (connection, MPD_TAG_ITEM_FILENAME);
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
			do {
				mpd_playlist_queue_add (connection, data->tag);
				data = mpd_data_get_next (data);
			} while (data != NULL);
			mpd_playlist_queue_commit (connection);
		}
		albumartist = (g_free (albumartist), NULL);
		album = (g_free (album), NULL);
		(data == NULL) ? NULL : (data = (mpd_data_free (data), NULL));
	}
	artist = (g_free (artist), NULL);
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


/* Handle right mouse click */
static gboolean gmpc_metadata_browser_album_browser_button_release_event (GmpcMetadataBrowser* self, GtkTreeView* tree, const GdkEventButton* event) {
	g_return_val_if_fail (self != NULL, FALSE);
	g_return_val_if_fail (tree != NULL, FALSE);
	if ((*event).button == 3) {
		if (gtk_tree_selection_count_selected_rows (gtk_tree_view_get_selection (tree)) > 0) {
			GtkMenu* menu;
			GtkImageMenuItem* item;
			GtkImageMenuItem* _tmp0_;
			GtkImage* _tmp1_;
			gboolean _tmp2_;
			menu = g_object_ref_sink ((GtkMenu*) gtk_menu_new ());
			item = g_object_ref_sink ((GtkImageMenuItem*) gtk_image_menu_item_new_from_stock ("gtk-add", NULL));
			g_signal_connect_object ((GtkMenuItem*) item, "activate", (GCallback) _gmpc_metadata_browser_album_add_clicked_gtk_menu_item_activate, self, 0);
			gtk_menu_shell_append ((GtkMenuShell*) menu, (GtkWidget*) ((GtkMenuItem*) item));
			_tmp0_ = NULL;
			item = (_tmp0_ = g_object_ref_sink ((GtkImageMenuItem*) gtk_image_menu_item_new_with_mnemonic ("_Replace")), (item == NULL) ? NULL : (item = (g_object_unref (item), NULL)), _tmp0_);
			_tmp1_ = NULL;
			gtk_image_menu_item_set_image (item, (GtkWidget*) (_tmp1_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_stock ("gtk-redo", GTK_ICON_SIZE_MENU))));
			(_tmp1_ == NULL) ? NULL : (_tmp1_ = (g_object_unref (_tmp1_), NULL));
			g_signal_connect_object ((GtkMenuItem*) item, "activate", (GCallback) _gmpc_metadata_browser_album_replace_clicked_gtk_menu_item_activate, self, 0);
			gtk_menu_shell_append ((GtkMenuShell*) menu, (GtkWidget*) ((GtkMenuItem*) item));
			gtk_menu_popup (menu, NULL, NULL, NULL, NULL, (*event).button, (*event).time);
			gtk_widget_show_all ((GtkWidget*) menu);
			return (_tmp2_ = TRUE, (menu == NULL) ? NULL : (menu = (g_object_unref (menu), NULL)), (item == NULL) ? NULL : (item = (g_object_unref (item), NULL)), _tmp2_);
		}
	}
	return FALSE;
}


static gboolean gmpc_metadata_browser_visible_func_album (GmpcMetadataBrowser* self, GtkTreeModel* model, GtkTreeIter* iter) {
	const char* _tmp0_;
	char* text;
	char* str;
	gboolean visible;
	gboolean _tmp2_;
	gboolean _tmp7_;
	g_return_val_if_fail (self != NULL, FALSE);
	g_return_val_if_fail (model != NULL, FALSE);
	_tmp0_ = NULL;
	text = (_tmp0_ = gtk_entry_get_text (self->priv->album_filter_entry), (_tmp0_ == NULL) ? NULL : g_strdup (_tmp0_));
	/* Visible if row is non-empty and first column is "HI" */
	str = NULL;
	visible = FALSE;
	if (g_utf8_get_char (g_utf8_offset_to_pointer (text, 0)) == '\0') {
		gboolean _tmp1_;
		return (_tmp1_ = TRUE, text = (g_free (text), NULL), str = (g_free (str), NULL), _tmp1_);
	}
	gtk_tree_model_get (model, &(*iter), 6, &str, -1, -1);
	_tmp2_ = FALSE;
	if (str != NULL) {
		char* _tmp6_;
		char* _tmp5_;
		char* _tmp4_;
		char* _tmp3_;
		_tmp6_ = NULL;
		_tmp5_ = NULL;
		_tmp4_ = NULL;
		_tmp3_ = NULL;
		_tmp2_ = strstr (_tmp4_ = g_utf8_normalize (_tmp3_ = g_utf8_casefold (str, -1), -1, G_NORMALIZE_DEFAULT), _tmp6_ = g_utf8_normalize (_tmp5_ = g_utf8_casefold (text, -1), -1, G_NORMALIZE_DEFAULT)) != NULL;
		_tmp6_ = (g_free (_tmp6_), NULL);
		_tmp5_ = (g_free (_tmp5_), NULL);
		_tmp4_ = (g_free (_tmp4_), NULL);
		_tmp3_ = (g_free (_tmp3_), NULL);
	} else {
		_tmp2_ = FALSE;
	}
	if (_tmp2_) {
		visible = TRUE;
	}
	return (_tmp7_ = visible, text = (g_free (text), NULL), str = (g_free (str), NULL), _tmp7_);
}


static gboolean gmpc_metadata_browser_browser_album_key_press_event (GmpcMetadataBrowser* self, GtkTreeView* widget, const GdkEventKey* event) {
	gunichar uc;
	g_return_val_if_fail (self != NULL, FALSE);
	g_return_val_if_fail (widget != NULL, FALSE);
	uc = (gunichar) gdk_keyval_to_unicode ((*event).keyval);
	if (uc > 0) {
		char* outbuf;
		gint i;
		gboolean _tmp0_;
		outbuf = g_strdup ("       ");
		i = g_unichar_to_utf8 (uc, outbuf);
		((gchar*) outbuf)[i] = '\0';
		gtk_entry_set_text (self->priv->album_filter_entry, outbuf);
		gtk_widget_grab_focus ((GtkWidget*) self->priv->album_filter_entry);
		gtk_editable_set_position ((GtkEditable*) self->priv->album_filter_entry, 1);
		return (_tmp0_ = TRUE, outbuf = (g_free (outbuf), NULL), _tmp0_);
	}
	return FALSE;
}


static void gmpc_metadata_browser_browser_album_entry_changed (GmpcMetadataBrowser* self, GtkEntry* entry) {
	const char* _tmp0_;
	char* text;
	g_return_if_fail (self != NULL);
	g_return_if_fail (entry != NULL);
	_tmp0_ = NULL;
	text = (_tmp0_ = gtk_entry_get_text (entry), (_tmp0_ == NULL) ? NULL : g_strdup (_tmp0_));
	if (strlen (text) > 0) {
		gtk_widget_show ((GtkWidget*) entry);
	} else {
		gtk_widget_hide ((GtkWidget*) entry);
		gtk_widget_grab_focus ((GtkWidget*) self->priv->tree_album);
	}
	gtk_tree_model_filter_refilter (self->priv->model_filter_album);
	text = (g_free (text), NULL);
}


/**
      * Songs 
      */
static void gmpc_metadata_browser_song_add_clicked (GmpcMetadataBrowser* self, GtkImageMenuItem* item) {
	mpd_Song* song;
	g_return_if_fail (self != NULL);
	g_return_if_fail (item != NULL);
	song = gmpc_metadata_browser_browser_get_selected_song (self);
	if (song != NULL) {
		mpd_playlist_add (connection, song->file);
	}
	(song == NULL) ? NULL : (song = (mpd_freeSong (song), NULL));
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


/* Handle right mouse click */
static gboolean gmpc_metadata_browser_song_browser_button_release_event (GmpcMetadataBrowser* self, GtkTreeView* tree, const GdkEventButton* event) {
	g_return_val_if_fail (self != NULL, FALSE);
	g_return_val_if_fail (tree != NULL, FALSE);
	if ((*event).button == 3) {
		if (gtk_tree_selection_count_selected_rows (gtk_tree_view_get_selection (tree)) > 0) {
			GtkMenu* menu;
			GtkImageMenuItem* item;
			GtkImageMenuItem* _tmp0_;
			GtkImage* _tmp1_;
			gboolean _tmp2_;
			menu = g_object_ref_sink ((GtkMenu*) gtk_menu_new ());
			item = g_object_ref_sink ((GtkImageMenuItem*) gtk_image_menu_item_new_from_stock ("gtk-add", NULL));
			g_signal_connect_object ((GtkMenuItem*) item, "activate", (GCallback) _gmpc_metadata_browser_song_add_clicked_gtk_menu_item_activate, self, 0);
			gtk_menu_shell_append ((GtkMenuShell*) menu, (GtkWidget*) ((GtkMenuItem*) item));
			_tmp0_ = NULL;
			item = (_tmp0_ = g_object_ref_sink ((GtkImageMenuItem*) gtk_image_menu_item_new_with_mnemonic ("_Replace")), (item == NULL) ? NULL : (item = (g_object_unref (item), NULL)), _tmp0_);
			_tmp1_ = NULL;
			gtk_image_menu_item_set_image (item, (GtkWidget*) (_tmp1_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_stock ("gtk-redo", GTK_ICON_SIZE_MENU))));
			(_tmp1_ == NULL) ? NULL : (_tmp1_ = (g_object_unref (_tmp1_), NULL));
			g_signal_connect_object ((GtkMenuItem*) item, "activate", (GCallback) _gmpc_metadata_browser_song_replace_clicked_gtk_menu_item_activate, self, 0);
			gtk_menu_shell_append ((GtkMenuShell*) menu, (GtkWidget*) ((GtkMenuItem*) item));
			gtk_menu_popup (menu, NULL, NULL, NULL, NULL, (*event).button, (*event).time);
			gtk_widget_show_all ((GtkWidget*) menu);
			return (_tmp2_ = TRUE, (menu == NULL) ? NULL : (menu = (g_object_unref (menu), NULL)), (item == NULL) ? NULL : (item = (g_object_unref (item), NULL)), _tmp2_);
		}
	}
	return FALSE;
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
		GtkTreeViewColumn* column;
		GtkCellRendererPixbuf* prenderer;
		GtkCellRendererText* trenderer;
		GtkVBox* _tmp6_;
		GtkEntry* _tmp7_;
		GtkScrolledWindow* _tmp8_;
		GmpcMpdDataModel* _tmp9_;
		GtkTreeModelFilter* _tmp10_;
		GtkTreeView* _tmp11_;
		GtkTreeViewColumn* _tmp12_;
		GtkCellRendererPixbuf* _tmp13_;
		GtkTreeViewColumn* _tmp14_;
		GtkCellRendererText* _tmp15_;
		GtkTreeViewColumn* _tmp16_;
		GtkCellRendererText* _tmp17_;
		GtkScrolledWindow* _tmp18_;
		GmpcMpdDataModel* _tmp19_;
		GtkTreeView* _tmp20_;
		GtkTreeViewColumn* _tmp21_;
		GtkCellRendererPixbuf* _tmp22_;
		GtkCellRendererText* _tmp23_;
		GtkTreeViewColumn* _tmp24_;
		GtkCellRendererText* _tmp25_;
		GtkScrolledWindow* _tmp26_;
		GtkEventBox* _tmp27_;
		_tmp0_ = NULL;
		self->priv->paned = (_tmp0_ = (GtkPaned*) g_object_ref_sink ((GtkHPaned*) gtk_hpaned_new ()), (self->priv->paned == NULL) ? NULL : (self->priv->paned = (g_object_unref (self->priv->paned), NULL)), _tmp0_);
		gtk_paned_set_position (self->priv->paned, cfg_get_single_value_as_int_with_default (config, gmpc_plugin_base_get_name ((GmpcPluginBase*) self), "pane-pos", 150));
		/* Bow with browsers */
		_tmp1_ = NULL;
		self->priv->browser_box = (_tmp1_ = (GtkBox*) g_object_ref_sink ((GtkVBox*) gtk_vbox_new (TRUE, 6)), (self->priv->browser_box == NULL) ? NULL : (self->priv->browser_box = (g_object_unref (self->priv->browser_box), NULL)), _tmp1_);
		gtk_paned_add1 (self->priv->paned, (GtkWidget*) self->priv->browser_box);
		/* Artist list  */
		box = g_object_ref_sink ((GtkVBox*) gtk_vbox_new (FALSE, 6));
		gtk_box_pack_start (self->priv->browser_box, (GtkWidget*) box, TRUE, TRUE, (guint) 0);
		_tmp2_ = NULL;
		self->priv->artist_filter_entry = (_tmp2_ = g_object_ref_sink ((GtkEntry*) gtk_entry_new ()), (self->priv->artist_filter_entry == NULL) ? NULL : (self->priv->artist_filter_entry = (g_object_unref (self->priv->artist_filter_entry), NULL)), _tmp2_);
		gtk_widget_set_no_show_all ((GtkWidget*) self->priv->artist_filter_entry, TRUE);
		g_signal_connect_object ((GtkEditable*) self->priv->artist_filter_entry, "changed", (GCallback) _gmpc_metadata_browser_browser_artist_entry_changed_gtk_editable_changed, self, 0);
		gtk_box_pack_start ((GtkBox*) box, (GtkWidget*) self->priv->artist_filter_entry, FALSE, FALSE, (guint) 0);
		sw = g_object_ref_sink ((GtkScrolledWindow*) gtk_scrolled_window_new (NULL, NULL));
		gtk_scrolled_window_set_policy (sw, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
		gtk_scrolled_window_set_shadow_type (sw, GTK_SHADOW_ETCHED_IN);
		gtk_box_pack_start ((GtkBox*) box, (GtkWidget*) sw, TRUE, TRUE, (guint) 0);
		_tmp3_ = NULL;
		self->priv->model_artist = (_tmp3_ = gmpc_mpddata_model_new (), (self->priv->model_artist == NULL) ? NULL : (self->priv->model_artist = (g_object_unref (self->priv->model_artist), NULL)), _tmp3_);
		_tmp4_ = NULL;
		self->priv->model_filter_artist = (_tmp4_ = (GtkTreeModelFilter*) gtk_tree_model_filter_new ((GtkTreeModel*) self->priv->model_artist, NULL), (self->priv->model_filter_artist == NULL) ? NULL : (self->priv->model_filter_artist = (g_object_unref (self->priv->model_filter_artist), NULL)), _tmp4_);
		gtk_tree_model_filter_set_visible_func (self->priv->model_filter_artist, _gmpc_metadata_browser_visible_func_artist_gtk_tree_model_filter_visible_func, g_object_ref (self), g_object_unref);
		_tmp5_ = NULL;
		self->priv->tree_artist = (_tmp5_ = g_object_ref_sink ((GtkTreeView*) gtk_tree_view_new_with_model ((GtkTreeModel*) self->priv->model_filter_artist)), (self->priv->tree_artist == NULL) ? NULL : (self->priv->tree_artist = (g_object_unref (self->priv->tree_artist), NULL)), _tmp5_);
		g_signal_connect_object ((GtkWidget*) self->priv->tree_artist, "button-press-event", (GCallback) _gmpc_metadata_browser_browser_button_press_event_gtk_widget_button_press_event, self, 0);
		g_signal_connect_object ((GtkWidget*) self->priv->tree_artist, "button-release-event", (GCallback) _gmpc_metadata_browser_artist_browser_button_release_event_gtk_widget_button_release_event, self, 0);
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
		_tmp6_ = NULL;
		box = (_tmp6_ = g_object_ref_sink ((GtkVBox*) gtk_vbox_new (FALSE, 6)), (box == NULL) ? NULL : (box = (g_object_unref (box), NULL)), _tmp6_);
		gtk_box_pack_start (self->priv->browser_box, (GtkWidget*) box, TRUE, TRUE, (guint) 0);
		_tmp7_ = NULL;
		self->priv->album_filter_entry = (_tmp7_ = g_object_ref_sink ((GtkEntry*) gtk_entry_new ()), (self->priv->album_filter_entry == NULL) ? NULL : (self->priv->album_filter_entry = (g_object_unref (self->priv->album_filter_entry), NULL)), _tmp7_);
		gtk_widget_set_no_show_all ((GtkWidget*) self->priv->album_filter_entry, TRUE);
		g_signal_connect_object ((GtkEditable*) self->priv->album_filter_entry, "changed", (GCallback) _gmpc_metadata_browser_browser_album_entry_changed_gtk_editable_changed, self, 0);
		gtk_box_pack_start ((GtkBox*) box, (GtkWidget*) self->priv->album_filter_entry, FALSE, FALSE, (guint) 0);
		_tmp8_ = NULL;
		sw = (_tmp8_ = g_object_ref_sink ((GtkScrolledWindow*) gtk_scrolled_window_new (NULL, NULL)), (sw == NULL) ? NULL : (sw = (g_object_unref (sw), NULL)), _tmp8_);
		gtk_scrolled_window_set_policy (sw, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
		gtk_scrolled_window_set_shadow_type (sw, GTK_SHADOW_ETCHED_IN);
		gtk_box_pack_start ((GtkBox*) box, (GtkWidget*) sw, TRUE, TRUE, (guint) 0);
		_tmp9_ = NULL;
		self->priv->model_albums = (_tmp9_ = gmpc_mpddata_model_new (), (self->priv->model_albums == NULL) ? NULL : (self->priv->model_albums = (g_object_unref (self->priv->model_albums), NULL)), _tmp9_);
		_tmp10_ = NULL;
		self->priv->model_filter_album = (_tmp10_ = (GtkTreeModelFilter*) gtk_tree_model_filter_new ((GtkTreeModel*) self->priv->model_albums, NULL), (self->priv->model_filter_album == NULL) ? NULL : (self->priv->model_filter_album = (g_object_unref (self->priv->model_filter_album), NULL)), _tmp10_);
		gtk_tree_model_filter_set_visible_func (self->priv->model_filter_album, _gmpc_metadata_browser_visible_func_album_gtk_tree_model_filter_visible_func, g_object_ref (self), g_object_unref);
		_tmp11_ = NULL;
		self->priv->tree_album = (_tmp11_ = g_object_ref_sink ((GtkTreeView*) gtk_tree_view_new_with_model ((GtkTreeModel*) self->priv->model_filter_album)), (self->priv->tree_album == NULL) ? NULL : (self->priv->tree_album = (g_object_unref (self->priv->tree_album), NULL)), _tmp11_);
		g_signal_connect_object ((GtkWidget*) self->priv->tree_album, "button-press-event", (GCallback) _gmpc_metadata_browser_browser_button_press_event_gtk_widget_button_press_event, self, 0);
		g_signal_connect_object ((GtkWidget*) self->priv->tree_album, "button-release-event", (GCallback) _gmpc_metadata_browser_album_browser_button_release_event_gtk_widget_button_release_event, self, 0);
		g_signal_connect_object ((GtkWidget*) self->priv->tree_album, "key-press-event", (GCallback) _gmpc_metadata_browser_browser_album_key_press_event_gtk_widget_key_press_event, self, 0);
		gtk_container_add ((GtkContainer*) sw, (GtkWidget*) self->priv->tree_album);
		/* setup the columns */
		_tmp12_ = NULL;
		column = (_tmp12_ = g_object_ref_sink (gtk_tree_view_column_new ()), (column == NULL) ? NULL : (column = (g_object_unref (column), NULL)), _tmp12_);
		_tmp13_ = NULL;
		prenderer = (_tmp13_ = g_object_ref_sink ((GtkCellRendererPixbuf*) gtk_cell_renderer_pixbuf_new ()), (prenderer == NULL) ? NULL : (prenderer = (g_object_unref (prenderer), NULL)), _tmp13_);
		g_object_set ((GObject*) prenderer, "height", self->priv->model_albums->icon_size, NULL);
		gtk_cell_layout_pack_start ((GtkCellLayout*) column, (GtkCellRenderer*) prenderer, FALSE);
		gtk_cell_layout_add_attribute ((GtkCellLayout*) column, (GtkCellRenderer*) prenderer, "pixbuf", 27);
		gtk_tree_view_append_column (self->priv->tree_album, column);
		_tmp14_ = NULL;
		column = (_tmp14_ = g_object_ref_sink (gtk_tree_view_column_new ()), (column == NULL) ? NULL : (column = (g_object_unref (column), NULL)), _tmp14_);
		_tmp15_ = NULL;
		trenderer = (_tmp15_ = g_object_ref_sink ((GtkCellRendererText*) gtk_cell_renderer_text_new ()), (trenderer == NULL) ? NULL : (trenderer = (g_object_unref (trenderer), NULL)), _tmp15_);
		gtk_cell_layout_pack_start ((GtkCellLayout*) column, (GtkCellRenderer*) trenderer, TRUE);
		gtk_cell_layout_add_attribute ((GtkCellLayout*) column, (GtkCellRenderer*) trenderer, "text", 14);
		gtk_tree_view_append_column (self->priv->tree_album, column);
		gtk_tree_view_column_set_title (column, _ ("Year"));
		_tmp16_ = NULL;
		column = (_tmp16_ = g_object_ref_sink (gtk_tree_view_column_new ()), (column == NULL) ? NULL : (column = (g_object_unref (column), NULL)), _tmp16_);
		_tmp17_ = NULL;
		trenderer = (_tmp17_ = g_object_ref_sink ((GtkCellRendererText*) gtk_cell_renderer_text_new ()), (trenderer == NULL) ? NULL : (trenderer = (g_object_unref (trenderer), NULL)), _tmp17_);
		gtk_cell_layout_pack_start ((GtkCellLayout*) column, (GtkCellRenderer*) trenderer, TRUE);
		gtk_cell_layout_add_attribute ((GtkCellLayout*) column, (GtkCellRenderer*) trenderer, "text", 6);
		gtk_tree_view_append_column (self->priv->tree_album, column);
		gtk_tree_view_set_search_column (self->priv->tree_album, 6);
		gtk_tree_view_column_set_title (column, _ ("Album"));
		g_signal_connect_object (gtk_tree_view_get_selection (self->priv->tree_album), "changed", (GCallback) _gmpc_metadata_browser_browser_album_changed_gtk_tree_selection_changed, self, 0);
		/* Song list */
		_tmp18_ = NULL;
		sw = (_tmp18_ = g_object_ref_sink ((GtkScrolledWindow*) gtk_scrolled_window_new (NULL, NULL)), (sw == NULL) ? NULL : (sw = (g_object_unref (sw), NULL)), _tmp18_);
		gtk_scrolled_window_set_policy (sw, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
		gtk_scrolled_window_set_shadow_type (sw, GTK_SHADOW_ETCHED_IN);
		gtk_box_pack_start (self->priv->browser_box, (GtkWidget*) sw, TRUE, TRUE, (guint) 0);
		_tmp19_ = NULL;
		self->priv->model_songs = (_tmp19_ = gmpc_mpddata_model_new (), (self->priv->model_songs == NULL) ? NULL : (self->priv->model_songs = (g_object_unref (self->priv->model_songs), NULL)), _tmp19_);
		_tmp20_ = NULL;
		self->priv->tree_songs = (_tmp20_ = g_object_ref_sink ((GtkTreeView*) gtk_tree_view_new_with_model ((GtkTreeModel*) self->priv->model_songs)), (self->priv->tree_songs == NULL) ? NULL : (self->priv->tree_songs = (g_object_unref (self->priv->tree_songs), NULL)), _tmp20_);
		g_signal_connect_object ((GtkWidget*) self->priv->tree_songs, "button-press-event", (GCallback) _gmpc_metadata_browser_browser_button_press_event_gtk_widget_button_press_event, self, 0);
		g_signal_connect_object ((GtkWidget*) self->priv->tree_songs, "button-release-event", (GCallback) _gmpc_metadata_browser_song_browser_button_release_event_gtk_widget_button_release_event, self, 0);
		gtk_container_add ((GtkContainer*) sw, (GtkWidget*) self->priv->tree_songs);
		/* setup the columns */
		_tmp21_ = NULL;
		column = (_tmp21_ = g_object_ref_sink (gtk_tree_view_column_new ()), (column == NULL) ? NULL : (column = (g_object_unref (column), NULL)), _tmp21_);
		_tmp22_ = NULL;
		prenderer = (_tmp22_ = g_object_ref_sink ((GtkCellRendererPixbuf*) gtk_cell_renderer_pixbuf_new ()), (prenderer == NULL) ? NULL : (prenderer = (g_object_unref (prenderer), NULL)), _tmp22_);
		gtk_cell_layout_pack_start ((GtkCellLayout*) column, (GtkCellRenderer*) prenderer, FALSE);
		gtk_cell_layout_add_attribute ((GtkCellLayout*) column, (GtkCellRenderer*) prenderer, "icon-name", 23);
		_tmp23_ = NULL;
		trenderer = (_tmp23_ = g_object_ref_sink ((GtkCellRendererText*) gtk_cell_renderer_text_new ()), (trenderer == NULL) ? NULL : (trenderer = (g_object_unref (trenderer), NULL)), _tmp23_);
		gtk_cell_layout_pack_start ((GtkCellLayout*) column, (GtkCellRenderer*) trenderer, FALSE);
		gtk_cell_layout_add_attribute ((GtkCellLayout*) column, (GtkCellRenderer*) trenderer, "text", 10);
		gtk_tree_view_column_set_title (column, _ ("Track"));
		gtk_tree_view_append_column (self->priv->tree_songs, column);
		_tmp24_ = NULL;
		column = (_tmp24_ = g_object_ref_sink (gtk_tree_view_column_new ()), (column == NULL) ? NULL : (column = (g_object_unref (column), NULL)), _tmp24_);
		_tmp25_ = NULL;
		trenderer = (_tmp25_ = g_object_ref_sink ((GtkCellRendererText*) gtk_cell_renderer_text_new ()), (trenderer == NULL) ? NULL : (trenderer = (g_object_unref (trenderer), NULL)), _tmp25_);
		gtk_cell_layout_pack_start ((GtkCellLayout*) column, (GtkCellRenderer*) trenderer, TRUE);
		gtk_cell_layout_add_attribute ((GtkCellLayout*) column, (GtkCellRenderer*) trenderer, "text", 7);
		gtk_tree_view_append_column (self->priv->tree_songs, column);
		gtk_tree_view_set_search_column (self->priv->tree_songs, 7);
		gtk_tree_view_column_set_title (column, _ ("Songs"));
		g_signal_connect_object (gtk_tree_view_get_selection (self->priv->tree_songs), "changed", (GCallback) _gmpc_metadata_browser_browser_songs_changed_gtk_tree_selection_changed, self, 0);
		/* The right view */
		_tmp26_ = NULL;
		self->priv->metadata_sw = (_tmp26_ = g_object_ref_sink ((GtkScrolledWindow*) gtk_scrolled_window_new (NULL, NULL)), (self->priv->metadata_sw == NULL) ? NULL : (self->priv->metadata_sw = (g_object_unref (self->priv->metadata_sw), NULL)), _tmp26_);
		gtk_scrolled_window_set_policy (self->priv->metadata_sw, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
		g_signal_connect_object ((GtkWidget*) self->priv->metadata_sw, "style-set", (GCallback) _gmpc_metadata_browser_browser_bg_style_changed_gtk_widget_style_set, self, 0);
		_tmp27_ = NULL;
		self->priv->metadata_box = (_tmp27_ = g_object_ref_sink ((GtkEventBox*) gtk_event_box_new ()), (self->priv->metadata_box == NULL) ? NULL : (self->priv->metadata_box = (g_object_unref (self->priv->metadata_box), NULL)), _tmp27_);
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
	MpdData* _tmp2_;
	const MpdData* _tmp1_;
	MpdData* _tmp0_;
	MpdData* _tmp3_;
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
	_tmp2_ = NULL;
	_tmp1_ = NULL;
	_tmp0_ = NULL;
	data = (_tmp2_ = (_tmp1_ = misc_sort_mpddata_by_album_disc_track ((_tmp0_ = data, data = NULL, _tmp0_)), (_tmp1_ == NULL) ? NULL :  (_tmp1_)), (data == NULL) ? NULL : (data = (mpd_data_free (data), NULL)), _tmp2_);
	_tmp3_ = NULL;
	gmpc_mpddata_model_set_mpd_data (self->priv->model_artist, (_tmp3_ = data, data = NULL, _tmp3_));
	(data == NULL) ? NULL : (data = (mpd_data_free (data), NULL));
}


static char* gmpc_metadata_browser_browser_get_selected_artist (GmpcMetadataBrowser* self) {
	GtkTreeIter iter = {0};
	GtkTreeSelection* _tmp0_;
	GtkTreeSelection* sel;
	GtkTreeModel* model;
	GtkTreeModel* _tmp4_;
	GtkTreeModel* _tmp3_;
	gboolean _tmp2_;
	GtkTreeModel* _tmp1_;
	char* _tmp6_;
	g_return_val_if_fail (self != NULL, NULL);
	_tmp0_ = NULL;
	sel = (_tmp0_ = gtk_tree_view_get_selection (self->priv->tree_artist), (_tmp0_ == NULL) ? NULL : g_object_ref (_tmp0_));
	model = NULL;
	/*this.model_artist;*/
	_tmp4_ = NULL;
	_tmp3_ = NULL;
	_tmp1_ = NULL;
	if ((_tmp2_ = gtk_tree_selection_get_selected (sel, &_tmp1_, &iter), model = (_tmp3_ = (_tmp4_ = _tmp1_, (_tmp4_ == NULL) ? NULL : g_object_ref (_tmp4_)), (model == NULL) ? NULL : (model = (g_object_unref (model), NULL)), _tmp3_), _tmp2_)) {
		char* artist;
		char* _tmp5_;
		artist = NULL;
		gtk_tree_model_get (model, &iter, 7, &artist, -1, -1);
		_tmp5_ = NULL;
		return (_tmp5_ = artist, (sel == NULL) ? NULL : (sel = (g_object_unref (sel), NULL)), (model == NULL) ? NULL : (model = (g_object_unref (model), NULL)), _tmp5_);
	}
	_tmp6_ = NULL;
	return (_tmp6_ = NULL, (sel == NULL) ? NULL : (sel = (g_object_unref (sel), NULL)), (model == NULL) ? NULL : (model = (g_object_unref (model), NULL)), _tmp6_);
}


static char* gmpc_metadata_browser_browser_get_selected_album (GmpcMetadataBrowser* self) {
	GtkTreeIter iter = {0};
	GtkTreeSelection* _tmp0_;
	GtkTreeSelection* sel;
	GtkTreeModel* model;
	GtkTreeModel* _tmp4_;
	GtkTreeModel* _tmp3_;
	gboolean _tmp2_;
	GtkTreeModel* _tmp1_;
	char* _tmp6_;
	g_return_val_if_fail (self != NULL, NULL);
	_tmp0_ = NULL;
	sel = (_tmp0_ = gtk_tree_view_get_selection (self->priv->tree_album), (_tmp0_ == NULL) ? NULL : g_object_ref (_tmp0_));
	model = NULL;
	/*this.model_albums;*/
	_tmp4_ = NULL;
	_tmp3_ = NULL;
	_tmp1_ = NULL;
	if ((_tmp2_ = gtk_tree_selection_get_selected (sel, &_tmp1_, &iter), model = (_tmp3_ = (_tmp4_ = _tmp1_, (_tmp4_ == NULL) ? NULL : g_object_ref (_tmp4_)), (model == NULL) ? NULL : (model = (g_object_unref (model), NULL)), _tmp3_), _tmp2_)) {
		char* album;
		char* _tmp5_;
		album = NULL;
		gtk_tree_model_get (model, &iter, 6, &album, -1, -1);
		_tmp5_ = NULL;
		return (_tmp5_ = album, (sel == NULL) ? NULL : (sel = (g_object_unref (sel), NULL)), (model == NULL) ? NULL : (model = (g_object_unref (model), NULL)), _tmp5_);
	}
	_tmp6_ = NULL;
	return (_tmp6_ = NULL, (sel == NULL) ? NULL : (sel = (g_object_unref (sel), NULL)), (model == NULL) ? NULL : (model = (g_object_unref (model), NULL)), _tmp6_);
}


static mpd_Song* gmpc_metadata_browser_browser_get_selected_song (GmpcMetadataBrowser* self) {
	GtkTreeIter iter = {0};
	GtkTreeSelection* _tmp0_;
	GtkTreeSelection* sel;
	GmpcMpdDataModel* _tmp4_;
	GmpcMpdDataModel* _tmp3_;
	gboolean _tmp2_;
	GtkTreeModel* _tmp1_;
	mpd_Song* _tmp7_;
	g_return_val_if_fail (self != NULL, NULL);
	_tmp0_ = NULL;
	sel = (_tmp0_ = gtk_tree_view_get_selection (self->priv->tree_songs), (_tmp0_ == NULL) ? NULL : g_object_ref (_tmp0_));
	_tmp4_ = NULL;
	_tmp3_ = NULL;
	_tmp1_ = NULL;
	if ((_tmp2_ = gtk_tree_selection_get_selected (sel, &_tmp1_, &iter), self->priv->model_songs = (_tmp3_ = (_tmp4_ = (GmpcMpdDataModel*) _tmp1_, (_tmp4_ == NULL) ? NULL : g_object_ref (_tmp4_)), (self->priv->model_songs == NULL) ? NULL : (self->priv->model_songs = (g_object_unref (self->priv->model_songs), NULL)), _tmp3_), _tmp2_)) {
		const mpd_Song* songs;
		const mpd_Song* _tmp5_;
		mpd_Song* _tmp6_;
		songs = NULL;
		gtk_tree_model_get ((GtkTreeModel*) self->priv->model_songs, &iter, 0, &songs, -1, -1);
		_tmp5_ = NULL;
		_tmp6_ = NULL;
		return (_tmp6_ = (_tmp5_ = songs, (_tmp5_ == NULL) ? NULL : mpd_songDup (_tmp5_)), (sel == NULL) ? NULL : (sel = (g_object_unref (sel), NULL)), _tmp6_);
	}
	_tmp7_ = NULL;
	return (_tmp7_ = NULL, (sel == NULL) ? NULL : (sel = (g_object_unref (sel), NULL)), _tmp7_);
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
		MpdData* _tmp2_;
		const MpdData* _tmp1_;
		MpdData* _tmp0_;
		MpdData* list;
		const MpdData* iter;
		MpdData* _tmp12_;
		const MpdData* _tmp11_;
		MpdData* _tmp10_;
		MpdData* _tmp13_;
		MpdData* _tmp14_;
		MpdData* _tmp17_;
		const MpdData* _tmp16_;
		MpdData* _tmp15_;
		MpdData* _tmp18_;
		/* Fill in the first browser */
		mpd_database_search_field_start (connection, MPD_TAG_ITEM_ALBUM);
		mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ARTIST, artist);
		data = mpd_database_search_commit (connection);
		_tmp2_ = NULL;
		_tmp1_ = NULL;
		_tmp0_ = NULL;
		data = (_tmp2_ = (_tmp1_ = misc_sort_mpddata_by_album_disc_track ((_tmp0_ = data, data = NULL, _tmp0_)), (_tmp1_ == NULL) ? NULL :  (_tmp1_)), (data == NULL) ? NULL : (data = (mpd_data_free (data), NULL)), _tmp2_);
		gmpc_mpddata_model_set_request_artist (self->priv->model_albums, artist);
		list = NULL;
		iter = mpd_data_get_first (data);
		if (iter != NULL) {
			do {
				mpd_Song* _tmp3_;
				char* _tmp5_;
				const char* _tmp4_;
				char* _tmp7_;
				const char* _tmp6_;
				MpdData* ydata;
				list = mpd_new_data_struct_append (list);
				list->type = MPD_DATA_TYPE_SONG;
				_tmp3_ = NULL;
				list->song = (_tmp3_ = mpd_newSong (), (list->song == NULL) ? NULL : (list->song = (mpd_freeSong (list->song), NULL)), _tmp3_);
				_tmp5_ = NULL;
				_tmp4_ = NULL;
				list->song->artist = (_tmp5_ = (_tmp4_ = artist, (_tmp4_ == NULL) ? NULL : g_strdup (_tmp4_)), list->song->artist = (g_free (list->song->artist), NULL), _tmp5_);
				_tmp7_ = NULL;
				_tmp6_ = NULL;
				list->song->album = (_tmp7_ = (_tmp6_ = iter->tag, (_tmp6_ == NULL) ? NULL : g_strdup (_tmp6_)), list->song->album = (g_free (list->song->album), NULL), _tmp7_);
				mpd_database_search_field_start (connection, MPD_TAG_ITEM_DATE);
				mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ARTIST, artist);
				mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ALBUM, iter->tag);
				ydata = mpd_database_search_commit (connection);
				if (ydata != NULL) {
					char* _tmp9_;
					const char* _tmp8_;
					_tmp9_ = NULL;
					_tmp8_ = NULL;
					list->song->date = (_tmp9_ = (_tmp8_ = ydata->tag, (_tmp8_ == NULL) ? NULL : g_strdup (_tmp8_)), list->song->date = (g_free (list->song->date), NULL), _tmp9_);
				}
				iter = mpd_data_get_next_real (iter, FALSE);
				(ydata == NULL) ? NULL : (ydata = (mpd_data_free (ydata), NULL));
			} while (iter != NULL);
		}
		_tmp12_ = NULL;
		_tmp11_ = NULL;
		_tmp10_ = NULL;
		list = (_tmp12_ = (_tmp11_ = misc_sort_mpddata_by_album_disc_track ((_tmp10_ = list, list = NULL, _tmp10_)), (_tmp11_ == NULL) ? NULL :  (_tmp11_)), (list == NULL) ? NULL : (list = (mpd_data_free (list), NULL)), _tmp12_);
		_tmp13_ = NULL;
		gmpc_mpddata_model_set_mpd_data (self->priv->model_albums, (_tmp13_ = list, list = NULL, _tmp13_));
		mpd_database_search_start (connection, TRUE);
		mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ARTIST, artist);
		_tmp14_ = NULL;
		data = (_tmp14_ = mpd_database_search_commit (connection), (data == NULL) ? NULL : (data = (mpd_data_free (data), NULL)), _tmp14_);
		_tmp17_ = NULL;
		_tmp16_ = NULL;
		_tmp15_ = NULL;
		data = (_tmp17_ = (_tmp16_ = misc_sort_mpddata_by_album_disc_track ((_tmp15_ = data, data = NULL, _tmp15_)), (_tmp16_ == NULL) ? NULL :  (_tmp16_)), (data == NULL) ? NULL : (data = (mpd_data_free (data), NULL)), _tmp17_);
		_tmp18_ = NULL;
		gmpc_mpddata_model_set_mpd_data (self->priv->model_songs, (_tmp18_ = data, data = NULL, _tmp18_));
		(data == NULL) ? NULL : (data = (mpd_data_free (data), NULL));
		(list == NULL) ? NULL : (list = (mpd_data_free (list), NULL));
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
		char* albumartist;
		MpdData* data;
		MpdData* _tmp4_;
		const MpdData* _tmp3_;
		MpdData* _tmp2_;
		MpdData* _tmp5_;
		album = gmpc_metadata_browser_browser_get_selected_album (self);
		albumartist = NULL;
		if (album != NULL) {
			MpdData* ydata;
			mpd_database_search_field_start (connection, MPD_TAG_ITEM_ALBUM_ARTIST);
			mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ALBUM, album);
			ydata = mpd_database_search_commit (connection);
			if (ydata != NULL) {
				if (string_get_length (ydata->tag) > 0) {
					char* _tmp1_;
					const char* _tmp0_;
					_tmp1_ = NULL;
					_tmp0_ = NULL;
					albumartist = (_tmp1_ = (_tmp0_ = ydata->tag, (_tmp0_ == NULL) ? NULL : g_strdup (_tmp0_)), albumartist = (g_free (albumartist), NULL), _tmp1_);
				}
			}
			(ydata == NULL) ? NULL : (ydata = (mpd_data_free (ydata), NULL));
		}
		/* Fill in the first browser */
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
		_tmp4_ = NULL;
		_tmp3_ = NULL;
		_tmp2_ = NULL;
		data = (_tmp4_ = (_tmp3_ = misc_sort_mpddata_by_album_disc_track ((_tmp2_ = data, data = NULL, _tmp2_)), (_tmp3_ == NULL) ? NULL :  (_tmp3_)), (data == NULL) ? NULL : (data = (mpd_data_free (data), NULL)), _tmp4_);
		_tmp5_ = NULL;
		gmpc_mpddata_model_set_mpd_data (self->priv->model_songs, (_tmp5_ = data, data = NULL, _tmp5_));
		album = (g_free (album), NULL);
		albumartist = (g_free (albumartist), NULL);
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
static void gmpc_metadata_browser_play_song (GmpcMetadataBrowser* self, GtkButton* button) {
	const mpd_Song* _tmp0_;
	mpd_Song* song;
	g_return_if_fail (self != NULL);
	g_return_if_fail (button != NULL);
	_tmp0_ = NULL;
	song = (_tmp0_ = (const mpd_Song*) g_object_get_data ((GObject*) button, "song"), (_tmp0_ == NULL) ? NULL : mpd_songDup (_tmp0_));
	if (song != NULL) {
		play_path (song->file);
	}
	(song == NULL) ? NULL : (song = (mpd_freeSong (song), NULL));
}


static void gmpc_metadata_browser_add_song (GmpcMetadataBrowser* self, GtkButton* button) {
	const mpd_Song* _tmp0_;
	mpd_Song* song;
	g_return_if_fail (self != NULL);
	g_return_if_fail (button != NULL);
	_tmp0_ = NULL;
	song = (_tmp0_ = (const mpd_Song*) g_object_get_data ((GObject*) button, "song"), (_tmp0_ == NULL) ? NULL : mpd_songDup (_tmp0_));
	if (song != NULL) {
		mpd_playlist_add (connection, song->file);
		(song == NULL) ? NULL : (song = (mpd_freeSong (song), NULL));
		return;
	}
	(song == NULL) ? NULL : (song = (mpd_freeSong (song), NULL));
}


static void gmpc_metadata_browser_replace_song (GmpcMetadataBrowser* self, GtkButton* button) {
	const mpd_Song* _tmp0_;
	mpd_Song* song;
	g_return_if_fail (self != NULL);
	g_return_if_fail (button != NULL);
	_tmp0_ = NULL;
	song = (_tmp0_ = (const mpd_Song*) g_object_get_data ((GObject*) button, "song"), (_tmp0_ == NULL) ? NULL : mpd_songDup (_tmp0_));
	if (song != NULL) {
		mpd_playlist_clear (connection);
		mpd_playlist_add (connection, song->file);
		mpd_player_play (connection);
		(song == NULL) ? NULL : (song = (mpd_freeSong (song), NULL));
		return;
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
		artist = (g_free (artist), NULL);
		album = (g_free (album), NULL);
		(song == NULL) ? NULL : (song = (mpd_freeSong (song), NULL));
		return;
	}
	if (artist != NULL) {
		char* albumartist;
		MpdData* data;
		/* Hunt albumartist */
		albumartist = NULL;
		if (album != NULL) {
			MpdData* ydata;
			mpd_database_search_field_start (connection, MPD_TAG_ITEM_ALBUM_ARTIST);
			mpd_database_search_add_constraint (connection, MPD_TAG_ITEM_ALBUM, album);
			ydata = mpd_database_search_commit (connection);
			if (ydata != NULL) {
				if (string_get_length (ydata->tag) > 0) {
					char* _tmp1_;
					const char* _tmp0_;
					_tmp1_ = NULL;
					_tmp0_ = NULL;
					albumartist = (_tmp1_ = (_tmp0_ = ydata->tag, (_tmp0_ == NULL) ? NULL : g_strdup (_tmp0_)), albumartist = (g_free (albumartist), NULL), _tmp1_);
				}
			}
			(ydata == NULL) ? NULL : (ydata = (mpd_data_free (ydata), NULL));
		}
		mpd_database_search_field_start (connection, MPD_TAG_ITEM_FILENAME);
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
			const MpdData* _tmp2_;
			const MpdData* iter;
			_tmp2_ = NULL;
			iter = misc_sort_mpddata_by_album_disc_track ((_tmp2_ = data, (_tmp2_ == NULL) ? NULL :  (_tmp2_)));
			do {
				mpd_playlist_queue_add (connection, iter->tag);
			} while ((iter = mpd_data_get_next_real (iter, FALSE)) != NULL);
			mpd_playlist_queue_commit (connection);
		}
		albumartist = (g_free (albumartist), NULL);
		(data == NULL) ? NULL : (data = (mpd_data_free (data), NULL));
	}
	artist = (g_free (artist), NULL);
	album = (g_free (album), NULL);
	(song == NULL) ? NULL : (song = (mpd_freeSong (song), NULL));
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
			GtkWidget* _tmp0_;
			GtkWidget* child;
			_tmp0_ = NULL;
			child = (_tmp0_ = (GtkWidget*) child_it->data, (_tmp0_ == NULL) ? NULL : g_object_ref (_tmp0_));
			{
				gtk_object_destroy ((GtkObject*) child);
				(child == NULL) ? NULL : (child = (g_object_unref (child), NULL));
			}
		}
	}
	(list == NULL) ? NULL : (list = (g_list_free (list), NULL));
}


static void _gmpc_metadata_browser_metadata_find_query_gtk_button_clicked (GtkButton* _sender, gpointer self) {
	gmpc_metadata_browser_metadata_find_query (self, _sender);
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


GtkWidget* gmpc_metadata_browser_metadata_box_show_song (GmpcMetadataBrowser* self, const mpd_Song* song) {
	GtkVBox* vbox;
	GtkLabel* label;
	GtkHBox* hbox;
	GtkAlignment* ali;
	GmpcMetaImage* artist_image;
	GtkTable* info_box;
	gint i;
	GtkLabel* pt_label;
	GtkLabel* _tmp6_;
	GtkLabel* _tmp7_;
	char* _tmp8_;
	GtkLabel* _tmp12_;
	GtkLabel* _tmp13_;
	char* _tmp14_;
	GmpcFavoritesButton* fav_button;
	GtkLabel* _tmp34_;
	char* _tmp35_;
	GtkAlignment* _tmp36_;
	GtkButton* button;
	GtkHBox* _tmp43_;
	GtkButton* _tmp44_;
	GtkButton* _tmp45_;
	GtkImage* _tmp46_;
	GmpcMetaTextView* text_view;
	char* _tmp47_;
	GmpcWidgetMore* _tmp48_;
	GmpcWidgetMore* frame;
	GmpcMetaTextView* _tmp49_;
	GmpcWidgetMore* _tmp51_;
	char* _tmp50_;
	GmpcWidgetSimilarSongs* similar_songs;
	GmpcSongLinks* song_links;
	GtkWidget* _tmp52_;
	g_return_val_if_fail (self != NULL, NULL);
	g_return_val_if_fail (song != NULL, NULL);
	vbox = g_object_ref_sink ((GtkVBox*) gtk_vbox_new (FALSE, 6));
	gtk_container_set_border_width ((GtkContainer*) vbox, (guint) 8);
	label = g_object_ref_sink ((GtkLabel*) gtk_label_new (""));
	gtk_label_set_selectable (label, TRUE);
	if (song->title != NULL) {
		char* _tmp0_;
		_tmp0_ = NULL;
		gtk_label_set_markup (label, _tmp0_ = g_markup_printf_escaped ("<span size='xx-large' weight='bold'>%s</span>", song->title));
		_tmp0_ = (g_free (_tmp0_), NULL);
	} else {
		char* _tmp1_;
		_tmp1_ = NULL;
		gtk_label_set_markup (label, _tmp1_ = g_markup_printf_escaped ("<span size='xx-large' weight='bold'>%s</span>", _ ("Unknown")));
		_tmp1_ = (g_free (_tmp1_), NULL);
	}
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
	pt_label = NULL;
	if (song->title != NULL) {
		GtkLabel* _tmp2_;
		char* _tmp3_;
		GtkHBox* dhbox;
		GtkLabel* _tmp4_;
		GtkButton* button;
		GtkImage* _tmp5_;
		_tmp2_ = NULL;
		label = (_tmp2_ = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp2_);
		gtk_label_set_selectable (label, TRUE);
		gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
		_tmp3_ = NULL;
		gtk_label_set_markup (label, _tmp3_ = g_markup_printf_escaped ("<b>%s:</b>", _ ("Title")));
		_tmp3_ = (g_free (_tmp3_), NULL);
		gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		dhbox = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 6));
		_tmp4_ = NULL;
		pt_label = (_tmp4_ = g_object_ref_sink ((GtkLabel*) gtk_label_new (song->title)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), _tmp4_);
		gtk_label_set_selectable (pt_label, TRUE);
		gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
		gtk_label_set_line_wrap (pt_label, TRUE);
		gtk_box_pack_start ((GtkBox*) dhbox, (GtkWidget*) pt_label, FALSE, FALSE, (guint) 0);
		/* Button to search for song with same title */
		button = g_object_ref_sink ((GtkButton*) gtk_button_new ());
		_tmp5_ = NULL;
		gtk_container_add ((GtkContainer*) button, (GtkWidget*) (_tmp5_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_stock ("gtk-find", GTK_ICON_SIZE_MENU))));
		(_tmp5_ == NULL) ? NULL : (_tmp5_ = (g_object_unref (_tmp5_), NULL));
		gtk_button_set_relief (button, GTK_RELIEF_NONE);
		g_object_set_data_full ((GObject*) button, "query", (void*) g_strdup_printf ("title=(%s)", song->title), (GDestroyNotify) g_free);
		g_signal_connect_object (button, "clicked", (GCallback) _gmpc_metadata_browser_metadata_find_query_gtk_button_clicked, self, 0);
		gtk_widget_set_tooltip_text ((GtkWidget*) button, _ ("Search songs with similar title"));
		gtk_box_pack_start ((GtkBox*) dhbox, (GtkWidget*) button, FALSE, FALSE, (guint) 0);
		gtk_table_attach (info_box, (GtkWidget*) dhbox, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		i++;
		(dhbox == NULL) ? NULL : (dhbox = (g_object_unref (dhbox), NULL));
		(button == NULL) ? NULL : (button = (g_object_unref (button), NULL));
	}
	/* Artist label */
	_tmp6_ = NULL;
	pt_label = (_tmp6_ = g_object_ref_sink ((GtkLabel*) gtk_label_new (song->artist)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), _tmp6_);
	gtk_label_set_selectable (pt_label, TRUE);
	_tmp7_ = NULL;
	label = (_tmp7_ = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp7_);
	gtk_label_set_selectable (label, TRUE);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
	_tmp8_ = NULL;
	gtk_label_set_markup (label, _tmp8_ = g_markup_printf_escaped ("<b>%s:</b>", _ ("Artist")));
	_tmp8_ = (g_free (_tmp8_), NULL);
	gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
	gtk_label_set_line_wrap (pt_label, TRUE);
	gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	i++;
	/* AlbumArtist label */
	if (song->albumartist != NULL) {
		GtkLabel* _tmp9_;
		GtkLabel* _tmp10_;
		char* _tmp11_;
		_tmp9_ = NULL;
		pt_label = (_tmp9_ = g_object_ref_sink ((GtkLabel*) gtk_label_new (song->albumartist)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), _tmp9_);
		gtk_label_set_selectable (pt_label, TRUE);
		_tmp10_ = NULL;
		label = (_tmp10_ = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp10_);
		gtk_label_set_selectable (label, TRUE);
		gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
		_tmp11_ = NULL;
		gtk_label_set_markup (label, _tmp11_ = g_markup_printf_escaped ("<b>%s:</b>", _ ("Album artist")));
		_tmp11_ = (g_free (_tmp11_), NULL);
		gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
		gtk_label_set_line_wrap (pt_label, TRUE);
		gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		i++;
	}
	/* Album */
	_tmp12_ = NULL;
	pt_label = (_tmp12_ = g_object_ref_sink ((GtkLabel*) gtk_label_new (song->album)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), _tmp12_);
	gtk_label_set_selectable (pt_label, TRUE);
	_tmp13_ = NULL;
	label = (_tmp13_ = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp13_);
	gtk_label_set_selectable (label, TRUE);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
	_tmp14_ = NULL;
	gtk_label_set_markup (label, _tmp14_ = g_markup_printf_escaped ("<b>%s:</b>", _ ("Album")));
	_tmp14_ = (g_free (_tmp14_), NULL);
	gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
	gtk_label_set_line_wrap (pt_label, TRUE);
	gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	i++;
	/* track */
	if (song->track != NULL) {
		GtkLabel* _tmp15_;
		GtkLabel* _tmp16_;
		char* _tmp17_;
		_tmp15_ = NULL;
		pt_label = (_tmp15_ = g_object_ref_sink ((GtkLabel*) gtk_label_new (song->track)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), _tmp15_);
		gtk_label_set_selectable (pt_label, TRUE);
		_tmp16_ = NULL;
		label = (_tmp16_ = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp16_);
		gtk_label_set_selectable (label, TRUE);
		gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
		_tmp17_ = NULL;
		gtk_label_set_markup (label, _tmp17_ = g_markup_printf_escaped ("<b>%s:</b>", _ ("Track")));
		_tmp17_ = (g_free (_tmp17_), NULL);
		gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
		gtk_label_set_line_wrap (pt_label, TRUE);
		gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		i++;
	}
	/* date */
	if (song->date != NULL) {
		GtkLabel* _tmp18_;
		GtkLabel* _tmp19_;
		char* _tmp20_;
		_tmp18_ = NULL;
		pt_label = (_tmp18_ = g_object_ref_sink ((GtkLabel*) gtk_label_new (song->date)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), _tmp18_);
		gtk_label_set_selectable (pt_label, TRUE);
		_tmp19_ = NULL;
		label = (_tmp19_ = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp19_);
		gtk_label_set_selectable (label, TRUE);
		gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
		_tmp20_ = NULL;
		gtk_label_set_markup (label, _tmp20_ = g_markup_printf_escaped ("<b>%s:</b>", _ ("Date")));
		_tmp20_ = (g_free (_tmp20_), NULL);
		gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
		gtk_label_set_line_wrap (pt_label, TRUE);
		gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		i++;
	}
	/* performer */
	if (song->performer != NULL) {
		GtkLabel* _tmp21_;
		GtkLabel* _tmp22_;
		char* _tmp23_;
		_tmp21_ = NULL;
		pt_label = (_tmp21_ = g_object_ref_sink ((GtkLabel*) gtk_label_new (song->performer)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), _tmp21_);
		gtk_label_set_selectable (pt_label, TRUE);
		_tmp22_ = NULL;
		label = (_tmp22_ = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp22_);
		gtk_label_set_selectable (label, TRUE);
		gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
		_tmp23_ = NULL;
		gtk_label_set_markup (label, _tmp23_ = g_markup_printf_escaped ("<b>%s:</b>", _ ("Performer")));
		_tmp23_ = (g_free (_tmp23_), NULL);
		gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
		gtk_label_set_line_wrap (pt_label, TRUE);
		gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		i++;
	}
	/* disc */
	if (song->disc != NULL) {
		GtkLabel* _tmp24_;
		GtkLabel* _tmp25_;
		char* _tmp26_;
		_tmp24_ = NULL;
		pt_label = (_tmp24_ = g_object_ref_sink ((GtkLabel*) gtk_label_new (song->disc)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), _tmp24_);
		gtk_label_set_selectable (pt_label, TRUE);
		_tmp25_ = NULL;
		label = (_tmp25_ = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp25_);
		gtk_label_set_selectable (label, TRUE);
		gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
		_tmp26_ = NULL;
		gtk_label_set_markup (label, _tmp26_ = g_markup_printf_escaped ("<b>%s:</b>", _ ("Disc")));
		_tmp26_ = (g_free (_tmp26_), NULL);
		gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
		gtk_label_set_line_wrap (pt_label, TRUE);
		gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		i++;
	}
	/* Genre */
	if (song->genre != NULL) {
		GtkLabel* _tmp27_;
		GtkLabel* _tmp28_;
		char* _tmp29_;
		_tmp27_ = NULL;
		pt_label = (_tmp27_ = g_object_ref_sink ((GtkLabel*) gtk_label_new (song->genre)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), _tmp27_);
		gtk_label_set_selectable (pt_label, TRUE);
		_tmp28_ = NULL;
		label = (_tmp28_ = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp28_);
		gtk_label_set_selectable (label, TRUE);
		gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
		_tmp29_ = NULL;
		gtk_label_set_markup (label, _tmp29_ = g_markup_printf_escaped ("<b>%s:</b>", _ ("Genre")));
		_tmp29_ = (g_free (_tmp29_), NULL);
		gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
		gtk_label_set_line_wrap (pt_label, TRUE);
		gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		i++;
	}
	/* Path */
	if (song->file != NULL) {
		GtkLabel* _tmp30_;
		char* _tmp31_;
		GtkLabel* _tmp32_;
		GtkHBox* dhbox;
		GtkButton* dbutton;
		GtkImage* _tmp33_;
		_tmp30_ = NULL;
		label = (_tmp30_ = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp30_);
		gtk_label_set_selectable (label, TRUE);
		gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.0f);
		_tmp31_ = NULL;
		gtk_label_set_markup (label, _tmp31_ = g_markup_printf_escaped ("<b>%s:</b>", _ ("Path")));
		_tmp31_ = (g_free (_tmp31_), NULL);
		gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		_tmp32_ = NULL;
		pt_label = (_tmp32_ = g_object_ref_sink ((GtkLabel*) gtk_label_new (song->file)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), _tmp32_);
		gtk_label_set_selectable (pt_label, TRUE);
		gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
		gtk_label_set_line_wrap (pt_label, TRUE);
		dhbox = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 6));
		gtk_box_pack_start ((GtkBox*) dhbox, (GtkWidget*) pt_label, FALSE, FALSE, (guint) 0);
		dbutton = g_object_ref_sink ((GtkButton*) gtk_button_new ());
		gtk_button_set_relief (dbutton, GTK_RELIEF_NONE);
		_tmp33_ = NULL;
		gtk_container_add ((GtkContainer*) dbutton, (GtkWidget*) (_tmp33_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_stock ("gtk-open", GTK_ICON_SIZE_MENU))));
		(_tmp33_ == NULL) ? NULL : (_tmp33_ = (g_object_unref (_tmp33_), NULL));
		g_object_set_data_full ((GObject*) dbutton, "path", (void*) g_path_get_dirname (song->file), (GDestroyNotify) g_free);
		g_signal_connect_object (dbutton, "clicked", (GCallback) _gmpc_metadata_browser_metadata_button_open_file_browser_path_gtk_button_clicked, self, 0);
		gtk_widget_set_tooltip_text ((GtkWidget*) dbutton, _ ("Open path to song in file browser"));
		gtk_box_pack_start ((GtkBox*) dhbox, (GtkWidget*) dbutton, FALSE, FALSE, (guint) 0);
		gtk_table_attach (info_box, (GtkWidget*) dhbox, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		i++;
		(dhbox == NULL) ? NULL : (dhbox = (g_object_unref (dhbox), NULL));
		(dbutton == NULL) ? NULL : (dbutton = (g_object_unref (dbutton), NULL));
	}
	/* Favored button */
	fav_button = g_object_ref_sink (gmpc_favorites_button_new ());
	gmpc_favorites_button_set_song (fav_button, song);
	_tmp34_ = NULL;
	label = (_tmp34_ = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp34_);
	gtk_label_set_selectable (label, TRUE);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
	_tmp35_ = NULL;
	gtk_label_set_markup (label, _tmp35_ = g_markup_printf_escaped ("<b>%s:</b>", _ ("Favored")));
	_tmp35_ = (g_free (_tmp35_), NULL);
	gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	_tmp36_ = NULL;
	ali = (_tmp36_ = g_object_ref_sink ((GtkAlignment*) gtk_alignment_new (0.0f, 0.5f, 0.f, 0.f)), (ali == NULL) ? NULL : (ali = (g_object_unref (ali), NULL)), _tmp36_);
	gtk_container_add ((GtkContainer*) ali, (GtkWidget*) fav_button);
	gtk_table_attach (info_box, (GtkWidget*) ali, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	i++;
	if (mpd_sticker_supported (connection)) {
		GmpcRating* rating_button;
		GtkLabel* _tmp37_;
		char* _tmp38_;
		GtkAlignment* _tmp39_;
		/* Favored button */
		rating_button = g_object_ref_sink (gmpc_rating_new (connection, song));
		_tmp37_ = NULL;
		label = (_tmp37_ = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp37_);
		gtk_label_set_selectable (label, TRUE);
		gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
		_tmp38_ = NULL;
		gtk_label_set_markup (label, _tmp38_ = g_markup_printf_escaped ("<b>%s:</b>", _ ("Rating")));
		_tmp38_ = (g_free (_tmp38_), NULL);
		gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		_tmp39_ = NULL;
		ali = (_tmp39_ = g_object_ref_sink ((GtkAlignment*) gtk_alignment_new (0.0f, 0.5f, 0.f, 0.f)), (ali == NULL) ? NULL : (ali = (g_object_unref (ali), NULL)), _tmp39_);
		gtk_container_add ((GtkContainer*) ali, (GtkWidget*) rating_button);
		gtk_table_attach (info_box, (GtkWidget*) ali, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
		i++;
		(rating_button == NULL) ? NULL : (rating_button = (g_object_unref (rating_button), NULL));
	}
	/* Comment */
	if (song->comment != NULL) {
		GtkLabel* _tmp40_;
		GtkLabel* _tmp41_;
		char* _tmp42_;
		_tmp40_ = NULL;
		pt_label = (_tmp40_ = g_object_ref_sink ((GtkLabel*) gtk_label_new (song->comment)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), _tmp40_);
		gtk_label_set_selectable (pt_label, TRUE);
		_tmp41_ = NULL;
		label = (_tmp41_ = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp41_);
		gtk_label_set_selectable (label, TRUE);
		gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.0f);
		_tmp42_ = NULL;
		gtk_label_set_markup (label, _tmp42_ = g_markup_printf_escaped ("<b>%s:</b>", _ ("Comment")));
		_tmp42_ = (g_free (_tmp42_), NULL);
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
	g_object_set_data_full ((GObject*) button, "song", mpd_songDup (song), (GDestroyNotify) mpd_freeSong);
	g_signal_connect_object (button, "clicked", (GCallback) _gmpc_metadata_browser_play_song_gtk_button_clicked, self, 0);
	_tmp43_ = NULL;
	hbox = (_tmp43_ = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 6)), (hbox == NULL) ? NULL : (hbox = (g_object_unref (hbox), NULL)), _tmp43_);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) button, FALSE, FALSE, (guint) 0);
	_tmp44_ = NULL;
	button = (_tmp44_ = g_object_ref_sink ((GtkButton*) gtk_button_new_from_stock ("gtk-add")), (button == NULL) ? NULL : (button = (g_object_unref (button), NULL)), _tmp44_);
	gtk_button_set_relief (button, GTK_RELIEF_NONE);
	g_object_set_data_full ((GObject*) button, "song", mpd_songDup (song), (GDestroyNotify) mpd_freeSong);
	g_signal_connect_object (button, "clicked", (GCallback) _gmpc_metadata_browser_add_song_gtk_button_clicked, self, 0);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) button, FALSE, FALSE, (guint) 0);
	_tmp45_ = NULL;
	button = (_tmp45_ = g_object_ref_sink ((GtkButton*) gtk_button_new_with_mnemonic ("_Replace")), (button == NULL) ? NULL : (button = (g_object_unref (button), NULL)), _tmp45_);
	_tmp46_ = NULL;
	gtk_button_set_image (button, (GtkWidget*) (_tmp46_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_stock ("gtk-redo", GTK_ICON_SIZE_BUTTON))));
	(_tmp46_ == NULL) ? NULL : (_tmp46_ = (g_object_unref (_tmp46_), NULL));
	gtk_button_set_relief (button, GTK_RELIEF_NONE);
	g_object_set_data_full ((GObject*) button, "song", mpd_songDup (song), (GDestroyNotify) mpd_freeSong);
	g_signal_connect_object (button, "clicked", (GCallback) _gmpc_metadata_browser_replace_song_gtk_button_clicked, self, 0);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) button, FALSE, FALSE, (guint) 0);
	gtk_table_attach (info_box, (GtkWidget*) hbox, (guint) 0, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	i++;
	/* Lyrics */
	text_view = g_object_ref_sink (gmpc_meta_text_view_new (META_SONG_TXT));
	gtk_text_view_set_left_margin ((GtkTextView*) text_view, 8);
	_tmp47_ = NULL;
	_tmp48_ = NULL;
	frame = (_tmp48_ = g_object_ref_sink (gmpc_widget_more_new (_tmp47_ = g_markup_printf_escaped ("<b>%s:</b>", _ ("Lyrics")), (GtkWidget*) text_view)), _tmp47_ = (g_free (_tmp47_), NULL), _tmp48_);
	gmpc_meta_text_view_query_text_from_song (text_view, song);
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) frame, FALSE, FALSE, (guint) 0);
	/* Guitar Tab */
	_tmp49_ = NULL;
	text_view = (_tmp49_ = g_object_ref_sink (gmpc_meta_text_view_new (META_SONG_GUITAR_TAB)), (text_view == NULL) ? NULL : (text_view = (g_object_unref (text_view), NULL)), _tmp49_);
	text_view->use_monospace = TRUE;
	gtk_text_view_set_left_margin ((GtkTextView*) text_view, 8);
	_tmp51_ = NULL;
	_tmp50_ = NULL;
	frame = (_tmp51_ = g_object_ref_sink (gmpc_widget_more_new (_tmp50_ = g_markup_printf_escaped ("<b>%s:</b>", _ ("Guitar Tabs")), (GtkWidget*) text_view)), (frame == NULL) ? NULL : (frame = (g_object_unref (frame), NULL)), _tmp51_);
	_tmp50_ = (g_free (_tmp50_), NULL);
	gmpc_meta_text_view_query_text_from_song (text_view, song);
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) frame, FALSE, FALSE, (guint) 0);
	similar_songs = g_object_ref_sink (gmpc_widget_similar_songs_new (song));
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) similar_songs, FALSE, FALSE, (guint) 0);
	song_links = g_object_ref_sink (gmpc_song_links_new (GMPC_SONG_LINKS_TYPE_SONG, song));
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) song_links, FALSE, FALSE, (guint) 0);
	/**
	         * Add it to the view
	         
	
	        this.metadata_box.add(vbox);
	        this.metadata_sw.show_all();
	        */
	_tmp52_ = NULL;
	return (_tmp52_ = (GtkWidget*) vbox, (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), (hbox == NULL) ? NULL : (hbox = (g_object_unref (hbox), NULL)), (ali == NULL) ? NULL : (ali = (g_object_unref (ali), NULL)), (artist_image == NULL) ? NULL : (artist_image = (g_object_unref (artist_image), NULL)), (info_box == NULL) ? NULL : (info_box = (g_object_unref (info_box), NULL)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), (fav_button == NULL) ? NULL : (fav_button = (g_object_unref (fav_button), NULL)), (button == NULL) ? NULL : (button = (g_object_unref (button), NULL)), (text_view == NULL) ? NULL : (text_view = (g_object_unref (text_view), NULL)), (frame == NULL) ? NULL : (frame = (g_object_unref (frame), NULL)), (similar_songs == NULL) ? NULL : (similar_songs = (g_object_unref (similar_songs), NULL)), (song_links == NULL) ? NULL : (song_links = (g_object_unref (song_links), NULL)), _tmp52_);
}


static void gmpc_metadata_browser_metadata_button_open_file_browser_path (GmpcMetadataBrowser* self, GtkButton* button) {
	const char* _tmp0_;
	char* path;
	g_return_if_fail (self != NULL);
	g_return_if_fail (button != NULL);
	_tmp0_ = NULL;
	path = (_tmp0_ = (const char*) g_object_get_data ((GObject*) button, "path"), (_tmp0_ == NULL) ? NULL : g_strdup (_tmp0_));
	if (path != NULL) {
		pl3_file_browser_open_path (path);
	}
	path = (g_free (path), NULL);
}


static void gmpc_metadata_browser_metadata_find_query (GmpcMetadataBrowser* self, GtkButton* button) {
	const char* _tmp0_;
	char* path;
	g_return_if_fail (self != NULL);
	g_return_if_fail (button != NULL);
	_tmp0_ = NULL;
	path = (_tmp0_ = (const char*) g_object_get_data ((GObject*) button, "query"), (_tmp0_ == NULL) ? NULL : g_strdup (_tmp0_));
	if (path != NULL) {
		pl3_find2_ec_database (NULL, path);
	}
	path = (g_free (path), NULL);
}


static void gmpc_metadata_browser_album_song_tree_row_activated (GmpcMetadataBrowser* self, GtkTreeView* tree, const GtkTreePath* path, GtkTreeViewColumn* column) {
	GtkTreeIter iter = {0};
	GtkTreeModel* _tmp0_;
	GtkTreeModel* model;
	g_return_if_fail (self != NULL);
	g_return_if_fail (tree != NULL);
	g_return_if_fail (path != NULL);
	g_return_if_fail (column != NULL);
	_tmp0_ = NULL;
	model = (_tmp0_ = gtk_tree_view_get_model (tree), (_tmp0_ == NULL) ? NULL : g_object_ref (_tmp0_));
	if (gtk_tree_model_get_iter (model, &iter, path)) {
		const mpd_Song* song;
		song = NULL;
		gtk_tree_model_get (model, &iter, 0, &song, -1, -1);
		if (song != NULL) {
			mpd_playlist_add (connection, song->file);
		}
	}
	(model == NULL) ? NULL : (model = (g_object_unref (model), NULL));
}


static void gmpc_metadata_browser_album_song_browser_add_clicked (GmpcMetadataBrowser* self, GtkImageMenuItem* item) {
	GtkTreeView* _tmp0_;
	GtkTreeView* tree;
	g_return_if_fail (self != NULL);
	g_return_if_fail (item != NULL);
	_tmp0_ = NULL;
	tree = (_tmp0_ = GTK_TREE_VIEW (g_object_get_data ((GObject*) item, "tree")), (_tmp0_ == NULL) ? NULL : g_object_ref (_tmp0_));
	if (tree != NULL) {
		GtkTreeIter iter = {0};
		GtkTreeModel* _tmp1_;
		GtkTreeModel* model;
		GtkTreeSelection* _tmp2_;
		GtkTreeSelection* sel;
		GtkTreeModel* _tmp6_;
		GtkTreeModel* _tmp5_;
		GList* _tmp4_;
		GtkTreeModel* _tmp3_;
		GList* list;
		_tmp1_ = NULL;
		model = (_tmp1_ = gtk_tree_view_get_model (tree), (_tmp1_ == NULL) ? NULL : g_object_ref (_tmp1_));
		_tmp2_ = NULL;
		sel = (_tmp2_ = gtk_tree_view_get_selection (tree), (_tmp2_ == NULL) ? NULL : g_object_ref (_tmp2_));
		_tmp6_ = NULL;
		_tmp5_ = NULL;
		_tmp4_ = NULL;
		_tmp3_ = NULL;
		list = (_tmp4_ = gtk_tree_selection_get_selected_rows (sel, &_tmp3_), model = (_tmp5_ = (_tmp6_ = _tmp3_, (_tmp6_ == NULL) ? NULL : g_object_ref (_tmp6_)), (model == NULL) ? NULL : (model = (g_object_unref (model), NULL)), _tmp5_), _tmp4_);
		{
			GList* path_collection;
			GList* path_it;
			path_collection = list;
			for (path_it = path_collection; path_it != NULL; path_it = path_it->next) {
				const GtkTreePath* _tmp7_;
				GtkTreePath* path;
				_tmp7_ = NULL;
				path = (_tmp7_ = (const GtkTreePath*) path_it->data, (_tmp7_ == NULL) ? NULL : gtk_tree_path_copy (_tmp7_));
				{
					if (gtk_tree_model_get_iter (model, &iter, path)) {
						const mpd_Song* song;
						song = NULL;
						gtk_tree_model_get (model, &iter, 0, &song, -1, -1);
						if (song != NULL) {
							mpd_playlist_queue_add (connection, song->file);
						}
					}
					(path == NULL) ? NULL : (path = (gtk_tree_path_free (path), NULL));
				}
			}
		}
		mpd_playlist_queue_commit (connection);
		(model == NULL) ? NULL : (model = (g_object_unref (model), NULL));
		(sel == NULL) ? NULL : (sel = (g_object_unref (sel), NULL));
		(list == NULL) ? NULL : (list = (_g_list_free_gtk_tree_path_free (list), NULL));
	}
	(tree == NULL) ? NULL : (tree = (g_object_unref (tree), NULL));
}


static void gmpc_metadata_browser_album_song_browser_replace_clicked (GmpcMetadataBrowser* self, GtkImageMenuItem* item) {
	g_return_if_fail (self != NULL);
	g_return_if_fail (item != NULL);
	mpd_playlist_clear (connection);
	gmpc_metadata_browser_album_song_browser_add_clicked (self, item);
	mpd_player_play (connection);
}


static void _gmpc_metadata_browser_album_song_browser_add_clicked_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self) {
	gmpc_metadata_browser_album_song_browser_add_clicked (self, _sender);
}


static void _gmpc_metadata_browser_album_song_browser_replace_clicked_gtk_menu_item_activate (GtkImageMenuItem* _sender, gpointer self) {
	gmpc_metadata_browser_album_song_browser_replace_clicked (self, _sender);
}


static gboolean gmpc_metadata_browser_album_song_tree_button_press_event (GmpcMetadataBrowser* self, GtkTreeView* tree, const GdkEventButton* event) {
	g_return_val_if_fail (self != NULL, FALSE);
	g_return_val_if_fail (tree != NULL, FALSE);
	if ((*event).button == 3) {
		GtkMenu* menu;
		GtkImageMenuItem* item;
		GtkImageMenuItem* _tmp0_;
		GtkImage* _tmp1_;
		gboolean _tmp2_;
		menu = g_object_ref_sink ((GtkMenu*) gtk_menu_new ());
		item = g_object_ref_sink ((GtkImageMenuItem*) gtk_image_menu_item_new_from_stock ("gtk-add", NULL));
		g_signal_connect_object ((GtkMenuItem*) item, "activate", (GCallback) _gmpc_metadata_browser_album_song_browser_add_clicked_gtk_menu_item_activate, self, 0);
		g_object_set_data ((GObject*) item, "tree", (void*) tree);
		gtk_menu_shell_append ((GtkMenuShell*) menu, (GtkWidget*) ((GtkMenuItem*) item));
		_tmp0_ = NULL;
		item = (_tmp0_ = g_object_ref_sink ((GtkImageMenuItem*) gtk_image_menu_item_new_with_mnemonic ("_Replace")), (item == NULL) ? NULL : (item = (g_object_unref (item), NULL)), _tmp0_);
		_tmp1_ = NULL;
		gtk_image_menu_item_set_image (item, (GtkWidget*) (_tmp1_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_stock ("gtk-redo", GTK_ICON_SIZE_MENU))));
		(_tmp1_ == NULL) ? NULL : (_tmp1_ = (g_object_unref (_tmp1_), NULL));
		g_object_set_data ((GObject*) item, "tree", (void*) tree);
		g_signal_connect_object ((GtkMenuItem*) item, "activate", (GCallback) _gmpc_metadata_browser_album_song_browser_replace_clicked_gtk_menu_item_activate, self, 0);
		gtk_menu_shell_append ((GtkMenuShell*) menu, (GtkWidget*) ((GtkMenuItem*) item));
		gtk_menu_popup (menu, NULL, NULL, NULL, NULL, (*event).button, (*event).time);
		gtk_widget_show_all ((GtkWidget*) menu);
		return (_tmp2_ = TRUE, (menu == NULL) ? NULL : (menu = (g_object_unref (menu), NULL)), (item == NULL) ? NULL : (item = (g_object_unref (item), NULL)), _tmp2_);
	}
	return FALSE;
}


static void _gmpc_metadata_browser_add_selected_song_gtk_button_clicked (GtkButton* _sender, gpointer self) {
	gmpc_metadata_browser_add_selected_song (self, _sender);
}


static void _gmpc_metadata_browser_replace_selected_song_gtk_button_clicked (GtkButton* _sender, gpointer self) {
	gmpc_metadata_browser_replace_selected_song (self, _sender);
}


static gboolean _gmpc_metadata_browser_album_song_tree_button_press_event_gtk_widget_button_press_event (GmpcMpdDataTreeview* _sender, const GdkEventButton* event, gpointer self) {
	return gmpc_metadata_browser_album_song_tree_button_press_event (self, _sender, event);
}


static void _gmpc_metadata_browser_album_song_tree_row_activated_gtk_tree_view_row_activated (GmpcMpdDataTreeview* _sender, const GtkTreePath* path, GtkTreeViewColumn* column, gpointer self) {
	gmpc_metadata_browser_album_song_tree_row_activated (self, _sender, path, column);
}


static void gmpc_metadata_browser_metadata_box_show_album (GmpcMetadataBrowser* self, const char* artist, const char* album) {
	GtkVBox* vbox;
	GtkLabel* label;
	const char* _tmp0_;
	const char* _tmp1_;
	char* _tmp2_;
	GtkHBox* hbox;
	GtkAlignment* ali;
	GmpcMetaImage* artist_image;
	mpd_Song* song;
	char* _tmp4_;
	const char* _tmp3_;
	char* _tmp6_;
	const char* _tmp5_;
	GtkTable* info_box;
	gint i;
	GmpcStatsLabel* pt_label;
	GtkLabel* _tmp7_;
	char* _tmp8_;
	GmpcStatsLabel* _tmp9_;
	GtkLabel* _tmp10_;
	char* _tmp11_;
	GmpcStatsLabel* _tmp12_;
	GtkLabel* _tmp13_;
	char* _tmp14_;
	GmpcStatsLabel* _tmp15_;
	GtkLabel* _tmp16_;
	char* _tmp17_;
	GtkHBox* _tmp18_;
	GtkButton* button;
	GtkButton* _tmp19_;
	GtkImage* _tmp20_;
	GmpcMetaTextView* text_view;
	char* _tmp21_;
	GmpcWidgetMore* _tmp22_;
	GmpcWidgetMore* frame;
	GtkLabel* _tmp23_;
	char* _tmp24_;
	GtkScrolledWindow* sw;
	GmpcMpdDataTreeview* song_tree;
	GmpcSongLinks* song_links;
	g_return_if_fail (self != NULL);
	g_return_if_fail (artist != NULL);
	g_return_if_fail (album != NULL);
	vbox = g_object_ref_sink ((GtkVBox*) gtk_vbox_new (FALSE, 6));
	gtk_container_set_border_width ((GtkContainer*) vbox, (guint) 8);
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
	_tmp2_ = NULL;
	gtk_label_set_markup (label, _tmp2_ = g_markup_printf_escaped ("<span size='xx-large' weight='bold'>%s - %s</span>", _tmp0_, _tmp1_));
	_tmp2_ = (g_free (_tmp2_), NULL);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) label, FALSE, FALSE, (guint) 0);
	/* Artist image */
	hbox = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 6));
	ali = g_object_ref_sink ((GtkAlignment*) gtk_alignment_new (0.f, 0.f, 0.f, 0.f));
	artist_image = g_object_ref_sink (gmpc_metaimage_new_size (META_ALBUM_ART, 250));
	gmpc_metaimage_set_squared (artist_image, FALSE);
	song = mpd_newSong ();
	_tmp4_ = NULL;
	_tmp3_ = NULL;
	song->artist = (_tmp4_ = (_tmp3_ = artist, (_tmp3_ == NULL) ? NULL : g_strdup (_tmp3_)), song->artist = (g_free (song->artist), NULL), _tmp4_);
	_tmp6_ = NULL;
	_tmp5_ = NULL;
	song->album = (_tmp6_ = (_tmp5_ = album, (_tmp5_ == NULL) ? NULL : g_strdup (_tmp5_)), song->album = (g_free (song->album), NULL), _tmp6_);
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
	_tmp7_ = NULL;
	label = (_tmp7_ = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp7_);
	gtk_label_set_selectable (label, TRUE);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
	_tmp8_ = NULL;
	gtk_label_set_markup (label, _tmp8_ = g_markup_printf_escaped ("<b>%s:</b>", _ ("Genres")));
	_tmp8_ = (g_free (_tmp8_), NULL);
	gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
	gtk_label_set_line_wrap ((GtkLabel*) pt_label, TRUE);
	gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	i++;
	/* Dates of songs */
	_tmp9_ = NULL;
	pt_label = (_tmp9_ = g_object_ref_sink (gmpc_stats_label_new (ALBUM_DATES_SONGS, song)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), _tmp9_);
	gtk_label_set_line_wrap ((GtkLabel*) pt_label, TRUE);
	_tmp10_ = NULL;
	label = (_tmp10_ = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp10_);
	gtk_label_set_selectable (label, TRUE);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.0f);
	_tmp11_ = NULL;
	gtk_label_set_markup (label, _tmp11_ = g_markup_printf_escaped ("<b>%s:</b>", _ ("Dates")));
	_tmp11_ = (g_free (_tmp11_), NULL);
	gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
	gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	i++;
	/* Total number of songs */
	_tmp12_ = NULL;
	pt_label = (_tmp12_ = g_object_ref_sink (gmpc_stats_label_new (ALBUM_NUM_SONGS, song)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), _tmp12_);
	gtk_label_set_line_wrap ((GtkLabel*) pt_label, TRUE);
	_tmp13_ = NULL;
	label = (_tmp13_ = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp13_);
	gtk_label_set_selectable (label, TRUE);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.0f);
	_tmp14_ = NULL;
	gtk_label_set_markup (label, _tmp14_ = g_markup_printf_escaped ("<b>%s:</b>", _ ("Songs")));
	_tmp14_ = (g_free (_tmp14_), NULL);
	gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
	gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	i++;
	/* Total playtime */
	_tmp15_ = NULL;
	pt_label = (_tmp15_ = g_object_ref_sink (gmpc_stats_label_new (ALBUM_PLAYTIME_SONGS, song)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), _tmp15_);
	gtk_label_set_line_wrap ((GtkLabel*) pt_label, TRUE);
	_tmp16_ = NULL;
	label = (_tmp16_ = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp16_);
	gtk_label_set_selectable (label, TRUE);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.0f);
	_tmp17_ = NULL;
	gtk_label_set_markup (label, _tmp17_ = g_markup_printf_escaped ("<b>%s:</b>", _ ("Playtime")));
	_tmp17_ = (g_free (_tmp17_), NULL);
	gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
	gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	i++;
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) hbox, FALSE, FALSE, (guint) 0);
	/* Player controls */
	_tmp18_ = NULL;
	hbox = (_tmp18_ = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 6)), (hbox == NULL) ? NULL : (hbox = (g_object_unref (hbox), NULL)), _tmp18_);
	button = g_object_ref_sink ((GtkButton*) gtk_button_new_from_stock ("gtk-add"));
	gtk_button_set_relief (button, GTK_RELIEF_NONE);
	g_signal_connect_object (button, "clicked", (GCallback) _gmpc_metadata_browser_add_selected_song_gtk_button_clicked, self, 0);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) button, FALSE, FALSE, (guint) 0);
	_tmp19_ = NULL;
	button = (_tmp19_ = g_object_ref_sink ((GtkButton*) gtk_button_new_with_mnemonic ("_Replace")), (button == NULL) ? NULL : (button = (g_object_unref (button), NULL)), _tmp19_);
	_tmp20_ = NULL;
	gtk_button_set_image (button, (GtkWidget*) (_tmp20_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_stock ("gtk-redo", GTK_ICON_SIZE_BUTTON))));
	(_tmp20_ == NULL) ? NULL : (_tmp20_ = (g_object_unref (_tmp20_), NULL));
	gtk_button_set_relief (button, GTK_RELIEF_NONE);
	g_signal_connect_object (button, "clicked", (GCallback) _gmpc_metadata_browser_replace_selected_song_gtk_button_clicked, self, 0);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) button, FALSE, FALSE, (guint) 0);
	gtk_table_attach (info_box, (GtkWidget*) hbox, (guint) 0, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	i++;
	text_view = g_object_ref_sink (gmpc_meta_text_view_new (META_ALBUM_TXT));
	gtk_text_view_set_left_margin ((GtkTextView*) text_view, 8);
	_tmp21_ = NULL;
	_tmp22_ = NULL;
	frame = (_tmp22_ = g_object_ref_sink (gmpc_widget_more_new (_tmp21_ = g_markup_printf_escaped ("<b>%s:</b>", _ ("Album information")), (GtkWidget*) text_view)), _tmp21_ = (g_free (_tmp21_), NULL), _tmp22_);
	gmpc_meta_text_view_query_text_from_song (text_view, song);
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) frame, FALSE, FALSE, (guint) 0);
	/* Song list */
	_tmp23_ = NULL;
	label = (_tmp23_ = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp23_);
	gtk_label_set_selectable (label, TRUE);
	_tmp24_ = NULL;
	gtk_label_set_markup (label, _tmp24_ = g_strdup_printf ("<b>%s</b>", _ ("Songs")));
	_tmp24_ = (g_free (_tmp24_), NULL);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) label, FALSE, FALSE, (guint) 0);
	sw = g_object_ref_sink ((GtkScrolledWindow*) gtk_scrolled_window_new (NULL, NULL));
	gtk_scrolled_window_set_policy (sw, GTK_POLICY_AUTOMATIC, GTK_POLICY_NEVER);
	gtk_scrolled_window_set_shadow_type (sw, GTK_SHADOW_ETCHED_IN);
	song_tree = g_object_ref_sink (gmpc_mpddata_treeview_new ("album-songs", TRUE, (GtkTreeModel*) self->priv->model_songs));
	g_signal_connect_object ((GtkWidget*) song_tree, "button-press-event", (GCallback) _gmpc_metadata_browser_album_song_tree_button_press_event_gtk_widget_button_press_event, self, 0);
	g_signal_connect_object ((GtkTreeView*) song_tree, "row-activated", (GCallback) _gmpc_metadata_browser_album_song_tree_row_activated_gtk_tree_view_row_activated, self, 0);
	gtk_container_add ((GtkContainer*) sw, (GtkWidget*) song_tree);
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) sw, FALSE, FALSE, (guint) 0);
	/* TODO right mouse menu 
	 Song links */
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
	(sw == NULL) ? NULL : (sw = (g_object_unref (sw), NULL));
	(song_tree == NULL) ? NULL : (song_tree = (g_object_unref (song_tree), NULL));
	(song_links == NULL) ? NULL : (song_links = (g_object_unref (song_links), NULL));
}


/**
     * This fills the view for artist 
     * <artist name>
     * <image> | <array with info>
     *           < buttonss>
     *
     * <artist info text>
     *
     * <similar artists>
     * <links>
     */
static void gmpc_metadata_browser_metadata_box_show_artist (GmpcMetadataBrowser* self, const char* artist) {
	GtkVBox* vbox;
	GtkLabel* label;
	const char* _tmp0_;
	char* _tmp1_;
	GtkHBox* hbox;
	GtkAlignment* ali;
	GmpcMetaImage* artist_image;
	mpd_Song* song;
	char* _tmp3_;
	const char* _tmp2_;
	GtkTable* info_box;
	gint i;
	GmpcStatsLabel* pt_label;
	GtkLabel* _tmp4_;
	char* _tmp5_;
	GmpcStatsLabel* _tmp6_;
	GtkLabel* _tmp7_;
	char* _tmp8_;
	GmpcStatsLabel* _tmp9_;
	GtkLabel* _tmp10_;
	char* _tmp11_;
	GmpcStatsLabel* _tmp12_;
	GtkLabel* _tmp13_;
	char* _tmp14_;
	GtkHBox* _tmp15_;
	GtkButton* button;
	GtkButton* _tmp16_;
	GtkImage* _tmp17_;
	GmpcMetaTextView* text_view;
	char* _tmp18_;
	GmpcWidgetMore* _tmp19_;
	GmpcWidgetMore* frame;
	GtkLabel* _tmp20_;
	char* _tmp21_;
	GmpcWidgetSimilarArtist* similar_artist;
	GmpcSongLinks* song_links;
	g_return_if_fail (self != NULL);
	g_return_if_fail (artist != NULL);
	vbox = g_object_ref_sink ((GtkVBox*) gtk_vbox_new (FALSE, 6));
	gtk_container_set_border_width ((GtkContainer*) vbox, (guint) 8);
	label = g_object_ref_sink ((GtkLabel*) gtk_label_new (""));
	gtk_label_set_selectable (label, TRUE);
	_tmp0_ = NULL;
	if (artist != NULL) {
		_tmp0_ = artist;
	} else {
		_tmp0_ = _ ("Unknown");
	}
	_tmp1_ = NULL;
	gtk_label_set_markup (label, _tmp1_ = g_markup_printf_escaped ("<span size='xx-large' weight='bold'>%s</span>", _tmp0_));
	_tmp1_ = (g_free (_tmp1_), NULL);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) label, FALSE, FALSE, (guint) 0);
	/* Artist image */
	hbox = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 6));
	ali = g_object_ref_sink ((GtkAlignment*) gtk_alignment_new (0.f, 0.f, 0.f, 0.f));
	artist_image = g_object_ref_sink (gmpc_metaimage_new_size (META_ARTIST_ART, 250));
	gmpc_metaimage_set_squared (artist_image, FALSE);
	song = mpd_newSong ();
	_tmp3_ = NULL;
	_tmp2_ = NULL;
	song->artist = (_tmp3_ = (_tmp2_ = artist, (_tmp2_ == NULL) ? NULL : g_strdup (_tmp2_)), song->artist = (g_free (song->artist), NULL), _tmp3_);
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
	_tmp4_ = NULL;
	label = (_tmp4_ = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp4_);
	gtk_label_set_selectable (label, TRUE);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
	_tmp5_ = NULL;
	gtk_label_set_markup (label, _tmp5_ = g_markup_printf_escaped ("<b>%s:</b>", _ ("Genres")));
	_tmp5_ = (g_free (_tmp5_), NULL);
	gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
	gtk_label_set_line_wrap ((GtkLabel*) pt_label, TRUE);
	gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	i++;
	/* Dates of songs */
	_tmp6_ = NULL;
	pt_label = (_tmp6_ = g_object_ref_sink (gmpc_stats_label_new (ARTIST_DATES_SONGS, song)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), _tmp6_);
	gtk_label_set_line_wrap ((GtkLabel*) pt_label, TRUE);
	_tmp7_ = NULL;
	label = (_tmp7_ = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp7_);
	gtk_label_set_selectable (label, TRUE);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.0f);
	_tmp8_ = NULL;
	gtk_label_set_markup (label, _tmp8_ = g_markup_printf_escaped ("<b>%s:</b>", _ ("Dates")));
	_tmp8_ = (g_free (_tmp8_), NULL);
	gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
	gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	i++;
	/* Total number of songs */
	_tmp9_ = NULL;
	pt_label = (_tmp9_ = g_object_ref_sink (gmpc_stats_label_new (ARTIST_NUM_SONGS, song)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), _tmp9_);
	gtk_label_set_line_wrap ((GtkLabel*) pt_label, TRUE);
	_tmp10_ = NULL;
	label = (_tmp10_ = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp10_);
	gtk_label_set_selectable (label, TRUE);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.0f);
	_tmp11_ = NULL;
	gtk_label_set_markup (label, _tmp11_ = g_markup_printf_escaped ("<b>%s:</b>", _ ("Songs")));
	_tmp11_ = (g_free (_tmp11_), NULL);
	gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
	gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	i++;
	/* Total playtime */
	_tmp12_ = NULL;
	pt_label = (_tmp12_ = g_object_ref_sink (gmpc_stats_label_new (ARTIST_PLAYTIME_SONGS, song)), (pt_label == NULL) ? NULL : (pt_label = (g_object_unref (pt_label), NULL)), _tmp12_);
	gtk_label_set_line_wrap ((GtkLabel*) pt_label, TRUE);
	_tmp13_ = NULL;
	label = (_tmp13_ = g_object_ref_sink ((GtkLabel*) gtk_label_new ("")), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp13_);
	gtk_label_set_selectable (label, TRUE);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.0f);
	_tmp14_ = NULL;
	gtk_label_set_markup (label, _tmp14_ = g_markup_printf_escaped ("<b>%s:</b>", _ ("Playtime")));
	_tmp14_ = (g_free (_tmp14_), NULL);
	gtk_table_attach (info_box, (GtkWidget*) label, (guint) 0, (guint) 1, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	gtk_misc_set_alignment ((GtkMisc*) pt_label, 0.0f, 0.5f);
	gtk_table_attach (info_box, (GtkWidget*) pt_label, (guint) 1, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	i++;
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) hbox, FALSE, FALSE, (guint) 0);
	/* Player controls */
	_tmp15_ = NULL;
	hbox = (_tmp15_ = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 6)), (hbox == NULL) ? NULL : (hbox = (g_object_unref (hbox), NULL)), _tmp15_);
	button = g_object_ref_sink ((GtkButton*) gtk_button_new_from_stock ("gtk-add"));
	gtk_button_set_relief (button, GTK_RELIEF_NONE);
	g_signal_connect_object (button, "clicked", (GCallback) _gmpc_metadata_browser_add_selected_song_gtk_button_clicked, self, 0);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) button, FALSE, FALSE, (guint) 0);
	_tmp16_ = NULL;
	button = (_tmp16_ = g_object_ref_sink ((GtkButton*) gtk_button_new_with_mnemonic ("_Replace")), (button == NULL) ? NULL : (button = (g_object_unref (button), NULL)), _tmp16_);
	_tmp17_ = NULL;
	gtk_button_set_image (button, (GtkWidget*) (_tmp17_ = g_object_ref_sink ((GtkImage*) gtk_image_new_from_stock ("gtk-redo", GTK_ICON_SIZE_BUTTON))));
	(_tmp17_ == NULL) ? NULL : (_tmp17_ = (g_object_unref (_tmp17_), NULL));
	gtk_button_set_relief (button, GTK_RELIEF_NONE);
	g_signal_connect_object (button, "clicked", (GCallback) _gmpc_metadata_browser_replace_selected_song_gtk_button_clicked, self, 0);
	gtk_box_pack_start ((GtkBox*) hbox, (GtkWidget*) button, FALSE, FALSE, (guint) 0);
	gtk_table_attach (info_box, (GtkWidget*) hbox, (guint) 0, (guint) 2, (guint) i, (guint) (i + 1), GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL, (guint) 0, (guint) 0);
	i++;
	text_view = g_object_ref_sink (gmpc_meta_text_view_new (META_ARTIST_TXT));
	gtk_text_view_set_left_margin ((GtkTextView*) text_view, 8);
	_tmp18_ = NULL;
	_tmp19_ = NULL;
	frame = (_tmp19_ = g_object_ref_sink (gmpc_widget_more_new (_tmp18_ = g_markup_printf_escaped ("<b>%s:</b>", _ ("Artist information")), (GtkWidget*) text_view)), _tmp18_ = (g_free (_tmp18_), NULL), _tmp19_);
	gmpc_meta_text_view_query_text_from_song (text_view, song);
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) frame, FALSE, FALSE, (guint) 0);
	_tmp20_ = NULL;
	label = (_tmp20_ = g_object_ref_sink ((GtkLabel*) gtk_label_new (_ ("Similar artist"))), (label == NULL) ? NULL : (label = (g_object_unref (label), NULL)), _tmp20_);
	gtk_label_set_selectable (label, TRUE);
	_tmp21_ = NULL;
	gtk_label_set_markup (label, _tmp21_ = g_strdup_printf ("<span weight='bold'>%s</span>", _ ("Similar artist")));
	_tmp21_ = (g_free (_tmp21_), NULL);
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.0f);
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) label, FALSE, FALSE, (guint) 0);
	similar_artist = g_object_ref_sink (gmpc_widget_similar_artist_new (self, connection, song));
	gtk_box_pack_start ((GtkBox*) vbox, (GtkWidget*) similar_artist, FALSE, FALSE, (guint) 0);
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
	(similar_artist == NULL) ? NULL : (similar_artist = (g_object_unref (similar_artist), NULL));
	(song_links == NULL) ? NULL : (song_links = (g_object_unref (song_links), NULL));
}


static gboolean _gmpc_metadata_browser_metadata_box_update_real_gsource_func (gpointer self) {
	return gmpc_metadata_browser_metadata_box_update_real (self);
}


static void gmpc_metadata_browser_metadata_box_update (GmpcMetadataBrowser* self) {
	g_return_if_fail (self != NULL);
	if (self->priv->update_timeout > 0) {
		g_source_remove (self->priv->update_timeout);
	}
	self->priv->update_timeout = g_idle_add (_gmpc_metadata_browser_metadata_box_update_real_gsource_func, self);
}


static gboolean gmpc_metadata_browser_metadata_box_update_real (GmpcMetadataBrowser* self) {
	char* artist;
	char* album;
	mpd_Song* song;
	gboolean _tmp2_;
	g_return_val_if_fail (self != NULL, FALSE);
	if (self->priv->block_update > 0) {
		self->priv->update_timeout = (guint) 0;
		return FALSE;
	}
	artist = gmpc_metadata_browser_browser_get_selected_artist (self);
	album = gmpc_metadata_browser_browser_get_selected_album (self);
	song = gmpc_metadata_browser_browser_get_selected_song (self);
	if (song != NULL) {
		GtkWidget* view;
		view = gmpc_metadata_browser_metadata_box_show_song (self, song);
		gtk_container_add ((GtkContainer*) self->priv->metadata_box, view);
		gtk_widget_show_all ((GtkWidget*) self->priv->metadata_sw);
		(view == NULL) ? NULL : (view = (g_object_unref (view), NULL));
	} else {
		gboolean _tmp1_;
		_tmp1_ = FALSE;
		if (album != NULL) {
			_tmp1_ = artist != NULL;
		} else {
			_tmp1_ = FALSE;
		}
		if (_tmp1_) {
			gmpc_metadata_browser_metadata_box_show_album (self, artist, album);
		} else {
			if (artist != NULL) {
				gmpc_metadata_browser_metadata_box_show_artist (self, artist);
			}
		}
	}
	self->priv->update_timeout = (guint) 0;
	return (_tmp2_ = FALSE, artist = (g_free (artist), NULL), album = (g_free (album), NULL), (song == NULL) ? NULL : (song = (mpd_freeSong (song), NULL)), _tmp2_);
}


/** 
     * Browser Interface bindings
     */
static void gmpc_metadata_browser_real_browser_add (GmpcPluginBrowserIface* base, GtkWidget* category_tree) {
	GmpcMetadataBrowser * self;
	GtkTreeView* _tmp0_;
	GtkTreeView* tree;
	GtkListStore* _tmp1_;
	GtkListStore* store;
	GtkTreeModel* _tmp2_;
	GtkTreeModel* model;
	GtkTreeIter iter = {0};
	GtkTreeRowReference* _tmp4_;
	GtkTreePath* _tmp3_;
	self = (GmpcMetadataBrowser*) base;
	g_return_if_fail (category_tree != NULL);
	_tmp0_ = NULL;
	tree = (_tmp0_ = GTK_TREE_VIEW (category_tree), (_tmp0_ == NULL) ? NULL : g_object_ref (_tmp0_));
	_tmp1_ = NULL;
	store = (_tmp1_ = GTK_LIST_STORE (gtk_tree_view_get_model (tree)), (_tmp1_ == NULL) ? NULL : g_object_ref (_tmp1_));
	_tmp2_ = NULL;
	model = (_tmp2_ = gtk_tree_view_get_model (tree), (_tmp2_ == NULL) ? NULL : g_object_ref (_tmp2_));
	playlist3_insert_browser (&iter, cfg_get_single_value_as_int_with_default (config, gmpc_plugin_base_get_name ((GmpcPluginBase*) self), "position", 100));
	gtk_list_store_set (store, &iter, 0, ((GmpcPluginBase*) self)->id, 1, _ (gmpc_plugin_base_get_name ((GmpcPluginBase*) self)), 3, "gtk-info", -1);
	/* Create a row reference */
	_tmp4_ = NULL;
	_tmp3_ = NULL;
	self->priv->rref = (_tmp4_ = gtk_tree_row_reference_new (model, _tmp3_ = gtk_tree_model_get_path (model, &iter)), (self->priv->rref == NULL) ? NULL : (self->priv->rref = (gtk_tree_row_reference_free (self->priv->rref), NULL)), _tmp4_);
	(_tmp3_ == NULL) ? NULL : (_tmp3_ = (gtk_tree_path_free (_tmp3_), NULL));
	(tree == NULL) ? NULL : (tree = (g_object_unref (tree), NULL));
	(store == NULL) ? NULL : (store = (g_object_unref (store), NULL));
	(model == NULL) ? NULL : (model = (g_object_unref (model), NULL));
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
	/* update if non selected */
	_tmp0_ = NULL;
	artist = (_tmp0_ = gmpc_metadata_browser_browser_get_selected_artist (self), artist = (g_free (artist), NULL), _tmp0_);
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
	g_return_if_fail (self != NULL);
	g_return_if_fail (conn != NULL);
	g_return_if_fail (server != NULL);
	if (self->priv->paned == NULL) {
		return;
	}
}


void gmpc_metadata_browser_set_artist (GmpcMetadataBrowser* self, const char* artist) {
	GtkTreeIter iter = {0};
	g_return_if_fail (self != NULL);
	g_return_if_fail (artist != NULL);
	self->priv->block_update++;
	gtk_tree_selection_unselect_all (gtk_tree_view_get_selection (self->priv->tree_artist));
	gtk_tree_selection_unselect_all (gtk_tree_view_get_selection (self->priv->tree_album));
	/* clear */
	gtk_entry_set_text (self->priv->artist_filter_entry, "");
	if (gtk_tree_model_get_iter_first ((GtkTreeModel*) self->priv->model_filter_artist, &iter)) {
		do {
			char* lartist;
			gboolean _tmp0_;
			lartist = NULL;
			gtk_tree_model_get ((GtkTreeModel*) self->priv->model_filter_artist, &iter, 7, &lartist, -1, -1);
			_tmp0_ = FALSE;
			if (lartist != NULL) {
				_tmp0_ = g_utf8_collate (lartist, artist) == 0;
			} else {
				_tmp0_ = FALSE;
			}
			if (_tmp0_) {
				GtkTreePath* _tmp1_;
				gtk_tree_selection_select_iter (gtk_tree_view_get_selection (self->priv->tree_artist), &iter);
				_tmp1_ = NULL;
				gtk_tree_view_scroll_to_cell (self->priv->tree_artist, _tmp1_ = gtk_tree_model_get_path ((GtkTreeModel*) self->priv->model_filter_artist, &iter), NULL, TRUE, 0.5f, 0.f);
				(_tmp1_ == NULL) ? NULL : (_tmp1_ = (gtk_tree_path_free (_tmp1_), NULL));
				self->priv->block_update--;
				gmpc_metadata_browser_metadata_box_clear (self);
				gmpc_metadata_browser_metadata_box_update (self);
				lartist = (g_free (lartist), NULL);
				return;
			}
			lartist = (g_free (lartist), NULL);
		} while (gtk_tree_model_iter_next ((GtkTreeModel*) self->priv->model_filter_artist, &iter));
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
	self->priv->block_update++;
	gmpc_metadata_browser_set_artist (self, artist);
	/* clear */
	gtk_entry_set_text (self->priv->album_filter_entry, "");
	if (gtk_tree_model_get_iter_first ((GtkTreeModel*) self->priv->model_filter_album, &iter)) {
		do {
			char* lalbum;
			gboolean _tmp0_;
			lalbum = NULL;
			gtk_tree_model_get ((GtkTreeModel*) self->priv->model_filter_album, &iter, 6, &lalbum, -1, -1);
			_tmp0_ = FALSE;
			if (lalbum != NULL) {
				_tmp0_ = g_utf8_collate (lalbum, album) == 0;
			} else {
				_tmp0_ = FALSE;
			}
			if (_tmp0_) {
				GtkTreePath* _tmp1_;
				gtk_tree_selection_select_iter (gtk_tree_view_get_selection (self->priv->tree_album), &iter);
				_tmp1_ = NULL;
				gtk_tree_view_scroll_to_cell (self->priv->tree_album, _tmp1_ = gtk_tree_model_get_path ((GtkTreeModel*) self->priv->model_filter_album, &iter), NULL, TRUE, 0.5f, 0.f);
				(_tmp1_ == NULL) ? NULL : (_tmp1_ = (gtk_tree_path_free (_tmp1_), NULL));
				gtk_tree_selection_unselect_all (gtk_tree_view_get_selection (self->priv->tree_songs));
				self->priv->block_update--;
				gmpc_metadata_browser_metadata_box_update (self);
				lalbum = (g_free (lalbum), NULL);
				return;
			}
			lalbum = (g_free (lalbum), NULL);
		} while (gtk_tree_model_iter_next ((GtkTreeModel*) self->priv->model_filter_album, &iter));
	}
	gtk_tree_selection_unselect_all (gtk_tree_view_get_selection (self->priv->tree_songs));
	self->priv->block_update--;
	gmpc_metadata_browser_metadata_box_clear (self);
	gmpc_metadata_browser_metadata_box_update (self);
}


void gmpc_metadata_browser_set_song (GmpcMetadataBrowser* self, const mpd_Song* song) {
	GtkTreeIter iter = {0};
	GtkWidget* view;
	g_return_if_fail (self != NULL);
	g_return_if_fail (song != NULL);
	self->priv->block_update++;
	if (song->artist != NULL) {
		gmpc_metadata_browser_set_artist (self, song->artist);
		if (song->album != NULL) {
			gmpc_metadata_browser_set_album (self, song->artist, song->album);
		}
	}
	if (gtk_tree_model_get_iter_first ((GtkTreeModel*) self->priv->model_songs, &iter)) {
		do {
			char* ltitle;
			gboolean _tmp0_;
			ltitle = NULL;
			gtk_tree_model_get ((GtkTreeModel*) self->priv->model_songs, &iter, 7, &ltitle, -1, -1);
			_tmp0_ = FALSE;
			if (ltitle != NULL) {
				_tmp0_ = g_utf8_collate (ltitle, song->title) == 0;
			} else {
				_tmp0_ = FALSE;
			}
			if (_tmp0_) {
				GtkTreePath* _tmp1_;
				gtk_tree_selection_select_iter (gtk_tree_view_get_selection (self->priv->tree_songs), &iter);
				_tmp1_ = NULL;
				gtk_tree_view_scroll_to_cell (self->priv->tree_songs, _tmp1_ = gtk_tree_model_get_path ((GtkTreeModel*) self->priv->model_songs, &iter), NULL, TRUE, 0.5f, 0.f);
				(_tmp1_ == NULL) ? NULL : (_tmp1_ = (gtk_tree_path_free (_tmp1_), NULL));
				self->priv->block_update--;
				gmpc_metadata_browser_metadata_box_update (self);
				ltitle = (g_free (ltitle), NULL);
				return;
			}
			ltitle = (g_free (ltitle), NULL);
		} while (gtk_tree_model_iter_next ((GtkTreeModel*) self->priv->model_songs, &iter));
	}
	self->priv->block_update--;
	gmpc_metadata_browser_metadata_box_clear (self);
	if (self->priv->update_timeout > 0) {
		g_source_remove (self->priv->update_timeout);
		self->priv->update_timeout = (guint) 0;
	}
	view = gmpc_metadata_browser_metadata_box_show_song (self, song);
	gtk_container_add ((GtkContainer*) self->priv->metadata_box, view);
	gtk_widget_show_all ((GtkWidget*) self->priv->metadata_box);
	(view == NULL) ? NULL : (view = (g_object_unref (view), NULL));
}


void gmpc_metadata_browser_select_browser (GmpcMetadataBrowser* self, GtkTreeView* tree) {
	GtkTreePath* path;
	GtkTreeModel* _tmp0_;
	GtkTreeModel* model;
	g_return_if_fail (self != NULL);
	g_return_if_fail (tree != NULL);
	path = gtk_tree_row_reference_get_path (self->priv->rref);
	_tmp0_ = NULL;
	model = (_tmp0_ = gtk_tree_row_reference_get_model (self->priv->rref), (_tmp0_ == NULL) ? NULL : g_object_ref (_tmp0_));
	if (path != NULL) {
		GtkTreeIter iter = {0};
		if (gtk_tree_model_get_iter (model, &iter, path)) {
			gtk_tree_selection_select_iter (gtk_tree_view_get_selection (tree), &iter);
		}
	}
	(path == NULL) ? NULL : (path = (gtk_tree_path_free (path), NULL));
	(model == NULL) ? NULL : (model = (g_object_unref (model), NULL));
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
	GMPC_PLUGIN_BASE_CLASS (klass)->get_version = gmpc_metadata_browser_real_get_version;
	GMPC_PLUGIN_BASE_CLASS (klass)->get_name = gmpc_metadata_browser_real_get_name;
	GMPC_PLUGIN_BASE_CLASS (klass)->save_yourself = gmpc_metadata_browser_real_save_yourself;
	G_OBJECT_CLASS (klass)->constructor = gmpc_metadata_browser_constructor;
	G_OBJECT_CLASS (klass)->finalize = gmpc_metadata_browser_finalize;
}


static void gmpc_metadata_browser_gmpc_plugin_browser_iface_interface_init (GmpcPluginBrowserIfaceIface * iface) {
	gmpc_metadata_browser_gmpc_plugin_browser_iface_parent_iface = g_type_interface_peek_parent (iface);
	iface->browser_add = gmpc_metadata_browser_real_browser_add;
	iface->browser_selected = gmpc_metadata_browser_real_browser_selected;
	iface->browser_unselected = gmpc_metadata_browser_real_browser_unselected;
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




