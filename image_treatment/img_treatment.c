#include <stdio.h>
#include <stdlib.h>
#include "img_treatment.h"

void free_binary_image(binary_image *b_image) {
    if(b_image)
    {
        if(b_image->pixel)
        {
            free(b_image->pixel);
        }

        for(int i=0; i<b_image->lr_size; i++)
        {
            if(!b_image->lr)
            {
                for(int j=0; j>b_image->lr->cr_size; j++)
                {
                    if(!b_image->lr->cr)
                    {
                        free(b_image->lr->cr);
                    }
                }
                free(b_image->lr);
            }
        }
        free(b_image);
    }
}

binary_image *bi_image_from_file(char *filename) {
    // const int NUM_PIXEL_BYTES = 3; /* Number of bytes per pixel. We only handle 24 bits bitmaps for now. */
    const int BMP_FILE_HEADER_LEN = 14;

    FILE *file = fopen(filename, "rb");
    if(file)
    {
        unsigned int file_len;
        unsigned int dib_header_len;
        int img_w, img_h;

        // Position 2: the file length (little-endian format)
        fseek(file, 2L, SEEK_SET);
        fread(&file_len, 4, 1, file);

        fseek(file, 14L, SEEK_SET);
        fread(&dib_header_len, 4, 1, file);
        fread(&img_w, 4, 1, file);
        fread(&img_h, 4, 1, file);

        printf("File len: %d; DIB header len: %d; Image width: %d; Image height: %d\n",
               file_len, dib_header_len, img_w, img_h);

        binary_image *b_image = malloc(sizeof (binary_image));
        if(!b_image) return 0;

        b_image->pixel = malloc(img_h*img_w*sizeof(b_image->pixel));
        if(!b_image->pixel) return 0;

        b_image->h = img_h;
        b_image->w = img_w;
        b_image->lr_size = 0;
        b_image->lr = NULL;

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

                /* NOT A GOOD WAY TO TURN A COLOR IMAGE TO GRAYSCALE /!\

                The right way is :
                grey =  0.2126 * r + 0.7152 * g + 0.0722 * b;
                because colors don't have the same luminance
                */

                b_image->pixel[img_w*i+j] = ((int)r + (int)g + (int)b) / 3;

                /*
                Here the image needs to be contrasted and calibrated.
                TODO - Improve contrast
                */

                //datax[i][j] = ((int)r + (int)g + (int)b) / 3;
            }

            if(padding != 0)
            {
                fseek(file, padding, SEEK_CUR);
            }
            // printf("Reached position:  %d\n", (int)ftell(file));
        }
        fclose(file);
        return b_image;
    }
    return 0;
}

binary_image *bi_detect_text_blocks(binary_image *b_img) {
    /*
    if(!b_img) return 0;

    // Is this pixel white ?
    int is_pix_white;
    // Do we already have a marked block ?
    int already_marked = 0;

    // Detect first line containing a black pixel
    for(int h=0; h<b_img->h; h++) {
        is_pix_white = 1;

        for(int w=0; w<b_img->w; w++) {
            unsigned char pixel = b_img->pixel[h*b_img->w + w];
            if(!pixel) {
                // Found a black pixel!
                is_pix_white = 0;
                break;
            }
        }
    }

    // TODO
    // Continue with detection. For every detected line rectangle,
    // update b_img.lr and b_img.lr_size. Don't forget to malloc !
*/
    return NULL;
}

binary_image *bi_image_detect_char_blocks(binary_image *b_img) {
    /*
    // TODO
    // For every b_img->lr...
    // Do not forget the malloc for the detected cr blocks

*/
    return NULL;
}


//Plus il y aura de lignes et plus elles seront remplies et plus cette m�thode ainsi que l'arrondissement des erreurs seront fonctionnels.
binary_image *bi_detect_text_lines(binary_image *b_img) {
    /*
    if(!b_img) return 0;


    int lines[b_img->h];


    // Detect first line containing a black pixel
    for(int h=0; h<b_img->h; h++) {
	lines[h]=0;

        for(int w=0; w<b_img->w; w++) {
            unsigned char pixel = b_img->pixel[h*b_img->w + w];
            if(!pixel) {
                // Found a black pixel!
		lines[h]=1;
                break;
            }
        }
    }
    unsigned int length = 0;
    for(int h=1; h < b_img->h; h++){
	 if(lines[h]==1 && lines[h-1]==1){
		length++;
	 }
	 else{
		if (length>0){
		      //Stocker le tuple (h,length) dans une structure dynamique
		      length = 0;
		}
	 }
    }
    //Pour arrondir les erreurs.
    unsigned int refer; // refer est �gale au length le plus courant;
    // parcour des tuples
    // parcours des tuples et supprime ceux ayant une length inf�rieur � refer/6
    // parcours les tuples ayant une length sup�rieur � 2refer while(ref*i<currentlength){i++}
    	// int temp = length/i;
    	// unsigned int j;
    	// for (j=0;j<i;j++){stocker le tuple (h+j*temp,temp)}
    	// supprimer le tuple de base
*/
    return NULL;
}



binary_image *bi_image_RLSA(binary_image *b_img, unsigned int expansion)
{
    if(!b_img) return 0;



    unsigned char horizontal_RLSA[b_img->w * b_img->h];
    unsigned char vertical_RLSA[b_img->w * b_img->h];



    for (size_t y = 0; y < b_img->h; y++) {
        unsigned int counter = 0;
        for (size_t x = 0; x < b_img->w; x++) {
            unsigned char pixel = b_img->pixel[y*b_img->w + x];
            //printf("pixel (%d, %d) = %d / counter = %d\n",x, y, pixel, counter);
            if (!pixel) { //if black pixel
                if (counter <= expansion) {
                    for (size_t i = 1; i <= counter; i++)
                        horizontal_RLSA[y*b_img->w + x - i] = 0;
                }
                counter = 0;
                horizontal_RLSA[y*b_img->w + x] = 0;
            } else { //if white pixel
                counter++;
                horizontal_RLSA[y*b_img->w + x] = 1;
            }
        }
        if (counter <= expansion) {
            for (size_t i = 1; i <= counter; i++) {
                horizontal_RLSA[(y+1)*b_img->w - i] = 0;
            }
        }
    }

    for (size_t x = 0; x < b_img->w; x++) {
        unsigned int counter = 0;
        for (size_t y = 0; y < b_img->h; y++) {
            unsigned char pixel = b_img->pixel[y*b_img->w + x];
            //printf("pixel (%d, %d) = %d / counter = %d\n",x, y, pixel, counter);
            if (!pixel) { //if black pixel
                if (counter <= expansion) {
                    for (size_t i = 1; i <= counter; i++)
                        vertical_RLSA[(y - i)*b_img->w + x] = 0;
                }
                counter = 0;
                vertical_RLSA[y*b_img->w + x] = 0;
            } else { //if white pixel
                counter++;
                vertical_RLSA[y*b_img->w + x] = 1;
            }
        }
        if (counter <= expansion) {
            for (size_t i = 1; i <= counter; i++) {
                vertical_RLSA[(b_img->h-i)*b_img->w + x] = 0;
            }
        }
    }


    binary_image *result_image = malloc(sizeof (binary_image));
    if(!result_image) return 0;

    result_image->pixel = malloc(b_img->w*b_img->h*sizeof(result_image->pixel));
    if(!result_image->pixel) return 0;

    result_image->h = b_img->h;
    result_image->w = b_img->w;
    result_image->lr_size = 0;
    result_image->lr = NULL;

    for (size_t i = 0; i < b_img->w * b_img->h; i++) {
        result_image->pixel[i] = vertical_RLSA[i] || horizontal_RLSA[i];

    }
    return result_image;
}


void mark_zone(unsigned char mark_array[], binary_image *rlsa_img, unsigned int x, unsigned int y,
unsigned int *min_x, unsigned int *min_y, unsigned int *max_x, unsigned int *max_y) {

    if (rlsa_img->pixel[rlsa_img->w * y + x] ||
        mark_array[rlsa_img->w * y + x]) return;

    mark_array[rlsa_img->w * y + x] = 1;

    *min_x = (x > *min_x) ? *min_x : x;
    *max_x = (x < *max_x) ? *max_x : x;

    *min_y = (y > *min_y) ? *min_y : y;
    *max_y = (y < *max_y) ? *max_y : y;

    if (x > 0)
        mark_zone(mark_array, rlsa_img, x-1, y, min_x, min_y, max_x, max_y);
    if (y > 0)
        mark_zone(mark_array, rlsa_img, x, y-1, min_x, min_y, max_x, max_y);
    if (x < rlsa_img->w)
        mark_zone(mark_array, rlsa_img, x+1, y, min_x, min_y, max_x, max_y);
    if (y < rlsa_img->h)
        mark_zone(mark_array, rlsa_img, x, y+1, min_x, min_y, max_x, max_y);

    return;
}

void bi_image_blocks_from_RLSA(binary_image *b_img, binary_image *rlsa_img)
{
    printf("Creating blocks...\n");

    unsigned char mark_array[b_img->w * b_img->h];
    for (size_t i = 0; i < b_img->w * b_img->h; i++)
        mark_array[i] = 0;

    unsigned int i = 0;

    for (unsigned int x = 0; x < b_img->w; x++) {
        for (unsigned int y = 0; y < b_img->h; y++) {

            unsigned int min_x = b_img->w - 1,
                         min_y = b_img->h - 1,
                         max_x = 0,
                         max_y = 0;

            if (!rlsa_img->pixel[rlsa_img->w * y + x] &&
                !mark_array[rlsa_img->w * y + x])
            {
                mark_zone(mark_array, rlsa_img, x, y,
                &min_x, &min_y, &max_x, &max_y);


                l_rect *rect = malloc(sizeof (l_rect));
                rect->min_x = min_x;
                rect->min_y = min_y;
                rect->max_x = max_x;
                rect->max_y = max_y;

                rect->next = b_img->lr;

                rect->cr_size = 0;
                rect->cr = 0;

                b_img->lr = rect;

                //printf("INSIDE : min = (%d, %d)   max = (%d, %d)\n", min_x, min_y, max_x, max_y);
                i++;
            }
        }
    }

    b_img->lr_size = i;
}
