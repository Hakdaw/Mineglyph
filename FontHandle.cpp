// Copyright (C) 2023 Hakdaw. All Rights Reserved.
#include "FontHandle.h"

#include "Logger.h"
#include "ImageHandle.h"
#include "PackHandle.h"
#include <iostream>
#include <ctime>
#include <sstream>
#include <string>
#include <wchar.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <filesystem>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftpfr.h>
#include <freetype/ftadvanc.h>

FT_Library FTLib;

FT_Face createFontFace(char* File, FT_Encoding Encode) {
    FT_Face FTFace;
    if (FT_New_Face(FTLib, File, 0, &FTFace)) { return nullptr; }
    if (FT_Select_Charmap(FTFace, Encode)) { return nullptr; }
    return FTFace;
}

/*
wchar_t CritChar[8] = { L'j', L'q', L'p', L'y', L'g', L'Q' };
wchar_t AdjustWhiteList[74] = {L'\'', L'"', L':', L';', L'*', L'-', L'+', L'=', L'~', L'`', L'^', L'(', L')', L'{', L'}', L'[', L']',L'<', L'>', L'¡°', L'¡±',
                               L'¡®', L'¡¤', L'¡¯', L'£»', L'£º',
                               0x00a0, 0x00a1, 0x00a2, 0x00a3, 0x00a4, 0x00a5, 0x00a6, 0x00a7, 0x00a8, 0x00a9, 0x00aa, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x00af,
                               0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x00b4, 0x00b5, 0x00b6, 0x00b7, 0x00b8, 0x00b9, 0x00ba, 0x00bb, 0x00bc, 0x00bd, 0x00be, 0x00bf,
                               0x0300, 0x0301, 0x0302, 0x0303, 0x0304, 0x0305, 0x0306, 0x0307, 0x0308, 0x0309, 0x030a, 0x030b, 0x030c, 0x030d, 0x030e, 0x030f,
};
*/

std::string getHex(int Data, int MinBytes, bool Upper) {
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

int coverHandle(FontHandle FHandle) {
    struct stat buffer;
    char* InFile = FHandle.InFile;
    char* OutPath = FHandle.OutPath;
    if (stat(InFile, &buffer) != 0) {
        setTextColor(RED);
        print((char*)"No Such Input File.");
        cleanColor();
        return 1;
    }
    std::string OutPathStr = OutPath;
    std::string InFileStr = InFile;
    std::string Name;
    
    if (OutPathStr.ends_with('/') || OutPathStr.ends_with('\\') ) {
        OutPathStr.pop_back();
    }
    if (FHandle.NewName == nullptr) {
        int StartIndex = InFileStr.find_last_of("/\\") + 1;
        int EndIndex = InFileStr.substr(StartIndex, InFileStr.length() - StartIndex).find_first_of("([@+_-.");
        Name = InFileStr.substr(StartIndex, EndIndex);
    }
    else {
        Name = FHandle.NewName;
    }
    std::string LogPath = OutPathStr + "/Mineglyph_Log_" + Name + ".log";
    std::string OutWorkPath = OutPathStr + "/" + Name;
    std::string BinPath = OutWorkPath + "/glyph_sizes.bin";
    std::string Module = "FontHandle";
    Logger Logger(Module, LogPath);

    Logger.Log(INFO, "Start Processing Font.\n");
    Logger.Log(DEBUG, " \n    Name: " + Name + "\n    Font: " + InFileStr + "\n    OutPath: " + OutWorkPath + "\n    Log: " + LogPath);
    clock_t StartClock = clock();
    if (!std::filesystem::exists(std::filesystem::path(OutWorkPath))) {
        std::filesystem::create_directories(std::filesystem::path(OutWorkPath));
    }

    std::ofstream BinFile(BinPath.c_str(), std::ios::binary | std::ios::out);

    unsigned char* GlyphSizesBin = new (unsigned char[65536]);
    memset(GlyphSizesBin, 0x0f, 65536);

    FT_Error FTErr = FT_Init_FreeType(&FTLib);
    if (FTErr) {
        Logger.Log(FATAL, "Initialization Failure.");
        return 1;
    }
    FT_Encoding Encode = FT_ENCODING_UNICODE;
    switch (FHandle.Encode)
    {
    case GB2312:
        Encode = FT_ENCODING_GB2312;
        break;
    case BIG5:
        Encode = FT_ENCODING_BIG5;
        break;
    case UNIC:
        Encode = FT_ENCODING_UNICODE;
        break;
    default:
        break;
    }
    FT_Face FTFace = createFontFace(InFile, Encode);
    if (FTFace == nullptr) {
        Logger.Log(FATAL, "Font Face Creation Failure.");
        return 1;
    }
    if (FHandle.Resolution < 16) {
        Logger.Log(WARN, "Resolution: " + std::to_string(FHandle.Resolution) + " is less than the minimum value of 16, reset to 16.");
        FHandle.Resolution = 16;
    }
    if (FHandle.Resolution > 256) {
        Logger.Log(WARN, "Resolution: " + std::to_string(FHandle.Resolution) + " is greater than the maximum value of 256, reset to 256.");
        FHandle.Resolution = 256;
    }
    if (FHandle.Resolution % 16 != 0) {
        int Reset = (FHandle.Resolution >> 4) * 16;
        Logger.Log(WARN, "Resolution: " + std::to_string(FHandle.Resolution) + " is not a Multiple of 16, reset to " + std::to_string(Reset) + ".");
        FHandle.Resolution = Reset;
    }
    FT_Error SetSizeErr = FT_Set_Pixel_Sizes(FTFace, FHandle.Resolution, FHandle.Resolution);
    if (SetSizeErr) {
        Logger.Log(FATAL, "Failed to Set Pixel Size.");
        return 1;
    }

    /*
    FT_Vector MaxOffset = {0, 0};

    for (int i = 0; i < 8; ++i) {
        wchar_t Char = CritChar[i];
        FT_Load_Char(FTFace, Char, FT_LOAD_DEFAULT | FT_LOAD_NO_AUTOHINT);
        int YOffset = -(((int)FTFace->glyph->bitmap.rows - (int)FTFace->glyph->bitmap_top) << 6);
        MaxOffset.y = (YOffset < MaxOffset.y) ? YOffset : MaxOffset.y;
    }
    */

    int SizeScale = FHandle.Resolution >> 4;

    std::vector<std::string> ImgFailed(0);
    int ImgSuccess = 0;
    std::vector<std::string> CharFailed(0);
    int CharSuccess = 0;
    char processbar[50];
    memset(processbar, '\0', 50);
    for (int i = 0x00; i <= 0xff; ++i) {
        ImageHandle ImgHandle;
        ImgHandle.path = OutWorkPath;
        ImgHandle.Resolution = FHandle.Resolution;
        ImgHandle.id = i;
        ImgHandle.name = getHex(i, 1, false);
        ImgHandle.Logger = &Logger;
        bool isError = false;
        for (int j = 0x00; j <= 0xff; ++j) {
            wchar_t Charcode = (i << 8) + j;

            float process = ((float)Charcode / 65536) * 100;
            memset(processbar, '>', ((int)process) >> 1);
            printf("\r[%-49s]%f%% Character: 0x%s", processbar, process, getHex(Charcode, 2, false));
            fflush(stdout);

            FT_UInt GIndex = FT_Get_Char_Index(FTFace, Charcode);
            FT_Error LoadErr = FT_Load_Glyph(FTFace, GIndex, FT_LOAD_DEFAULT | FT_LOAD_NO_AUTOHINT | FT_OUTLINE_HIGH_PRECISION);
            if (LoadErr) {
                Logger.Log(PM, "\0");
                Logger.Log(ERR, "Load Font Glyph Error. Character: 0x" + getHex(Charcode, 2, false));
                isError = true;
                ImgHandle.Chunks[j].ErrorChunk = true;
                CharFailed.push_back(getHex(Charcode, 2, false));
                continue;
            }
            FT_Error RenderErr = FT_Render_Glyph(FTFace->glyph, FT_RENDER_MODE_MONO);
            if (RenderErr) {
                Logger.Log(PM, "\0");
                Logger.Log(ERR, "Render Font Glyph Error. Character: 0x" + getHex(Charcode, 2, false));
                isError = true;
                ImgHandle.Chunks[j].ErrorChunk = true;
                CharFailed.push_back(getHex(Charcode, 2, false));
                continue;
            }
            FT_Glyph Glyph;
            FT_Error GlyphErr = FT_Get_Glyph(FTFace->glyph, &Glyph);
            if (GlyphErr) {
                Logger.Log(PM, "\0");
                Logger.Log(ERR, "Reset Font Glyph Error. Character: 0x" + getHex(Charcode, 2, false));
                isError = true;
                ImgHandle.Chunks[j].ErrorChunk = true;
                CharFailed.push_back(getHex(Charcode, 2, false));
                continue;
            }
            FT_Vector Offset = {0, 0};
            //Offset.x = -((int)FTFace->glyph->bitmap_left << 6);
            //Offset.y = -(((int)FTFace->glyph->bitmap.rows - (int)FTFace->glyph->bitmap_top) << 6);

            /*
            if (((int)FTFace->glyph->bitmap.rows - (int)FTFace->glyph->bitmap_top) > 0) {
                FT_Glyph_Transform(Glyph, 0, &Offset);
            }
            */

            FT_Error BitmapErr = FT_Glyph_To_Bitmap(&Glyph, FT_RENDER_MODE_MONO, 0, true);
            if (BitmapErr) {
                Logger.Log(PM, "\0");
                Logger.Log(ERR, "Load Font Bitmap Error. Character: 0x" + getHex(Charcode, 2, false));
                isError = true;
                ImgHandle.Chunks[j].ErrorChunk = true;
                CharFailed.push_back(getHex(Charcode, 2, false));
                continue;
            }
            int LOffset = ((FT_BitmapGlyph)Glyph)->left;
            int TOffset = ((FT_BitmapGlyph)Glyph)->top;
            FT_BitmapGlyph BitmapGlyph = (FT_BitmapGlyph)Glyph;
            ImgHandle.Chunks[j].OffsetLeft = LOffset;
            ImgHandle.Chunks[j].OffsetTop = TOffset;
            
            ImgHandle.Chunks[j].Bitmap = BitmapGlyph->bitmap;
            ImgHandle.Chunks[j].id = j;
            ImgHandle.Chunks[j].name = getHex(j, 1, false);
            ImgHandle.Chunks[j].ErrorChunk = false;
            int BinStart = (LOffset <= SizeScale) ? 0 : (LOffset / SizeScale);
            int BinEnd = ceil((float)(LOffset + BitmapGlyph->bitmap.width) / (float)SizeScale);
            BinEnd = (BinEnd > 15) ? 15 : BinEnd;
            unsigned char BinData = (BinStart << 4) + BinEnd;
            GlyphSizesBin[(int)Charcode] = BinData;
            CharSuccess++;
        }
        int Ret = HandleImage(&ImgHandle);
        if (Ret == 0) {
            ImgSuccess++;
        }
        else {
            ImgFailed.push_back(getHex(i, 1, false));
        }
    }

    BinFile << GlyphSizesBin;
    BinFile.close();
    free(GlyphSizesBin);

    printf("\r[==================================================]100%% All Processing is Commplete.");
    fflush(stdout);

    clock_t EndClock = clock();

    std::cout << std::endl << std::endl;
    setBgColor(GREEN);
    Logger.Log(INFO, "Done.");
    cleanColor();
    std::cout << std::endl << std::endl;
    Logger.Log(MESSAGE, "Execution Details:\nTakes " + std::to_string((double)(EndClock - StartClock) / CLOCKS_PER_SEC) + "secs.\n\nImages:\n    Total: 256\n    Successful: " + std::to_string(ImgSuccess) + "\n    Failed: " + std::to_string(ImgFailed.size())
        + "\n\nCharacters:\n    Total: 65536\n    Successful: " + std::to_string(CharSuccess) + "\n    Failed: " + std::to_string(CharFailed.size()) + "\n");
    
    if (ImgFailed.size() > 0 || CharFailed.size() > 0 ) {
        Logger.Log(PM, "This Following Contents are Abnormal:\n");
        Logger.Log(PM, "Images:");
        setTextColor(RED);
        for (auto abnid : ImgFailed) {
            Logger.Log(PM, "unicode_page_" + abnid);
        }
        cleanColor();
        Logger.Log(PM, "Characters:");
        setTextColor(RED);
        for (auto abnid : CharFailed) {
            Logger.Log(PM, "0x" + abnid);
        }
        cleanColor();
    }

    if (FHandle.Pack) {
        Logger.Log(INFO, "Wait Packing...");
        
        HandlePack(OutWorkPath, OutPathStr, Name, FTFace->family_name);
    }

    return 0;
}