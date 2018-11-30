#include <malloc.h>

#include "img_scale.h"
#include "img_treatment.h"

/*
    This main() tests the image_to_01 function.
*/
int main(void)
{
    // An image SZ * SZ having some particular rectangle defined in positoin (MIN_X, MIN_Y)..(MAX_X, MAX_Y)
    const int SZ = 50;
    const int MIN_X = 10;
    const int MIN_Y = 10;
    const int MAX_X = 20;
    const int MAX_Y = 20;

    // The target size (SZ ==> TARGET_SZ)
    const int TARGET_SZ = 13;

    binary_image *bi = malloc (sizeof(bi));
    bi->pixel = malloc (SZ * SZ * sizeof(unsigned char));
    bi->h = bi->w = SZ;

    // Fill bi with white except the special rectangle zone
    for (int i = 0; i<SZ; i++)
    {
        for (int j = 0; j<SZ; j++)
        {
            bi->pixel[i*SZ + j] = '1'; // white
            if(i >= MIN_Y && i <= MAX_Y && j >= MIN_X && j <= MAX_X)
            {
                bi->pixel[i*SZ + j] = '0'; // special rectangle : black
            }
        }
    }

    // Define a c_rect with the exact position of the special rectangle
    c_rect c;
    c.min_x = MIN_X;
    c.min_y = MIN_Y;
    c.max_x = MAX_X;
    c.max_y = MAX_Y;

    int *transf_rect = malloc (sizeof(int) * TARGET_SZ * TARGET_SZ);

    // Fill target array with 'X"
    for(int i = 0; i < TARGET_SZ*TARGET_SZ; i++)
        transf_rect[i] = 'X';

    // Call function to test
    image_to_01(bi, &c, transf_rect);

    // Print initial binary image (with the special rectangle upper left)
    for (int i = 0; i<SZ; i++)
    {
        for (int j = 0; j<SZ; j++)
        {
            printf("%c", bi->pixel[i*SZ + j]);
        }
        printf("\n");
    }
    printf("\n");

    // Print the target. Should be all black (0)
    for (int i = 0; i<TARGET_SZ; i++)
    {
        for (int j = 0; j<TARGET_SZ; j++)
        {
            printf("%c", transf_rect[i*TARGET_SZ + j]);
        }
        printf("\n");
    }
    printf("\n");

    free(transf_rect);
    free_binary_image(bi);

    return 0;
}
