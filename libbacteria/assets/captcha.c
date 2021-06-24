#include <gd.h>
#include <stdio.h>
#include <time.h>
typedef unsigned int ui;

typedef struct {
	ui r;
	ui g;
	ui b;
	ui a;
} RGBa;

int initColor( gdImagePtr im , RGBa * color){
	return gdImageColorAllocateAlpha(im, color->r, color->g, color->b, color->a);
}
RGBa randColor(void){
	srand(time(NULL));
	RGBa ret;
	ret.r=rand() % 255;
	ret.g=rand() % 255;
	ret.b=rand() % 255;
	ret.a=rand() % 255;
	return ret;
}
void getRandStr(unsigned char * ret, size_t len){
	char b=0;
	for(len-1;len--;){
		while ( (b < '0' || b > '9') && (b < 'a' || b > 'z') && (b <'A' || b >'Z') ){ 
			b = rand() % 127;
		}
		//printf("Ret: %c\n", b);
		ret[len]= b;
		b = 0;
	}
	ret[len]=0;
}


gdImagePtr initImage(ui x, ui y, 
		RGBa * color){
	gdImagePtr im = gdImageCreate(x, y);
	int bg = initColor( im , color );
	gdImageRectangle(im, 0,0, x,y, bg);
	return im;
}
#define MAX(a,b) a > b ? a : b;
#define RANDCLR(alpha) gdImageColorAllocateAlpha(im,rand() % 255, rand() % 255, rand() %255, rand() % alpha)
void randomPixels(gdImagePtr im, ui min,ui max, ui width, ui height){
	  srand( time(NULL) );
	  if(min > max) return;
	  ui count = 0;
	  while( count < min ) count = rand() % max;
	  ui x, y;
	  for(ui i = count; i--; ){
		 x = rand() % width;
		 y = rand() % height;
		 gdImageSetPixel(im, x , y ,  RANDCLR(15) ); 


	  }
	  
	  
}
void lines(gdImagePtr im, ui width, ui height, ui count){
	  ui x, y,x1,y1;
	  while(count--){
	  	x = rand() % width; y = rand() % height;
	  	x1 = rand() % width; y1 = rand() % height;
	  	gdImageLine(im, x, y, x1, y1, RANDCLR(5));
      	  }
} 





int main() {
      int black;
      int white;
      RGBa bg = randColor();
      RGBa black_ = {0,0,0,255};
      RGBa white_ = {255,255,255,255};
      gdImagePtr im = initImage(100, 40, &bg);

      /* Allocate the color black (red, green and blue all minimum).
        Since this is the first color in a new image, it will
        be the background color. */
      black = initColor(im, &black_ );

      /* Allocate the color white (red, green and blue all maximum). */
      white = initColor(im, &white_);

      /* Draw a line from the upper left to the lower right,
        using white color index. */
      gdImageLine(im, 0, 0, 63, 63, white);
      /*
	gdImageStringTTF (	gdImage 	*	im,
	int 	*	brect,
	int 		fg,
	char 	*	fontlist,
	double 		ptsize,
	double 		angle,
	int 		x,
	int 		y,
	char 	*	string	)
	*/
      char randStr[3];
      getRandStr( randStr, sizeof(randStr) );
          gdImageStringTTF(im, NULL, black, "./dummy.ttf",
      10, 0, 0, 10, randStr);
  getRandStr( randStr, sizeof(randStr) );
          gdImageStringTTF(im, NULL, black, "./dummy.ttf",
      10, 0, 10, 21, randStr);
  getRandStr( randStr, sizeof(randStr) );
          gdImageStringTTF(im, NULL, black, "./dummy.ttf",
      10, 0.1, 20, 35, randStr);
  getRandStr( randStr, sizeof(randStr) );
          gdImageStringTTF(im, NULL, black, "./dummy.ttf",
      10, -0.2, 60, 10, randStr);
  getRandStr( randStr, sizeof(randStr) );
          gdImageStringTTF(im, NULL, black, "./dummy.ttf",
      10, 0.1, 60, 32, randStr);
randomPixels(im, 100, 200, 100, 40);
lines(im, 100, 40, 5);
gdImageContrast(im, 0.5);


int size;
void * data = gdImageJpegPtr(im, &size, 50);
      for(ui i =0; i < size; i++){
	printf("%c ", ((char*)data)[i]);
      }
      puts("");
      //gdImageJpeg(im, jpegout, -1);

      /* Close the files. */

      /* Destroy the image in memory. */
      gdImageDestroy(im);
    }
