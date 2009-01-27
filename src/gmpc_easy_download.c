/* Gnome Music Player Client (GMPC)
 * Copyright (C) 2004-2009 Qball Cow <qball@sarine.nl>
 * Project homepage: http://gmpcwiki.sarine.nl/
 
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <curl/curl.h>
#include <libmpd/debug_printf.h>
#include <libsoup/soup.h>
#include "gmpc_easy_download.h"
#include "main.h"
#define CURL_TIMEOUT 10 

static int quit = FALSE;
/*
static size_t write_data(void *buffer, size_t size, size_t nmemb,void *stream )
{
    gmpc_easy_download_struct *dld = stream;
        if(!size || !nmemb)
		return 0;
	if(dld->data == NULL)
	{
		dld->size = 0;
	}
	dld->data = g_realloc(dld->data,(gulong)(size*nmemb+dld->size)+1);

	memset(&(dld->data)[dld->size], '\0', (size*nmemb)+1);
	memcpy(&(dld->data)[dld->size], buffer, size*nmemb);

	dld->size += size*nmemb;
	if(dld->size >= dld->max_size && dld->max_size > 0)
	{
		return 0;
	}
	return size*nmemb;
}
*/
int gmpc_easy_download(const char *url,gmpc_easy_download_struct *dld)
{

    SoupSession *session = NULL;
    SoupMessage *msg = NULL;
    int status;
	int success = FALSE;

	/*int res;*/
	if(!dld) return 0;
	if(url == NULL) return 0;
	if(url[0] == '\0') return 0;
	/**
	 * Make sure it's clean
	 */
	gmpc_easy_download_clean(dld);

    /** Check for local url */
    if(strncmp(url, "http://", 7) && g_file_test(url, G_FILE_TEST_EXISTS))
    {
        gsize size;
        if(g_file_get_contents(url, &(dld->data),&(size), NULL))
        {
            dld->size = (int)size;
            return 1;
        }
        return 0;
    }

    if(cfg_get_single_value_as_int_with_default(config, "Network Settings", "Use Proxy", FALSE))
    {
            char *value = cfg_get_single_value_as_string(config, "Network Settings", "Proxy Address");
            gint port =  cfg_get_single_value_as_int_with_default(config, "Network Settings", "Proxy Port",8080);
            if(value)
            {
                gchar *ppath = g_strdup_printf("http://%s:%i", value, port);
                SoupURI *uri = soup_uri_new(ppath);
                session = soup_session_sync_new_with_options(SOUP_SESSION_PROXY_URI, uri,NULL);
                soup_uri_free(uri);
                g_free(ppath);
                g_free(value);
            }
    }
    if(!session){
        session = soup_session_sync_new();
    }

    msg = soup_message_new("GET",url);
    status = soup_session_send_message(session, msg);
    if(SOUP_STATUS_IS_SUCCESSFUL (status)){
        soup_message_body_flatten(msg->response_body);
        dld->size = msg->response_body->length;
        dld->data = (char *)msg->response_body->data;
        msg->response_body->data = NULL;
        msg->response_body->length = 0;
        success = 1;

        printf("gmpc easy download success: %s\n",url);
    }
    else 
    {
        printf("gmpc easy download fail: %s: %s\n",url, soup_status_get_phrase(status));
        success = 0;
    }
    g_object_unref(session);
	if(success) return 1;
	if(dld->data) q_free(dld->data);
	dld->data = NULL;
	return 0;
}

void gmpc_easy_download_clean(gmpc_easy_download_struct *dld)
{
	if(dld->data)q_free(dld->data);
	dld->data = NULL;
	dld->size = 0;
}

/***
 * preferences window
 */
static GtkWidget *proxy_pref_frame = NULL;
static void proxy_pref_destroy(GtkWidget *container)
{
	gtk_container_remove(GTK_CONTAINER(container), proxy_pref_frame);
	proxy_pref_frame = NULL;
}
static void proxy_pref_use_proxy_toggled(GtkWidget *toggle_button)
{
	cfg_set_single_value_as_int(config, "Network Settings", "Use Proxy",
			gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle_button)));
}
static void proxy_pref_http_adress_changed(GtkWidget *entry)
{
		cfg_set_single_value_as_string(config, "Network Settings", "Proxy Address",(char *)gtk_entry_get_text(GTK_ENTRY(entry)));
}
static void proxy_pref_http_port_changed(GtkWidget *entry)
{
		cfg_set_single_value_as_int(config, "Network Settings", "Proxy Port",gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry)));
}
static void proxy_pref_construct(GtkWidget *container)
{
	GtkWidget *temp = NULL, *vbox,*hbox;
	gchar *value = NULL;
	/* Create frame and create a widget with markup for the frame */
	proxy_pref_frame = gtk_frame_new("");
	temp = gtk_label_new(_("Proxy"));
	value = g_markup_printf_escaped("<b>%s</b>", _("Proxy"));
	gtk_label_set_markup(GTK_LABEL(temp),value); 
	g_free(value);
	gtk_frame_set_label_widget(GTK_FRAME(proxy_pref_frame), temp);
	gtk_frame_set_shadow_type(GTK_FRAME(proxy_pref_frame), GTK_SHADOW_NONE);
	/* setup vbox for inside the frame */
	vbox = gtk_vbox_new(FALSE,6);
	gtk_container_set_border_width(GTK_CONTAINER(vbox),12);
	gtk_container_add(GTK_CONTAINER(proxy_pref_frame), vbox);
	/* enable/disable */
	temp = gtk_check_button_new_with_label(_("Use a proxy for internet connectivity"));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(temp), 
			cfg_get_single_value_as_int_with_default(config, "Network Settings", "Use Proxy", FALSE));
	gtk_box_pack_start(GTK_BOX(vbox), temp,FALSE,FALSE,0);
	g_signal_connect(G_OBJECT(temp), "toggled", G_CALLBACK(proxy_pref_use_proxy_toggled), NULL);
	/* Add other widgets */
	hbox= gtk_hbox_new(FALSE,6);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,0);
	/* label */
	temp = gtk_label_new(_("HTTP Proxy:"));
	gtk_box_pack_start(GTK_BOX(hbox), temp, FALSE,FALSE,0);
	/* entry (address)*/
	temp = gtk_entry_new();
	value = cfg_get_single_value_as_string(config, "Network Settings", "Proxy Address");
	if(value) {
		gtk_entry_set_text(GTK_ENTRY(temp),value); 
	}
	gtk_entry_set_width_chars(GTK_ENTRY(temp), 20);
	gtk_box_pack_start(GTK_BOX(hbox), temp, FALSE,FALSE,0);
	g_signal_connect(G_OBJECT(temp), "changed", G_CALLBACK(proxy_pref_http_adress_changed), NULL);
	/* label */
	temp = gtk_label_new(_("Port:"));
	gtk_box_pack_start(GTK_BOX(hbox), temp, FALSE,FALSE,0);
	/* spinbox (port) */
	temp = gtk_spin_button_new_with_range(1,65536,1);	
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(temp),(double)
			cfg_get_single_value_as_int_with_default(config, "Network Settings", "Proxy Port",8080));
	gtk_box_pack_start(GTK_BOX(hbox), temp, FALSE,FALSE,0);
	g_signal_connect(G_OBJECT(temp), "value-changed", G_CALLBACK(proxy_pref_http_port_changed), NULL);


	gtk_widget_show_all(proxy_pref_frame);
	gtk_container_add(GTK_CONTAINER(container), proxy_pref_frame);
}

void quit_easy_download(void)
{
    debug_printf(DEBUG_INFO,"quitting easy download\n");
    quit = TRUE;
}
gmpcPrefPlugin proxyplug_pref = {
	.construct		= proxy_pref_construct,
	.destroy		= proxy_pref_destroy
};
gmpcPlugin proxyplug = {
	.name 			= N_("Proxy"),
	.version 	 	= {0,0,0},
	.plugin_type	= GMPC_INTERNALL,
	.pref			= &proxyplug_pref
};

/**
 * LIBSOUP BASED ASYNC DOWNLOADER
 */
typedef struct {
    SoupMessage *msg;
    gchar *uri;
    GEADAsyncCallback callback; 
    gpointer userdata;
    
}_GEADAsyncHandler;
static SoupSession *soup_session = NULL;
static void gmpc_easy_async_status_update(SoupMessage *msg, SoupBuffer *buffer, gpointer data)
{
    _GEADAsyncHandler *d = data;
    d->callback((GEADAsyncHandler *)d,GEAD_PROGRESS, d->userdata);
}

static void gmpc_easy_async_callback(SoupSession *session, SoupMessage *msg, gpointer data)
{
    _GEADAsyncHandler *d = data;
    if(SOUP_STATUS_IS_SUCCESSFUL (msg->status_code)){
        d->callback((GEADAsyncHandler *)d,GEAD_DONE, d->userdata);
    }
    else if (msg->status_code == SOUP_STATUS_CANCELLED)
    {
        printf("Cancelled\n");
        d->callback((GEADAsyncHandler *)d,GEAD_CANCELLED,d->userdata);
    }
    else {
        printf("Failed\n");
        d->callback((GEADAsyncHandler *)d,GEAD_FAILED,d->userdata);
    }
}


void gmpc_easy_async_free_handler(GEADAsyncHandler *handle)
{
    _GEADAsyncHandler *d = (_GEADAsyncHandler *)handle;
    /*
    if(d->msg){
        g_object_unref(d->msg);
        d->msg = NULL;
    }
    */
    g_free(d->uri);
    g_free(d);
}

goffset gmpc_easy_handler_get_content_size(GEADAsyncHandler *handle)
{
    _GEADAsyncHandler *d = (_GEADAsyncHandler *)handle;
    return soup_message_headers_get_content_length(d->msg->response_headers);
}

const char  * gmpc_easy_handler_get_uri(GEADAsyncHandler *handle)
{
    _GEADAsyncHandler *d = (_GEADAsyncHandler *)handle;
    return d->uri;
}

const char * gmpc_easy_handler_get_data(GEADAsyncHandler *handle, goffset *length)
{
    _GEADAsyncHandler *d = (_GEADAsyncHandler *)handle;
    SoupBuffer *buf = soup_message_body_flatten(d->msg->response_body);
    soup_buffer_free(buf);
    if(length)
	    *length = d->msg->response_body->length; 
    return d->msg->response_body->data;
}

void gmpc_easy_async_cancel(GEADAsyncHandler *handle)
{
    _GEADAsyncHandler *d = (_GEADAsyncHandler *)handle;
    soup_session_cancel_message(soup_session, d->msg, SOUP_STATUS_CANCELLED);
}

GEADAsyncHandler *gmpc_easy_async_downloader(const gchar *uri, GEADAsyncCallback callback, gpointer user_data)
{
    SoupMessage *msg;
    _GEADAsyncHandler *d;
    if(soup_session == NULL) {
        if(cfg_get_single_value_as_int_with_default(config, "Network Settings", "Use Proxy", FALSE))
        {
            char *value = cfg_get_single_value_as_string(config, "Network Settings", "Proxy Address");
            gint port =  cfg_get_single_value_as_int_with_default(config, "Network Settings", "Proxy Port",8080);
            if(value)
            {
                gchar *ppath = g_strdup_printf("http://%s:%i", value, port);
                SoupURI *puri = soup_uri_new(ppath);
                soup_session = soup_session_async_new_with_options(SOUP_SESSION_PROXY_URI, puri,NULL);
                soup_uri_free(puri);
                g_free(ppath);
                g_free(value);
            }
        }
        if(!soup_session){
            soup_session = soup_session_async_new();
        }
    }

    msg = soup_message_new("GET", uri);
    if(!msg) return NULL;

    d = g_malloc0(sizeof(*d));
    d->msg = msg;
    d->uri = g_strdup(uri);
    d->callback = callback;
    d->userdata = user_data;

//    soup_message_body_set_accumulate(d->msg->response_body,TRUE);
    g_signal_connect_after(msg, "got-chunk", G_CALLBACK(gmpc_easy_async_status_update), d);
    soup_session_queue_message(soup_session, msg, gmpc_easy_async_callback, d);
    
    return (GEADAsyncHandler*)d;
}


void gmpc_easy_async_quit(void)
{
    if(soup_session)
    {
        soup_session_abort(soup_session);
        g_object_unref(soup_session);
        soup_session = NULL;
    }
}
