#include <gtk/gtk.h>
#include <stdlib.h>
#include "../image_treatment/img_treatment.h"
#include "../neural_network/neural_network.h"
#include "config.h"
#include "../image_treatment/img_scale.h"

enum Bin_Img_Type {DO_NOTHING, GRAYSCALE, B_AND_W, B_AND_W_DENOISED, RLSA} bin_img_type;

extern GtkBuilder *builder; /* Used to get widgets from different functions */
extern binary_image *b_image;

/** Neural network used for the OCR tab. The one used in the XOR tab is a local variable */
neur_net *ocr_nn;

/** Connect the draw_picture() function to the draw event for the bmp image only once */
// static int is_img_draw_connected = 0;

// TODO: checks: is the bitmap file really a bitmap ? If so, is it a 24 bitmap ? Without compression, etc ?

/** Helper function, to be deleted. Displays statistics on the binary image. */
void check_pixels(char *cutpoint)
{
    int b_pix = 0, w_pix = 0, w_255_pix = 0, t_pix = 0;
    int p_0_50 = 0, p_51_100 = 0, p_101_150 = 0, p_151_200 = 0, p_201_255 = 0;

    int w = b_image->w;
    int h = b_image->h;

    for(int i=0; i<h; i++)
    {
        for(int j=0; j<w; j++)
        {
            int pix = b_image->pixel[i*w+j];
            t_pix++;
            if(pix == 0)
                b_pix++;
            else if(pix == 1)
                w_pix++;
            else if (pix == 255)
                w_255_pix++;

            if(0 <= pix && pix < 50)
                p_0_50++;
            else if (51 <= pix && pix < 100)
                p_51_100++;
            else if (101 <= pix && pix < 150)
                p_101_150++;
            else if (151 <= pix && pix < 200)
                p_151_200++;
            else
                p_201_255++;
        }
    }
    printf("Check stats [%s]: TOTAL=%d, black=%d, white-1=%d, white-255=%d, others=%d\n", cutpoint, t_pix, b_pix, w_pix, w_255_pix, t_pix-b_pix-w_pix-w_255_pix);
    printf("\t0..50=[%d] 51..100=[%d] 101..150=[%d] 151..200=[%d] 201..255=[%d]\n", p_0_50, p_51_100, p_101_150, p_151_200, p_201_255);
}

/** Draws 'widget' from pixbuf 'data' taking into account the widget's dimensions
    and scaling the data using a simple bilinear interpolation.
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


                // RLSA pixel range = (0 and 1)

                if(bin_img_type == GRAYSCALE && col_pix >= 1)
                    col_pix /= 255.0;

                double r, g, b;
                r = g = b = col_pix;

                if (bin_img_type == RLSA && col_pix == 2)
                    g = b = 0;

                cairo_set_source_rgb (cr, r, g, b);
                cairo_move_to(cr, x, y);
                cairo_line_to(cr, x, y);

                cairo_stroke (cr);
            }
        }
    }
    return FALSE;
}

/** 'Select' button event handling.
*/
void on_oeil_de_surimi_select_img_clicked(GtkButton *button, GtkImage *image)
{
    GtkEntry *file_gtk_entry = GTK_ENTRY(gtk_builder_get_object(builder, "oeil_de_surimi_bmp_file_name"));
    gchar *file_name = NULL;
    GtkWidget *dialog = NULL;

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
    }
    else
    {
        g_free(file_name);
        if(dialog)
        {
            gtk_widget_destroy (dialog);
        }
        return;
    }

    printf("Chosen file name:%s\n", file_name);
    gtk_entry_set_text(file_gtk_entry, file_name);

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
        // Systematically, free the binary image, if any (this is for multiple Select runs, with different files)
        if(b_image)
            free_binary_image(b_image);

        gtk_image_clear(image);
        // TODO: Check if multiple calls to g_signal_connect have an incidence on performances
        // If yes, how to refresh image in GTK3 ? GTK2 function gtk_image_set_from_image() was removed in GTK3
        g_signal_connect(image, "draw", G_CALLBACK(draw_picture), pix);
    }

    b_image = bi_image_from_file(file_name);

    g_free (file_name);
    if(dialog)
    {
        gtk_widget_destroy (dialog);
    }

    // TODO: Check if multiple calls to g_signal_connect have an incidence on performances (see above TODO)
    GtkDrawingArea *drawing_area = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "oeil_de_surimi_drawing_area"));
    g_signal_connect(drawing_area, "draw", G_CALLBACK(draw_callback), NULL);
    bin_img_type = GRAYSCALE;
    gtk_widget_queue_draw(GTK_WIDGET(drawing_area));
}

void on_oeil_de_surimi_train_ocr_btn_clicked(GtkButton *button)
{
//    // Get needed values to build the NN
//    GtkSpinButton *hid_lay_spin_btn = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "oeil_de_surimi_ocr_hid_lay"));
//    const double hidden_layers = gtk_spin_button_get_value(hid_lay_spin_btn);
//    GtkSpinButton *hid_neur_spin_btn = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "oeil_de_surimi_ocr_neur_per_hl"));
//    const double hidden_neurons = gtk_spin_button_get_value(hid_neur_spin_btn);
//    GtkSpinButton *inputs_btn = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "oeil_de_surimi_ocr_inputs"));
//    const double input_neurons = gtk_spin_button_get_value(inputs_btn);
//    GtkSpinButton *outputs_btn = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "oeil_de_surimi_ocr_outputs"));
//    const double output_neurons = gtk_spin_button_get_value(outputs_btn);
//
//    // Build a NN with the specified parameters
//    neur_net *nn =  instantiate(input_neurons, hidden_layers, hidden_neurons, output_neurons);
//
//    // Get needed values to train the NN
//    GtkSpinButton *epochs_spin_btn = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "oeil_de_surimi_ocr_epochs"));
//    const double num_epochs = gtk_spin_button_get_value(epochs_spin_btn);
//    GtkSpinButton *learning_rate_spin_btn = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "oeil_de_surimi_ocr_learn_rate"));
//    const double learning_rate = gtk_spin_button_get_value(learning_rate_spin_btn);
//
//    // Train it for OCR
//    // TODO: externalize the num_epochs parameter !
//    ocr_train(nn, learning_rate, epochs_spin_btn);
//
//    // Get the widgets to show results
//    GtkEntry *xor_out_1_entry = GTK_ENTRY(gtk_builder_get_object(builder, "oeil_de_surimi_xor_out_1_entry"));
//    GtkEntry *xor_out_2_entry = GTK_ENTRY(gtk_builder_get_object(builder, "oeil_de_surimi_xor_out_2_entry"));
//    GtkEntry *xor_out_3_entry = GTK_ENTRY(gtk_builder_get_object(builder, "oeil_de_surimi_xor_out_3_entry"));
//    GtkEntry *xor_out_4_entry = GTK_ENTRY(gtk_builder_get_object(builder, "oeil_de_surimi_xor_out_4_entry"));
//
//
//    // Run NN 4 times & update GUI to show results
//    double inputs[4][2] =
//    {
//        {0.0, 0.0},
//        {0.0, 1.0},
//        {1.0, 0.0},
//        {1.0, 1.0}
//    };
//
//    gchar *text1 = g_strdup_printf ("%.2f", *feed_forward(nn, inputs[0]));
//    gchar *text2 = g_strdup_printf ("%.2f", *feed_forward(nn, inputs[1]));
//    gchar *text3 = g_strdup_printf ("%.2f", *feed_forward(nn, inputs[2]));
//    gchar *text4 = g_strdup_printf ("%.2f", *feed_forward(nn, inputs[3]));
//
//    gtk_entry_set_text(xor_out_1_entry, text1);
//    gtk_entry_set_text(xor_out_2_entry, text2);
//    gtk_entry_set_text(xor_out_3_entry, text3);
//    gtk_entry_set_text(xor_out_4_entry, text4);
//
//    g_free(text1);
//    g_free(text2);
//    g_free(text3);
//    g_free(text4);
//
//    neur_net_free(nn);
}

void bi_from_gray_to_b_and_w(void)
{
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

void on_oeil_de_surimi_b_and_w_btn_clicked(GtkButton *button, GtkDrawingArea *drawing_area)
{
    check_pixels("BEF B_W");
    bin_img_type = B_AND_W;
    bi_from_gray_to_b_and_w();
    check_pixels("AFTER B_W");
    gtk_widget_queue_draw(GTK_WIDGET(drawing_area));
}

void on_oeil_de_surimi_run_xor_btn_clicked(GtkButton *button)
{
    // Get needed values to build the NN
    GtkSpinButton *hid_lay_spin_btn = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "oeil_de_surimi_hid_lay_val"));
    const double hidden_layers = gtk_spin_button_get_value(hid_lay_spin_btn);
    GtkSpinButton *hid_neur_spin_btn = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "oeil_de_surimi_hid_neur_val"));
    const double hidden_neurons = gtk_spin_button_get_value(hid_neur_spin_btn);

    // Build a NN with 2 input neurons, 1 output neuron and the GUI-specified layers & neurons
    neur_net *nn =  instantiate(2, hidden_layers, hidden_neurons, 1);

    // Get needed values to train the NN
    // TODO: externalize num_epochs in nn_xor_learn() function and uncomment below !
    GtkSpinButton *epochs_spin_btn = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "oeil_de_surimi_epochs_val"));
    const double num_epochs = gtk_spin_button_get_value(epochs_spin_btn);
    GtkSpinButton *learning_rate_spin_btn = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "oeil_de_surimi_learning_rate_val"));
    const double learning_rate = gtk_spin_button_get_value(learning_rate_spin_btn);

    // Train it for XOR
    // TODO -- nn_xor_learn is not defined anymore (Steph)
    // nn_xor_learn(nn, num_epochs, learning_rate);
    // TODO: externalize the num_epochs parameter !
    xor_train(nn, learning_rate, num_epochs);

    // Get the widgets to show results
    GtkEntry *xor_out_1_entry = GTK_ENTRY(gtk_builder_get_object(builder, "oeil_de_surimi_xor_out_1_entry"));
    GtkEntry *xor_out_2_entry = GTK_ENTRY(gtk_builder_get_object(builder, "oeil_de_surimi_xor_out_2_entry"));
    GtkEntry *xor_out_3_entry = GTK_ENTRY(gtk_builder_get_object(builder, "oeil_de_surimi_xor_out_3_entry"));
    GtkEntry *xor_out_4_entry = GTK_ENTRY(gtk_builder_get_object(builder, "oeil_de_surimi_xor_out_4_entry"));


    // Run NN 4 times & update GUI to show results
    double inputs[4][2] =
    {
        {0.0, 0.0},
        {0.0, 1.0},
        {1.0, 0.0},
        {1.0, 1.0}
    };

    gchar *text1 = g_strdup_printf ("%.2f", *feed_forward(nn, inputs[0]));
    gchar *text2 = g_strdup_printf ("%.2f", *feed_forward(nn, inputs[1]));
    gchar *text3 = g_strdup_printf ("%.2f", *feed_forward(nn, inputs[2]));
    gchar *text4 = g_strdup_printf ("%.2f", *feed_forward(nn, inputs[3]));

    gtk_entry_set_text(xor_out_1_entry, text1);
    gtk_entry_set_text(xor_out_2_entry, text2);
    gtk_entry_set_text(xor_out_3_entry, text3);
    gtk_entry_set_text(xor_out_4_entry, text4);

    g_free(text1);
    g_free(text2);
    g_free(text3);
    g_free(text4);

    neur_net_free(nn);
}

// Initializes the XOR neural network spin buttons values from the config file
void on_oeil_de_surimi_xor_def_nn_values_btn_clicked(GtkButton *button)
{
    GtkSpinButton *hid_lay_val_btn = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "oeil_de_surimi_hid_lay_val"));
    GtkSpinButton *hid_neur_val_btn = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "oeil_de_surimi_hid_neur_val"));

    gtk_spin_button_set_value(hid_lay_val_btn, cf_get_num_hid_lay_xor());
    gtk_spin_button_set_value(hid_neur_val_btn, cf_get_num_hid_neur_xor());
}

// Initializes the XOR training spin buttons values from the config file
void on_oeil_de_surimi_xor_def_train_values_btn_clicked(GtkButton *button)
{
    GtkSpinButton *epochs_val_btn = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "oeil_de_surimi_epochs_val"));
    GtkSpinButton *learning_rate_btn = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "oeil_de_surimi_learning_rate_val"));

    gtk_spin_button_set_value(epochs_val_btn, cf_get_num_train_epochs_xor());
    gtk_spin_button_set_value(learning_rate_btn, cf_get_learn_rate_xor());
}

// Initializes the OCR neural network spin buttons values from the config file
void on_oeil_de_surimi_ocr_def_nn_values_btn_clicked(GtkButton *button)
{
    GtkSpinButton *hid_lay_val_btn = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "oeil_de_surimi_ocr_hid_lay"));
    GtkSpinButton *hid_neur_val_btn = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "oeil_de_surimi_ocr_neur_per_hl"));
    GtkSpinButton *inputs_btn = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "oeil_de_surimi_ocr_inputs"));
    GtkSpinButton *outputs_btn = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "oeil_de_surimi_ocr_outputs"));

    gtk_spin_button_set_value(hid_lay_val_btn, cf_get_num_hid_lay_ocr());
    gtk_spin_button_set_value(hid_neur_val_btn, cf_get_num_hid_neur_ocr());
    gtk_spin_button_set_value(inputs_btn, cf_get_num_input_ocr());
    gtk_spin_button_set_value(outputs_btn, cf_get_num_output_ocr());
}

// Initializes the OCR training spin buttons values from the config file
void on_oeil_de_surimi_ocr_def_train_values_btn_clicked(GtkButton *button)
{
    GtkSpinButton *epochs_val_btn = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "oeil_de_surimi_ocr_epochs"));
    GtkSpinButton *learning_rate_btn = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "oeil_de_surimi_ocr_learn_rate"));

    gtk_spin_button_set_value(epochs_val_btn, cf_get_num_train_epochs_ocr());
    gtk_spin_button_set_value(learning_rate_btn, cf_get_learn_rate_ocr());
}

void show_image_info(binary_image *bi, char *msg)
{
    printf("\n ####################### %s ###################### \n", msg);
    printf("W:%ul  H:%ul  lr_size:%ul\n", bi->w, bi->h, bi->lr_size);
    printf("\n ####################### %s ###################### \n", msg);
}

void on_oeil_de_surimi_img_rlsa_btn_clicked(GtkButton *button, GtkDrawingArea *drawing_area)
{

    /*
        binary_image *rlsa_img = bi_image_RLSA(b_image, cf_get_rlsa_expansion());

        bi_image_blocks_from_RLSA(b_image, rlsa_img);

        printf("creating char bimg list...\n");
        unsigned int sl = 16;
        char_bimg_list *list = gen_char_bimg_list(b_image, sl);
        printf("done creating char bimg list !\n");

        printf("creating preview_nn_input...\n");
        b_image = preview_nn_input(list);
        printf("done creating preview_nn_input !\n");
    */

    unsigned int h = character_mediant_height(b_image, 50);
    printf("MEDIAN HEIGHT = %u\n", h);

    //printf("%zu, %zu\n",b_image->w,b_image->h );



    // check_pixels("BEF RLSA");

    show_image_info(b_image, "(1) b_image");
    binary_image *rlsa_img = bi_image_RLSA(b_image, cf_get_rlsa_expansion(), cf_get_rlsa_expansion());
    show_image_info(b_image, "(2) b_image");
    show_image_info(rlsa_img, "(2) rlsa_img");

    bi_image_blocks_from_RLSA(b_image, rlsa_img);
    show_image_info(b_image, "(3) b_image");
    show_image_info(rlsa_img, "(3) rlsa_img");

    binary_image *preview_blocks = bi_image_show_blocks(b_image);

    printf("lr_size = %i\n",b_image->lr_size );

    free_binary_image(b_image);
    b_image = preview_blocks;


    gtk_widget_queue_draw(GTK_WIDGET(drawing_area));
    bin_img_type = RLSA;

    l_rect *rect = b_image->lr;




    for (; rect; rect = rect->next)
    {
        //l_rect *rect = b_image->lr;
        printf("[<%u,%u><%u,%u>]  ", rect->min_x, rect->min_y, rect->max_x, rect->max_y);
        //rect = rect->next;
    }
    printf("\n");




    //check_pixels("AFTER RLSA");
//    check_pixels("AFTER RLSA BEF B_W");
//    bi_from_gray_to_b_and_w();
//    check_pixels("AFTER RLSA AFTER B_W");

}

void on_oeil_de_surimi_load_btn_clicked(GtkButton *button)
{
    if(ocr_nn)
        neur_net_free(ocr_nn);

    printf("Building new nn from %s\n", cf_get_ocr_train_file());
    ocr_nn = nn_load(cf_get_ocr_train_file());
}

void on_oeil_de_surimi_save_btn_clicked(GtkButton *button)
{
    if(ocr_nn) {
        printf("Saving nn config to file");
        nn_save(ocr_nn, cf_get_ocr_train_file());
    }
}

void on_oeil_de_surimi_test_theo_btn_clicked(GtkButton *button)
{
    int *letter = get_training_character();

    printf("Here's a %c\n", (char)letter[0]);
    for(int i = 1; i < TRAIN_IMG_SZ*TRAIN_IMG_SZ+1; i++)
    {
        printf("%c", (char)letter[i]);
        if (i % TRAIN_IMG_SZ == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
}

void on_oeil_de_surimi_test_gabriel_btn_clicked(GtkButton *button, GtkDrawingArea *drawing_area)
{
    printf("Automatic RLSA !\n");

    unsigned int mediant_height = character_mediant_height(b_image, 50);
    printf("mediant_height = %u\n", mediant_height);

    binary_image *rlsa_img = bi_image_RLSA(b_image, b_image->w-2, mediant_height * 2);
    bi_image_blocks_from_RLSA(b_image, rlsa_img);
    smallen_charboxes(b_image);

    binary_image *preview_blocks = bi_image_show_blocks(b_image);

    free_binary_image(b_image);
    b_image = preview_blocks;

    gtk_widget_queue_draw(GTK_WIDGET(drawing_area));
    bin_img_type = RLSA;
}
