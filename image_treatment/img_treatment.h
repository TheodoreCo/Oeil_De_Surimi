#ifndef IMG_TREATMENT_H_INCLUDED
#define IMG_TREATMENT_H_INCLUDED

// TODO: there's some redundancy in c_rect/l_rect definitions
// Namely, a l_rect lives inside a binary_image struct which has a width (w1 and w2 are redundant)
// Namely, a c_rect lives inside a l_rect struct which already defines the h1 and h2.
// But storing things this way allows a comfy usage...
// See if this structures can be optimized for best memory/usage fit.

/*
    A c_rect defines the coordinates of the up-left and down-right points of
    the rectangle surronding a character.
*/
typedef struct c_rect {
    // Up-left coordinates of the rectangle surrounding the line
    int min_x, min_y;
    // Down-right coordinates of the rectangle surrounding the line
    int max_x, max_y;
    // TODO: see if useful to keep track of the character the NN-recognized character !
    // int c;
} c_rect;

/*
    A l_rect defines the coordinates of the up-left and down-right points of
    the rectancle surrounding a line of characters.

    Maybe not the best way to represent text blocks
*/
typedef struct l_rect l_rect;
struct l_rect {
    // Up-left coordinates of the rectangle surrounding the line
    unsigned int min_x, min_y;
    // Down-right coordinates of the rectangle surrounding the line
    unsigned int max_x, max_y;
    //pointer to next l_rect
    l_rect *next;
    // Size of the array of char rectangles
    unsigned int cr_size;
    // An array of char rectangles
    c_rect *cr;
};

/*
    A structure representing a collection of mono-byte pixels.
    Includes a l_rect array to track the rectangles surronding the text lines in the image.
*/
typedef struct binary_image {
    // Width & height...
    unsigned int w,h;
    // ... and data. Every pixel is represented by a single byte
    unsigned char *pixel;
    // Size of the array of line rectangles
    unsigned int lr_size;
    // An array of line rectangles
    l_rect *lr;
} binary_image;

/*
    Future input data for neural network
*/
typedef struct char_bimg char_bimg;
struct char_bimg
{
    double *pixel; //array of pixels of size (w * h)
    char_bimg *next;
    unsigned int x, y;

    char result; // deducted character (output of neural network)
};
/*
    List of char_bimg elements. Useful to pass a list of characters to the neural network.
*/
typedef struct char_bimg_list char_bimg_list;
struct char_bimg_list
{
    unsigned int side_length;
    unsigned int size;

    char_bimg *first;
};


typedef struct char_pos char_pos;
struct char_pos
{
    unsigned char c;
    unsigned int lpos;
};

/*
    Builds a binary image given a file name.
    File should be a 32-bit bitmap file with some restrictions (no compression etc.)
*/
binary_image *bi_image_from_file(char *filename);

void binary_image_to_file(char filename[], binary_image *b_image );


/*
    Detects lines of text in the binary image and returns an enriched image
    (containing the detected line-surrounding rectangles).
    Assumption 1: the image is a B/W one and the text lines are horizontal.
    Assumption 2: the image is denoised (no black pixels other than those composing the letters)
*/
//binary_image *bi_detect_text_blocks(binary_image *b_img);

/*
    Detects char blocks in the binary image and returns an enriched image
    (containing the detected char-surrounding rectangles)
*/
//binary_image *bi_image_detect_char_blocks(binary_image *b_img);


/*
    Returns a image containing the zones (black pixels) of text blocks in an image
    using "expansion" as the number of pixel to feed the RLSA algorithm.
*/
binary_image *bi_image_RLSA(binary_image *b_img, unsigned int h_expansion, unsigned int v_expansion);


/*
    Modify the b_img to have l_rect computed from the rlsa_img
*/
void bi_image_blocks_from_RLSA(binary_image *b_img, binary_image *rlsa_img);

/*
    Return image with highlighted blocks (l_rect)
*/
binary_image *bi_image_show_blocks(binary_image *b_img);

/** Frees memory malloc'd for the binary_image */
void free_binary_image(binary_image *b_imgage);

// TODO: other useful functions, like denoising, etc.

binary_image *preview_nn_input(char_bimg_list *list);

char_bimg_list *gen_char_bimg_list(binary_image *b_img, unsigned int side_length);

void resize_img(binary_image *original_b_img, l_rect *rect, unsigned int side_length, double *result);

unsigned int character_mediant_height(binary_image *b_img, unsigned int max);

void smallen_charboxes(binary_image *b_image);

unsigned char *getmatrix(char *filename);

char_bimg_list *image_segmentation(binary_image *b_image, unsigned int side_length);



#endif // IMG_TREATMENT_H_INCLUDED
