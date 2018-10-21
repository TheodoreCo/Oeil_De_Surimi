#include <gtk/gtk.h>
#include <stdlib.h>
#include "../image_treatment/img_treatment.h"
#include "../neural_network/neural_network.h"
#include "config.h"

enum Bin_Img_Type {DO_NOTHING, GRAYSCALE, B_AND_W, B_AND_W_DENOISED, RLSA} bin_img_type;

extern GtkBuilder *builder; /* Used to get widgets from different functions */
extern binary_image *b_image;

// TODO: checks: is the bitmap file really a bitmap ? If so, is it a 24 bitmap ? Without compression, etc ?

/*
* Draws 'widget' from pixbuf 'data' taking into account the widget's dimensions
* and scaling the data using a simple bilinear interpolation.
*/
gboolean draw_picture(GtkWidget *widget, cairo_t *cr, gpointer data)
{
    gint width=gtk_widget_get_allocated_width(widget);
    gint height=gtk_widget_get_allocated_height(widget);

    GdkPixbuf *temp=gdk_pixbuf_scale_simple((GdkPixbuf*)data, width, height, GDK_INTERP_BILINEAR);
    gdk_cairo_set_source_pixbuf(cr, temp, 0, 0);
    cairo_paint(cr);

    g_object_unref(temp);
    return FALSE;
}

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

    b_image = bi_image_from_file(file_name);

    g_free (file_name);

    if(dialog)
    {
        gtk_widget_destroy (dialog);
    }
}

void bi_from_gray_to_b_and_w(void) {
    int img_w = b_image->w;
    int img_h = b_image->h;
    for(int y=0; y<img_h; y++)
    {
        for(int x=0; x<img_w; x++)
        {
            b_image->pixel[y*img_w + x] = (b_image->pixel[y*img_w + x]/255.0 > cf_get_b_and_w_threshold()) ? 1 : 0;
        }
    }
}

gboolean draw_callback(GtkWidget *widget, cairo_t *cr, gpointer data)
{
    if(b_image && bin_img_type != DO_NOTHING)
    {
        guint width,height;

        width   = gtk_widget_get_allocated_width(widget);
        height  = gtk_widget_get_allocated_height(widget);

        gtk_widget_set_size_request (widget, width, height);

        cairo_set_line_width (cr, 1.0);
        cairo_set_line_cap (cr, CAIRO_LINE_CAP_ROUND);

        int img_w = b_image->w;
        int img_h = b_image->h;
        for(int y=0; y<img_h; y++)
        {
            for(int x=0; x<img_w; x++)
            {
                double col_pix = b_image->pixel[y*img_w + x];
//                if(bin_img_type == GRAYSCALE || bin_img_type == RLSA) {
//                    col_pix = b_image->pixel[y*img_w + x]/255.0;
//                } else {
//                    col_pix = b_image->pixel[y*img_w + x];
//                }
                if(col_pix > 1)
                    col_pix /= 255.0;

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
    check_pixels("BEF B_W");
    bin_img_type = B_AND_W;
    bi_from_gray_to_b_and_w();
    check_pixels("AFTER B_W");
    gtk_widget_queue_draw(GTK_WIDGET(drawing_area));
}

void on_oeil_de_surimi_run_xor_btn_clicked(GtkButton *button) {
    // Get needed values to build the NN
    GtkSpinButton *hid_lay_spin_btn = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "oeil_de_surimi_hid_lay_val"));
    const double hidden_layers = gtk_spin_button_get_value(hid_lay_spin_btn);
    GtkSpinButton *hid_neur_spin_btn = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "oeil_de_surimi_hid_neur_val"));
    const double hidden_neurons = gtk_spin_button_get_value(hid_neur_spin_btn);

    // Build a NN with 2 input neurons, 1 output neuron and the GUI-specified layers & neurons
    neur_net *nn =  instantiate(2, hidden_layers, hidden_neurons, 1);

    // Get needed values to train the NN
    GtkSpinButton *epochs_spin_btn = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "oeil_de_surimi_epochs_val"));
    const double num_epochs = gtk_spin_button_get_value(epochs_spin_btn);
    GtkSpinButton *learning_rate_spin_btn = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "oeil_de_surimi_learning_rate_val"));
    const double learning_rate = gtk_spin_button_get_value(learning_rate_spin_btn);

    // Train it for XOR
    // TODO -- nn_xor_learn is not defined anymore
    // nn_xor_learn(nn, num_epochs, learning_rate);

    // Get the widgets to show results
    GtkEntry *xor_out_1_entry = GTK_ENTRY(gtk_builder_get_object(builder, "oeil_de_surimi_xor_out_1_entry"));
    GtkEntry *xor_out_2_entry = GTK_ENTRY(gtk_builder_get_object(builder, "oeil_de_surimi_xor_out_2_entry"));
    GtkEntry *xor_out_3_entry = GTK_ENTRY(gtk_builder_get_object(builder, "oeil_de_surimi_xor_out_3_entry"));
    GtkEntry *xor_out_4_entry = GTK_ENTRY(gtk_builder_get_object(builder, "oeil_de_surimi_xor_out_4_entry"));


    // Run NN 4 times & update GUI to show results
    double const inputs[4][2] = {
        {0.0, 0.0},
        {0.0, 1.0},
        {1.0, 0.0},
        {1.0, 1.0}
    };

    gchar *text1 = g_strdup_printf ("%.2f", *nn_run(nn, inputs[0]));
    gchar *text2 = g_strdup_printf ("%.2f", *nn_run(nn, inputs[1]));
    gchar *text3 = g_strdup_printf ("%.2f", *nn_run(nn, inputs[2]));
    gchar *text4 = g_strdup_printf ("%.2f", *nn_run(nn, inputs[3]));

    gtk_entry_set_text(xor_out_1_entry, text1);
    gtk_entry_set_text(xor_out_2_entry, text2);
    gtk_entry_set_text(xor_out_3_entry, text3);
    gtk_entry_set_text(xor_out_4_entry, text4);

    g_free(text1);
    g_free(text2);
    g_free(text3);
    g_free(text4);

    // TODO: nn_free() is not defined anymore (Steph)
    // nn_free(nn);
}

void on_oeil_de_surimi_def_nn_values_btn_clicked(GtkButton *button) {
    // TODO
    // Reinitialize default values
}

void on_oeil_de_surimi_img_rlsa_btn_clicked(GtkButton *button, GtkDrawingArea *drawing_area) {
    check_pixels("BEF RLSA");
    binary_image *rlsa_img = bi_image_RLSA(b_image, 20);

    free_binary_image(b_image);
    b_image = rlsa_img;

    gtk_widget_queue_draw(GTK_WIDGET(drawing_area));
    bin_img_type = RLSA;
    check_pixels("AFTER RLSA");
//    check_pixels("AFTER RLSA BEF B_W");
//    bi_from_gray_to_b_and_w();
//    check_pixels("AFTER RLSA AFTER B_W");
}

// Helper functions, to be deleted
void check_pixels(char *cutpoint) {
    int b_pix = 0, w_pix = 0, t_pix = 0;
    int p_0_50 = 0, p_51_100 = 0, p_101_150 = 0, p_151_200 = 0, p_201_255 = 0;

    int w = b_image->w;
    int h = b_image->h;

    for(int i=0; i<h; i++) {
        for(int j=0; j<w; j++) {
            unsigned char pix = b_image->pixel[i*w+j];
            t_pix++;
            if(pix == 0)
                b_pix++;
            if(pix == 1)
                w_pix++;
            if(0<=pix && pix < 50)
                p_0_50++;
            else if (51<= pix && pix < 100)
                p_51_100++;
            else if (101 <= pix && pix < 150)
                p_101_150++;
            else if (151 <= pix && pix < 200)
                p_151_200++;
            else
                p_201_255++;
        }
    }
    printf("Check stats [%s]: TOTAL=%d, black=%d, white=%d, others=%d\n", cutpoint, t_pix, b_pix, w_pix, t_pix-b_pix-w_pix);
    printf("\t0..50=[%d] 51..100=[%d] 101..150=[%d] 151..200=[%d] 201..255=[%d]\n", p_0_50, p_51_100, p_101_150, p_151_200, p_201_255);
}

