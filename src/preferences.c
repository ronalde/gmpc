#include <gtk/gtk.h>
#include <glade/glade.h>
#include <string.h>
#include <time.h>
#include <config.h>

#include "plugin.h"

#include "main.h"
#include "playlist3.h"
#include "config1.h"

#define PLUGIN_STATS -200

static void plugin_stats_construct(GtkWidget *);
static void plugin_stats_destroy(GtkWidget *);
GladeXML *plugin_stat_xml = NULL;
/* About "plugin" */

/* End About */
GtkListStore *plugin_store = NULL;
GladeXML *xml_preferences_window = NULL;
gboolean running = 0;

/* Glade Prototypes, without glade these would be static */
void preferences_window_connect(GtkWidget *but);
void preferences_window_disconnect(GtkWidget *but);
void auth_enable_toggled(GtkToggleButton *but);
void entry_auth_changed(GtkEntry *entry);
void create_preferences_window(void);
void preferences_window_destroy(void);

int plugin_last;

static void pref_plugin_changed(void)
{
	GtkTreeSelection *sel = gtk_tree_view_get_selection(GTK_TREE_VIEW (glade_xml_get_widget(xml_preferences_window, "plugin_tree")));
	GtkTreeModel *model = GTK_TREE_MODEL(plugin_store);
	GtkTreeIter iter;
	int id = 0;
	if(plugin_last >= 0)
	{
		plugins[plugin_last]->pref->destroy(glade_xml_get_widget(xml_preferences_window, "plugin_container"));
		plugin_last = -1;

	}
	else if(plugin_last == PLUGIN_STATS)
	{
		plugin_stats_destroy(glade_xml_get_widget(xml_preferences_window, "plugin_container"));
	}
	if(gtk_tree_selection_get_selected(sel, &model, &iter))
	{
		gtk_tree_model_get(GTK_TREE_MODEL(plugin_store), &iter, 0, &id, -1);
		if(id >= 0 && plugins[id]->pref)
		{
			if(plugins[id]->pref->construct)
			{
				char *buf = NULL;
				if(plugins[id]->plugin_type != GMPC_INTERNALL && !(plugins[id]->id&PLUGIN_ID_INTERNALL))
				{
					buf = g_strdup_printf("<span size=\"xx-large\"><b>%s</b></span>\n<i>Plugin version: %i.%i.%i</i>", 
							plugins[id]->name,
							plugins[id]->version[0],plugins[id]->version[1], plugins[id]->version[2]);
				}
				else
				{
					buf = g_strdup_printf("<span size=\"xx-large\"><b>%s</b></span>",
							_(plugins[id]->name));
				}

				plugins[id]->pref->construct(glade_xml_get_widget(xml_preferences_window, "plugin_container"));
				plugin_last = id;
				gtk_label_set_markup(GTK_LABEL(glade_xml_get_widget(xml_preferences_window, "plugin_label")),buf);
				q_free(buf);
				return;
			}
		}
		else if(id == PLUGIN_STATS)
		{
			gtk_label_set_markup(GTK_LABEL(glade_xml_get_widget(xml_preferences_window, 
						"plugin_label")),
					_("<span size=\"xx-large\"><b>Plugins</b></span>"));

			plugin_stats_construct(glade_xml_get_widget(xml_preferences_window,
						"plugin_container"));
			plugin_last = id;
			return;
		}
	}
	gtk_label_set_markup(GTK_LABEL(glade_xml_get_widget(xml_preferences_window, "plugin_label")),
			"<span size=\"xx-large\"><b>Nothing Selected</b></span>");
}

void create_preferences_window()
{
	GtkWidget *pl3_win = glade_xml_get_widget(pl3_xml, "pl3_win");
	GtkWidget *dialog;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkWidget *label;
	int plugs = 0;
	int i=0;
	char *string = NULL;

	if(running)
	{
		if(xml_preferences_window == NULL)
		{
			running = 0;
		} 
		else
		{
			dialog = glade_xml_get_widget(xml_preferences_window, "preferences_window");
			gtk_window_present(GTK_WINDOW(dialog));
			return;
		}
	}
	plugin_last = -1;
	string = gmpc_get_full_glade_path("gmpc.glade");
	xml_preferences_window = glade_xml_new(string, "preferences_window", NULL);
	q_free(string);
	/* check for errors and axit when there is no gui file */
	if(xml_preferences_window == NULL)  g_error("Couldnt initialize GUI. Please check installation\n");


	/* set info from struct */
	/* hostname */
	dialog = glade_xml_get_widget(xml_preferences_window, "preferences_window");
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(pl3_win));
	gtk_widget_show_all(GTK_WIDGET(dialog));
	running = 1;

	plugin_store = gtk_list_store_new(2, G_TYPE_INT, G_TYPE_STRING);
	column = gtk_tree_view_column_new ();
	gtk_tree_view_column_set_title(column, _("Plugins:"));
	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (column, renderer, TRUE);
	gtk_tree_view_column_set_attributes (column,renderer,"markup", 1, NULL);	
	gtk_tree_view_append_column (GTK_TREE_VIEW (glade_xml_get_widget(xml_preferences_window, "plugin_tree")), column);


	g_signal_connect(G_OBJECT(gtk_tree_view_get_selection(GTK_TREE_VIEW (glade_xml_get_widget(xml_preferences_window, "plugin_tree")))), 
			"changed", G_CALLBACK(pref_plugin_changed), NULL);
	gtk_tree_view_set_model(GTK_TREE_VIEW(glade_xml_get_widget(xml_preferences_window, "plugin_tree")), GTK_TREE_MODEL(plugin_store));

	/* internals */
	for(i=0; i< num_plugins; i++)
	{
		if(plugins[i]->pref != NULL)
		{
			if(plugins[i]->id&PLUGIN_ID_INTERNALL)
			{
				GtkTreeIter iter;
				gtk_list_store_append(GTK_LIST_STORE(plugin_store), &iter);
				gtk_list_store_set(GTK_LIST_STORE(plugin_store), &iter,
						0, plugin_get_pos(plugins[i]->id)/*^PLUGIN_ID_MARK*/,
						1, _(plugins[i]->name), -1);
				if(gtk_tree_selection_count_selected_rows(gtk_tree_view_get_selection(
								GTK_TREE_VIEW(glade_xml_get_widget(xml_preferences_window, "plugin_tree")))) == 0)
				{
					gtk_tree_selection_select_iter(gtk_tree_view_get_selection(
								GTK_TREE_VIEW(glade_xml_get_widget(xml_preferences_window, "plugin_tree"))),&iter);
				}
			}
			else
			{
				plugs++;
			}
		}
	}
	/* plugins */
	{
		GtkTreeIter iter;
		gtk_list_store_append(GTK_LIST_STORE(plugin_store), &iter);
		gtk_list_store_set(GTK_LIST_STORE(plugin_store), &iter, 0,PLUGIN_STATS, 1, _("<b>Plugins:</b>"), -1);
		for(i=0; i< num_plugins; i++)
		{
			if(plugins[i]->pref != NULL && plugins[i]->id&PLUGIN_ID_MARK)
			{
				gtk_list_store_append(GTK_LIST_STORE(plugin_store), &iter);
				gtk_list_store_set(GTK_LIST_STORE(plugin_store), &iter,
						0, plugin_get_pos(plugins[i]->id),
						1, plugins[i]->name,
						-1);
			}
		}
	}

	label = glade_xml_get_widget(xml_preferences_window, "plugin_label_box");
	gtk_widget_modify_bg(label, GTK_STATE_NORMAL, &dialog->style->light[GTK_STATE_SELECTED]);
/*	label = glade_xml_get_widget(xml_preferences_window, "plugin_label");
	gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &dialog->style->fg[GTK_STATE_SELECTED]);
*/	glade_xml_signal_autoconnect(xml_preferences_window);	

}

/* destory the preferences window */
void preferences_window_destroy()
{
	GtkWidget *dialog = glade_xml_get_widget(xml_preferences_window, "preferences_window");
	if(plugin_last >= 0)
	{
		plugins[plugin_last]->pref->destroy(glade_xml_get_widget(xml_preferences_window, "plugin_container"));
		plugin_last = -1;

	}	                                                                                                     	
	else if(plugin_last == PLUGIN_STATS)
	{
		plugin_stats_destroy(glade_xml_get_widget(xml_preferences_window, "plugin_container"));
	}
	gtk_widget_destroy(dialog);
	g_object_unref(xml_preferences_window);
	g_object_unref(plugin_store);
	plugin_store = NULL;
	xml_preferences_window = NULL;
	running = 0;
}

static void pref_plugin_enabled(GtkCellRendererToggle *rend, gchar *path, GtkListStore *store)
{
	GtkTreeIter iter;
	if(gtk_tree_model_get_iter_from_string(GTK_TREE_MODEL(store), &iter, path))
	{
		int toggled;
		gmpcPlugin *plug = NULL;
		gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, 0, &toggled, 3, &plug, -1);
		if(plug->set_enabled != NULL)
		{
			plug->set_enabled(!toggled);
			gtk_list_store_set(store, &iter, 0,plug->get_enabled(),-1);
		}


	}

}
static void plugin_stats_construct(GtkWidget *container)
{
	gchar *path = gmpc_get_full_glade_path("gmpc.glade");
	plugin_stat_xml = glade_xml_new(path, "plugin_stats_vbox",NULL);
	q_free(path);
	if(plugin_stat_xml)
	{
		GtkWidget *tree = glade_xml_get_widget(plugin_stat_xml, "plugin_stats_tree");
		GtkListStore *store = NULL;
		GtkTreeIter iter;
		GtkCellRenderer *renderer = NULL;
		int i=0;
		GtkWidget *vbox = glade_xml_get_widget(plugin_stat_xml, "plugin_stats_vbox");


		/**
		 * new 
		 */
		store = gtk_list_store_new(5,G_TYPE_BOOLEAN, G_TYPE_STRING, G_TYPE_STRING,G_TYPE_POINTER,G_TYPE_STRING);

		gtk_tree_view_set_model(GTK_TREE_VIEW(tree),GTK_TREE_MODEL(store));
		renderer = gtk_cell_renderer_toggle_new();
		g_object_set_data(G_OBJECT(renderer), "editable", GINT_TO_POINTER(1));
		gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW(tree), -1,_("Enabled"), renderer, "active", 0,NULL);	
		g_signal_connect(G_OBJECT(renderer), "toggled", G_CALLBACK(pref_plugin_enabled), store);
		renderer = gtk_cell_renderer_text_new();
		gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW(tree), -1,_("Name"), renderer, "text", 1,NULL);
		renderer = gtk_cell_renderer_text_new();
		gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW(tree), -1,_("Function"), renderer, "text", 2,NULL);
		renderer = gtk_cell_renderer_text_new();
		gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW(tree), -1,_("Version"), renderer, "text", 4,NULL);
		for(i=0;i<num_plugins;i++)
		{
			if(plugins[i]->id&PLUGIN_ID_MARK)
			{
                gchar *version = g_strdup_printf("%i.%i.%i",plugins[i]->version[0], plugins[i]->version[1],plugins[i]->version[2]);
				gtk_list_store_append(store, &iter);
				gtk_list_store_set(store, &iter, 0,TRUE,1, plugins[i]->name,3,(plugins[i]),4,version, -1);
                g_free(version);
				if(plugins[i]->get_enabled != NULL)
				{
					gtk_list_store_set(store, &iter, 0,plugins[i]->get_enabled(),-1);
				}
				switch(plugins[i]->plugin_type)
				{
					case GMPC_PLUGIN_DUMMY:
						gtk_list_store_set(store, &iter, 2, _("Dummy"),-1);
						break;
					case GMPC_PLUGIN_PL_BROWSER:
						gtk_list_store_set(store, &iter, 2, _("Browser Extension"),-1);
						break;
					case GMPC_PLUGIN_META_DATA:
						gtk_list_store_set(store, &iter, 2, _("Metadata Provider"),-1);
						break;
          case GMPC_PLUGIN_PL_BROWSER|GMPC_PLUGIN_META_DATA:
						gtk_list_store_set(store, &iter, 2, _("Metadata Provider and Browser Extension"),-1);
            break;
					case GMPC_PLUGIN_NO_GUI:
						gtk_list_store_set(store, &iter, 2, _("Misc."),-1);
						break;                                                         					

					default:
						gtk_list_store_set(store, &iter, 2, _("Unknown"),-1);
						break;

				}
			}

		}
		gtk_container_add(GTK_CONTAINER(container),vbox);
	}

}
static void plugin_stats_destroy(GtkWidget *container)
{
	if(plugin_stat_xml)
	{
		GtkWidget *vbox = glade_xml_get_widget(plugin_stat_xml, "plugin_stats_vbox");
		GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(glade_xml_get_widget(plugin_stat_xml, "plugin_stats_tree")));
		gtk_container_remove(GTK_CONTAINER(container),vbox);
		if(model)g_object_unref(model);
		g_object_unref(plugin_stat_xml);
		plugin_stat_xml = NULL;
	}
}


void preferences_show_pref_window(int plugin_id)
{
	GtkTreeIter iter;
	if(xml_preferences_window == NULL)
		create_preferences_window();
	if(plugin_store)
	{
		if(gtk_tree_model_get_iter_first(GTK_TREE_MODEL(plugin_store), &iter))
		{
			do{	
				int pos;
				gtk_tree_model_get(GTK_TREE_MODEL(plugin_store), &iter, 0, &pos, -1); 
				if(pos == plugin_get_pos(plugin_id))
				{
					GtkTreeSelection *select = gtk_tree_view_get_selection(
							GTK_TREE_VIEW (glade_xml_get_widget(xml_preferences_window, "plugin_tree")));
					gtk_tree_selection_select_iter(select, &iter);
					return;
				}
			}while(gtk_tree_model_iter_next(GTK_TREE_MODEL(plugin_store),  &iter));
		}
	}	

}
