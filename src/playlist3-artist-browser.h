#ifndef __PLAYLIST_ARTIST_BROWSER_H__
#define __PLAYLIST_ARTIST_BROWSER_H__
void pl3_artist_browser_add();
long unsigned pl3_artist_browser_view_folder(GtkTreeIter *iter_cat);
void pl3_artist_browser_fill_tree(GtkTreeIter *iter);
void pl3_browse_artist_add_folder();
void pl3_browse_artist_replace_folder();

#endif
