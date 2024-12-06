//
// Created by prang on 30.10.2024.
//

#ifndef FILEVAULTROOT_HELPERUTILS_H
#define FILEVAULTROOT_HELPERUTILS_H

#include <filesystem>
#include <string>
#include "../GlobalDefinitions.h"

namespace fs = std::filesystem;

class HelperUtils {
public:
  void repairLostEncFileStructs(std::vector<fs::path> &directorys);
  void repairAllLostStruct();
  static wchar_t* ConvertWStringToWChar(const std::wstring& wstr);

  std::wstring ConvertStringToWString(const std::string &str);
  std::string ConvertWStringToString(const std::wstring &wstr);
  std::string ConvertVectorToString(const std::vector<unsigned char> &vec);
private:
  bool printDebug = true;
};


#endif //FILEVAULTROOT_HELPERUTILS_H
