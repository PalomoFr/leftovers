#include "image_edit.h"

void getpixel(guchar *p, guchar *r, guchar *g, guchar *b) {
	*r = p[0];
	*g = p[1];
	*b = p[2];
}

void setpixel(guchar *p, guchar r, guchar g, guchar b) {
	p[0] = r;
	p[1] = g;
	p[2] = b;
}

void grayscale(images_structure *images) {
	GdkPixbuf *src = images->current;
	GdkPixbuf *dst = images->edited;

	int width, height, rowstride, n_channels;
	guchar *src_pixels, *dst_pixels;
	long offset;
	guchar r, g, b;

	n_channels = gdk_pixbuf_get_n_channels (src);

	g_assert (gdk_pixbuf_get_colorspace (src) == GDK_COLORSPACE_RGB);
	g_assert (gdk_pixbuf_get_bits_per_sample (src) == 8);
	g_assert (!gdk_pixbuf_get_has_alpha (src));
	g_assert (n_channels == 3);

	width = gdk_pixbuf_get_width (src);
	height = gdk_pixbuf_get_height (src);

	rowstride = gdk_pixbuf_get_rowstride (src);
	src_pixels = gdk_pixbuf_get_pixels (src);
	dst_pixels = gdk_pixbuf_get_pixels (dst);

	int i, j;
	for (i = 0; i < height; ++i) {
		for (j = 0; j < width; ++j) {
			offset =  i*rowstride + j*n_channels;
			getpixel(src_pixels+offset, &r, &g, &b);
			guchar gray = (guchar)(0.299 * r + 0.587 * g + 0.114 * b);
			setpixel(dst_pixels+offset, gray, gray, gray);
		}
	}	
}

void tint(images_structure *images, gdouble r_tint, gdouble g_tint, gdouble b_tint) {
	GdkPixbuf *edit = gdk_pixbuf_copy(images->current);

	int width, height, rowstride, n_channels;
	guchar *pixels;
	long offset;
	guchar r, g, b;

	n_channels = gdk_pixbuf_get_n_channels (edit);

	g_assert (gdk_pixbuf_get_colorspace (edit) == GDK_COLORSPACE_RGB);
	g_assert (gdk_pixbuf_get_bits_per_sample (edit) == 8);
	g_assert (!gdk_pixbuf_get_has_alpha (edit));
	g_assert (n_channels == 3);

	width = gdk_pixbuf_get_width (edit);
	height = gdk_pixbuf_get_height (edit);

	rowstride = gdk_pixbuf_get_rowstride (edit);
	pixels = gdk_pixbuf_get_pixels (edit);

	r_tint /= 100;
	g_tint /= 100;
	b_tint /= 100;

	int i, j;
	for (i = 0; i < height; ++i) {
		for (j = 0; j < width; ++j) {
			offset =  i*rowstride + j*n_channels;
			getpixel(pixels+offset, &r, &g, &b);
			if (r_tint > 0)
				r += (guchar)((255-r)*r_tint);
			else
				r += (guchar)(r*r_tint);

			if (g_tint > 0)
				g += (guchar)((255-g)*g_tint);
			else
				g += (guchar)(g*g_tint);

			if (b_tint > 0)
				b += (guchar)((255-b)*b_tint);
			else
				b += (guchar)(b*b_tint);
			setpixel(pixels+offset, r, g, b);
		}
	}

	images->edited = edit;
}

void shade(images_structure *images, gdouble shadeVal) {
		GdkPixbuf *edit = gdk_pixbuf_copy(images->current);

	int width, height, rowstride, n_channels;
	guchar *pixels;
	long offset;
	guchar r, g, b;

	n_channels = gdk_pixbuf_get_n_channels (edit);

	g_assert (gdk_pixbuf_get_colorspace (edit) == GDK_COLORSPACE_RGB);
	g_assert (gdk_pixbuf_get_bits_per_sample (edit) == 8);
	g_assert (!gdk_pixbuf_get_has_alpha (edit));
	g_assert (n_channels == 3);

	width = gdk_pixbuf_get_width (edit);
	height = gdk_pixbuf_get_height (edit);

	rowstride = gdk_pixbuf_get_rowstride (edit);
	pixels = gdk_pixbuf_get_pixels (edit);

	shadeVal /= 100;

	int i, j;
	for (i = 0; i < height; ++i) {
		for (j = 0; j < width; ++j) {
			offset =  i*rowstride + j*n_channels;
			getpixel(pixels+offset, &r, &g, &b);
			if (shadeVal > 0) {
				r += (guchar)((255-r)*shadeVal);
				g += (guchar)((255-g)*shadeVal);
				b += (guchar)((255-b)*shadeVal);
			} else {
				r += (guchar)(r*shadeVal);
				g += (guchar)(g*shadeVal);
				b += (guchar)(b*shadeVal);
			}
			setpixel(pixels+offset, r, g, b);
		}
	}
	images->edited = edit;
}


void image_show(images_structure *images) {
	int w_max = gtk_widget_get_allocated_width(images->frame);
	int h_max = gtk_widget_get_allocated_height(images->frame);
	int w = gdk_pixbuf_get_width(images->edited);
	int h = gdk_pixbuf_get_height(images->edited);
	//g_print("%d %d %d %d", w, h, w_max, h_max);
	if (w > w_max) {
		h *= (float)w_max/w;
		w = w_max;
	}
	if (h > h_max) {
		w *= (float)h_max/h;
		h = h_max;
	}
	images->viewed = gdk_pixbuf_scale_simple(images->edited, w, h, GDK_INTERP_BILINEAR);

	gtk_image_set_from_pixbuf(GTK_IMAGE(images->view), images->viewed);
}