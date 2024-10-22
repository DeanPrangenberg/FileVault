#ifndef ROOTPROJECT_SAVEFILEOPERATIONS_H
#define ROOTPROJECT_SAVEFILEOPERATIONS_H

#pragma once
#include <vector>
#include <string>
#include <windows.h>
#include "../CppFileVaultMain//src/GlobalDefinitions.h"

using globalDefinitions::FileData;

const bool printDebug = false;

extern "C" {
__declspec(dllexport) void writeFileDataToJson(const FileData *fileDataList, size_t fileDataSize, const wchar_t *filePath);
__declspec(dllexport) bool findAndCompleteStruct(FileData& partialStruct, const std::wstring& filePath);
__declspec(dllexport) bool deleteStructFromJson(const FileData& targetStruct, const std::wstring& filePath);

}

#endif //ROOTPROJECT_SAVEFILEOPERATIONS_H