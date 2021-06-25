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
//INITLUAFUNC(getImageDataPtrChar); // WARNING!!! Ahtung!!! Dont use it, because then some can get memory of program!
//INITLUAFUNC(getImageDataPtrVoid);
INITLUAFUNC(getImageDataSize);
INITLUAFUNC(getImageDataRaw);
INITLUAFUNC(gdFree);
INITLUAFUNC(gdWriteText);

static const struct luaL_reg imagesLib [] = {
	LUAPAIR(gdInitImage)
	LUAPAIR(gdInitImageFromFile)
	LUAPAIR(gdSetPixel)
	LUAPAIR(gdDrawRandomPixels)
	LUAPAIR(gdDrawRandomLines)
	LUAPAIR(getImageData)
	//LUAPAIR(getImageDataPtrChar)
	//LUAPAIR(getImageDataPtrVoid)
	LUAPAIR(getImageDataSize)
	LUAPAIR(writeToFile)
	LUAPAIR(gdInitColor)
	LUAPAIR(gdSetColors)
	LUAPAIR(gdAddColors)
	LUAPAIR(gdFoundColor)
	LUAPAIR(gdRandColor)
	LUAPAIR(gdGetRandStr)
	LUAPAIR(gdDrawRect)
	LUAPAIR(gdDrawLine)
	LUAPAIR(gdFree)
	LUAPAIR(getImageDataRaw)
	LUAPAIR(gdWriteText)
	{NULL,NULL}
};

int luaopen_libimages(lua_State *L);

/*
union imageRawData{
	void * vData;
	unsigned char * cData;
};
*/
struct ImageData{
	unsigned char * raw;
	int size;
};

