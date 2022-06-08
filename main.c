#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <strings.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

unsigned char* color_to_gray(unsigned char* Image, int sizeV, int sizeH, int step) {
    int k = 0;
    unsigned char* grayImage = (unsigned char*)malloc(sizeV*sizeH*sizeof(unsigned char));
    if ( grayImage == NULL) {
            printf("Memory allocation error at color_to_gray(): %d, %d\n", sizeV, sizeH );
            return 1;
    }

    for (int i = 0; i < sizeV*sizeH*step; i = i + step) {
        grayImage[k] = 0.299*Image[i] + 0.587*Image[i + 1] + 0.114*Image[i + 2];
        k = k + 1;
    }
    return grayImage;

}


int main() {

    char* inputPath = "hampster.png";
    int iw, ih, n; 
    unsigned char *idata = stbi_load(inputPath, &iw, &ih, &n, 0);
    if (idata == NULL) {
        printf("ERROR: can't read file %s\n", inputPath );
        return 1;
    }
    
    unsigned char* odata = (unsigned char*)malloc(ih*iw*n*sizeof(unsigned char));
    unsigned char* newImage = (unsigned char*)malloc(ih*iw*sizeof(unsigned char));
    if ( (odata == NULL) || (newImage == NULL) ) {
            printf("Memory allocation error at main()" );
            return 1;
        }

    newImage = color_to_gray(idata, iw, ih, n);   
    char* outputPath = "result.png";

    stbi_write_png(outputPath, iw, ih, 1, newImage, 0);

    free(newImage);
    free(odata);
    stbi_image_free(idata);

    return 0;
}
