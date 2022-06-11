#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <strings.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define E 8

int forsort(const void* x, const void* y) {
    return ( *(int*)x - *(int*)y );
}

void dfs(int i, int j, int color, int sizeV, int sizeH, unsigned char * mark, unsigned char * Image){
    mark[i*sizeV+j] = color;
    if(i+1<sizeH-1&&abs(Image[i*sizeV+j]-Image[sizeV*(i+1)+j])<E&&!mark[sizeV*(i+1)+j]){
        dfs(i+1, j, color, sizeV, sizeH, mark, Image);
    }
    if(j+1<sizeV-1&&abs(Image[i*sizeV+j]-Image[sizeV*i+j+1])<E&&!mark[sizeV*i+j+1]){
        dfs(i,j+1, color, sizeV, sizeH, mark, Image);
    }
    if(i+1<sizeH-1&&j+1<sizeV-1&&abs(Image[i*sizeV+j]-Image[sizeV*(i+1)+j+1])<E&&!mark[sizeV*(i+1)+j+1]){
        dfs(i+1,j+1, color, sizeV, sizeH, mark, Image);
    }
    if(i-1>2&&abs(Image[i*sizeV+j]-Image[sizeV*(i-1)+j])<E&&!mark[sizeV*(i-1)+j]){
        dfs(i-1,j, color, sizeV, sizeH, mark, Image);
    }
    /*if(j-1>2&&abs(Image[i*sizeV+j]-Image[sizeV*i+j-1])<E&&!mark[sizeV*i+j-1]){
        dfs(i,j-1, color, sizeV, sizeH, mark, Image);
    }
    if(i-1>2&&j-1>2&&abs(Image[i*sizeV+j]-Image[sizeV*(i-1)+j-1])<E&&!mark[sizeV*(i-1)+j-1]){
        dfs(i-1,j-1, color, sizeV, sizeH, mark, Image);
    }
    if(i-1>2&&j+1<sizeV-1&&abs(Image[i*sizeV+j]-Image[sizeV*(i-1)+j+1])<E&&!mark[sizeV*(i-1)+j+1]){
        dfs(i-1,j+1, color, sizeV, sizeH, mark, Image);
    }
    if(i+1<sizeH-1&&j-1>2&&abs(Image[i*sizeV+j]-Image[sizeV*(i+1)+j-1])<E&&!mark[sizeV*(i+1)+j-1]){
        dfs(i+1,j-1, color, sizeV, sizeH, mark, Image);
    }*/
    return;
}

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
unsigned char*  gray_to_bw( unsigned char* Image, int sizeV, int sizeH, int t_black, int t_white, int t_gray) {
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

void  median_filter( unsigned char* Image, int sizeV, int sizeH) {
    int i, j;
    unsigned char* a = (unsigned char*)malloc(9*sizeof(unsigned char));
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
                qsort(a, 9, sizeof(unsigned char), forsort);
                Image[sizeV*i+j] = a[4];
        }
    }
    return;
}

void  gaus_filter( unsigned char* Image, int sizeV, int sizeH) {
    int i, j;
    unsigned char a;
    for (i = 2; i < sizeH-2; i++) {
        for (j = 2; j < sizeV-2; j++) {
		a = 0;
                a += 0.000789*Image[sizeV*(i-2)+j-2] + 0.006581*Image[sizeV*(i-1)+j-2] + 0.013347*Image[sizeV*(i)+j-2] + 0.006581*Image[sizeV*(i+1)+j-2] + 0.000789*Image[sizeV*(i+2)+j-2];
		a += 0.006581*Image[sizeV*(i-2)+j-1] + 0.054901*Image[sizeV*(i-1)+j-1] + 0.111345*Image[sizeV*(i)+j-1] + 0.054901*Image[sizeV*(i+1)+j-1] + 0.006581*Image[sizeV*(i+2)+j-1];
		a += 0.013327*Image[sizeV*(i-2)+j] + 0.111345*Image[sizeV*(i-1)+j] + 0.225821*Image[sizeV*(i)+j] + 0.111345*Image[sizeV*(i+1)+j] + 0.013347*Image[sizeV*(i+2)+j];
		a += 0.006581*Image[sizeV*(i-2)+j+1] + 0.054901*Image[sizeV*(i-1)+j+1] + 0.111345*Image[sizeV*(i)+j+1] + 0.054901*Image[sizeV*(i+1)+j+1] + 0.006581*Image[sizeV*(i+2)+j+1];
		a += 0.000789*Image[sizeV*(i-2)+j+2] + 0.006581*Image[sizeV*(i-1)+j+2] + 0.013347*Image[sizeV*(i)+j+2] + 0.006581*Image[sizeV*(i+1)+j+2] + 0.000789*Image[sizeV*(i+2)+j+2];
		Image[sizeV*i+j] = a;
        }
    }
    return;
}
unsigned char* gray_to_color(unsigned char* Image, int sizeV, int sizeH) {
    unsigned char* colorImage = (unsigned char*)malloc(sizeV*sizeH*4*sizeof(unsigned char));
    if ( colorImage == NULL) {
            printf("Memory allocation error at color_to_gray(): %d, %d\n", sizeV, sizeH );
            return 1;
    }

    for (int i = 0; i < sizeV*sizeH; i = i + 1) {
        colorImage[4*i] = 0.4*Image[i]+78+0.2*Image[i-1];
	colorImage[4*i+1] = 0.4*Image[i]+46;
	colorImage[4*i+2] = 0.4*Image[i]+153;
	colorImage[4*i+3] = 255;
    }
    return colorImage;

}

int main() {

    char* inputPath = "hampster.png";
    int iw, ih, n; 
    int i, j, k;
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
    //gaus_filter(newImage, iw, ih);
    int t_black = 97;
    int t_white = 220;
    int t_gray = 150;
    //gray_to_bw(newImage, iw, ih, t_black, t_white, t_gray);
    //gaus_filter(newImage, iw, ih);
    //gaus_filter(newImage, iw, ih);
    //unsigned char* colorImage = (unsigned char*)malloc(iw*ih*n*sizeof(unsigned char));
    //colorImage = gray_to_color(newImage, iw, ih);
    unsigned char* col = (unsigned char*)malloc(ih*iw*sizeof(unsigned char));
    for (i = 0; i < iw*ih; i++) {
        col[i] = 0;
    }
    k = 55;
    for (i = 2; i < ih-1; i++) {
        for (j = 2; j < iw-1; j++) {
        if (col[iw*i+j] == 0) {
            dfs(i, j, k, iw, ih, col, newImage);
            k = k + 50;
	}
	}
    }
    for (i = 0; i < iw*ih; i++) {
        odata[i*n] = 78+col[i];//+0.5*col[i-1]
        odata[i*n+1] = 46+col[i];
        odata[i*n+2] = 153+col[i];
        if (n == 4) odata[i*n+3] = 255;
    }
    char* outputPath = "result.png";

    stbi_write_png(outputPath, iw, ih, 1, col, 0);

    free(newImage);
    //free(colorImage);
    free(odata);
    stbi_image_free(idata);

    return 0;
}
