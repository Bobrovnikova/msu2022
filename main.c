#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <strings.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int forsort(const void* x, const void* y) {
    return ( *(int*)x - *(int*)y );
}

float* color_to_gray(float* Image, int sizeV, int sizeH, int step) {
    int k = 0;
    float* grayImage = (float*)malloc(sizeV*sizeH*sizeof(float));
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
float*  gray_to_bw( float* Image, int sizeV, int sizeH, int t_black, int t_white, int t_gray) {
    int i, j;
    for (i = 2; i < sizeH-1; i++) {
        for (j = 2; j < sizeV-1; j++) {
            if (Image[sizeV*i+j] < t_black) Image[sizeV*i+j] = 0;
	    else if (Image[sizeV*i+j] < t_gray) Image[sizeV*i+j] = 90;
            else if (Image[sizeV*i+j] > t_white) Image[sizeV*i+j] = 255;
	    else Image[sizeV*i+j] = 160;
        }
    }
}

void  median_filter( float* Image, int sizeV, int sizeH) {
    int i, j;
    float* a = (float*)malloc(9*sizeof(float));
    for (i = 2; i < sizeH-1; i++) {
        for (j = 2; j < sizeV-1; j++) {
                a[0] = Image[sizeV*(i-1)+j-1];
                a[1] = Image[sizeV*i+j-1];
                a[2] = Image[sizeV*(i+1)+j-1];
                a[3] = Image[sizeV*(i-1)+j];
                a[4] = Image[sizeV*i+j];
                a[5] = Image[sizeV*(i+1)+j];
                a[6] = Image[sizeV*(i-1)+j+1];
                a[7] = Image[sizeV*i+j+1];
                a[8] = Image[sizeV*(i+1)+j+1];
                qsort(a, 9, sizeof(float), forsort);
                Image[sizeV*i+j] = a[4];
        }
    }
    return;
}

void  gaus_filter( float* Image, int sizeV, int sizeH) {
    int i, j;
    for (i = 2; i < sizeH-2; i++) {
        for (j = 2; j < sizeV-2; j++) {
                Image[sizeV*i+j] += 0.000789*Image[sizeV*(i-2)+j-2] + 0.006581*Image[sizeV*(i-1)+j-2] + 0.013347*Image[sizeV*(i)+j-2] + 0.006581*Image[sizeV*(i+1)+j-2] + 0.000789*Image[sizeV*(i+2)+j-2];
		Image[sizeV*i+j] += 0.006581*Image[sizeV*(i-2)+j-1] + 0.054901*Image[sizeV*(i-1)+j-1] + 0.111345*Image[sizeV*(i)+j-1] + 0.054901*Image[sizeV*(i+1)+j-1] + 0.006581*Image[sizeV*(i+2)+j-1];
		Image[sizeV*i+j] += 0.013327*Image[sizeV*(i-2)+j] + 0.111345*Image[sizeV*(i-1)+j] + 0.225821*Image[sizeV*(i)+j] + 0.111345*Image[sizeV*(i+1)+j] + 0.013347*Image[sizeV*(i+2)+j];
		Image[sizeV*i+j] += 0.006581*Image[sizeV*(i-2)+j+1] + 0.054901*Image[sizeV*(i-1)+j+1] + 0.111345*Image[sizeV*(i)+j+1] + 0.054901*Image[sizeV*(i+1)+j+1] + 0.006581*Image[sizeV*(i+2)+j+1];
		Image[sizeV*i+j] += 0.000789*Image[sizeV*(i-2)+j+2] + 0.006581*Image[sizeV*(i-1)+j+2] + 0.013347*Image[sizeV*(i)+j+2] + 0.006581*Image[sizeV*(i+1)+j+2] + 0.000789*Image[sizeV*(i+2)+j+2];
        }
    }
    return;
}

int main() {

    char* inputPath = "hampster.png";
    int iw, ih, n; 
    float *idata = stbi_load(inputPath, &iw, &ih, &n, 0);
    if (idata == NULL) {
        printf("ERROR: can't read file %s\n", inputPath );
        return 1;
    }
    
    float* odata = (float*)malloc(ih*iw*n*sizeof(float));
    float* newImage = (float*)malloc(ih*iw*sizeof(float));
    if ( (odata == NULL) || (newImage == NULL) ) {
            printf("Memory allocation error at main()" );
            return 1;
        }

    newImage = color_to_gray(idata, iw, ih, n);
    gaus_filter(newImage, iw, ih);
    int t_black = 92;
    int t_white = 215;
    int t_gray = 145;
    gray_to_bw(newImage, iw, ih, t_black, t_white, t_gray);
   
    char* outputPath = "result.png";

    stbi_write_png(outputPath, iw, ih, 1, newImage, 0);

    free(newImage);
    free(odata);
    stbi_image_free(idata);

    return 0;
}
