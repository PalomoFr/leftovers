#include <gtk/gtk.h>
#include "img_edit_gui.h"

static void on_open_image (GtkButton* button) {	
	// set up a dialog
	GtkWidget *toplevel = gtk_widget_get_toplevel (GTK_WIDGET (button));
	GtkWidget *dialog = gtk_file_chooser_dialog_new ("Open image",
	                                                 GTK_WINDOW (toplevel),
	                                                 GTK_FILE_CHOOSER_ACTION_OPEN,
	                                                 "_Open", GTK_RESPONSE_ACCEPT,
	                                                 "_Cancel", GTK_RESPONSE_CANCEL,
	                                                 NULL);

	// set up a filter for only images
	GtkFileFilter *filter = gtk_file_filter_new ();
	gtk_file_filter_add_pixbuf_formats (filter);
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), filter);
	
	// If dialog ended successfuly, manage the image
	if (gtk_dialog_run (GTK_DIALOG (dialog)) != GTK_RESPONSE_ACCEPT) {
		gtk_widget_destroy (dialog);
		return;
	}
	gchar *filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
	gtk_widget_destroy(dialog);
	
	img_edit_window(filename);
}

static void on_send_text (GtkButton* button, GtkWidget *textFields[2]) {
	GtkTextBuffer *messageBuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textFields[0]));
	
	GtkTextIter s, e;
	gtk_text_buffer_get_bounds(messageBuf, &s, &e);
	gchar *message = gtk_text_buffer_get_text(messageBuf, &s, &e, FALSE);
	gtk_text_buffer_set_text(messageBuf, "", -1);

	// | name this function however you want, #include the headerfile to your file in this file
	// | it takes gchar * as an argument, edit it however you want, 
	// V the result will then be shown in the main window (appended at the end)
	//modify_message_function(message);

	GtkTextBuffer *chatBuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textFields[1]));
	gtk_text_buffer_get_end_iter(chatBuf, &e);
	gtk_text_buffer_insert(chatBuf, &e, message, -1);
} 

void exit_app(GtkWidget* window, gboolean *runtime) {
	*runtime = FALSE;
}

int main (int argc, char *argv[]) {
	GtkWidget *window;
	GtkWidget *grid;
	GtkWidget *topBox;
	GtkWidget *botBox;

	GtkWidget *name;
	GtkWidget *chat;
	GtkWidget *message;
	GtkWidget *sendBtn;
	GtkWidget *imgBtn;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_widget_set_size_request (window, 400, 500);

	gtk_window_set_title(GTK_WINDOW(window), "Chat client");

	gtk_container_set_border_width(GTK_CONTAINER(window), 15);

	grid = gtk_grid_new();
	gtk_grid_set_column_spacing (GTK_GRID(grid), 5);
	gtk_grid_set_row_spacing(GTK_GRID(grid), 5);


	topBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
	gtk_widget_set_hexpand(topBox, TRUE);

	name = gtk_label_new("<big>Placeholder for Name</big>");
	gtk_widget_set_valign (name, GTK_ALIGN_END);
	gtk_widget_set_halign (name, GTK_ALIGN_START);
	gtk_label_set_use_markup(GTK_LABEL(name), TRUE);

	imgBtn = gtk_button_new_with_label("Send an image");
	gtk_widget_set_halign (imgBtn, GTK_ALIGN_END);
	gtk_box_pack_start(GTK_BOX(topBox), name, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(topBox), imgBtn, TRUE, TRUE, 0);

	gtk_grid_attach(GTK_GRID(grid), topBox, 0, 0, 1, 1);


	chat = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(chat), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(chat), FALSE);
	gtk_widget_set_vexpand (chat, TRUE);
	gtk_widget_set_hexpand (chat, TRUE);
	gtk_grid_attach(GTK_GRID(grid), chat, 0, 1, 1, 1);


	botBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
	message = gtk_text_view_new();
	gtk_widget_set_size_request(message, 70, 30);
	gtk_widget_set_hexpand (message, TRUE);	

	sendBtn = gtk_button_new_with_label("Send");
	gtk_widget_set_size_request(sendBtn, 70, 30);
	gtk_box_pack_start(GTK_BOX(botBox), message, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(botBox), sendBtn, FALSE, FALSE, 0);

	gtk_grid_attach(GTK_GRID(grid), botBox, 0, 2, 1, 1);

	gtk_container_add(GTK_CONTAINER(window), grid);

	GtkWidget *textFields[2];
	textFields[0] = message;
	textFields[1] = chat;
	

	g_signal_connect(imgBtn, "clicked", G_CALLBACK (on_open_image), NULL);
	g_signal_connect(sendBtn, "clicked", G_CALLBACK (on_send_text), textFields);

	//g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), G_OBJECT(window));
	gboolean runtime = TRUE;
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(exit_app), &runtime);

	gtk_widget_show_all(window);

	// use g_message to log out imporatnt events in the process, such as server connection...
	g_message("GUI started");
	
	while (gtk_main_iteration_do(FALSE)) {
		if (!runtime)
			break;

		//other callback handling

		//this loop needs to be running infinitely, 
		//if you need to wait in your program anywhere, 
		//(and it cannot be done only once before the loop)
		//we will need to make it into threads

	}

	return EXIT_SUCCESS;
}