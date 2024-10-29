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
    int fileIDLength;
    const wchar_t* AlgorithmenType;
    const wchar_t* OriginalFilePath;
    const wchar_t* EncryptedFilePath;
    const wchar_t* DecryptedFilePath;
    unsigned char* Key;
    int keyLength;
    unsigned char* Iv;
    int ivLength;
  };

  // enums
  enum AlgorithmType {
    AES128,
    AES256
  };
}
#endif // GLOBALDEFINITIONS_H