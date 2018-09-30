#include <gtk/gtk.h>
#include "image_operation.h"

int main (int argc, char *argv[])
{
    GtkBuilder *builder;
    GtkWidget *window;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "gui/main_window.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "oeil_de_surimi_main_ocr"));
    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(builder);
    gtk_widget_show(window);
    gtk_main();

    return 0;
}

// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}

void on_oeil_de_surimi_select_img_clicked(GtkButton *button, GtkImage *image)
{
    // GTK_IMAGE(image);
    GtkWidget *dialog = gtk_file_chooser_dialog_new(
         "Open File",
         NULL,
         GTK_FILE_CHOOSER_ACTION_OPEN,
         ("Cancel"), GTK_RESPONSE_CANCEL,
         ("Open"), GTK_RESPONSE_ACCEPT,
         NULL);

    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
        // printf("Chosen file name:%s\n", filename);

        GError *error = NULL;
        GdkPixbuf *pix = gdk_pixbuf_new_from_file (filename, &error);
        if (pix == NULL)
        {
            g_printerr ("Error loading file: #%d %s\n", error->code, error->message);
            g_error_free (error);
            return;
        }
        else
        {
            // pix = gdk_pixbuf_scale_simple(pix, 200, 200, GDK_INTERP_BILINEAR);
            // gtk_image_set_from_pixbuf(image, pix);
            gtk_image_clear(image);
            g_signal_connect(image, "draw", G_CALLBACK(draw_picture), pix);
        }

        g_free (filename);
    }

    gtk_widget_destroy (dialog);
}
