#include "images.h"
#include <string.h>

static struct gdColor colors[MAXCOLORS];
static unsigned char cColors = 0;

int gdInitColor(struct image *im, RGBa *color) {
  return gdImageColorAllocateAlpha(im->im, color->r, color->g, color->b,
                                   color->a);
}

int gdSetColors(struct image *im, const int i, char *name, RGBa *color) {
  if (i > MAXCOLORS || i < 0)
    return -1;
  strncpy(colors[i].name, name, MAXNAMECOLOR);
  colors[i].color = gdInitColor(im, color);
  return 1;
}

void gdAddColors(struct image *im, char *name, RGBa *color) {
  if (gdSetColors(im, cColors, name, color) == -1)
    return;
  cColors++;
}

int gdFoundColor(const char *name) {
  for (unsigned int i = cColors; i--;) {
    if (strcmp(colors[i].name, name) == 0) {
      return colors[i].color;
    }
  }
  return 0;
}

RGBa gdRandColor(void) {
  srand(time(NULL));
  RGBa ret;
  ret.r = rand() % 255;
  ret.g = rand() % 255;
  ret.b = rand() % 255;
  ret.a = rand() % 255;
  return ret;
}

void gdGetRandStr(unsigned char *ret, size_t len) {
  char b = 0;
  ret[len] = 0;
  for (len - 1; len--;) {
    while ((b < '0' || b > '9') && (b < 'a' || b > 'z') &&
           (b < 'A' || b > 'Z')) {
      b = rand() % 127;
    }
    // printf("Ret: %c\n", b);
    ret[len] = b;
    b = 0;
  }
}
#define MAXSIZEYX                                                              \
  if (x > im->width)                                                           \
    return -1;                                                                 \
  if (y > im->height)                                                          \
    return -1;

int gdDrawRect(struct image *im, ui x, ui y, ui x1, ui y1, int color) {
  MAXSIZEYX;
  gdImageRectangle(im->im, x, y, x1, y1, color);
  return 1;
}
int gdDrawLine(struct image *im, ui x, ui y, ui x1, ui y1, int color) {
  MAXSIZEYX;
  gdImageLine(im->im, x, y, x1, y1, color);
}
struct image gdInitImageFromFile(const char *filename, ui width, ui height) {
  gdImagePtr im = gdImageCreateFromFile(filename);
  struct image rt;
  rt.im = im;
  rt.width = width;
  rt.height = height;
  return rt;
}
struct image gdInitImage(ui width, ui height, RGBa *bgcolor) {
  gdImagePtr im = gdImageCreate(width, height);
  struct image rt;
  rt.im = im;
  rt.width = width;
  rt.height = height;

  int bg = gdInitColor(&rt, bgcolor);
  gdImageRectangle(im, 0, 0, width, height, bg);

  return rt;
}

#define MAX(a, b) a > b ? a : b;
#define RANDCLR(alpha)                                                         \
  gdImageColorAllocateAlpha(im->im, rand() % 255, rand() % 255, rand() % 255,  \
                            rand() % alpha)

void gdDrawRandomPixels(struct image *im, ui min, ui max) {
  srand(time(NULL));
  if (min > max)
    return;
  ui count = 0;
  while (count < min)
    count = rand() % max;
  ui x, y;
  for (ui i = count; i--;) {
    x = rand() % im->width;
    y = rand() % im->height;
    gdSetPixel(im, x, y, RANDCLR(15));
  }
}

int gdSetPixel(struct image *im, ui x, ui y, int color) {
  MAXSIZEYX;
  gdImageSetPixel(im->im, x, y, color);
  return 1;
}

void gdDrawRandomLines(struct image *im, ui count) {
  ui x, y, x1, y1;
  while (count--) {
    x = rand() % im->width;
    y = rand() % im->height;
    x1 = rand() % im->width;
    y1 = rand() % im->height;
    gdDrawLine(im, x, y, x1, y1, RANDCLR(5));
  }
}

void * getImageData(struct image *im, ui quality, int * size) {
  void * data = gdImageJpegPtr(im->im, size, quality);
#ifdef MEMORYDEBUG
  printf("data is: %p\n", data);
#endif
  return data;
}

int writeToFile(void (*fun)(gdImagePtr, FILE *, int), const char namefile[],
                struct image *im, ui quality) {
  FILE *out;
  out = fopen(namefile, "wb");
  fun(im->im, out, quality);
  fclose(out);
}

captcha gdDrawCaptcha(void) {

  int black;
  int white;
  RGBa bg = gdRandColor();
  struct image im = gdInitImage(100, 40, &bg);
  RGBa black_ = {0, 0, 0, 255};       // for lua
  RGBa white_ = {255, 255, 255, 255}; // for lua
  gdAddColors(&im, "black", &black_);
  gdAddColors(&im, "white", &white_);
  black = gdFoundColor("black"); // for lua
  white = gdFoundColor("white"); // for lua
  gdDrawLine(&im, 0, 0, im.width, im.height, white);

  captcha rt;

  char randStr[3];
  randStr[ sizeof(randStr)-1 ] = 0;

  bzero(rt.captcha, CAPTCHALEN);

#define RANDSTRCAPTCHA(a, b, c, file)                                          \
  gdGetRandStr(randStr, sizeof(randStr)-1);                                      \
  gdImageStringTTF(im.im, NULL, black, file, 10, a, b, c, randStr);            \
  sprintf(rt.captcha, "%s%s", rt.captcha, randStr);

  RANDSTRCAPTCHA(0, 0, 10, "./dummy.ttf");
  RANDSTRCAPTCHA(0, 10, 21, "./dummy.ttf");
  RANDSTRCAPTCHA(0.1, 20, 35, "./dummy.ttf");
  RANDSTRCAPTCHA(-0.2, 60, 10, "./dummy.ttf");

  gdDrawRandomPixels(&im, 100, 200);
  gdDrawRandomLines(&im, 5);
  gdImageContrast(im.im, 0.5);

  int size;
  void * data = getImageData(&im, 95, &size);

  rt.raw = data;
  rt.size = size;
  gdImageDestroy(im.im);
  if(im.textOnImage != NULL) free(im.textOnImage);
  return rt;
}

#undef MAX
#undef RANDCLR
