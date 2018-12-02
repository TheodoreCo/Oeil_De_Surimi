#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "../img_treatment.h"

int main (int argc, char *argv[])
{
    if (argc != 3){
        printf("Wrong usage :\nImageSegmentation <source_image.bmp> <dst_image.bmp>");
        return 1;
    }

    binary_image *b_image = bi_image_from_file(argv[1]);

    unsigned int mediant_height = character_mediant_height(b_image, 50);
    //printf("mediant character height = %u\n", mediant_height);

    binary_image *rlsa_img = bi_image_RLSA(b_image, b_image->w-2, mediant_height * 2);
    bi_image_blocks_from_RLSA(b_image, rlsa_img);
    smallen_charboxes(b_image);

    binary_image *preview_blocks = bi_image_show_blocks(b_image);

    free_binary_image(b_image);
    b_image = preview_blocks;

    binary_image_to_file(argv[2], b_image);

    printf("'%s' successfully created!\n", argv[2]);
}
