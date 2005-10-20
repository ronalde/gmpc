#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <glade/glade.h>

#include "main.h"
#include "misc.h"
#include "strfsong.h"
#include "playlist3.h"
#include "playlist3-tag-browser.h"
#include "config1.h"


void pl3_custom_tag_browser_add_folder();
void pl3_custom_tag_browser_replace_folder();

extern config_obj *config;
extern GladeXML *pl3_xml;

enum{
	PL3_TB_ARTIST,
	PL3_TB_TYPE,
	PL3_TB_TITLE,
	PL3_TB_ICON,
	PL3_TB_ROWS
};


/* internal */
GtkWidget *pl3_tb_tree = NULL;
GtkListStore *pl3_tb_store = NULL;
GtkWidget *pl3_tb_sw = NULL;


int pl3_tag_browser_button_press_event(GtkTreeView *tree, GdkEventButton *event)
{
	GtkTreeSelection *sel = gtk_tree_view_get_selection(tree);
	if(event->button != 3 || 
		gtk_tree_selection_count_selected_rows(sel) < 2|| !mpd_check_connected(connection))
	{
		return FALSE; 	
	}
	return TRUE;
}


void pl3_tag_browser_init()
{
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column = NULL;
	GValue value;
	pl3_tb_store = gtk_list_store_new (PL3_TB_ROWS, 
			GTK_TYPE_STRING, /* path to file */
			GTK_TYPE_INT,	/* type, FILE/PLAYLIST/FOLDER  */
			GTK_TYPE_STRING,	/* title to display */
			GTK_TYPE_STRING); /* icon type */



	renderer = gtk_cell_renderer_pixbuf_new ();

	column = gtk_tree_view_column_new ();
	gtk_tree_view_column_pack_start (column, renderer, FALSE);
	gtk_tree_view_column_set_attributes (column,renderer,"stock-id", PL3_TB_ICON,NULL);
	memset(&value, 0, sizeof(value));
	/* set value for ALL */
	g_value_init(&value, G_TYPE_FLOAT);
	g_value_set_float(&value, 0.0);
	g_object_set_property(G_OBJECT(renderer), "yalign", &value); 

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (column, renderer, TRUE);
	gtk_tree_view_column_set_attributes (column,renderer,"text", PL3_TB_TITLE, NULL);


	/* set up the tree */
	pl3_tb_tree= gtk_tree_view_new_with_model(GTK_TREE_MODEL(pl3_tb_store));
	/* insert the column in the tree */
	gtk_tree_view_append_column (GTK_TREE_VIEW (pl3_tb_tree), column);                                         	
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(pl3_tb_tree), FALSE);
	gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(pl3_tb_tree), TRUE);
	gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(pl3_tb_tree)), GTK_SELECTION_MULTIPLE);

	/* setup signals */
//	g_signal_connect(G_OBJECT(pl3_tb_tree), "row-activated",G_CALLBACK(pl3_artist_browser_row_activated), NULL); 
	g_signal_connect(G_OBJECT(pl3_tb_tree), "button-press-event", G_CALLBACK(pl3_tag_browser_button_press_event), NULL);
//	g_signal_connect(G_OBJECT(pl3_tb_tree), "button-release-event", G_CALLBACK(pl3_artist_browser_button_release_event), NULL);
//	g_signal_connect(G_OBJECT(pl3_tb_tree), "key-press-event", G_CALLBACK(pl3_artist_browser_playlist_key_press), NULL);

	/* set up the scrolled window */
	pl3_tb_sw = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(pl3_tb_sw), GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(pl3_tb_sw), GTK_SHADOW_ETCHED_IN);
	gtk_container_add(GTK_CONTAINER(pl3_tb_sw), pl3_tb_tree);

	/* set initial state */
	printf("initialized artist playlist treeview\n");
	g_object_ref(G_OBJECT(pl3_tb_sw));
}


















































































void pl3_custom_tag_browser_reload()
{
	GtkTreeIter iter;
	if(pl3_tb_tree == NULL )
	{
		return;
	}

	if(gtk_tree_model_get_iter_first(GTK_TREE_MODEL(pl3_tb_tree),&iter))
	{
		gchar *title = NULL;
		do{
			gtk_tree_model_get(GTK_TREE_MODEL(pl3_tb_tree), &iter, PL3_CAT_TITLE, &title, -1);
			if(!strcmp(title, _("Tag Browser")))
			{
				GtkTreeIter child;
				if(gtk_tree_model_iter_children(GTK_TREE_MODEL(pl3_tb_tree), &child, &iter))
				{
					do{
						gtk_tree_store_remove(pl3_tb_tree, &child);

					}while(gtk_tree_store_iter_is_valid(pl3_tb_tree, &child));
				}
				pl3_custom_tag_browser_list_add(&iter);
				return;
			}
		}while(gtk_tree_model_iter_next(GTK_TREE_MODEL(pl3_tb_tree), &iter));
	}
}

void pl3_custom_tag_browser_list_add(GtkTreeIter *iter)
{
	conf_mult_obj *list;
	/* get the configured tag browsers */
	list = cfg_get_multiple_as_string(config, "playlist", "advbrows");
	if(list != NULL)
	{
		conf_mult_obj *data = list;
		do{
			if(strlen(data->key) && strlen(data->value))
			{
				pl3_custom_tag_browser_add_single(iter,data->key, data->value);
			}
			data = data->next;
		}while(data != NULL);
		cfg_free_multiple(list);
	}

}
void pl3_custom_tag_browser_add()
{
	if(mpd_server_check_version(connection,0,12,0))
	{
		GtkTreeIter iter;


		/* add the root node to tag-browser */
		gtk_tree_store_append(pl3_tb_tree, &iter, NULL);
		gtk_tree_store_set(pl3_tb_tree, &iter, 
				PL3_CAT_TYPE, PL3_BROWSE_CUSTOM_TAG,
				PL3_CAT_TITLE, _("Tag Browser"),        	
				PL3_CAT_INT_ID, "",
				PL3_CAT_ICON_ID, "media-artist",
				PL3_CAT_PROC, TRUE,
				PL3_CAT_ICON_SIZE,GTK_ICON_SIZE_DND,
				PL3_CAT_BROWSE_FORMAT, "",
				-1);
		pl3_custom_tag_browser_list_add(&iter);
	}
	else
	{
		debug_printf(DEBUG_INFO, "pl3_custom_tag_browser_add(): Option not supported for mpd version < 0.12");
	}
}


void pl3_custom_tag_browser_add_single(GtkTreeIter *piter, char *title, char *format)
{
	GtkTreeIter iter,child;
	gtk_tree_store_append(pl3_tb_tree, &iter, piter);
	gtk_tree_store_set(pl3_tb_tree, &iter, 
			PL3_CAT_TYPE, PL3_BROWSE_CUSTOM_TAG,
			PL3_CAT_TITLE, title,        	
			PL3_CAT_INT_ID, "",
			PL3_CAT_ICON_ID, "media-artist",
			PL3_CAT_PROC, FALSE,
			PL3_CAT_ICON_SIZE,GTK_ICON_SIZE_DND,
			PL3_CAT_BROWSE_FORMAT, format,
			-1);
	/* add fantom child for lazy tree */
	gtk_tree_store_append(pl3_tb_tree, &child, &iter);
}


void pl3_custom_tag_browser_fill_tree(GtkTreeIter *iter)
{
	char *first_tag, *second_tag;
	char *format;
	char **tk_format= NULL;
	GtkTreePath *path = NULL;
	int depth = 0;
	int len=0;
	GtkTreeIter child,child2;
	gtk_tree_model_get(GTK_TREE_MODEL(pl3_tb_tree),iter, 1, &first_tag,2,&second_tag,PL3_CAT_BROWSE_FORMAT, &format, -1);
	gtk_tree_store_set(pl3_tb_tree, iter, 4, TRUE, -1);

	path = gtk_tree_model_get_path(GTK_TREE_MODEL(pl3_tb_tree), iter);
	if(path == NULL)
	{
		printf("Failed to get path\n");
		return;
	}
	depth = gtk_tree_path_get_depth(path) -2;
	gtk_tree_path_free(path);	

	if (!mpd_check_connected(connection) || path < 0)
	{
		return;
	}

	tk_format = g_strsplit(format, "|",0);
	if(tk_format ==NULL)
	{
		printf("failed to split\n");
		return;
	}
	for(len=0;tk_format[len] != NULL;len++)	
	{
		if(mpd_misc_get_tag_by_name(tk_format[len])== -1)
		{
			printf("invallid tag: %s\n", tk_format[len]);
			g_strfreev(tk_format);
			return;
		}
	}
	/* the things we do when on level 0 */
	if(depth == 0)
	{
		MpdData *data = mpd_playlist_get_unique_tags(connection,mpd_misc_get_tag_by_name(tk_format[0]),-1);

		while(data != NULL)
		{	
			gtk_tree_store_append (pl3_tb_tree, &child, iter);
			gtk_tree_store_set (pl3_tb_tree, &child,
					0, PL3_BROWSE_CUSTOM_TAG,
					1, data->value.tag, /* the field */
					2, data->value.tag, /* the artist name, if(1 and 2 together its an artist field) */
					3, "media-album",
					4, FALSE,
					PL3_CAT_ICON_SIZE,1,
					PL3_CAT_BROWSE_FORMAT, format,
					-1);
			if(len > 1)
			{
				gtk_tree_store_append(pl3_tb_tree, &child2, &child);
			}

			data = mpd_data_get_next(data);
		}
		if(gtk_tree_model_iter_children(GTK_TREE_MODEL(pl3_tb_tree), &child, iter))
		{
			gtk_tree_store_remove(pl3_tb_tree, &child); 
		}
	}
	/* if where inside a artist */
	else if(depth == 1)
	{
		MpdData *data = mpd_playlist_get_unique_tags(connection,mpd_misc_get_tag_by_name(tk_format[1]),
				mpd_misc_get_tag_by_name(tk_format[0]),first_tag,-1 );
		if(data == NULL)
		{
			printf("no sub data\n");
		}
		while(data != NULL){
			gtk_tree_store_append (pl3_tb_tree, &child, iter);
			gtk_tree_store_set (pl3_tb_tree, &child,
					0, PL3_BROWSE_CUSTOM_TAG,
					1, data->value.tag,
					2, first_tag,
					3, "media-artist", 
					4, FALSE, 
					PL3_CAT_ICON_SIZE,1,
					PL3_CAT_BROWSE_FORMAT, format,
					-1);
			data = mpd_data_get_next(data);
			if(len > 2)
			{
				gtk_tree_store_append(pl3_tb_tree, &child2, &child);			
			}                                                        		
		}
		if(gtk_tree_model_iter_children(GTK_TREE_MODEL(pl3_tb_tree), &child, iter))
		{
			gtk_tree_store_remove(pl3_tb_tree, &child); 
		}
	}
	else if(depth == 2 && len >= 2)
	{
		MpdData *data = mpd_playlist_get_unique_tags(connection,
				mpd_misc_get_tag_by_name(tk_format[2]),
				mpd_misc_get_tag_by_name(tk_format[1]),first_tag,
				mpd_misc_get_tag_by_name(tk_format[0]),second_tag,
				-1);
		if(data == NULL)
		{
			debug_printf(DEBUG_WARNING, "pl3_custom_tag_browser_fill_tree: no sub data %s %s %s %s %s\n",
					tk_format[2],
					tk_format[1],first_tag,
					tk_format[0],second_tag);
		}

		while(data != NULL){
			gtk_tree_store_append (pl3_tb_tree, &child, iter);
			gtk_tree_store_set (pl3_tb_tree, &child,
					0, PL3_BROWSE_CUSTOM_TAG,
					1, data->value.tag,
					2, first_tag,
					3, "media-artist", 
					4, TRUE, 
					PL3_CAT_ICON_SIZE,1,
					PL3_CAT_BROWSE_FORMAT, format,
					-1);
			data = mpd_data_get_next(data);
		}
		if(gtk_tree_model_iter_children(GTK_TREE_MODEL(pl3_tb_tree), &child, iter))
		{
			gtk_tree_store_remove(pl3_tb_tree, &child); 
		}                                                                                       		
	}
	g_strfreev(tk_format);
}

long unsigned pl3_custom_tag_browser_view_folder(GtkTreeIter *iter_cat)
{
	char *first_tag, *second_tag, *format;
	char **tk_format;
	int i = 0, depth;
	GtkTreePath *path;
	GtkTreeIter iter;
	long unsigned time =0;
	gtk_tree_model_get(GTK_TREE_MODEL(pl3_tb_tree), iter_cat, 2 , &first_tag, 1,&second_tag,PL3_CAT_BROWSE_FORMAT, &format, -1);

	if (check_connection_state ())
	{
		return 0;
	}

	path = gtk_tree_model_get_path(GTK_TREE_MODEL(pl3_tb_tree), iter_cat);
	if(path == NULL)
	{
		printf("Failed to get path\n");
		return 0;
	}
	depth = gtk_tree_path_get_depth(path) -2;


	if(depth <= 0)
	{
		/*lowest level, do nothing */
		gtk_tree_path_free(path);	
		return 0;
	}                                    	

	tk_format = g_strsplit(format, "|",0);
	if(tk_format ==NULL)
	{
		printf("failed to split\n");
		gtk_tree_path_free(path);	
		return 0;
	}


	for(i=0;tk_format[i] != NULL;i++)	
	{

		if(mpd_misc_get_tag_by_name(tk_format[i])== -1)
		{

			g_strfreev(tk_format);
			gtk_tree_path_free(path);	
			return 0;
		}

	}

	if(depth == 1)
	{
		MpdData *data = mpd_playlist_find_adv(connection,TRUE, 
				mpd_misc_get_tag_by_name(tk_format[0]), first_tag, -1);
		/* lowest level selected*/
		while(data != NULL)
		{
			gchar buffer[1024];
			char *markdata = cfg_get_single_value_as_string_with_default(config, "playlist", "browser_markup",DEFAULT_MARKUP_BROWSER);
			strfsong (buffer, 1024,markdata,data->value.song);
			cfg_free_string(markdata);
			if(data->value.song->time != MPD_SONG_NO_TIME)
			{
				time += data->value.song->time;
			}
			if(data->value.song->file == NULL)
			{
				debug_printf(DEBUG_WARNING,"pl3_browser_view_folder: crap mpdSong has no file attribute.\n");
			}
			gtk_list_store_append (pl3_tb_store, &iter);
			gtk_list_store_set (pl3_tb_store, &iter,
					2, buffer,
					0, data->value.song->file,
					1, PL3_ENTRY_SONG,
					5,"media-audiofile",
					-1);
			data = mpd_data_get_next(data);
		}
	}
	else if(depth == 2)
	{
		/* second level */
		MpdData *data = mpd_playlist_find_adv(connection,TRUE,
				mpd_misc_get_tag_by_name(tk_format[0]),first_tag,
				mpd_misc_get_tag_by_name(tk_format[1]), second_tag, -1);
		while (data != NULL)
		{
			gchar buffer[1024];
			char *markdata = cfg_get_single_value_as_string_with_default(config, "playlist", "browser_markup",DEFAULT_MARKUP_BROWSER);
			strfsong (buffer, 1024,markdata,data->value.song);
			cfg_free_string(markdata);
			if(data->value.song->time != MPD_SONG_NO_TIME)
			{
				time += data->value.song->time;
			}
			if(data->value.song->file == NULL)
			{
				debug_printf(DEBUG_WARNING,"pl3_browser_view_folder: crap mpdSong has no file attribute.\n");
			}
			gtk_list_store_append (pl3_tb_store, &iter);
			gtk_list_store_set (pl3_tb_store, &iter,
					2, buffer,
					0, data->value.song->file,
					1, PL3_ENTRY_SONG,
					5,"media-audiofile",
					-1);

			data = mpd_data_get_next(data);
		}

	}
	else if (depth == 3)
	{
		char *first;
		/* take the upper one */
		/* go 2 up */
		if(gtk_tree_path_up(path) && gtk_tree_path_up(path))
		{
			MpdData *data  = NULL;
			gtk_tree_model_get_iter(GTK_TREE_MODEL(pl3_tb_tree), &iter, path);
			gtk_tree_model_get(GTK_TREE_MODEL(pl3_tb_tree), &iter, 1, &first, -1);

			/* artist and album is selected */
			data = mpd_playlist_find_adv(connection,TRUE,
					mpd_misc_get_tag_by_name(tk_format[0]),	first,
					mpd_misc_get_tag_by_name(tk_format[1]), first_tag,
					mpd_misc_get_tag_by_name(tk_format[2]), second_tag,-1);
			while (data != NULL)
			{
				gchar buffer[1024];
				char *markdata = cfg_get_single_value_as_string_with_default(config, "playlist", "browser_markup",
						DEFAULT_MARKUP_BROWSER);
				strfsong (buffer, 1024,markdata,data->value.song);
				cfg_free_string(markdata);
				if(data->value.song->time != MPD_SONG_NO_TIME)
				{
					time += data->value.song->time;
				}
				if(data->value.song->file == NULL)
				{
					debug_printf(DEBUG_WARNING,"pl3_browser_view_folder: crap mpdSong has no file attribute.\n");
				}
				gtk_list_store_append (pl3_tb_store, &iter);
				gtk_list_store_set (pl3_tb_store, &iter,
						2, buffer,
						0, data->value.song->file,
						1, PL3_ENTRY_SONG,
						5,"media-audiofile",
						-1);

				data = mpd_data_get_next(data);
			}
		}
	}
	gtk_tree_path_free(path);	
	g_strfreev(tk_format);
	return time;
}


void pl3_custom_tag_browser_right_mouse_menu(GdkEventButton *event)
{
	/* we need an model and a iter */
	GtkTreeModel *model = GTK_TREE_MODEL(pl3_tb_tree);
	GtkTreeIter iter;
	int depth = 0;
	GtkTreePath *path;	
	/* se need a GtkTreeSelection to know what is selected */
	GtkTreeSelection *selection = gtk_tree_view_get_selection((GtkTreeView *)glade_xml_get_widget (pl3_xml, "cat_tree"));
	
	/* get and check for selected */
	if(!gtk_tree_selection_get_selected(selection,&model, &iter))
	{
		/* Nothin selected? then we don't have todo anything */
		return;
	}
	/* now we want to know what level we are, and what we need to show */
	path = gtk_tree_model_get_path(GTK_TREE_MODEL(pl3_tb_tree), &iter);
	if(path == NULL)
	{
		debug_printf(DEBUG_INFO,"Failed to get path\n");
		return;
	}
	depth = gtk_tree_path_get_depth(path);

	gtk_tree_path_free(path);	
	if(depth > 2)
	{
		/* here we have:  Add. Replace*/
		GtkWidget *item;
		GtkWidget *menu = gtk_menu_new();	
		/* add the add widget */
		item = gtk_image_menu_item_new_from_stock(GTK_STOCK_ADD,NULL);
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
		g_signal_connect(G_OBJECT(item), "activate", G_CALLBACK(pl3_custom_tag_browser_add_folder), NULL);		


		/* add the replace widget */
		item = gtk_image_menu_item_new_with_label("Replace");
		gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(item),
				gtk_image_new_from_stock(GTK_STOCK_REDO, GTK_ICON_SIZE_MENU));                   	
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
		g_signal_connect(G_OBJECT(item), "activate", G_CALLBACK(pl3_custom_tag_browser_replace_folder), NULL);
		
		gtk_widget_show_all(GTK_WIDGET(menu));
		gtk_menu_popup(GTK_MENU(menu), NULL, NULL,NULL, NULL, event->button, event->time);

	}
}
void pl3_custom_tag_browser_replace_folder()
{
	/* clear the playlist */
	mpd_playlist_clear(connection);
	/* add the songs */
	pl3_custom_tag_browser_add_folder();
	/* play */
	mpd_player_play(connection);
}
void pl3_custom_tag_browser_add_folder()
{
	char *first_tag, *second_tag, *format;
	char **tk_format;
	int i = 0, depth;
	GtkTreePath *path;
	GtkTreeModel *model = GTK_TREE_MODEL(pl3_tb_tree);
	GtkTreeIter iter,iter_cat;
	GtkTreeSelection *selection = gtk_tree_view_get_selection((GtkTreeView *)glade_xml_get_widget (pl3_xml, "cat_tree"));
	/* get and check for selected */
	if(!gtk_tree_selection_get_selected(selection,&model, &iter_cat))
	{
		/* Nothin selected? then we don't have todo anything */
		return;
	}

	if (check_connection_state ())
	{
		return;
	}
	gtk_tree_model_get(GTK_TREE_MODEL(pl3_tb_tree), &iter_cat, 2 , &first_tag, 1,&second_tag,PL3_CAT_BROWSE_FORMAT, &format, -1);

	path = gtk_tree_model_get_path(GTK_TREE_MODEL(pl3_tb_tree), &iter_cat);
	if(path == NULL)
	{
		printf("Failed to get path\n");
		return;
	}
	depth = gtk_tree_path_get_depth(path) -2;


	if(depth <= 0)
	{
		/*lowest level, do nothing */
		gtk_tree_path_free(path);	
		return;
	}                                    	

	tk_format = g_strsplit(format, "|",0);
	if(tk_format ==NULL)
	{
		printf("failed to split\n");
		gtk_tree_path_free(path);	
		return;
	}


	for(i=0;tk_format[i] != NULL;i++)	
	{

		if(mpd_misc_get_tag_by_name(tk_format[i])== -1)
		{

			g_strfreev(tk_format);
			gtk_tree_path_free(path);	
			return;
		}

	}

	if(depth == 1)
	{
		MpdData *data = mpd_playlist_find_adv(connection,TRUE, 
				mpd_misc_get_tag_by_name(tk_format[0]), first_tag, -1);
		/* lowest level selected*/
		if(data != NULL)
		{
			while(data != NULL)
			{
				mpd_playlist_queue_add(connection,data->value.song->file);
				data = mpd_data_get_next(data);
			}
			mpd_playlist_queue_commit(connection);
		}
	}
	else if(depth == 2)
	{
		/* second level */
		MpdData *data = mpd_playlist_find_adv(connection,TRUE,
				mpd_misc_get_tag_by_name(tk_format[0]),first_tag,
				mpd_misc_get_tag_by_name(tk_format[1]), second_tag, -1);

		if(data != NULL)
		{
			while(data != NULL)
			{
				mpd_playlist_queue_add(connection,data->value.song->file);
				data = mpd_data_get_next(data);
			}
			mpd_playlist_queue_commit(connection);
		}

	}
	else if (depth == 3)
	{
		char *first;
		/* take the upper one */
		/* go 2 up */
		if(gtk_tree_path_up(path) && gtk_tree_path_up(path))
		{
			MpdData *data  = NULL;
			gtk_tree_model_get_iter(GTK_TREE_MODEL(pl3_tb_tree), &iter, path);
			gtk_tree_model_get(GTK_TREE_MODEL(pl3_tb_tree), &iter, 1, &first, -1);

			/* artist and album is selected */
			data = mpd_playlist_find_adv(connection,TRUE,
					mpd_misc_get_tag_by_name(tk_format[0]),	first,
					mpd_misc_get_tag_by_name(tk_format[1]), first_tag,
					mpd_misc_get_tag_by_name(tk_format[2]), second_tag,-1);

			if(data != NULL)
			{
				while(data != NULL)
				{
					mpd_playlist_queue_add(connection,data->value.song->file);
					data = mpd_data_get_next(data);
				}
				mpd_playlist_queue_commit(connection);
			}

		}
	}
	gtk_tree_path_free(path);	
	g_strfreev(tk_format);
	return ;
}

void pl3_custom_tag_browser_show_info(GtkTreeView *tree, GtkTreeIter *iter)
{
	GtkTreeModel *model = gtk_tree_view_get_model(tree);
	if(mpd_server_check_version(connection,0,12,0))
	{
		char *path;
		MpdData *data;
		gtk_tree_model_get (model, iter, PL3_SONG_ID, &path, -1);
		data = mpd_playlist_find_adv(connection,TRUE,MPD_TAG_ITEM_FILENAME,path,-1);
		while(data != NULL)
		{
			call_id3_window_song(mpd_songDup(data->value.song));
			data = mpd_data_get_next(data);
		}
	}
}

void pl3_custom_tag_browser_row_activated(GtkTreeView *tree, GtkTreePath *tp)
{
      GtkTreeIter iter;
      gchar *song_id;
      gtk_tree_model_get_iter(gtk_tree_view_get_model(tree), &iter, tp);
      gtk_tree_model_get(gtk_tree_view_get_model(tree), &iter, PL3_SONG_ID,&song_id, -1);
      if(song_id == NULL) return;
      pl3_push_statusbar_message("Added a song");
      mpd_playlist_queue_add(connection, song_id);
      mpd_playlist_queue_commit(connection);
}

void pl3_custom_tag_browser_category_selection_changed(GtkTreeView *tree,GtkTreeIter *iter)
{
	 long unsigned time= 0;
	 gchar *string;        			
	 gtk_list_store_clear(pl3_tb_store);	
	 time = pl3_custom_tag_browser_view_folder(iter);
	 gtk_tree_view_set_model(tree, GTK_TREE_MODEL(pl3_tb_store));
	 string = format_time(time);
	 gtk_statusbar_push(GTK_STATUSBAR(glade_xml_get_widget(pl3_xml, "statusbar2")),0, string);
	 g_free(string);
}

