#pragma once
#include <gd.h>
#include <stdio.h>
#include <time.h>
typedef unsigned int ui;
#define MAXNAMECOLOR 128
#define MAXCOLORS 128
#define CAPTCHALEN (3*4)+2


struct gdColor{
	char name[MAXNAMECOLOR];
	int color;
};
typedef struct {
	ui r;
	ui g;
	ui b;
	ui a;
} RGBa;

typedef struct {
	gdImagePtr im;
	char captcha[CAPTCHALEN];
	void * raw;
	int size;
} captcha;

struct image{
	gdImagePtr im;
	ui width;
	ui height;
	char * textOnImage;
};

struct image initImage(ui x, ui y, 
		RGBa * color);

int gdInitColor( struct image * im , RGBa * color);

RGBa gdRandColor(void);

struct image gdInitImage(ui width, ui height, RGBa * bgcolor);
struct image gdInitImageFromFile(const char * filename, ui width, ui height);

void * getImageData(struct image *im, ui quality, int * size);

void gdDrawRandomPixels(struct image * im, ui min,ui max);
void gdDrawRandomLines(struct image * im, ui count);
int gdSetPixel(struct image * im, ui x, ui y, int color);
int gdDrawRect(struct image * im, ui x, ui y, ui x1, ui x2, int color);
int gdDrawLine(struct image * im, ui x, ui y, ui x1, ui x2, int color);
void gdGetRandStr(unsigned char * ret, size_t len);

void gdAddColor(struct image *im,char * name,  RGBa * color);
int gdSetColor(struct image *im, const int i, char * name,  RGBa * color);
int gdFoundColor(const char * name);


int writeToFile( void (*fun)(gdImagePtr, FILE*, int), const char namefile[], struct image * im, ui quality);
captcha gdDrawCaptcha(void) __attribute__((deprecated));

void gdImageClear(struct image * im);

