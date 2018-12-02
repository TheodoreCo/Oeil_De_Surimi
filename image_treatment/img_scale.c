#include <math.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

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


int *get_training_character(void)
{

    FILE *fp;
    char str[TRAIN_IMG_SZ+2];
    char* filename = "./train_letters/train_letters";

    if(train_chars[0][0] == 0)
    {
        fp = fopen(filename, "r");
        if (fp == NULL)
        {
            printf("Could not open file %s",filename);
            return NULL;
        }

        int file_counter = 0;
        int line_num = 0;
        while (fgets(str, TRAIN_IMG_SZ+2, fp) != NULL)
        {
            if(str[1] == '\0') // Empty line, switch file
            {
                file_counter++;
                line_num = 0;
                continue;
            }

            if(str[2] == '\0')
            {
                train_chars[file_counter][0] = (int)str[0];
                continue;
            }

            // Otherwise, the only possibility is an entire line
            for(int i = 0; i < TRAIN_IMG_SZ; i++)
            {
                train_chars[file_counter][1+line_num*TRAIN_IMG_SZ + i] = str[i];
            }
            line_num++;
        }
        fclose(fp);
    }

    // Now choose one already stored element from train_chars
    int r = rand() % NUM_OF_TRAIN_CHARS; // Random value between 0 and NUM_OF_TRAIN_CHARS-1

    return train_chars[r];
}
