#include <gtk/gtk.h>
#include <stdlib.h> /* for malloc */
#include "image_operation.h"
#include "../neural_network/neural_network.h"
#include "../image_treatment/img_treatment.h"
#include "config.h"

extern enum Bin_Img_Type {DO_NOTHING, GRAYSCALE, B_AND_W, B_AND_W_DENOISED} bin_img_type;
binary_image *b_image;

GtkBuilder *builder; /* Used to get widgets from different functions */

int main (int argc, char *argv[])
{
    // Following commented line has no effect if GTK is used !
    // setlocale (LC_NUMERIC , "C");
    // Solution was to call setlocale just when needed (from cf_get_app_config())

    if(argc > 1)
    {
        printf("Launching Oeil_de_Surimi in CONSOLE mode...\n");
        // Usage: ./Oeil_de_Surimi bitmap_file text_file config_file
        // No other arguments for the time being (all arguments read from the configuration file)

        config *co = NULL;
        // 1. Test read configuration file
        if(argc == 3)
            co = cf_read_config(NULL);
        else
            co = cf_read_config(argv[3]);

        if (co == NULL) {
            perror("Reading config file error");
            return -1;
        }

        cf_key_val_to_string(NULL);
        cf_free_config();
    }
    else
    {
        GtkWidget *window;
        gtk_init(&argc, &argv);

        //cf_get_config("./def_config.txt");
        if(!cf_read_config(NULL))
            printf("Cannot build config from config file...\n");
        cf_key_val_to_string(NULL);

        builder = gtk_builder_new();
        gtk_builder_add_from_file(builder, "gui/main_window2.glade", NULL);

        window = GTK_WIDGET(gtk_builder_get_object(builder, "oeil_de_surimi_main_ocr"));
        gtk_builder_connect_signals(builder, NULL);

        /* Connect signal wiring for the drawing area (the binary image).
           We don't want to draw immediatley after loading the bitmap image ==> DO_NOTHING */
        bin_img_type = DO_NOTHING;
//        GtkDrawingArea *drawing_area = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "oeil_de_surimi_drawing_area"));
//        g_signal_connect(drawing_area, "draw", G_CALLBACK(draw_callback), NULL);

        gtk_widget_show(window);
        gtk_main();
    }

    return 0;
}

/* Called when window is closed. Frees memory. */
void on_window_main_destroy(void)
{
    if(b_image)
        free_binary_image(b_image);

    g_object_unref(builder);
    cf_free_config();
    gtk_main_quit();
}
