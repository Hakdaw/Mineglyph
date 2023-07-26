// Copyright (C) 2023 Hakdaw. All Rights Reserved.
#include "Logger.h"

#include <string>
#include <fstream>

void print(char* Text)
{
	std::cout << Text << std::endl;
}

void setColor(int Color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Color);
}

void setTextColor(COLORS Color)
{
	int WColor = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;
	switch (Color)
	{
	case RED:
		WColor = FOREGROUND_RED;
		break;
	case GREEN:
		WColor = FOREGROUND_GREEN;
		break;
	case BLUE:
		WColor = FOREGROUND_BLUE;
		break;
	case WHITE:
		break;
	case YELLOW:
		WColor = FOREGROUND_GREEN | FOREGROUND_RED;
		break;
	case INTENSITY:
		WColor = FOREGROUND_INTENSITY;
		break;
	default:
		break;
	}
	setColor(WColor);
}

void setTextColors(COLORS Color[])
{
	int ColorNum = sizeof(Color);
	if (ColorNum < 1) { return; }
	int WColor;
	switch (Color[0]) {
	case RED:
		WColor = FOREGROUND_RED;
		break;
	case GREEN:
		WColor = FOREGROUND_GREEN;
		break;
	case BLUE:
		WColor = FOREGROUND_BLUE;
		break;
	case WHITE:
		WColor = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;
		break;
	case YELLOW:
		WColor = FOREGROUND_GREEN | FOREGROUND_RED;
		break;
	case INTENSITY:
		WColor = FOREGROUND_INTENSITY;
		break;
	default:
		WColor = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;
		break;
	}
	if (ColorNum >= 2) {
		for (int i = 1; i < ColorNum; ++i) {
			switch (Color[i]) {
			case RED:
				WColor |= FOREGROUND_RED;
				break;
			case GREEN:
				WColor |= FOREGROUND_GREEN;
				break;
			case BLUE:
				WColor |= FOREGROUND_BLUE;
				break;
			case WHITE:
				WColor |= FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;
				break;
			case YELLOW:
				WColor |= FOREGROUND_GREEN | FOREGROUND_RED;
				break;
			case INTENSITY:
				WColor |= FOREGROUND_INTENSITY;
				break;
			default:
				WColor |= FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;
				break;
			}
		}
	}
	setColor(WColor);
}

void setBgColor(COLORS Color)
{
	int WColor = BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN;
	switch (Color)
	{
	case RED:
		WColor = BACKGROUND_RED;
		break;
	case GREEN:
		WColor = BACKGROUND_GREEN;
		break;
	case BLUE:
		WColor = BACKGROUND_BLUE;
		break;
	case WHITE:
		break;
	case YELLOW:
		WColor = BACKGROUND_GREEN | BACKGROUND_RED;
		break;
	case INTENSITY:
		WColor = BACKGROUND_INTENSITY;
		break;
	default:
		break;
	}
	setColor(WColor);
}

void setBgColors(COLORS Color[])
{
	int ColorNum = sizeof(Color);
	if (ColorNum < 1) { return; }
	int WColor;
	switch (Color[0]) {
	case RED:
		WColor = BACKGROUND_RED;
		break;
	case GREEN:
		WColor = BACKGROUND_GREEN;
		break;
	case BLUE:
		WColor = BACKGROUND_BLUE;
		break;
	case WHITE:
		WColor = BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN;
		break;
	case YELLOW:
		WColor = BACKGROUND_GREEN | BACKGROUND_RED;
		break;
	case INTENSITY:
		WColor = BACKGROUND_INTENSITY;
		break;
	default:
		WColor = BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN;
		break;
	}
	if (ColorNum >= 2) {
		for (int i = 1; i < ColorNum; ++i) {
			switch (Color[i]) {
			case RED:
				WColor |= BACKGROUND_RED;
				break;
			case GREEN:
				WColor |= BACKGROUND_GREEN;
				break;
			case BLUE:
				WColor |= BACKGROUND_BLUE;
				break;
			case WHITE:
				WColor |= BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN;
				break;
			case YELLOW:
				WColor |= BACKGROUND_GREEN | BACKGROUND_RED;
				break;
			case INTENSITY:
				WColor |= BACKGROUND_INTENSITY;
				break;
			default:
				WColor |= BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN;
				break;
			}
		}
	}
	setColor(WColor);
}

void cleanColor()
{
	setColor(7);
}


void Logger::Log(int Level, std::string Text)
{
	switch (Level)
	{
	case INFO:
		std::cout << "[I][" << Logger::Module << "]" << Text << std::endl;
		LogStream << "[I][" << Logger::Module << "]" << Text << std::endl;
		cleanColor();
		break;
	case MESSAGE:
		std::cout << "[M][" << Logger::Module << "]" << Text << std::endl;
		LogStream << "[M][" << Logger::Module << "]" << Text << std::endl;
		cleanColor();
		break;
	case WARN:
		setBgColor(YELLOW);
		std::cout << "[W][" << Logger::Module << "]" << Text << std::endl;
		LogStream << "[W][" << Logger::Module << "]" << Text << std::endl;
		cleanColor();
		break;
	case ERR:
		setBgColor(RED);
		std::cout << "[E][" << Logger::Module << "]" << Text << std::endl;
		LogStream << "[E][" << Logger::Module << "]" << Text << std::endl;
		cleanColor();
		break;
	case FATAL:
		setColor(BACKGROUND_RED | BACKGROUND_INTENSITY);
		std::cout << "[F][" << Logger::Module << "]" << Text << std::endl;
		LogStream << "[F][" << Logger::Module << "]" << Text << std::endl;
		cleanColor();
		break;
	case DEBUG:
		setTextColor(INTENSITY);
		std::cout << "[D][" << Logger::Module << "]" << Text << std::endl;
		LogStream << "[D][" << Logger::Module << "]" << Text << std::endl;
		cleanColor();
		break;
	case PM:
		std::cout << Text << std::endl;
		LogStream << Text << std::endl;
		cleanColor();
		break;
	default:
		break;
	}
}
