#include"images/images.h"
#include "lua.h"

INITLUAFUNC(gdInitImage);
INITLUAFUNC(gdInitImageFromFile);
INITLUAFUNC(gdSetPixel);
INITLUAFUNC(gdDrawRandomPixels);
INITLUAFUNC(gdDrawRandomLines);
INITLUAFUNC(getImageData);
INITLUAFUNC(writeToFile);
INITLUAFUNC(gdInitColor);
INITLUAFUNC(gdSetColors);
INITLUAFUNC(gdAddColors);
INITLUAFUNC(gdFoundColor);
INITLUAFUNC(gdRandColor);
INITLUAFUNC(gdGetRandStr);
INITLUAFUNC(gdDrawRect);
INITLUAFUNC(gdDrawLine);

static const struct luaL_reg imagesLib [] = {
	LUAPAIR(gdInitImage)
	LUAPAIR(gdInitImageFromFile)
	LUAPAIR(gdSetPixel)
	LUAPAIR(gdDrawRandomPixels)
	LUAPAIR(gdDrawRandomLines)
	LUAPAIR(getImageData)
	LUAPAIR(writeToFile)
	LUAPAIR(gdInitColor)
	LUAPAIR(gdSetColors)
	LUAPAIR(gdAddColors)
	LUAPAIR(gdFoundColor)
	LUAPAIR(gdRandColor)
	LUAPAIR(gdGetRandStr)
	LUAPAIR(gdDrawRect)
	LUAPAIR(gdDrawLine)
	{NULL,NULL}
};

