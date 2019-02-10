#include <gtk/gtk.h>
#include "img_edit_gui.h"

void on_close(GtkButton *closeBtn) {
	GtkWidget *toplevel = gtk_widget_get_toplevel (GTK_WIDGET (closeBtn));
	gtk_widget_destroy(toplevel);
}

void img_edit_window(gchar *path) {
	GtkWidget *window;
	GtkWidget *header;
	GtkWidget *grid;
	GtkWidget *mainBox;
	GtkWidget *imageView;

	GtkWidget *filters;
	GtkWidget *filtersView;
	GtkWidget *filtersFrame;
	GtkWidget *settingsFrame;
	GtkWidget *btnBox;

	GtkWidget *name;
	GtkWidget *image;
	
	GtkWidget *gray;
	GtkWidget *blur;
	GtkWidget *edges;

	GtkWidget *closeBtn;
	GtkWidget *sendBtn;
	GtkWidget *removeBtn;
	GtkWidget *applyBtn;	

	GError *err = NULL;

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_widget_set_size_request (window, 900, 600);

	gtk_window_set_title(GTK_WINDOW(window), "Edit image");

	mainBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

	header = gtk_header_bar_new();
	char name_text[256];
	sprintf(name_text, "<big>%s</big>", path);
	name = gtk_label_new(name_text);
	gtk_label_set_use_markup(GTK_LABEL(name), TRUE);
	gtk_header_bar_set_custom_title(GTK_HEADER_BAR(header), name);
	closeBtn = gtk_button_new_with_label("Close");
	gtk_widget_set_size_request(closeBtn, 70, 30);
	sendBtn = gtk_button_new_with_label("Send");
	gtk_widget_set_size_request(sendBtn, 70, 30);
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header), closeBtn);
	gtk_header_bar_pack_end(GTK_HEADER_BAR(header), sendBtn);

	gtk_box_pack_start(GTK_BOX(mainBox), header, FALSE, FALSE, 0);

	grid = gtk_grid_new();
	gtk_grid_set_column_spacing (GTK_GRID(grid), 15);
	gtk_grid_set_row_spacing(GTK_GRID(grid), 10);


	imageView = gtk_scrolled_window_new(NULL,NULL);
	GdkPixbuf *pb = gdk_pixbuf_new_from_file(path, &err);
	image = gtk_image_new_from_pixbuf(pb);
	gtk_widget_set_vexpand (imageView, TRUE);
	gtk_widget_set_hexpand (imageView, TRUE);

	gtk_container_add(GTK_CONTAINER(imageView),image);
	gtk_grid_attach(GTK_GRID(grid), imageView, 0, 1, 1, 3);


	filtersFrame = gtk_frame_new("Filters");
	gtk_frame_set_label_align(GTK_FRAME(filtersFrame), 0.1, 0.5);
	filtersView = gtk_scrolled_window_new(NULL,NULL);
	gtk_container_set_border_width(GTK_CONTAINER(filtersView), 5);
	filters = gtk_button_box_new(GTK_ORIENTATION_VERTICAL);
	gtk_box_set_spacing(GTK_BOX(filters), 5);
	gray = gtk_button_new_with_label("Grayscale");
	blur = gtk_button_new_with_label("Blur");
	edges = gtk_button_new_with_label("Edge detection");
	gtk_box_pack_start(GTK_BOX(filters), gray, FALSE, FALSE, 0);
	gtk_widget_set_valign (gray, GTK_ALIGN_START);
	gtk_box_pack_start(GTK_BOX(filters), blur, FALSE, FALSE, 0);
	gtk_widget_set_valign (blur, GTK_ALIGN_START);
	gtk_box_pack_start(GTK_BOX(filters), edges, FALSE, FALSE, 0);
	gtk_widget_set_valign (edges, GTK_ALIGN_START);
	gtk_widget_set_valign (filters, GTK_ALIGN_START);

	gtk_container_add(GTK_CONTAINER(filtersView),filters);
	gtk_widget_set_vexpand (filtersView, TRUE);
	gtk_widget_set_vexpand (filtersFrame, TRUE);
	gtk_container_add(GTK_CONTAINER(filtersFrame),filtersView);
	gtk_grid_attach(GTK_GRID(grid), filtersFrame, 1, 1, 1, 1);


	settingsFrame = gtk_frame_new("Settings");
	gtk_frame_set_label_align(GTK_FRAME(settingsFrame), 0.1, 0.5);
	gtk_widget_set_size_request(settingsFrame, 155, 200);
	gtk_grid_attach(GTK_GRID(grid), settingsFrame, 1, 2, 1, 1);


	btnBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_widget_set_size_request(btnBox, 150, 30);
	gtk_widget_set_vexpand (btnBox, FALSE);
	removeBtn = gtk_button_new_with_label("Remove all");
	gtk_widget_set_size_request(removeBtn, 75, 30);

	applyBtn = gtk_button_new_with_label("Apply");
	gtk_widget_set_size_request(applyBtn, 75, 30);
	gtk_box_pack_start(GTK_BOX(btnBox), removeBtn, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(btnBox), applyBtn, FALSE, FALSE, 0);

	gtk_grid_attach(GTK_GRID(grid), btnBox, 1, 3, 1, 1);

	g_object_set (grid, "margin", 15, NULL);
	gtk_box_pack_start(GTK_BOX(mainBox), grid, TRUE, TRUE, 0);

	gtk_container_add(GTK_CONTAINER(window), mainBox);

	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_widget_destroy), G_OBJECT(window));
	g_signal_connect(closeBtn, "clicked", G_CALLBACK(on_close), NULL);

	gtk_widget_show_all(window);

	int w_max = gtk_widget_get_allocated_width(imageView);
	int h_max = gtk_widget_get_allocated_height(imageView);
	int w = gdk_pixbuf_get_width(pb);
	int h = gdk_pixbuf_get_height(pb);
	g_print("%d %d %d %d", w, h, w_max, h_max);
	if (w > w_max) {
		h *= (float)w_max/w;
		w = w_max;
	}
	if (h > h_max) {
		w *= (float)h_max/h;
		h = h_max;
	}
	pb = gdk_pixbuf_scale_simple(pb, w, h, GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf(GTK_IMAGE(image), pb);
}