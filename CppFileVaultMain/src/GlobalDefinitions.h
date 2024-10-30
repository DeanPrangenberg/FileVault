#ifndef GLOBALDEFINITIONS_H
#define GLOBALDEFINITIONS_H

#include <string>
#include <vector>

namespace globalDefinitions {
  // variables
  extern const std::wstring encFileSuffix;
  extern const wchar_t* jsonFileName;
  extern const std::string markIdentifier;

  // structs
  struct FileData {
    unsigned char* FileID;
    size_t fileIDLength;
    const wchar_t* AlgorithmenType;
    const wchar_t* OriginalFilePath;
    const wchar_t* EncryptedFilePath;
    const wchar_t* DecryptedFilePath;
    unsigned char* Key;
    size_t keyLength;
    unsigned char* Iv;
    size_t ivLength;
  };

  // enums
  enum AlgorithmType {
    AES128,
    AES256
  };
}
#endif // GLOBALDEFINITIONS_H