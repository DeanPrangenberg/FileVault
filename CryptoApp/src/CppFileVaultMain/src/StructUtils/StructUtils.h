#ifndef ROOTPROJECT_STRUCTUTILS_H
#define ROOTPROJECT_STRUCTUTILS_H

#include <filesystem>
#include "../GlobalDefinitions.h"
#include "../../../shared/FileData.h"

using globalDefinitions::AlgorithmType;

namespace fs = std::filesystem;

class StructUtils {
public:
    static FileData createFileDataStruct(const globalDefinitions::AlgorithmType &algorithmenType, const fs::path &originalFilePath);

  static wchar_t *ConvertWStringToWChar(const std::wstring &input);

private:
  static std::wstring AlgorithmTypeToString(globalDefinitions::AlgorithmType type);

};

#endif //ROOTPROJECT_STRUCTUTILS_H