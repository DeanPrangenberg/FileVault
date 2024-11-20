#ifndef ROOTPROJECT_STRUCTUTILS_H
#define ROOTPROJECT_STRUCTUTILS_H

#include <filesystem>
#include "../GlobalDefinitions.h"

using globalDefinitions::AlgorithmType;
using globalDefinitions::FileData;

namespace fs = std::filesystem;

class StructUtils {
public:
    static globalDefinitions::FileData createFileDataStruct(const globalDefinitions::AlgorithmType &algorithmenType, const fs::path &originalFilePath);

  static wchar_t *ConvertWStringToWChar(const std::wstring &input);

private:
  static std::wstring AlgorithmTypeToString(globalDefinitions::AlgorithmType type);

};

#endif //ROOTPROJECT_STRUCTUTILS_H