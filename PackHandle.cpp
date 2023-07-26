// Copyright (C) 2023 Hakdaw. All Rights Reserved.

#include "PackHandle.h"

#include "Logger.h"

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <string>
#include <filesystem>
#include <fstream>
#include <ctime>
#include <vector>

#include <ziplib/ZipArchive.h>
#include <ziplib/ZipFile.h>

std::string getHex3(int Data, int MinBytes, bool Upper) {
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

std::string TargetUnitablePath = "assets/minecraft/textures/font/";
std::string TargetBinPath = "assets/minecraft/font/glyph_sizes.bin";

int HandlePack(std::string Path, std::string OutPath, std::string Name, std::string FontName) {
	std::string FilePath = OutPath;
	std::string WorkPath = Path;
	if (FilePath.ends_with('/') || FilePath.ends_with('\\')) {
		FilePath.pop_back();
	}
	if (WorkPath.ends_with('/') || WorkPath.ends_with('\\')) {
		WorkPath.pop_back();
	}
	std::string LogPath = FilePath + "/Mineglyph_Pack_Log_" + Name + ".log";

	Logger Logger("PackHandle", LogPath);

	FilePath = FilePath + "/" + Name + ".zip";
	if (!std::filesystem::exists(std::filesystem::path(OutPath))) {
		std::filesystem::create_directories(std::filesystem::path(OutPath));
	}
	std::string BinFile = WorkPath + "/glyph_sizes.bin";
	std::string FaceFile = WorkPath + "/unicode_page_00.png";
	struct stat buffer;

	if (stat(BinFile.c_str(), &buffer) != 0) {
		Logger.Log(FATAL, "No Such Input Path.");
		cleanColor();
		return 1;
	}
	if (stat(FilePath.c_str(), &buffer) == 0) {
		Logger.Log(WARN, "Target File " + FilePath + " Already Exists and will be Overwritten.");
		cleanColor();
		remove(FilePath.c_str());
	}

	Logger.Log(INFO, "Start Packing Resources Pack.\n");
	Logger.Log(PM, "Package File: " + FilePath);
	Logger.Log(PM, "Log File: " + LogPath);
	clock_t StartClock = clock();

	std::vector<std::string> PkgFailed(0);
	int PkgSuccess = 0;
	char processbar[50];
	memset(processbar, '\0', 50);
	
	ZipArchive::Ptr Archive = ZipFile::Open(FilePath);
	ZipArchiveEntry::Ptr BinEntry = Archive->CreateEntry(TargetBinPath);
	std::ifstream BinStream(BinFile, std::ios::binary);
	StoreMethod::Ptr BinComp = StoreMethod::Create();
	//BinComp->SetCompressionLevel(DeflateMethod::CompressionLevel::L1);
	bool bret = BinEntry->SetCompressionStream(
		BinStream,
		BinComp,
		ZipArchiveEntry::CompressionMode::Immediate
	);
	if (bret) {
		PkgSuccess++;
	}
	else {
		PkgFailed.push_back("Glyph_Sizes.bin");
	}
	BinStream.close();

	ZipArchiveEntry::Ptr FaceEntry = Archive->CreateEntry("pack.png");
	std::ifstream FaceStream(FaceFile, std::ios::binary);
	StoreMethod::Ptr FaceComp = StoreMethod::Create();
	bool fret = FaceEntry->SetCompressionStream(
		FaceStream,
		FaceComp,
		ZipArchiveEntry::CompressionMode::Immediate
	);
	if (fret) {
		PkgSuccess++;
	}
	else {
		PkgFailed.push_back("pack.png");
	}
	FaceStream.close();

	std::stringstream CfgStream;
	CfgStream << "{\"pack\":{\"pack_format\":3,\"description\" : \"" + FontName + " Mineglyph Font Pack, Copyright belongs to Original Author.\"}}";

	ZipArchiveEntry::Ptr CfgEntry = Archive->CreateEntry("pack.mcmeta");
	StoreMethod::Ptr CfgComp = StoreMethod::Create();
	bool cret = CfgEntry->SetCompressionStream(
		CfgStream,
		CfgComp,
		ZipArchiveEntry::CompressionMode::Immediate
	);
	if (cret) {
		PkgSuccess++;
	}
	else {
		PkgFailed.push_back("pack.png");
	}

	for (int i = 0; i <= 0xff; ++i) {
			std::string Code = getHex3(i, 1, false);

			float process = ((float)i / 256) * 100;
			memset(processbar, '>', ((int)process) >> 1);
			printf("\r[%-49s]%f%% Pos: 0x%s", processbar, process, Code);
			fflush(stdout);
			
			std::string File = WorkPath + "/unicode_page_" + Code + ".png";
			struct stat cbuffer;
			if (stat(File.c_str(), &cbuffer) != 0) {
				Logger.Log(PM, "\0");
				Logger.Log(WARN, "Missing Unicode Table: 0x" + Code);
				cleanColor();
				continue;
			}
			std::string TargetStr = TargetUnitablePath + "unicode_page_" + Code + ".png";
			ZipArchiveEntry::Ptr DstEntry = Archive->CreateEntry(TargetStr);
			std::ifstream DstStream(File, std::ios::binary);
			StoreMethod::Ptr DstComp = StoreMethod::Create();
			//DstComp->SetCompressionLevel(DeflateMethod::CompressionLevel::L1);
			bool ret = DstEntry->SetCompressionStream(
				DstStream,
				DstComp,
				ZipArchiveEntry::CompressionMode::Immediate
			);
			if (ret) {
				PkgSuccess++;
			}
			else {
				PkgFailed.push_back(Code);
			}
			DstStream.close();
	}

	ZipFile::SaveAndClose(Archive, FilePath);

	printf("\r[==================================================]100%% Packaging Commpleted.");
	fflush(stdout);
	std::cout << std::endl;
	setBgColor(GREEN);
	Logger.Log(INFO, "Done.");
	cleanColor();

	clock_t EndClock = clock();
	std::cout << std::endl << std::endl;
	Logger.Log(MESSAGE, "Execution Details:\nTakes " + std::to_string((double)(EndClock - StartClock) / CLOCKS_PER_SEC) + "secs.\n\n    Total: 259\n    Successful: " + std::to_string(PkgSuccess) + "\n    Failed: " + std::to_string(PkgFailed.size()) + "\n");
	
	if (PkgFailed.size() > 0) {
		Logger.Log(PM, "This Following Contents are Abnormal:\n");
		Logger.Log(PM, "Pos:");
		setTextColor(RED);
		for (auto abnid : PkgFailed) {
			Logger.Log(PM, abnid);
		}
		cleanColor();
	}

	return 0;
}