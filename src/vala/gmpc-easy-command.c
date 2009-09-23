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
#include <gmpc-plugin.h>
#include <gtk/gtk.h>
#include <gtktransition.h>
#include <config.h>
#include <glib/gi18n-lib.h>
#include <stdlib.h>
#include <string.h>
#include <plugin.h>
#include <config1.h>
#include <playlist3-messages.h>
#include <gdk/gdk.h>
#include <cairo.h>
#include <float.h>
#include <math.h>


#define GMPC_EASY_TYPE_COMMAND (gmpc_easy_command_get_type ())
#define GMPC_EASY_COMMAND(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), GMPC_EASY_TYPE_COMMAND, GmpcEasyCommand))
#define GMPC_EASY_COMMAND_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), GMPC_EASY_TYPE_COMMAND, GmpcEasyCommandClass))
#define GMPC_EASY_IS_COMMAND(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GMPC_EASY_TYPE_COMMAND))
#define GMPC_EASY_IS_COMMAND_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GMPC_EASY_TYPE_COMMAND))
#define GMPC_EASY_COMMAND_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), GMPC_EASY_TYPE_COMMAND, GmpcEasyCommandClass))

typedef struct _GmpcEasyCommand GmpcEasyCommand;
typedef struct _GmpcEasyCommandClass GmpcEasyCommandClass;
typedef struct _GmpcEasyCommandPrivate GmpcEasyCommandPrivate;
#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))
#define _g_free0(var) (var = (g_free (var), NULL))
#define _cairo_destroy0(var) ((var == NULL) ? NULL : (var = (cairo_destroy (var), NULL)))
#define _cairo_pattern_destroy0(var) ((var == NULL) ? NULL : (var = (cairo_pattern_destroy (var), NULL)))

struct _GmpcEasyCommand {
	GmpcPluginBase parent_instance;
	GmpcEasyCommandPrivate * priv;
};

struct _GmpcEasyCommandClass {
	GmpcPluginBaseClass parent_class;
};

struct _GmpcEasyCommandPrivate {
	GtkEntryCompletion* completion;
	GtkListStore* store;
	guint signals;
	GtkWindow* window;
	gint* version;
	gint version_length1;
	gint version_size;
};

typedef void (*GmpcEasyCommandCallback) (void* data, const char* param, void* user_data);

static gpointer gmpc_easy_command_parent_class = NULL;

GType gmpc_easy_command_get_type (void);
#define GMPC_EASY_COMMAND_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), GMPC_EASY_TYPE_COMMAND, GmpcEasyCommandPrivate))
enum  {
	GMPC_EASY_COMMAND_DUMMY_PROPERTY
};
#define GMPC_EASY_COMMAND_use_transition TRUE
#define GMPC_EASY_COMMAND_some_unique_name VERSION
static const char* gmpc_easy_command_real_get_name (GmpcPluginBase* base);
static gint* gmpc_easy_command_real_get_version (GmpcPluginBase* base, int* result_length1);
static gboolean gmpc_easy_command_real_get_enabled (GmpcPluginBase* base);
static void gmpc_easy_command_real_set_enabled (GmpcPluginBase* base, gboolean state);
static gboolean gmpc_easy_command_completion_function (GmpcEasyCommand* self, GtkEntryCompletion* comp, const char* key, GtkTreeIter* iter);
guint gmpc_easy_command_add_entry (GmpcEasyCommand* self, const char* name, const char* pattern, const char* hint, GmpcEasyCommandCallback* callback, void* userdata);
static void gmpc_easy_command_activate (GmpcEasyCommand* self, GtkEntry* entry);
static gboolean gmpc_easy_command_key_press_event (GmpcEasyCommand* self, GtkEntry* widget, const GdkEventKey* event);
static gboolean gmpc_easy_command_popup_expose_handler (GmpcEasyCommand* self, GtkWindow* widget, const GdkEventExpose* event);
static gboolean _gmpc_easy_command_popup_expose_handler_gtk_widget_expose_event (GtkWindow* _sender, const GdkEventExpose* event, gpointer self);
static void _gmpc_easy_command_activate_gtk_entry_activate (GtkEntry* _sender, gpointer self);
static gboolean _gmpc_easy_command_key_press_event_gtk_widget_key_press_event (GtkEntry* _sender, const GdkEventKey* event, gpointer self);
static gboolean gmpc_easy_command_focus_out_event (GmpcEasyCommand* self, GtkEntry* entry, const GdkEventFocus* event);
static gboolean _gmpc_easy_command_focus_out_event_gtk_widget_focus_out_event (GtkEntry* _sender, const GdkEventFocus* event, gpointer self);
void gmpc_easy_command_popup (GmpcEasyCommand* self);
void gmpc_easy_command_help_window_destroy (GtkDialog* window, gint response);
static void _gmpc_easy_command_help_window_destroy_gtk_dialog_response (GtkDialog* _sender, gint response_id, gpointer self);
void gmpc_easy_command_help_window (void* data, const char* param);
GmpcEasyCommand* gmpc_easy_command_new (void);
GmpcEasyCommand* gmpc_easy_command_construct (GType object_type);
static gboolean _gmpc_easy_command_completion_function_gtk_entry_completion_match_func (GtkEntryCompletion* completion, const char* key, GtkTreeIter* iter, gpointer self);
static GObject * gmpc_easy_command_constructor (GType type, guint n_construct_properties, GObjectConstructParam * construct_properties);
static void gmpc_easy_command_finalize (GObject* obj);
static void _vala_array_destroy (gpointer array, gint array_length, GDestroyNotify destroy_func);
static void _vala_array_free (gpointer array, gint array_length, GDestroyNotify destroy_func);
static gint _vala_array_length (gpointer array);



static const char* gmpc_easy_command_real_get_name (GmpcPluginBase* base) {
	GmpcEasyCommand * self;
	const char* result;
	self = (GmpcEasyCommand*) base;
	result = _ ("Gmpc Easy Command");
	return result;
}


static gint* gmpc_easy_command_real_get_version (GmpcPluginBase* base, int* result_length1) {
	GmpcEasyCommand * self;
	gint* result;
	gint* _tmp0_;
	self = (GmpcEasyCommand*) base;
	result = (_tmp0_ = self->priv->version, *result_length1 = self->priv->version_length1, _tmp0_);
	return result;
}


static gboolean gmpc_easy_command_real_get_enabled (GmpcPluginBase* base) {
	GmpcEasyCommand * self;
	gboolean result;
	self = (GmpcEasyCommand*) base;
	result = (gboolean) cfg_get_single_value_as_int_with_default (config, gmpc_plugin_base_get_name ((GmpcPluginBase*) self), "enabled", 1);
	return result;
}


static void gmpc_easy_command_real_set_enabled (GmpcPluginBase* base, gboolean state) {
	GmpcEasyCommand * self;
	gboolean _tmp0_;
	self = (GmpcEasyCommand*) base;
	_tmp0_ = FALSE;
	if (!state) {
		_tmp0_ = self->priv->window != NULL;
	} else {
		_tmp0_ = FALSE;
	}
	if (_tmp0_) {
		GtkWindow* _tmp1_;
		gtk_object_destroy ((GtkObject*) self->priv->window);
		self->priv->window = (_tmp1_ = NULL, _g_object_unref0 (self->priv->window), _tmp1_);
	}
	cfg_set_single_value_as_int (config, gmpc_plugin_base_get_name ((GmpcPluginBase*) self), "enabled", (gint) state);
}


static gpointer _g_object_ref0 (gpointer self) {
	return self ? g_object_ref (self) : NULL;
}


static gboolean gmpc_easy_command_completion_function (GmpcEasyCommand* self, GtkEntryCompletion* comp, const char* key, GtkTreeIter* iter) {
	gboolean result;
	char* value;
	GtkTreeModel* model;
	g_return_val_if_fail (self != NULL, FALSE);
	g_return_val_if_fail (comp != NULL, FALSE);
	g_return_val_if_fail (key != NULL, FALSE);
	value = NULL;
	model = _g_object_ref0 (gtk_entry_completion_get_model (comp));
	gtk_tree_model_get (model, iter, 1, &value, -1);
	if (value != NULL) {
		char* a;
		a = g_strdup_printf ("^%s.*", key);
		result = g_regex_match_simple (a, value, G_REGEX_CASELESS, 0);
		_g_free0 (a);
		_g_free0 (value);
		_g_object_unref0 (model);
		return result;
	}
	result = FALSE;
	_g_free0 (value);
	_g_object_unref0 (model);
	return result;
}


guint gmpc_easy_command_add_entry (GmpcEasyCommand* self, const char* name, const char* pattern, const char* hint, GmpcEasyCommandCallback* callback, void* userdata) {
	guint result;
	GtkTreeIter iter = {0};
	g_return_val_if_fail (self != NULL, 0U);
	g_return_val_if_fail (name != NULL, 0U);
	g_return_val_if_fail (pattern != NULL, 0U);
	g_return_val_if_fail (hint != NULL, 0U);
	self->priv->signals++;
	gtk_list_store_append (self->priv->store, &iter);
	gtk_list_store_set (self->priv->store, &iter, 0, self->priv->signals, 1, name, 2, pattern, 3, callback, 4, userdata, 5, hint, -1, -1);
	result = self->priv->signals;
	return result;
}


static glong string_get_length (const char* self) {
	glong result;
	g_return_val_if_fail (self != NULL, 0L);
	result = g_utf8_strlen (self, -1);
	return result;
}


static char* string_strip (const char* self) {
	char* result;
	char* _result_;
	g_return_val_if_fail (self != NULL, NULL);
	_result_ = g_strdup (self);
	g_strstrip (_result_);
	result = _result_;
	return result;
}


static char* string_substring (const char* self, glong offset, glong len) {
	char* result;
	glong string_length;
	const char* start;
	g_return_val_if_fail (self != NULL, NULL);
	string_length = g_utf8_strlen (self, -1);
	if (offset < 0) {
		offset = string_length + offset;
		g_return_val_if_fail (offset >= 0, NULL);
	} else {
		g_return_val_if_fail (offset <= string_length, NULL);
	}
	if (len < 0) {
		len = string_length - offset;
	}
	g_return_val_if_fail ((offset + len) <= string_length, NULL);
	start = g_utf8_offset_to_pointer (self, offset);
	result = g_strndup (start, ((gchar*) g_utf8_offset_to_pointer (start, len)) - ((gchar*) start));
	return result;
}


static void gmpc_easy_command_activate (GmpcEasyCommand* self, GtkEntry* entry) {
	GtkTreeModel* model;
	char* value_unsplit;
	GtkTreeIter iter = {0};
	GtkWindow* _tmp19_;
	g_return_if_fail (self != NULL);
	g_return_if_fail (entry != NULL);
	model = (GtkTreeModel*) self->priv->store;
	value_unsplit = g_strdup (gtk_entry_get_text (entry));
	if (string_get_length (value_unsplit) == 0) {
		GtkWindow* _tmp0_;
		gtk_object_destroy ((GtkObject*) self->priv->window);
		self->priv->window = (_tmp0_ = NULL, _g_object_unref0 (self->priv->window), _tmp0_);
		_g_free0 (value_unsplit);
		return;
	}
	{
		char** _tmp1_;
		char** value_collection;
		int value_collection_length1;
		int value_it;
		value_collection = _tmp1_ = g_strsplit (value_unsplit, ";", 0);
		value_collection_length1 = _vala_array_length (_tmp1_);
		for (value_it = 0; value_it < _vala_array_length (_tmp1_); value_it = value_it + 1) {
			char* value;
			value = g_strdup (value_collection[value_it]);
			{
				gboolean found;
				found = FALSE;
				if (gtk_tree_model_get_iter_first (model, &iter)) {
					{
						gboolean _tmp2_;
						_tmp2_ = TRUE;
						while (TRUE) {
							char* name;
							char* pattern;
							char* test;
							GmpcEasyCommandCallback _tmp4_;
							GDestroyNotify callback_target_destroy_notify;
							void* callback_target;
							GmpcEasyCommandCallback callback;
							void* data;
							char* _tmp5_;
							char* _tmp6_;
							gboolean _tmp7_;
							if (!_tmp2_) {
								gboolean _tmp3_;
								_tmp3_ = FALSE;
								if (gtk_tree_model_iter_next (model, &iter)) {
									_tmp3_ = !found;
								} else {
									_tmp3_ = FALSE;
								}
								if (!_tmp3_) {
									break;
								}
							}
							_tmp2_ = FALSE;
							name = NULL;
							pattern = NULL;
							test = NULL;
							callback_target = NULL;
							callback = (_tmp4_ = NULL, callback_target = NULL, callback_target_destroy_notify = NULL, _tmp4_);
							data = NULL;
							gtk_tree_model_get (model, &iter, 1, &name, 2, &pattern, 3, &callback, 4, &data, -1);
							test = (_tmp5_ = g_strdup_printf ("%s[ ]*%s$", name, pattern), _g_free0 (test), _tmp5_);
							if ((_tmp7_ = g_regex_match_simple (test, _tmp6_ = string_strip (value), G_REGEX_CASELESS, 0), _g_free0 (_tmp6_), _tmp7_)) {
								char* param;
								char* param_str;
								param = NULL;
								if (string_get_length (value) > string_get_length (name)) {
									char* _tmp8_;
									param = (_tmp8_ = string_substring (value, string_get_length (name), (glong) (-1)), _g_free0 (param), _tmp8_);
								} else {
									char* _tmp9_;
									param = (_tmp9_ = g_strdup (""), _g_free0 (param), _tmp9_);
								}
								param_str = string_strip (param);
								callback (data, param_str, callback_target);
								found = TRUE;
								_g_free0 (param);
								_g_free0 (param_str);
							}
							_g_free0 (name);
							_g_free0 (pattern);
							_g_free0 (test);
							(callback_target_destroy_notify == NULL) ? NULL : callback_target_destroy_notify (callback_target);
							callback = NULL;
							callback_target = NULL;
							callback_target_destroy_notify = NULL;
						}
					}
				}
				if (!found) {
					if (gtk_tree_model_get_iter_first (model, &iter)) {
						{
							gboolean _tmp10_;
							_tmp10_ = TRUE;
							while (TRUE) {
								char* name;
								char* pattern;
								char* test;
								GmpcEasyCommandCallback _tmp12_;
								GDestroyNotify callback_target_destroy_notify;
								void* callback_target;
								GmpcEasyCommandCallback callback;
								void* data;
								char* _tmp14_;
								char* _tmp13_;
								if (!_tmp10_) {
									gboolean _tmp11_;
									_tmp11_ = FALSE;
									if (gtk_tree_model_iter_next (model, &iter)) {
										_tmp11_ = !found;
									} else {
										_tmp11_ = FALSE;
									}
									if (!_tmp11_) {
										break;
									}
								}
								_tmp10_ = FALSE;
								name = NULL;
								pattern = NULL;
								test = NULL;
								callback_target = NULL;
								callback = (_tmp12_ = NULL, callback_target = NULL, callback_target_destroy_notify = NULL, _tmp12_);
								data = NULL;
								gtk_tree_model_get (model, &iter, 1, &name, 2, &pattern, 3, &callback, 4, &data, -1);
								test = (_tmp14_ = g_strdup_printf ("^%s.*", _tmp13_ = string_strip (value)), _g_free0 (test), _tmp14_);
								_g_free0 (_tmp13_);
								if (g_regex_match_simple (test, name, G_REGEX_CASELESS, 0)) {
									char* param;
									char* param_str;
									param = NULL;
									if (string_get_length (value) > string_get_length (name)) {
										char* _tmp15_;
										param = (_tmp15_ = string_substring (value, string_get_length (name), (glong) (-1)), _g_free0 (param), _tmp15_);
									} else {
										char* _tmp16_;
										param = (_tmp16_ = g_strdup (""), _g_free0 (param), _tmp16_);
									}
									param_str = string_strip (param);
									callback (data, param_str, callback_target);
									found = TRUE;
									_g_free0 (param);
									_g_free0 (param_str);
								}
								_g_free0 (name);
								_g_free0 (pattern);
								_g_free0 (test);
								(callback_target_destroy_notify == NULL) ? NULL : callback_target_destroy_notify (callback_target);
								callback = NULL;
								callback_target = NULL;
								callback_target_destroy_notify = NULL;
							}
						}
					}
				}
				if (!found) {
					char* _tmp18_;
					char* _tmp17_;
					playlist3_show_error_message (_tmp18_ = g_strdup_printf ("Unknown command: '%s'", _tmp17_ = string_strip (value)), ERROR_INFO);
					_g_free0 (_tmp18_);
					_g_free0 (_tmp17_);
				}
				_g_free0 (value);
			}
		}
		value_collection = (_vala_array_free (value_collection, value_collection_length1, (GDestroyNotify) g_free), NULL);
	}
	gtk_object_destroy ((GtkObject*) self->priv->window);
	self->priv->window = (_tmp19_ = NULL, _g_object_unref0 (self->priv->window), _tmp19_);
	_g_free0 (value_unsplit);
}


static gboolean gmpc_easy_command_key_press_event (GmpcEasyCommand* self, GtkEntry* widget, const GdkEventKey* event) {
	gboolean result;
	g_return_val_if_fail (self != NULL, FALSE);
	g_return_val_if_fail (widget != NULL, FALSE);
	if ((*event).keyval == 0xff1b) {
		GtkWindow* _tmp0_;
		gtk_object_destroy ((GtkObject*) self->priv->window);
		self->priv->window = (_tmp0_ = NULL, _g_object_unref0 (self->priv->window), _tmp0_);
		result = TRUE;
		return result;
	}
	if ((*event).keyval == 0xff09) {
		gtk_editable_set_position (GTK_EDITABLE (widget), -1);
		result = TRUE;
		return result;
	}
	result = FALSE;
	return result;
}


static gboolean gmpc_easy_command_popup_expose_handler (GmpcEasyCommand* self, GtkWindow* widget, const GdkEventExpose* event) {
	gboolean result;
	cairo_t* ctx;
	gint width;
	gint height;
	cairo_pattern_t* pattern;
	g_return_val_if_fail (self != NULL, FALSE);
	g_return_val_if_fail (widget != NULL, FALSE);
	ctx = gdk_cairo_create ((GdkDrawable*) ((GtkWidget*) widget)->window);
	width = ((GtkWidget*) widget)->allocation.width;
	height = ((GtkWidget*) widget)->allocation.height;
	if (gtk_widget_is_composited ((GtkWidget*) widget)) {
		cairo_set_operator (ctx, CAIRO_OPERATOR_SOURCE);
		cairo_set_source_rgba (ctx, 1.0, 1.0, 1.0, 0.0);
	} else {
		cairo_set_source_rgb (ctx, 1.0, 1.0, 1.0);
	}
	cairo_paint (ctx);
	cairo_rectangle (ctx, 1.0, 1.0, (double) (width - 2), (double) (height - 2));
	pattern = cairo_pattern_create_linear (0.0, 0.0, 0.0, (double) height);
	cairo_pattern_add_color_stop_rgba (pattern, 0.0, 0.0, 0.0, 0.2, 0.5);
	cairo_pattern_add_color_stop_rgba (pattern, 0.5, 0.0, 0.0, 0.0, 1.0);
	cairo_pattern_add_color_stop_rgba (pattern, 1.0, 0.0, 0.0, 0.2, 0.5);
	cairo_set_source (ctx, pattern);
	cairo_fill_preserve (ctx);
	cairo_set_source_rgba (ctx, 1.0, 1.0, 1.0, 1.0);
	cairo_stroke (ctx);
	cairo_rectangle (ctx, 0.0, 0.0, (double) width, (double) height);
	cairo_set_source_rgba (ctx, 0.0, 0.0, 0.0, 1.0);
	cairo_stroke (ctx);
	result = FALSE;
	_cairo_destroy0 (ctx);
	_cairo_pattern_destroy0 (pattern);
	return result;
}


static gboolean _gmpc_easy_command_popup_expose_handler_gtk_widget_expose_event (GtkWindow* _sender, const GdkEventExpose* event, gpointer self) {
	return gmpc_easy_command_popup_expose_handler (self, _sender, event);
}


static void _gmpc_easy_command_activate_gtk_entry_activate (GtkEntry* _sender, gpointer self) {
	gmpc_easy_command_activate (self, _sender);
}


static gboolean _gmpc_easy_command_key_press_event_gtk_widget_key_press_event (GtkEntry* _sender, const GdkEventKey* event, gpointer self) {
	return gmpc_easy_command_key_press_event (self, _sender, event);
}


static gboolean _gmpc_easy_command_focus_out_event_gtk_widget_focus_out_event (GtkEntry* _sender, const GdkEventFocus* event, gpointer self) {
	return gmpc_easy_command_focus_out_event (self, _sender, event);
}


void gmpc_easy_command_popup (GmpcEasyCommand* self) {
	g_return_if_fail (self != NULL);
	if (!gmpc_plugin_base_get_enabled ((GmpcPluginBase*) self)) {
		return;
	}
	if (self->priv->window == NULL) {
		GtkWindow* _tmp0_;
		GtkEntry* entry;
		self->priv->window = (_tmp0_ = g_object_ref_sink ((GtkWindow*) gtk_window_new (GTK_WINDOW_TOPLEVEL)), _g_object_unref0 (self->priv->window), _tmp0_);
		entry = g_object_ref_sink ((GtkEntry*) gtk_entry_new ());
		gtk_window_set_role (self->priv->window, "easy command");
		gtk_window_set_type_hint (self->priv->window, GDK_WINDOW_TYPE_HINT_DIALOG);
		gtk_window_set_decorated (self->priv->window, FALSE);
		gtk_window_set_modal (self->priv->window, TRUE);
		gtk_window_set_keep_above (self->priv->window, TRUE);
		gtk_container_set_border_width ((GtkContainer*) self->priv->window, (guint) 24);
		gtk_entry_set_width_chars (entry, 50);
		gtk_container_add ((GtkContainer*) self->priv->window, (GtkWidget*) entry);
		if (gtk_widget_is_composited ((GtkWidget*) self->priv->window)) {
			GdkScreen* screen;
			GdkColormap* colormap;
			screen = _g_object_ref0 (gtk_window_get_screen (self->priv->window));
			colormap = _g_object_ref0 (gdk_screen_get_rgba_colormap (screen));
			gtk_widget_set_colormap ((GtkWidget*) self->priv->window, colormap);
			_g_object_unref0 (screen);
			_g_object_unref0 (colormap);
		}
		g_object_set ((GtkWidget*) self->priv->window, "app-paintable", TRUE, NULL);
		g_signal_connect_object ((GtkWidget*) self->priv->window, "expose-event", (GCallback) _gmpc_easy_command_popup_expose_handler_gtk_widget_expose_event, self, 0);
		if (!playlist3_window_is_hidden ()) {
			gtk_window_set_transient_for (self->priv->window, (GtkWindow *)playlist3_get_window ());
			self->priv->window->position = (guint) GTK_WIN_POS_CENTER_ON_PARENT;
		}
		gtk_entry_set_completion (entry, self->priv->completion);
		g_signal_connect_object (entry, "activate", (GCallback) _gmpc_easy_command_activate_gtk_entry_activate, self, 0);
		g_signal_connect_object ((GtkWidget*) entry, "key-press-event", (GCallback) _gmpc_easy_command_key_press_event_gtk_widget_key_press_event, self, 0);
		g_signal_connect_object ((GtkWidget*) entry, "focus-out-event", (GCallback) _gmpc_easy_command_focus_out_event_gtk_widget_focus_out_event, self, 0);
		gtk_widget_show_all ((GtkWidget*) self->priv->window);
		gtk_window_present (self->priv->window);
		gtk_widget_grab_focus ((GtkWidget*) entry);
		_g_object_unref0 (entry);
	} else {
		gtk_window_present (self->priv->window);
	}
}


static gboolean gmpc_easy_command_focus_out_event (GmpcEasyCommand* self, GtkEntry* entry, const GdkEventFocus* event) {
	gboolean result;
	GtkWindow* _tmp0_;
	g_return_val_if_fail (self != NULL, FALSE);
	g_return_val_if_fail (entry != NULL, FALSE);
	gtk_object_destroy ((GtkObject*) self->priv->window);
	self->priv->window = (_tmp0_ = NULL, _g_object_unref0 (self->priv->window), _tmp0_);
	result = FALSE;
	return result;
}


void gmpc_easy_command_help_window_destroy (GtkDialog* window, gint response) {
	g_return_if_fail (window != NULL);
	gtk_object_destroy ((GtkObject*) window);
}


static void _gmpc_easy_command_help_window_destroy_gtk_dialog_response (GtkDialog* _sender, gint response_id, gpointer self) {
	gmpc_easy_command_help_window_destroy (_sender, response_id);
}


void gmpc_easy_command_help_window (void* data, const char* param) {
	GmpcEasyCommand* ec;
	GtkDialog* window;
	GtkTreeView* tree;
	GtkTreeModelSort* _tmp0_;
	GtkScrolledWindow* sw;
	GtkCellRendererText* renderer;
	GtkTreeViewColumn* column;
	GtkCellRendererText* _tmp1_;
	GtkTreeViewColumn* _tmp2_;
	GtkLabel* label;
	ec = _g_object_ref0 ((GmpcEasyCommand*) data);
	window = g_object_ref_sink ((GtkDialog*) gtk_dialog_new_with_buttons (_ ("Easy Command help"), NULL, 0, "gtk-close", GTK_RESPONSE_OK, NULL, NULL));
	gtk_window_set_default_size ((GtkWindow*) window, 600, 400);
	tree = g_object_ref_sink ((GtkTreeView*) gtk_tree_view_new ());
	gtk_tree_view_set_model (tree, (GtkTreeModel*) (_tmp0_ = (GtkTreeModelSort*) gtk_tree_model_sort_new_with_model ((GtkTreeModel*) ec->priv->store)));
	_g_object_unref0 (_tmp0_);
	gtk_tree_view_set_rules_hint (tree, TRUE);
	gtk_tree_view_set_search_column (tree, 1);
	sw = g_object_ref_sink ((GtkScrolledWindow*) gtk_scrolled_window_new (NULL, NULL));
	gtk_container_set_border_width ((GtkContainer*) sw, (guint) 8);
	gtk_scrolled_window_set_shadow_type (sw, GTK_SHADOW_ETCHED_IN);
	gtk_scrolled_window_set_policy (sw, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_container_add ((GtkContainer*) sw, (GtkWidget*) tree);
	renderer = g_object_ref_sink ((GtkCellRendererText*) gtk_cell_renderer_text_new ());
	column = g_object_ref_sink (gtk_tree_view_column_new ());
	gtk_tree_view_append_column (tree, column);
	gtk_tree_view_column_set_title (column, _ ("Command"));
	gtk_cell_layout_pack_start ((GtkCellLayout*) column, (GtkCellRenderer*) renderer, FALSE);
	gtk_cell_layout_add_attribute ((GtkCellLayout*) column, (GtkCellRenderer*) renderer, "text", 1);
	gtk_tree_view_column_set_sort_column_id (column, 1);
	renderer = (_tmp1_ = g_object_ref_sink ((GtkCellRendererText*) gtk_cell_renderer_text_new ()), _g_object_unref0 (renderer), _tmp1_);
	column = (_tmp2_ = g_object_ref_sink (gtk_tree_view_column_new ()), _g_object_unref0 (column), _tmp2_);
	gtk_tree_view_append_column (tree, column);
	gtk_cell_layout_pack_start ((GtkCellLayout*) column, (GtkCellRenderer*) renderer, FALSE);
	gtk_tree_view_column_set_title (column, _ ("Usage"));
	gtk_cell_layout_add_attribute ((GtkCellLayout*) column, (GtkCellRenderer*) renderer, "text", 5);
	gtk_tree_view_column_set_sort_column_id (column, 5);
	label = g_object_ref_sink ((GtkLabel*) gtk_label_new (""));
	gtk_label_set_markup (label, _ ("The following commands can be used in the easy command window.\nThe easy command window can be opened by pressing ctrl-space"));
	gtk_misc_set_alignment ((GtkMisc*) label, 0.0f, 0.5f);
	gtk_misc_set_padding ((GtkMisc*) label, 8, 6);
	gtk_box_pack_start ((GtkBox*) window->vbox, (GtkWidget*) label, FALSE, FALSE, (guint) 0);
	gtk_box_pack_start ((GtkBox*) window->vbox, (GtkWidget*) sw, TRUE, TRUE, (guint) 0);
	gtk_widget_show_all ((GtkWidget*) window);
	g_signal_connect (window, "response", (GCallback) _gmpc_easy_command_help_window_destroy_gtk_dialog_response, NULL);
	_g_object_unref0 (ec);
	_g_object_unref0 (window);
	_g_object_unref0 (tree);
	_g_object_unref0 (sw);
	_g_object_unref0 (renderer);
	_g_object_unref0 (column);
	_g_object_unref0 (label);
}


GmpcEasyCommand* gmpc_easy_command_construct (GType object_type) {
	GmpcEasyCommand * self;
	self = g_object_newv (object_type, 0, NULL);
	return self;
}


GmpcEasyCommand* gmpc_easy_command_new (void) {
	return gmpc_easy_command_construct (GMPC_EASY_TYPE_COMMAND);
}


static gboolean _gmpc_easy_command_completion_function_gtk_entry_completion_match_func (GtkEntryCompletion* completion, const char* key, GtkTreeIter* iter, gpointer self) {
	return gmpc_easy_command_completion_function (self, completion, key, iter);
}


static GObject * gmpc_easy_command_constructor (GType type, guint n_construct_properties, GObjectConstructParam * construct_properties) {
	GObject * obj;
	GObjectClass * parent_class;
	GmpcEasyCommand * self;
	parent_class = G_OBJECT_CLASS (gmpc_easy_command_parent_class);
	obj = parent_class->constructor (type, n_construct_properties, construct_properties);
	self = GMPC_EASY_COMMAND (obj);
	{
		GtkListStore* _tmp1_;
		GtkEntryCompletion* _tmp2_;
		GtkCellRendererText* renderer;
		((GmpcPluginBase*) self)->plugin_type = 8 + 4;
		self->priv->store = (_tmp1_ = gtk_list_store_new (6, G_TYPE_UINT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_POINTER, G_TYPE_POINTER, G_TYPE_STRING, NULL), _g_object_unref0 (self->priv->store), _tmp1_);
		self->priv->completion = (_tmp2_ = gtk_entry_completion_new (), _g_object_unref0 (self->priv->completion), _tmp2_);
		gtk_entry_completion_set_model (self->priv->completion, (GtkTreeModel*) self->priv->store);
		gtk_entry_completion_set_text_column (self->priv->completion, 1);
		gtk_entry_completion_set_inline_completion (self->priv->completion, TRUE);
		gtk_entry_completion_set_inline_selection (self->priv->completion, TRUE);
		gtk_entry_completion_set_popup_completion (self->priv->completion, TRUE);
		gtk_entry_completion_set_match_func (self->priv->completion, _gmpc_easy_command_completion_function_gtk_entry_completion_match_func, g_object_ref (self), g_object_unref);
		renderer = g_object_ref_sink ((GtkCellRendererText*) gtk_cell_renderer_text_new ());
		gtk_cell_layout_pack_end ((GtkCellLayout*) self->priv->completion, (GtkCellRenderer*) renderer, FALSE);
		gtk_cell_layout_add_attribute ((GtkCellLayout*) self->priv->completion, (GtkCellRenderer*) renderer, "text", 5);
		g_object_set ((GObject*) renderer, "foreground", "grey", NULL, NULL);
		gmpc_easy_command_add_entry (self, _ ("Help"), "", _ ("Get a list of available commands"), (GmpcEasyCommandCallback*) gmpc_easy_command_help_window, self);
		_g_object_unref0 (renderer);
	}
	return obj;
}


static void gmpc_easy_command_class_init (GmpcEasyCommandClass * klass) {
	gmpc_easy_command_parent_class = g_type_class_peek_parent (klass);
	g_type_class_add_private (klass, sizeof (GmpcEasyCommandPrivate));
	GMPC_PLUGIN_BASE_CLASS (klass)->get_name = gmpc_easy_command_real_get_name;
	GMPC_PLUGIN_BASE_CLASS (klass)->get_version = gmpc_easy_command_real_get_version;
	GMPC_PLUGIN_BASE_CLASS (klass)->get_enabled = gmpc_easy_command_real_get_enabled;
	GMPC_PLUGIN_BASE_CLASS (klass)->set_enabled = gmpc_easy_command_real_set_enabled;
	G_OBJECT_CLASS (klass)->constructor = gmpc_easy_command_constructor;
	G_OBJECT_CLASS (klass)->finalize = gmpc_easy_command_finalize;
}


static void gmpc_easy_command_instance_init (GmpcEasyCommand * self) {
	gint* _tmp0_;
	self->priv = GMPC_EASY_COMMAND_GET_PRIVATE (self);
	self->priv->completion = NULL;
	self->priv->store = NULL;
	self->priv->signals = (guint) 0;
	self->priv->window = NULL;
	self->priv->version = (_tmp0_ = g_new0 (gint, 3), _tmp0_[0] = 0, _tmp0_[1] = 0, _tmp0_[2] = 1, _tmp0_);
	self->priv->version_length1 = 3;
	_tmp0_ = NULL;
}


static void gmpc_easy_command_finalize (GObject* obj) {
	GmpcEasyCommand * self;
	self = GMPC_EASY_COMMAND (obj);
	_g_object_unref0 (self->priv->completion);
	_g_object_unref0 (self->priv->store);
	_g_object_unref0 (self->priv->window);
	self->priv->version = (g_free (self->priv->version), NULL);
	G_OBJECT_CLASS (gmpc_easy_command_parent_class)->finalize (obj);
}


GType gmpc_easy_command_get_type (void) {
	static GType gmpc_easy_command_type_id = 0;
	if (gmpc_easy_command_type_id == 0) {
		static const GTypeInfo g_define_type_info = { sizeof (GmpcEasyCommandClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) gmpc_easy_command_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (GmpcEasyCommand), 0, (GInstanceInitFunc) gmpc_easy_command_instance_init, NULL };
		gmpc_easy_command_type_id = g_type_register_static (GMPC_PLUGIN_TYPE_BASE, "GmpcEasyCommand", &g_define_type_info, 0);
	}
	return gmpc_easy_command_type_id;
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




