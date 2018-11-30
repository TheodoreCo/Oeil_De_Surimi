#ifndef IMG_SCALE_H_INCLUDED
#define IMG_SCALE_H_INCLUDED

#include "img_treatment.h" // for c_rect structure

/*
    Function to scale an image (of given size) to an image of different size
    Initial image is a B&W image. This function will use the most simple algorithm,
    "Nearest Neighbor Interpolation": https://en.wikipedia.org/wiki/Nearest-neighbor_interpolation

    Note: Probably the hqx algorithm https://en.wikipedia.org/wiki/Hqx would give better results in our case
    (we are in B&W), because it conserves edges. But hqx is much more complex than our chosen algorithm.
*/
int *image_to_01(binary_image *image, c_rect *c, int *ret_val);


#endif // IMG_SCALE_H_INCLUDED
