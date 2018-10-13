#include <gtk/gtk.h>
#include <stdlib.h> /* for malloc */
#include "image_operation.h"
#include "../neural_network/neural_network.h"
#include "../image_treatment/img_treatment.h"

extern enum Bin_Img_Type {DO_NOTHING, GRAYSCALE, B_AND_W, B_AND_W_DENOISED} bin_img_type;
binary_image *b_image;

GtkBuilder *builder; /* Used to get widgets from different functions */

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
    if(b_image) {
        if(b_image->pixel) {
            free(b_image->pixel);
        }
        for(int i=0; i<b_image->lr_size; i++) {
            if(!b_image->lr) {
                for(int j=0; j>b_image->lr->cr_size; j++) {
                    if(!b_image->lr->cr) {
                        free(b_image->lr->cr);
                    }
                }
                free(b_image->lr);
            }
        }
        free(b_image);
    }

    g_object_unref(builder);
    gtk_main_quit();
}
