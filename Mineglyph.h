// Copyright (C) 2023 Hakdaw. All Rights Reserved.
#pragma once

#include <iostream>
#include <string>
#include "FontHandle.h"

#define APP_NAME "Mineglyph"
#define APP_DESCRIPT "This is a Tool that Converts TTF or OTF Fonts into Minecraft Unicode Bitmap Font."
#define APP_COPYRIGHT "Copyright (c) 2023 Hakdaw. All Rights Reserved."
#define RELEASE_VERSION "RELEASE"
#define MAJOR_VERSION 1
#define MINIOR_VERSION 0
#define REVISE_VERSION 0
#define VERSION_NUM ((MAJOR_VERSION*100)+(MINIOR_VERSION*10)+REVISE_VERSION)
#define REVISE_DATE "2023/1/16 UTC+8"
std::string VERSION_NAME = std::to_string(MAJOR_VERSION) + "." + std::to_string(MINIOR_VERSION) + "." + std::to_string(REVISE_VERSION);

#define TP_FREETYPE "FreeType"
#define TP_FREETYPE_VERSION "2.12.1"
#define TP_FREETYPE_COPYRIGHT "Portions of this software are copyright (c) 2022 The FreeType Project(www.freetype.org).All rights reserved."
#define TP_FREETYPE_WEBSITE "www.freetype.org"
#define TP_FREETYPE_LICENSE "freetype.org/license.html"

#define TP_LIBPNG "LibPNG"
#define TP_LIBPNG_VERSION "1.6.39"
#define TP_LIBPNG_WEBSITE "www.libpng.org"
#define TP_LIBPNG_LICENSE "www.libpng.org/pub/png/src/libpng-LICENSE.txt"

#define TP_ZLIB "ZLib"
#define TP_ZLIB_VERSION "1.2.11"
#define TP_ZLIB_WEBSITE "www.zlib.net"
#define TP_ZLIB_LICENSE "www.zlib.net/zlib_license.html"

#define TP_ZIPLIB "ZipLib"
#define TP_ZIPLIB_VERSION "176e4b6"
#define TP_ZIPLIB_WEBSITE "bitbucket.org/wbenny/ziplib/branches/"
#define TP_ZIPLIB_LICENSE "bitbucket.org/wbenny/ziplib/src/master/Licence.txt"

class OPTIONS {
public:
	char* InFile = nullptr;
	char* OutPath = nullptr;
	char* Encode = (char*)"UNICODE";
	char* Rename = nullptr;
	char* Resolution = (char*)"64";
	bool Pack = false;
	bool Error = false;
private:

};