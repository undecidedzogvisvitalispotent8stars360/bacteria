#include "libimages.h"

int luaopen_libimages(lua_State *L) {
  luaL_openlib(L, "images", imagesLib, 0);
  return 1;
}

#define GETLUANUM(what)                                                        \
  lua_pushstring(L, "" #what);                                                 \
  lua_gettable(L, -2);                                                         \
  if (!lua_isnumber(L, -1)) {                                                  \
    luaL_error(L, "invalid " #what);                                           \
  }                                                                            \
  int gd_##what = lua_tonumber(L, -1);                                         \
  lua_pop(L, 1);

//#define DEBUG_LIBIMAGES_LUA

INITLUAFUNC(gdInitImage) {
  int width = (int)luaL_checknumber(L, 1);
  int height = (int)luaL_checknumber(L, 2);
  GETLUANUM(red);
  GETLUANUM(green);
  GETLUANUM(blue);
  GETLUANUM(alpha);
#ifdef DEBUG_LIBIMAGES_LUA
  printf("width:%d height:%d red:%d green:%d blue:%d alpha: %d\n", width,
         height, gd_red, gd_green, gd_blue, gd_alpha);
#endif
  RGBa color = {gd_red, gd_green, gd_blue};
  size_t nbytes = sizeof(struct image) + sizeof(gdImagePtr) +
                  sizeof(ui) * (width - 1) + sizeof(ui) * (height - 1) +
                  sizeof(char *); // TODO: to macros
  struct image *ret = (struct image *)lua_newuserdata(L, nbytes);
  struct image d = gdInitImage(width, height, &color);
  ret->im = d.im;
  ret->width = d.width;
  ret->height = d.height;
  ret->textOnImage = NULL;
  return 1;
}

#define GETIMAGE                                                               \
  struct image *in = (struct image *)lua_touserdata(L, 1);                     \
  if (in == NULL)                                                              \
    luaL_error(L, "imagestruct is broken");

#define GETIMAGEDATA                                                           \
  struct ImageData *in = (struct ImageData *)lua_touserdata(L, 1);             \
  if (in == NULL)                                                              \
    luaL_error(L, "imagedata is broken");

INITLUAFUNC(gdFree) {
  GETIMAGE;
  gdImageDestroy(in->im);
  if (in->textOnImage != NULL)
    free(in->textOnImage);
  in->width = 0;
  in->height = 0;
  lua_pushboolean(L, 1);
  return 1;
}

INITLUAFUNC(gdInitImageFromFile) {
  const char *filepath = (const char *)luaL_checkstring(L, 1);
  int width = (int)luaL_checknumber(L, 2);
  int height = (int)luaL_checknumber(L, 3);
  struct image d = gdInitImageFromFile(filepath, width, height);
  size_t nbytes = sizeof(struct image) + sizeof(gdImagePtr) +
                  sizeof(ui) * (width - 1) + sizeof(ui) * (height - 1) +
                  sizeof(char *);
  struct image *ret = (struct image *)lua_newuserdata(L, nbytes);
  ret->im = d.im;
  ret->width = d.width;
  ret->height = d.height;
  ret->textOnImage = NULL;
  return 1;
}

#define GET1XY(a, b)                                                           \
  int x1 = (int)luaL_checknumber(L, a);                                        \
  int y1 = (int)luaL_checknumber(L, b);

#define GETXY(a, b)                                                            \
  int x = (int)luaL_checknumber(L, a);                                         \
  int y = (int)luaL_checknumber(L, b);

INITLUAFUNC(gdSetPixel) {
  GETIMAGE;
  GETXY(2, 3);
  int color = (int)luaL_checknumber(L, 4);
  gdSetPixel(in, x, y, color);

  return 0;
}

INITLUAFUNC(gdDrawRandomPixels) {
  GETIMAGE;
  int min = (int)luaL_checknumber(L, 2);
  int max = (int)luaL_checknumber(L, 3);
  gdDrawRandomPixels(in, min, max);

  return 0;
}

INITLUAFUNC(gdDrawRandomLines) {
  GETIMAGE;
  int count = (int)luaL_checknumber(L, 2);
  gdDrawRandomLines(in, count);
  return 0;
}
/*
INITLUAFUNC(getImageDataPtrChar){
        GETIMAGEDATA;
        if(in->raw.cData == NULL)luaL_error(L, "cData broken");
        lua_pushlightuserdata(L, in->raw.cData);
        return 1;
}

INITLUAFUNC(getImageDataPtrVoid){
        GETIMAGEDATA;
        if(in->raw.vData == NULL)luaL_error(L, "vData broken");
        lua_pushlightuserdata(L, in->raw.vData);
        return 1;
}
*/

INITLUAFUNC(getImageDataSize) {
  GETIMAGEDATA;
  lua_pushnumber(L, in->size);
  return 1;
}

INITLUAFUNC(getImageDataRaw) {
  GETIMAGEDATA;

  lua_pushlstring(L, in->raw, in->size);

  return 1;
}

INITLUAFUNC(getImageData) {
  GETIMAGE;
  int quality = (int)luaL_checknumber(L, 2);
  // int b = (int)lua_toboolean(L, 3);
#ifdef MEMORYDEBUG
  printf("In ptr: %p width:%d height:%d \n", in->im, in->width, in->height);
#endif
  int size;
  void *data = getImageData(in, quality, &size);
#ifdef MEMORYDEBUG
  printf("data is: %p\n", data);
#endif
  // printf("DataPtr: %p\n", data);
  size_t nbytes = sizeof(unsigned char *) + sizeof(int) * (size - 1);
  struct ImageData *ret = (struct ImageData *)lua_newuserdata(L, nbytes);
  /*if(b)
          ret->raw.cData=(unsigned char*)data;
  else
          ret->raw.vData=data;
  */
  // ret->raw = malloc(sizeof(char) * size + 1);
  // bzero(ret->raw, size);
  // ret->raw[size]=0;
  // memcpy(ret->raw, data,size);
  // puts("ret raw");
  ret->raw = data;
  ret->size = size;
  return 1;
}

INITLUAFUNC(writeToFile) {
  GETIMAGE;
  // TODO: add png, gif ... etc
  const char *filepath = (const char *)luaL_checkstring(L, 2);
  int quality = (int)luaL_checknumber(L, 3);
  writeToFile(gdImageJpeg, filepath, in, quality);
  return 0;
}

INITLUAFUNC(gdDrawRect) {
  GETIMAGE;
  GETXY(2, 3);
  GET1XY(4, 5);
  int color = (int)luaL_checknumber(L, 6);
  gdDrawRect(in, x, y, x1, y1, color);
  return 0;
}

INITLUAFUNC(gdDrawLine) {
  GETIMAGE;
  GETXY(2, 3);
  GET1XY(4, 5);
  int color = (int)luaL_checknumber(L, 6);
  gdDrawLine(in, x, y, x1, y1, color);
  return 0;
}

INITLUAFUNC(gdWriteText) {
  GETIMAGE;
  GETXY(2, 3);
  char *font = (char *)luaL_checkstring(L, 4);
  double ptSize = luaL_checknumber(L, 5);
  double angle = luaL_checknumber(L, 6);
  int color = (int)luaL_checknumber(L, 7);
  char *text = (char *)luaL_checkstring(L, 8);
  gdImageStringTTF(in->im, NULL, color, font, ptSize, angle, x, y, text);
  return 0;
}

/*colors setglobal maybe?*/
INITLUAFUNC(gdInitColor) {
  GETIMAGE;
  GETLUANUM(red);
  GETLUANUM(green);
  GETLUANUM(blue);
  GETLUANUM(alpha);
  RGBa color = {gd_red, gd_green, gd_blue, gd_alpha};
  lua_pushnumber(L, gdInitColor(in, &color));
  return 1;
}

INITLUAFUNC(gdSetColor) {
  GETIMAGE;
  char *nameColor = (char *)luaL_checkstring(L, 2);
  int c = luaL_checknumber(L, 3);
  GETLUANUM(red);
  GETLUANUM(green);
  GETLUANUM(blue);
  GETLUANUM(alpha);
  RGBa color = {gd_red, gd_green, gd_blue, gd_alpha};
  gdSetColor(in, c, nameColor, &color);
  return 0;
}

INITLUAFUNC(gdAddColor) {
  GETIMAGE;
  char *nameColor = (char *)luaL_checkstring(L, 2);
  GETLUANUM(red);
  GETLUANUM(green);
  GETLUANUM(blue);
  GETLUANUM(alpha);
  RGBa color = {gd_red, gd_green, gd_blue, gd_alpha};
  // printf("Color r:%d g:%d b:%d a:%d\n", gd_red, gd_green, gd_blue,gd_alpha);
  gdAddColor(in, nameColor, &color);
  return 0;
}

INITLUAFUNC(gdFoundColor) {
  GETIMAGE;
  char *nameColor = (char *)luaL_checkstring(L, 2);
  lua_pushnumber(L, gdFoundColor(nameColor));
  return 1;
}

INITLUAFUNC(gdRandColor) {
  lua_newtable(L);
  RGBa rt = gdRandColor();
  LUA_PUSHTABLENUMBER(L, "red", rt.r);
  LUA_PUSHTABLENUMBER(L, "green", rt.g);
  LUA_PUSHTABLENUMBER(L, "blue", rt.b);
  LUA_PUSHTABLENUMBER(L, "alpha", rt.a);
  return 1;
}

INITLUAFUNC(gdGetRandStr) {
  int size = luaL_checknumber(L, 1);
  char randStr[size + 1];
  gdGetRandStr(randStr, size - 1);
  randStr[size] = 0;
  lua_pushstring(L, randStr);
  return 1;
}

#undef GETLUANUM
