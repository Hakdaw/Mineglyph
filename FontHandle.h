// Copyright (C) 2023 Hakdaw. All Rights Reserved.
#pragma once

#include <iostream>
#include <string>

#define FONTHANDLE_H_

enum ENCODE {
	UNIC,
	BIG5,
	GB2312
};

class FontHandle
{
public:
	char* NewName;
	char* InFile;
	char* OutPath;
	int Resolution;
	int Encode;
	bool Pack = false;
private:

};

int coverHandle(FontHandle FHhandle);