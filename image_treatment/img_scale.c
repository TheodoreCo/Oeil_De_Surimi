#include <math.h>
#include <malloc.h>

#include "img_scale.h"

int *image_to_01(binary_image *image, c_rect *c, int *ret_val)
{
    // c_rect dimensions
    unsigned int w = c->max_x - c->min_x + 1;
    unsigned int h = c->max_y - c->min_y + 1;

    for (unsigned int i = 0; i < TRAIN_IMG_SZ; i++)
    {
        for (unsigned int j = 0; j < TRAIN_IMG_SZ; j++)
        {
            int ii = round (i*w) / TRAIN_IMG_SZ;
            int jj = round (j*h) / TRAIN_IMG_SZ;
            ret_val[i * TRAIN_IMG_SZ + j] = image->pixel[image->w * (c->min_y + ii) + c->min_x + jj];
        }
    }

    return ret_val;
}
