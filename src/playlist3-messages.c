#include <gtk/gtk.h>
#include "main.h" 

extern GladeXML *pl3_xml;
static gboolean error_visible = FALSE;
guint timeout_callback = 0;

void playlist3_show_error_message(const gchar *message, ErrorLevel el)
{
	GtkWidget *event;
	GtkWidget *label = NULL; 
	if(error_visible)
	{
		playlist3_close_error();
		g_source_remove(timeout_callback);
		timeout_callback = 0;
	}
	/* Error */
	error_visible = TRUE;

	event = glade_xml_get_widget(pl3_xml, "error_hbox"); 
	/* right image */
	switch(el)
	{
		case ERROR_CRITICAL:
			label = gtk_image_new_from_stock(GTK_STOCK_DIALOG_ERROR, GTK_ICON_SIZE_BUTTON);
			break;
		case ERROR_WARNING:
			label = gtk_image_new_from_stock(GTK_STOCK_DIALOG_WARNING, GTK_ICON_SIZE_BUTTON);
			break;
		default:
			label = gtk_image_new_from_stock(GTK_STOCK_DIALOG_INFO, GTK_ICON_SIZE_BUTTON);
			break;
	}

	gtk_box_pack_start(GTK_BOX(event), label, FALSE, TRUE, 0);
	label = gtk_label_new("") ;
	gtk_label_set_markup(GTK_LABEL(label),message);
	gtk_misc_set_alignment(GTK_MISC(label), 0,0.5);
	gtk_label_set_ellipsize(GTK_LABEL(label), PANGO_ELLIPSIZE_END);
	gtk_box_pack_start(GTK_BOX(event), label, TRUE, TRUE, 0);

	event = glade_xml_get_widget(pl3_xml, "error_event");
	gtk_widget_show_all(event);
	
	timeout_callback = g_timeout_add(5000, G_CALLBACK(playlist3_close_error), NULL);

}	

void playlist3_error_add_widget(GtkWidget *widget)
{
	GtkWidget *event = glade_xml_get_widget(pl3_xml, "error_hbox"); 
	gtk_box_pack_end(GTK_BOX(event), widget, FALSE, TRUE, 0);	
	gtk_widget_show_all(event);
}

void playlist3_close_error(void)
{
	if(error_visible)
	{
		error_visible = FALSE;
		g_source_remove(timeout_callback);
		timeout_callback = 0;

		if(pl3_xml)
		{
			GtkWidget *event = glade_xml_get_widget(pl3_xml, "error_event");
			gtk_widget_hide(event);
			event = glade_xml_get_widget(pl3_xml, "error_hbox"); 
			gtk_container_foreach(GTK_CONTAINER(event), (GtkCallback)(gtk_widget_destroy), NULL);
		}
	}
}