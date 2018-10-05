#ifndef IMAGE_OPERATION_H_INCLUDED
#define IMAGE_OPERATION_H_INCLUDED
gboolean draw_picture(GtkWidget *widget, cairo_t *cr, gpointer data);
//void img_bmp_info(char *filename, unsigned int *file_len, int *dib_header_len);
void img_bmp_to_binary(char *filename);

#endif // IMAGE_OPERATION_H_INCLUDED
