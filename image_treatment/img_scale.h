#ifndef IMG_SCALE_H_INCLUDED
#define IMG_SCALE_H_INCLUDED

#include "img_treatment.h" // for c_rect structure

// Dataset training characters have a 28x28 dimension.
#define TRAIN_IMG_SZ 28

// 26 uppercase, 27 lowercase (there are 2 models of 'a')
#define NUM_OF_TRAIN_CHARS 52

// To hold every training character as a one-dimension array.
// In first position we should have the training character itself.
int train_chars[NUM_OF_TRAIN_CHARS][TRAIN_IMG_SZ * TRAIN_IMG_SZ + 1];

/**
    Function to scale an image (of given size) to an image of different size
    Initial image is a B&W image. This function will use the most simple algorithm,
    "Nearest Neighbor Interpolation": https://en.wikipedia.org/wiki/Nearest-neighbor_interpolation

    Note: Probably the hqx algorithm https://en.wikipedia.org/wiki/Hqx would give better results in our case
    (we are in B&W), because it conserves edges. But hqx is much more complex than our chosen algorithm.
*/
int *image_to_01(binary_image *image, c_rect *c, int *ret_val);

/**
    Returns a random element of train_chars after building it lazily.

    Returns an array of int of size TRAIN_IMG_SZ * TRAIN_IMG_SZ + 1
    First element in this array = the character to recognize (in int format)
    The next elements = the corresponding 01 file without any LF or CR.

    This function will:
    - look into Dataset/Majuscules and Dataset/Minuscules folders and load all files it finds
    - for every file:
        - the file name = the first elemeent in the returned array (naming convention)
        - file is read and stored in the global variable train_chars

*/
int *get_training_character(void);

/**
    Parameter bi binary_image (the result of the RLSA algorithm) has all char blocks as l_rect (line rectangles)
    This function will "normalize" the image, computing the real l_rect and attaching c_rect blocks to the computed l_rect ones.
*/
void normalize(binary_image *bi);

/**
    Prepares the binary image: the rectangle c_rect are processed to squared c_rect (because the OCR was trained with chars contained in squares)
*/
void rect_to_square(binary_image *bi);

#endif // IMG_SCALE_H_INCLUDED
