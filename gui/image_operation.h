#ifndef IMAGE_OPERATION_H_INCLUDED
#define IMAGE_OPERATION_H_INCLUDED

gboolean draw_picture(GtkWidget *widget, cairo_t *cr, gpointer data);

/* Builds the binary image by calling bi_image_from_file() */
void on_oeil_de_surimi_select_img_clicked(GtkButton *button, GtkImage *image);

/* Callback function to draw the binary image */
gboolean draw_callback(GtkWidget *widget, cairo_t *cr, gpointer data);

void on_oeil_de_surimi_grayscale_btn_clicked(GtkButton *button, GtkDrawingArea *drawing_area);

void on_oeil_de_surimi_b_and_w_btn_clicked(GtkButton *button, GtkDrawingArea *drawing_area);

void on_oeil_de_surimi_run_xor_btn_clicked(GtkButton *button);

void on_oeil_de_surimi_def_nn_values_btn_clicked(GtkButton *button);

void on_oeil_de_surimi_img_rlsa_btn_clicked(GtkButton *button);

//void img_bmp_info(char *filename, unsigned int *file_len, int *dib_header_len);

#endif // IMAGE_OPERATION_H_INCLUDED
