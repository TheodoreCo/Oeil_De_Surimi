#include <gtk/gtk.h>
#include <stdlib.h> /* for malloc */
#include "image_operation.h"
#include "../neural_network/neural_network.h"

unsigned char **datax;
int img_w;
int img_h;

static enum Bin_Img_Type {DO_NOTHING, GRAYSCALE, B_AND_W, B_AND_W_DENOISED} bin_img_type;
static GtkBuilder *builder; /* Used to get widgets from different functions */

/*
    Callback function to draw the binary image
*/
gboolean draw_callback(GtkWidget *widget, cairo_t *cr, gpointer data)
{
    if(datax && bin_img_type != DO_NOTHING)
    {
        guint width,height;

        width   = gtk_widget_get_allocated_width(widget);
        height  = gtk_widget_get_allocated_height(widget);

        gtk_widget_set_size_request (widget, width, height);

        cairo_set_line_width (cr, 1.0);
        cairo_set_line_cap (cr, CAIRO_LINE_CAP_ROUND);
        for(int y=0; y<img_h; y++)
        {
            for(int x=0; x<img_w; x++)
            {
                double col_pix = datax[y][x]/255.0;
                if (bin_img_type == B_AND_W)
                {
                    col_pix = col_pix > 0.5 ? 1 : 0;
                }
                cairo_set_source_rgb (cr, col_pix, col_pix, col_pix);
//                printf(" %u", col_pix);
                cairo_move_to(cr, x, y);
                cairo_line_to(cr, x, y);

                cairo_stroke (cr);
            }
        }
    }
    return FALSE;
}

void on_oeil_de_surimi_grayscale_btn_clicked(GtkButton *button, GtkDrawingArea *drawing_area)
{
    bin_img_type = GRAYSCALE;
    gtk_widget_queue_draw(GTK_WIDGET(drawing_area));
}

void on_oeil_de_surimi_b_and_w_btn_clicked(GtkButton *button, GtkDrawingArea *drawing_area)
{
    bin_img_type = B_AND_W;
    gtk_widget_queue_draw(GTK_WIDGET(drawing_area));
}


int main (int argc, char *argv[])
{
    GtkWidget *window;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "gui/main_window2.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "oeil_de_surimi_main_ocr"));
    gtk_builder_connect_signals(builder, NULL);

    /* Connect signal wiring for the drawing area (the binary image).
       We don't want to draw immediatley after loading the bitmap image ==> DO_NOTHING */
    bin_img_type = DO_NOTHING;
    GtkDrawingArea *drawing_area = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "oeil_de_surimi_drawing_area"));
    g_signal_connect(drawing_area, "draw", G_CALLBACK(draw_callback), NULL);

    gtk_widget_show(window);
    gtk_main();

    return 0;
}

/* Called when window is closed. Frees memory. */
void on_window_main_destroy(void)
{
    if(datax)
    {
        for(int i = 0; i < img_h; i++)
            free(datax[i]);
        free(datax);
    }

    g_object_unref(builder);
    gtk_main_quit();
}

/* Displays the bitmap image. Fills the **datax, img_w and img_h variables. */
void on_oeil_de_surimi_select_img_clicked(GtkButton *button, GtkImage *image)
{
    GtkEntry *file_gtk_entry = GTK_ENTRY(gtk_builder_get_object(builder, "oeil_de_surimi_bmp_file_name"));
    guint16 file_name_len = gtk_entry_get_text_length(file_gtk_entry);
    printf("Displayed file name length = '%d'\n", file_name_len);
    gchar *file_name = NULL;
    GtkWidget *dialog = NULL;

    if(file_name_len == 0)
    {
        // Get the parent window for the dialog
        GtkWindow *parent_window = GTK_WINDOW(gtk_builder_get_object(builder, "oeil_de_surimi_main_ocr"));

        dialog = gtk_file_chooser_dialog_new(
                      "Open File",
                      parent_window,
                      GTK_FILE_CHOOSER_ACTION_OPEN,
                      ("Cancel"), GTK_RESPONSE_CANCEL,
                      ("Open"), GTK_RESPONSE_ACCEPT,
                      NULL);

        if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
        {
            file_name = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
        } else {
            return;
        }

        printf("Chosen file name:%s\n", file_name);
        gtk_entry_set_text(file_gtk_entry, file_name);
    } else {
        // We already have the file name: no need to open file chooser dialog
        file_name = malloc(file_name_len * sizeof (gchar));
        strcpy(file_name, gtk_entry_get_text(file_gtk_entry));
    }

    GError *error = NULL;
    GdkPixbuf *pix = gdk_pixbuf_new_from_file (file_name, &error);
    if (pix == NULL)
    {
        g_printerr ("Error loading file: #%d %s\n", error->code, error->message);
        g_error_free (error);
        return;
    }
    else
    {
        gtk_image_clear(image);
        g_signal_connect(image, "draw", G_CALLBACK(draw_picture), pix);
    }

    img_bmp_to_binary(file_name);
    g_free (file_name);

    if(dialog)
    {
        gtk_widget_destroy (dialog);
    }
}
