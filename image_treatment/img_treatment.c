#include <stdio.h>
#include <stdlib.h>
#include "img_treatment.h"


binary_image *bi_image_from_file(char *filename)
{
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
        b_image->lr = 0;

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

    return NULL;
}

binary_image *bi_image_detect_char_blocks(binary_image *b_img) {
    // TODO
    // For every b_img->lr...
    // Do not forget the malloc for the detected cr blocks


    return b_img;
}


//Plus il y aura de lignes et plus elles seront remplies et plus cette méthode ainsi que l'arrondissement des erreurs seront fonctionnels.
binary_image *bi_detect_text_lines(binary_image *b_img) {
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
    unsigned int refer; // refer est égale au length le plus courant;
    // parcour des tuples 
    // parcours des tuples et supprime ceux ayant une length inférieur à refer/6
    // parcours les tuples ayant une length supérieur à 2refer while(ref*i<currentlength){i++}
    	// int temp = length/i;
    	// unsigned int j;
    	// for (j=0;j<i;j++){stocker le tuple (h+j*temp,temp)}
    	// supprimer le tuple de base

    return NULL;
}
