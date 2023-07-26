// Copyright (C) 2023 Hakdaw. All Rights Reserved.
#pragma once

#include <iostream>
#include <Windows.h>
#include <string>
#include <filesystem>
#include <fstream>

#define LOGGER_H_

enum COLORS {
	RED,
	GREEN,
	BLUE,
	WHITE,
	YELLOW,
	INTENSITY
};

enum LogLevel {
	INFO,
	MESSAGE,
	WARN,
	ERR,
	FATAL,
	DEBUG,
	PM
};


void print(char* Text);
void setTextColor(COLORS Color);
void setTextColors(COLORS Color[]);
void setBgColor(COLORS Color);
void setBgColors(COLORS Color[]);
void cleanColor();
void setColor(int Color);

class Logger {
public:
	std::string Module;
	std::string LogFile;
	std::ofstream LogStream;
	Logger() {};
	Logger(std::string Module_, std::string LogFile_) {
		Module = Module_;
		LogFile = LogFile_;
		LogStream = std::ofstream(LogFile_, std::ios::out);
	}
	void Log(int Level, std::string Text);
private:
};