#include <gtk/gtk.h>
#include <stdlib.h> /* for malloc */
#include "image_operation.h"
#include "../image_treatment/img_treatment.h"
#include "config.h"
#include "../neural_network/neural_network.h"

extern enum Bin_Img_Type {DO_NOTHING, GRAYSCALE, B_AND_W, B_AND_W_DENOISED} bin_img_type;
binary_image *b_image = NULL;

GtkBuilder *builder = NULL; /* Used to get widgets from different functions */

extern neur_net *ocr_nn;

int main (int argc, char *argv[])
{
    ocr_nn = NULL;
    // Following commented line has no effect if GTK is used !
    // setlocale (LC_NUMERIC , "C");
    // Solution was to call setlocale just when needed (from cf_get_app_config())

    GtkWidget *window;
    gtk_init(&argc, &argv);

    //cf_get_config("./def_config.txt");
    if(!cf_read_config(NULL))
        printf("Cannot build config from config file...\n");
    cf_key_val_to_string(NULL);

    builder = gtk_builder_new_from_file("gui/main_window2.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "oeil_de_surimi_main_ocr"));
    gtk_builder_connect_signals(builder, NULL);

    /* Connect signal wiring for the drawing area (the binary image).
       We don't want to draw immediatley after loading the bitmap image ==> DO_NOTHING */
    bin_img_type = DO_NOTHING;
//        GtkDrawingArea *drawing_area = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "oeil_de_surimi_drawing_area"));
//        g_signal_connect(drawing_area, "draw", G_CALLBACK(draw_callback), NULL);

    gtk_widget_show(window);

    // Fill the XOR NN tab with default values from config file
    on_oeil_de_surimi_xor_def_nn_values_btn_clicked(NULL);
    on_oeil_de_surimi_xor_def_train_values_btn_clicked(NULL);

    // Fill the OCR NN tab with default values from config file
    on_oeil_de_surimi_ocr_def_nn_values_btn_clicked(NULL);
    on_oeil_de_surimi_ocr_def_train_values_btn_clicked(NULL);
    gtk_main();

    return 0;
}

/* Called when window is closed. Frees memory. */
void on_window_main_destroy(void)
{
    if(b_image)
        free_binary_image(b_image);

    if(ocr_nn)
        neur_net_free(ocr_nn);

    g_object_unref(builder);
    cf_free_config();
    gtk_main_quit();
}
