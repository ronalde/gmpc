#include <gtk/gtk.h>
#include <glade/glade.h>
#include "libmpdclient.h"
#include "strfsong.h"   
#include "main.h"
#include "playlist2.h"

extern GladeXML *pl2_xml; 
extern GtkTreeModel *pl2_fil;
/* testing */
GdkRectangle rect;
GtkWidget *tipwindow;
gulong timeout_tooltip = 0;
static gboolean mw_tooltip_timeout(GtkWidget *tv);
PangoLayout *layout_tooltip = NULL;

gchar *get_tooltip_text()
{
	GtkTreeIter iter;
	GtkTreePath *path;
	int id = -1;
	gchar result[1024];
	if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(glade_xml_get_widget(pl2_xml, "pl_tree")), rect.x, rect.y, &path, NULL, NULL, NULL))
	{
		mpd_InfoEntity *ent;
		gtk_tree_model_get_iter(GTK_TREE_MODEL(pl2_fil), &iter, path);	
		gtk_tree_model_get(GTK_TREE_MODEL(pl2_fil),&iter,SONG_POS, &id, -1); 
		g_print("song id: %i\n", id);
		mpd_sendPlaylistInfoCommand(info.connection, id);
		if((ent = mpd_getNextInfoEntity(info.connection)) != NULL)
		{
			strfsong(result, 1024,
	                		"[%name%\n&[<b>Artist:</b>\t%artist%\n]<b>Title:</b>\t%title%[\n%album%]]"
					"|%name%|[<b>Artist:</b>\t%artist%\n]<b>Title:</b>\t%title%[\n<b>Album:</b>\t%album%]|%shortfile%|", ent->info.song);
			mpd_freeInfoEntity(ent);	
		}
		else
		{
			strcpy(result, "not found");
		}
		mpd_finishCommand(info.connection);
		return g_strdup(result);
	}


	
	return g_strdup("Couldn't find any");
}



void mw_paint_tip(GtkWidget *widget, GdkEventExpose *event)
{
	GtkStyle *style;
	char *tooltiptext = get_tooltip_text();
	if(tooltiptext == NULL) tooltiptext = g_strdup("oeps");
	pango_layout_set_markup(layout_tooltip, tooltiptext, strlen(tooltiptext));
	pango_layout_set_wrap(layout_tooltip, PANGO_WRAP_WORD);
	pango_layout_set_width(layout_tooltip, 300000);
	style = tipwindow->style;

	gtk_paint_flat_box (style, tipwindow->window, GTK_STATE_NORMAL, GTK_SHADOW_OUT,
			NULL, tipwindow, "tooltip", 0, 0, -1, -1);


	gtk_paint_layout (style, tipwindow->window, GTK_STATE_NORMAL, TRUE,
			NULL, tipwindow, "tooltip", 4, 4, layout_tooltip);
	/*
	   g_object_unref(layout);
	   */
	g_free(tooltiptext);
	return;
}

gboolean mw_tooltip_timeout(GtkWidget *tv)
{
	int scr_w,scr_h, w, h, x, y;
	char *tooltiptext = NULL;

	tooltiptext = get_tooltip_text();

	tipwindow = gtk_window_new(GTK_WINDOW_POPUP);
	tipwindow->parent = tv;
	gtk_widget_set_app_paintable(tipwindow, TRUE);
	gtk_window_set_resizable(GTK_WINDOW(tipwindow), FALSE);
	gtk_widget_set_name(tipwindow, "gtk-tooltips");
	g_signal_connect(G_OBJECT(tipwindow), "expose_event",
			G_CALLBACK(mw_paint_tip), NULL);
	gtk_widget_ensure_style (tipwindow);

	layout_tooltip = gtk_widget_create_pango_layout (tipwindow, NULL);
	pango_layout_set_wrap(layout_tooltip, PANGO_WRAP_WORD);
	pango_layout_set_width(layout_tooltip, 300000);
	pango_layout_set_markup(layout_tooltip, tooltiptext, strlen(tooltiptext));
	scr_w = gdk_screen_width();
	scr_h = gdk_screen_height();
	pango_layout_get_size (layout_tooltip, &w, &h);
	w = PANGO_PIXELS(w) + 8;
	h = PANGO_PIXELS(h) + 8;

	gdk_window_get_pointer(NULL, &x, &y, NULL);
//	if (GTK_WIDGET_NO_WINDOW(mw.window))
//		y+=mw.window->allocation.y;

	x -= ((w >> 1) + 4);

	if ((x + w) > scr_w)
		x -= (x + w) - scr_w;
	else if (x < 0)
		x = 0;

	if ((y + h + 4) > scr_h)
		y = y - h;
	else
		y = y + 6;
	/*
	   g_object_unref(layout);
	   */
	g_free(tooltiptext);
	gtk_widget_set_size_request(tipwindow, w, h);
	gtk_window_move(GTK_WINDOW(tipwindow), x, y);
	gtk_widget_show(tipwindow);

	return FALSE;
}

gboolean mw_motion_cb (GtkWidget *tv, GdkEventMotion *event, gpointer null)
{
	GtkTreePath *path;
//	if(!settings.list_tooltip) return FALSE;

	if(rect.y == 0 && rect.height == 0 && timeout_tooltip)
	{
		g_source_remove(timeout_tooltip);
		timeout_tooltip = 0;
		if (tipwindow) {
			gtk_widget_destroy(tipwindow);
			tipwindow = NULL;
		}
		return FALSE;

	}
	if (timeout_tooltip) {
		if (((int)event->y > rect.y) && (((int)event->y - rect.height) < rect.y))
			return FALSE;

		if(event->y == 0)
		{
			g_source_remove(timeout_tooltip);
			return FALSE;

		}
		/* We've left the cell.  Remove the timeout and create a new one below */
		if (tipwindow) {
			gtk_widget_destroy(tipwindow);
			tipwindow = NULL;
		}

		g_source_remove(timeout_tooltip);
	}

	if(gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(tv), event->x, event->y, &path, NULL, NULL, NULL))
	{
		gtk_tree_view_get_cell_area(GTK_TREE_VIEW(tv), path, NULL, &rect);
		gtk_tree_path_free(path);
		if(rect.y != 0 && rect.height != 0) timeout_tooltip = g_timeout_add(500, (GSourceFunc)mw_tooltip_timeout, tv);
	}
	return FALSE;
}

void mw_leave_cb (GtkWidget *w, GdkEventCrossing *e, gpointer n)
{
	if (timeout_tooltip) {
		g_source_remove(timeout_tooltip);
		timeout_tooltip = 0;
	}
	if (tipwindow) {
		gtk_widget_destroy(tipwindow);
		g_object_unref(layout_tooltip);
		tipwindow = NULL;
	}
}

