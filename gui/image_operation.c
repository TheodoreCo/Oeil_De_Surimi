#include <gtk/gtk.h>
#include <stdlib.h>

extern unsigned char **datax;
extern int img_h, img_w;

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

//void img_bmp_info(char *filename, unsigned int *file_len, int *dib_header_len)
//{
//    FILE *file = fopen(filename, "rb");
//    if(file)
//    {
//        // Position 2: the file length (little-endian format)
//        fseek(file, 2L, SEEK_SET);
//        fread(file_len, 4, 1, file);
//
//        fseek(file, 14L, SEEK_SET);
//        fread(dib_header_len, 4, 1, file);
//        fread(&img_w, 4, 1, file);
//        fread(&img_h, 4, 1, file);
//
//        fclose(file);
//    }
//}

void img_bmp_to_binary(char *filename)
{
    // const int NUM_PIXEL_BYTES = 3; /* Number of bytes per pixel. We only handle 24 bits bitmaps for now. */
    const int BMP_FILE_HEADER_LEN = 14;

    FILE *file = fopen(filename, "rb");
    if(file)
    {
        unsigned int file_len;
        unsigned int dib_header_len;

        // Position 2: the file length (little-endian format)
        fseek(file, 2L, SEEK_SET);
        fread(&file_len, 4, 1, file);

        fseek(file, 14L, SEEK_SET);
        fread(&dib_header_len, 4, 1, file);
        fread(&img_w, 4, 1, file);
        fread(&img_h, 4, 1, file);

        printf("File len: %d; DIB header len: %d; Image width: %d; Image height: %d\n",
               file_len, dib_header_len, img_w, img_h);

        datax= malloc(img_h*sizeof(unsigned char *));
        for(int i = 0; i < img_h; i++)
            datax[i] = malloc(sizeof(unsigned char) * img_w);
        /*
        Is there some padding ? "The bits representing the bitmap pixels are packed in rows.
        The size of each row is rounded up to a multiple of 4 bytes (a 32-bit DWORD) by padding." (wiki)
        */
        int padding = img_w % 4;

        fseek(file, BMP_FILE_HEADER_LEN + dib_header_len, SEEK_SET);
        /*
        "Usually pixels are stored "bottom-up", starting in the lower left corner,
        going from left to right, and then row by row from the bottom to the top of the image." (wiki)
        ==> img_h descending loop, img_w ascending loop.
        */
        for(int i = img_h-1; i >= 0; i--)
        {
            for(int j = 0; j < img_w; j++)
            {
                unsigned char r, g, b;
                fread(&r, sizeof(unsigned char), 1, file);
                fread(&g, sizeof(unsigned char), 1, file);
                fread(&b, sizeof(unsigned char), 1, file);

                datax[i][j] = ((int)r + (int)g + (int)b) / 3;
            }

            if(padding != 0)
            {
                fseek(file, padding, SEEK_CUR);
            }
            // printf("Reached position:  %d\n", (int)ftell(file));
        }
        fclose(file);
    }
}
