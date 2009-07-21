
#ifndef __GMPC_IMAGE_H__
#define __GMPC_IMAGE_H__

#include <glib.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixdata.h>
#include <stdlib.h>
#include <string.h>

G_BEGIN_DECLS


#define GMPC_TYPE_IMAGE (gmpc_image_get_type ())
#define GMPC_IMAGE(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), GMPC_TYPE_IMAGE, GmpcImage))
#define GMPC_IMAGE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), GMPC_TYPE_IMAGE, GmpcImageClass))
#define GMPC_IS_IMAGE(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GMPC_TYPE_IMAGE))
#define GMPC_IS_IMAGE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GMPC_TYPE_IMAGE))
#define GMPC_IMAGE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), GMPC_TYPE_IMAGE, GmpcImageClass))

typedef struct _GmpcImage GmpcImage;
typedef struct _GmpcImageClass GmpcImageClass;
typedef struct _GmpcImagePrivate GmpcImagePrivate;

struct _GmpcImage {
	GtkEventBox parent_instance;
	GmpcImagePrivate * priv;
};

struct _GmpcImageClass {
	GtkEventBoxClass parent_class;
};


GType gmpc_image_get_type (void);
void gmpc_image_set_pixbuf (GmpcImage* self, GdkPixbuf* buf, gboolean border);
void gmpc_image_clear_pixbuf (GmpcImage* self);
GmpcImage* gmpc_image_new (void);
GmpcImage* gmpc_image_construct (GType object_type);
const char* gmpc_image_get_text (GmpcImage* self);
void gmpc_image_set_text (GmpcImage* self, const char* value);


G_END_DECLS

#endif