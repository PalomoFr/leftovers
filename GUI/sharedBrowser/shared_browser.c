#include <gtk/gtk.h>
#include "shared_browser.h"

typedef struct file_t{
	const char *name;
	GtkWidget *widget;
	GdkPixbuf *pb;
} file_t;


void shared_browser(const char *path) {
	GtkWidget *window;
	GtkWidget *grid;

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_widget_set_size_request (window, 600, 600);

	gtk_window_set_title(GTK_WINDOW(window), "Shared files");
	gtk_widget_show_all(window);

	grid = gtk_grid_new();
	gtk_grid_set_column_spacing (GTK_GRID(grid), 15);
	gtk_grid_set_row_spacing(GTK_GRID(grid), 15);

	GDir *dir = g_dir_open(path, 0, NULL);
	unsigned files_n = 1;
	file_t *files = malloc(files_n*sizeof(file_t));

	g_print("Hi, trying to open %s", path);
	size_t i = 0;
	while ((files[i].name = g_dir_read_name(dir)) != NULL){
		files_n++;
		files = realloc(files, files_n*sizeof(file_t));

		char* full_path = malloc(strlen(path)+strlen(files[i].name));
		strcpy(full_path, path); 
		strcat(full_path, files[i].name);

		files[i].pb = gdk_pixbuf_new_from_file(full_path, NULL);

		int w_max = gtk_widget_get_allocated_width(window)/3;
		int h_max = gtk_widget_get_allocated_height(window)/3;
		int w = gdk_pixbuf_get_width(files[i].pb);
		int h = gdk_pixbuf_get_height(files[i].pb);
		
		if (w > w_max) {
			h *= (float)w_max/w;
			w = w_max;
		}
		if (h > h_max) {
			w *= (float)h_max/h;
			h = h_max;
		}

		files[i].pb = gdk_pixbuf_scale_simple(files[i].pb, w, h, GDK_INTERP_BILINEAR);
		files[i].widget = gtk_image_new_from_pixbuf(files[i].pb);
		free(full_path);

		gtk_grid_attach(GTK_GRID(grid), files[i].widget, i, 0, 1, 1);

		i++;
	}
	files_n--;

	g_dir_close (dir);

	g_object_set (grid, "margin", 15, NULL);
	gtk_container_add(GTK_CONTAINER(window), grid);
	gtk_widget_show_all(window);
	
	//free(files);
}