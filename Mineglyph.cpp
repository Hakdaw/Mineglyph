// Copyright (C) 2023 Hakdaw. All Rights Reserved.

#include "Mineglyph.h"

#include "Logger.h"
#include "FontHandle.h"

#include <iostream>
#include <algorithm>


void printVersion() {
    std::cout << APP_NAME << std::endl << VERSION_NAME << " " << RELEASE_VERSION << " " << REVISE_DATE << std::endl << APP_COPYRIGHT << std::endl;
}

void printHelp() {
    std::cout << APP_NAME << std::endl << APP_COPYRIGHT << std::endl << std::endl << APP_DESCRIPT << std::endl << std::endl
        << "Usage:" << std::endl
        << "    mineglyph [Command] [Options...]" << std::endl << std::endl
        << "Command:" << std::endl
        << "    help        Display Help Page." << std::endl
        << "    version     Display Version." << std::endl
        << "    copyright   Copyright Infomation Statement." << std::endl
        << "    cv          Covert Font." << std::endl << std::endl
        << "Options:" << std::endl
        << "    -i, -in <FILE>          Input Font File (Absolute) Path.[Required]" << std::endl << std::endl
        << "    -o, -out <PATH>         Output Font Folder (Absolute) Path.[Required]" << std::endl << std::endl
        << "    -e, -encode <ENCODE>    Specify the Font Encoding.(Optional)" << std::endl
        << "        ENCODE: unicode, big5, gb2312 (Default: unicode)" << std::endl << std::endl
        << "    -n, -name <NAME>        Font Name of Output.(Optional)" << std::endl << std::endl
        << "    -r, -resolution <INT>   Specify the Height or Width of Character Bitmap.(Optional)" << std::endl
        << "        INT: Multiples of 16 in the range of 16 to 256. (Default: 64)" << std::endl << std::endl
        << "    -withpack               When the Processing Font is Complete, it is Packaged as a Minecraft Resource Package." << std::endl;
}

void printCopyright() {
    std::cout << APP_NAME << std::endl << std::endl << APP_COPYRIGHT << std::endl << TP_FREETYPE_COPYRIGHT << std::endl << std::endl
        << "Third-Party Libraries:" << std::endl << std::endl
        << TP_FREETYPE << std::endl
        << "    Version: " << TP_FREETYPE_VERSION << std::endl
        << "    Website: " << TP_FREETYPE_WEBSITE << std::endl
        << "    License: " << TP_FREETYPE_LICENSE << std::endl << std::endl
        << TP_LIBPNG << std::endl
        << "    Version: " << TP_LIBPNG_VERSION << std::endl
        << "    Website: " << TP_LIBPNG_WEBSITE << std::endl
        << "    License: " << TP_LIBPNG_LICENSE << std::endl << std::endl
        << TP_ZLIB << std::endl
        << "    Version: " << TP_ZLIB_VERSION << std::endl
        << "    Website: " << TP_ZLIB_WEBSITE << std::endl
        << "    License: " << TP_ZLIB_LICENSE << std::endl << std::endl
        << TP_ZIPLIB << std::endl
        << "    Version: " << TP_ZIPLIB_VERSION << std::endl
        << "    Website: " << TP_ZIPLIB_WEBSITE << std::endl
        << "    License: " << TP_ZIPLIB_LICENSE << std::endl;
}

void UnexCommand(std::string Command) {
    setTextColor(RED);
    std::cout << "Unknown Command: " << Command << std::endl << std::endl;
    cleanColor();
}

void UnexOption(std::string Option) {
    setTextColor(RED);
    std::cout << "Unknown Option: " << Option << std::endl << std::endl;
    cleanColor();
}

void OptionExcept(std::string Exception) {
    setTextColor(RED);
    std::cout << "Option Exception: " << Exception << std::endl << std::endl;
    cleanColor();
}

OPTIONS parseOptions(int argc, char* argv[]) {
    OPTIONS Options;
    bool isEntry = false;
    char **pEntry = (char**)"";
    for (int i = 2; i < argc; ++i) {
        std::string Option = argv[i];
        if (isEntry) {
            *pEntry = argv[i];
            isEntry = false;
            continue;
        }
        if (argv[i][0] != '-') {
            UnexOption(Option);
            Options.Error = true;
            break;
        }
        std::transform(Option.begin(), Option.end(), Option.begin(), ::tolower);
        switch (tolower(argv[i][1])) {
        case 'o':
            if (Option != "-o" && Option != "-out") {
                UnexOption(Option);
                Options.Error = true;
                break;
            }
            pEntry = &Options.OutPath;
            isEntry = true;
            break;
        case 'i':
            if (Option != "-i" && Option != "-in") {
                UnexOption(Option);
                Options.Error = true;
                break;
            }
            pEntry = &Options.InFile;
            isEntry = true;
            break;
        case 'n':
            if (Option != "-n" && Option != "-name") {
                UnexOption(Option);
                Options.Error = true;
                break;
            }
            pEntry = &Options.Rename;
            isEntry = true;
            break;
        case 'e':
            if (Option != "-e" && Option != "-encode") {
                UnexOption(Option);
                Options.Error = true;
                break;
            }
            pEntry = &Options.Encode;
            isEntry = true;
            break;
        case 'r':
            if (Option != "-r" && Option != "-resolution") {
                UnexOption(Option);
                Options.Error = true;
                break;
            }
            pEntry = &Options.Resolution;
            isEntry = true;
            break;
        case 'w':
            if (Option == "-withpack") {
                Options.Pack = true;
                break;
            }
            UnexOption(Option);
            Options.Error = true;
            break;
        default:
            UnexOption(Option);
            Options.Error = true;
            break;
        }
        if (Options.Error) {
            break;
        }
    }
    return Options;
}

int convertMain(int argc, char* argv[]) {
    OPTIONS Options = parseOptions(argc, argv);
    if (Options.Error) {
        OptionExcept("Error Options.");
        return 2;
    }
    if (Options.InFile == nullptr || Options.OutPath == nullptr) {
        OptionExcept("No Such Options.");
    }
    FontHandle FHandle{};
    FHandle.InFile = Options.InFile;
    FHandle.OutPath = Options.OutPath;
    FHandle.NewName = Options.Rename;
    FHandle.Pack = Options.Pack;
    std::string Encode = Options.Encode;
    FHandle.Resolution = atoi(Options.Resolution);
    std::transform(Encode.begin(), Encode.end(), Encode.begin(), ::tolower);
    switch (tolower(Options.Encode[0]))
    {
    case 'u':
        if (Encode == "unicode") {
            FHandle.Encode = UNIC;
            break;
        }
        setTextColor(YELLOW);
        std::cout << "Unknown Encoding: " << Options.Encode << ", UNICODE will be used." << std::endl << std::endl;
        cleanColor();
        break;
    case 'g':
        if (Encode == "gb2312" || Encode == "gb-2312") {
            FHandle.Encode = GB2312;
            break;
        }
        setTextColor(YELLOW);
        std::cout << "Unknown Encoding: " << Options.Encode << ", UNICODE will be used." << std::endl << std::endl;
        cleanColor();
        break;
    case 'b':
        if (Encode == "big5" || Encode == "big-5") {
            FHandle.Encode = BIG5;
            break;
        }
        setTextColor(YELLOW);
        std::cout << "Unknown Encoding: " << Options.Encode << ", UNICODE will be used." << std::endl << std::endl;
        cleanColor();
        break;
    default:
        setTextColor(YELLOW);
        std::cout << "Unknown Encoding: " << Options.Encode << ", UNICODE will be used." << std::endl << std::endl;
        cleanColor();
        break;
    }
    return coverHandle(FHandle);
}

int main(int argc, char* argv[])
{
    if (argc <= 1) {
        cleanColor();
        printHelp();
        return 0;
    }
    std::string Command = argv[1];
    switch (argv[1][0]) {
        case 'h':
            if (Command != "help") {
                UnexCommand(Command);
                return 1;
            }
            printHelp();
            break;
        case 'v':
            if (Command != "version") {
                UnexCommand(Command);
                return 1;
            }
            printVersion();
            break;
        case 'c':
            if (Command == "cv") {
                return convertMain(argc, argv);
            }
            else if (Command == "copyright") {
                printCopyright();
                return 0;
            }
            UnexCommand(Command);
            return 1;
            break;
        default:
            UnexCommand(Command);
            return 1;
            break;
    } 
    cleanColor();
    return 0;
}
