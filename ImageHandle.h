// Copyright (C) 2023 Hakdaw. All Rights Reserved.

#pragma once

#include "Logger.h"

#include <string>
#include <freetype/freetype.h>

#define IMAGEHANDLE_H_

class BitmapChunk
{
public:
	int id;
	std::string name;
	FT_Bitmap Bitmap;
	int OffsetLeft;
	int OffsetTop;
	bool ErrorChunk = false;
private:
};

class ImageHandle
{
public:
	std::string path;
	int id;
	std::string name;
	BitmapChunk* Chunks = new BitmapChunk[256];
	int Resolution;
	Logger* Logger;
private:

};

int HandleImage(ImageHandle* handle);
int createPackFace(FT_Bitmap CharMap, std::string path, int LOffset, int TOffset);