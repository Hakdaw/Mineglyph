// Copyright (C) 2023 Hakdaw. All Rights Reserved.

#include "ImageHandle.h"

#include "Logger.h"

#include <string>
#include <sstream>
#include <wchar.h>
#include <algorithm>
#include <libpng/png.h>

#include <iostream>
#include <vector>

png_byte Alpha[1] = { 0x00 };

std::string getHex2(int Data, int MinBytes, bool Upper) {
	std::stringstream StrStream;
	std::string RetHex;
	if (Upper) {
		StrStream << std::setiosflags(std::ios::uppercase) << std::hex << Data;
	}
	else {
		StrStream << std::hex << Data;
	}

	StrStream >> RetHex;
	int NeedCP = (MinBytes << 1) - RetHex.length();
	if (NeedCP > 0)
	{
		RetHex.insert(0, NeedCP, '0');
	}
	return RetHex;
}

int HandleImage(ImageHandle* handle) {
	std::string OutFile = handle->path + "/unicode_page_" + handle->name + ".png";
	//std::cout << OutFile << " " << std::hex << handle->id;

	int Size = handle->Resolution << 4;
	int bitSize = Size >> 3;
	int bitResolution = handle->Resolution >> 3;

	FILE* ImageFile;
	errno_t ferr = fopen_s(&ImageFile, OutFile.c_str(), "wb");
	if (ferr) {
		handle->Logger->Log(ERR, "[ImageHandle]Cannot Write Image: " + OutFile);
		return 1;
	}
	if (!ImageFile) {
		handle->Logger->Log(ERR, "[ImageHandle]Cannot Write Image: " + OutFile);
		return 1;
	}
	png_structp ImgStruct = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (ImgStruct == NULL) {
		handle->Logger->Log(ERR, "[ImageHandle]Cannot Create PNG Struct.");
		return 1;
	}
	png_infop ImgInfo = png_create_info_struct(ImgStruct);
	if (ImgInfo == NULL) {
		handle->Logger->Log(ERR, "[ImageHandle]Cannot Create PNG Struct.");
		return 1;
	}

	png_bytepp ImgMatrix = (png_bytepp)png_malloc(ImgStruct, Size * sizeof(png_bytep));
	for (int i = 0; i < Size; ++i) {
		ImgMatrix[i] = (png_bytep)png_malloc(ImgStruct, bitSize);
		memset(ImgMatrix[i], 0x0, (sizeof(png_byte)) * bitSize);
	}

	for (int i = 0; i < 16; ++i) {
		for (int j = 0; j < 16; ++j) {
			BitmapChunk* chunk = &handle->Chunks[(i << 4) + j];
			if (chunk->ErrorChunk) {
				handle->Logger->Log(WARN, "[ImageHandle]Error Chunk: 0x" + getHex2((i << 4) + j, 1, false));
				continue;
			}
			int OffsetL = chunk->OffsetLeft >> 3;
			int CoverSize = handle->Resolution - chunk->OffsetTop - (handle->Resolution >> 2);
			CoverSize = (CoverSize < 0) ? 0 : CoverSize;
			for (int k = 0; k < handle->Resolution; ++k) {
				if (k < CoverSize || k >= (chunk->Bitmap.rows + CoverSize)) {
					continue;
				}
				int RowIndex = (i * handle->Resolution) + k;
				int BufIndex = (k - CoverSize) * chunk->Bitmap.pitch;
				for (int l = 0; l < chunk->Bitmap.pitch + OffsetL; ++l) {
					
					if (l < OffsetL) {
						continue;
					}
					ImgMatrix[RowIndex][(j * bitResolution) + l] = chunk->Bitmap.buffer[BufIndex + l - OffsetL];
				}
			}
		}
	}
	png_set_compression_level(ImgStruct, 9);
	png_set_compression_window_bits(ImgStruct,	14);
	png_const_color_16p BaseColor{};
	png_set_tRNS(ImgStruct, ImgInfo, Alpha, 1, BaseColor);
	png_color Palette[2]{0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	png_set_PLTE(ImgStruct, ImgInfo, Palette, 2);
	png_init_io(ImgStruct, ImageFile);
	png_set_IHDR(ImgStruct, ImgInfo, Size, Size, 1, PNG_COLOR_TYPE_PALETTE, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(ImgStruct, ImgInfo);
	
	png_write_rows(ImgStruct, ImgMatrix, Size);
	png_write_end(ImgStruct, NULL);
	
	png_free(ImgStruct, 0);
	free(ImgMatrix);
	fclose(ImageFile);
	return 0;
}

int createPackFace(FT_Bitmap CharMap, std::string path, int LOffset, int TOffset) {
	int Resolution = (std::max)(CharMap.width, CharMap.rows);
	std::string OutFile = path + "/pack.png";
	FILE* ImageFile;
	errno_t ferr = fopen_s(&ImageFile, OutFile.c_str(), "wb");
	if (ferr) {
		return 1;
	}
	if (!ImageFile) {
		return 1;
	}
	png_structp ImgStruct = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (ImgStruct == NULL) {
		return 1;
	}
	png_infop ImgInfo = png_create_info_struct(ImgStruct);
	if (ImgInfo == NULL) {
		return 1;
	}

	png_bytepp ImgMatrix = (png_bytepp)png_malloc(ImgStruct, Resolution * sizeof(png_bytep));
	for (int i = 0; i < Resolution; ++i) {
		ImgMatrix[i] = (png_bytep)png_malloc(ImgStruct, Resolution >> 3);
		memset(ImgMatrix[i], 0x0, (sizeof(png_byte)) * Resolution >> 3);
	}

	for (int i = 0; i < Resolution; ++i) {
		if (i < TOffset) {
			continue;
		}
		for (int j = 0; j < (Resolution >> 3); ++j) {
			if ((j << 3) < LOffset) {
				continue;
			}
			if ((j << 3) >= (LOffset + CharMap.width)) {
				break;
			}
			ImgMatrix[i][j] = CharMap.buffer[(i - TOffset) * CharMap.pitch + (j - (LOffset >> 3))];
		}
		if (i >= TOffset + CharMap.rows) {
			break;
		}
	}

	png_init_io(ImgStruct, ImageFile);
	
	png_set_IHDR(ImgStruct, ImgInfo, Resolution, Resolution, 1, PNG_COLOR_TYPE_PALETTE, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(ImgStruct, ImgInfo);

	png_write_rows(ImgStruct, ImgMatrix, Resolution);
	png_write_end(ImgStruct, NULL);

	png_free(ImgStruct, 0);
	if (ImgStruct == NULL) {
		return 1;
	}
}