#ifndef GLOBALDEFINITIONS_H
#define GLOBALDEFINITIONS_H

#include <string>
#include <vector>
#include <bitset>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace globalDefinitions {
  // variables
  extern const std::wstring encFileSuffix;
  extern const std::string markIdentifier;

  // structs
  struct FileData {
    unsigned char *FileID = nullptr;
    size_t fileIDLength = 0;
    unsigned char *EncryptionID = nullptr;
    size_t EncryptionIDLength = 0;
    unsigned char *LastUpdateID = nullptr;
    size_t LastUpdateIDLength = 0;
    wchar_t *AlgorithmenType = nullptr;
    wchar_t *OriginalFilePath = nullptr;
    wchar_t *EncryptedFilePath = nullptr;
    wchar_t *DecryptedFilePath = nullptr;
    unsigned char *Key = nullptr;
    size_t keyLength = 0;
    unsigned char *Iv = nullptr;
    size_t ivLength = 0;

    bool operator==(const FileData &other) const {
      return
          fileIDLength == other.fileIDLength
          && EncryptionIDLength == other.EncryptionIDLength
          && LastUpdateIDLength == other.LastUpdateIDLength
          && keyLength == other.keyLength
          && ivLength == other.ivLength
          && std::equal(FileID, FileID + fileIDLength, other.FileID)
          && std::equal(EncryptionID, EncryptionID + EncryptionIDLength, other.EncryptionID)
          && std::equal(LastUpdateID, LastUpdateID + LastUpdateIDLength, other.LastUpdateID)
          && wcscmp(AlgorithmenType, other.AlgorithmenType) == 0
          && wcscmp(OriginalFilePath, other.OriginalFilePath) == 0
          && wcscmp(EncryptedFilePath, other.EncryptedFilePath) == 0
          && wcscmp(DecryptedFilePath, other.DecryptedFilePath) == 0;
    }
  };

  inline void cleanupFileData(FileData &data) {
    delete[] data.FileID;
    delete[] data.EncryptionID;
    delete[] data.LastUpdateID;
    delete[] data.AlgorithmenType;
    delete[] data.OriginalFilePath;
    delete[] data.EncryptedFilePath;
    delete[] data.DecryptedFilePath;
    delete[] data.Key;
    delete[] data.Iv;

    data.FileID = nullptr;
    data.fileIDLength = 0;
    data.EncryptionID = nullptr;
    data.EncryptionIDLength = 0;
    data.LastUpdateID = nullptr;
    data.LastUpdateIDLength = 0;
    data.AlgorithmenType = nullptr;
    data.OriginalFilePath = nullptr;
    data.EncryptedFilePath = nullptr;
    data.DecryptedFilePath = nullptr;
    data.Key = nullptr;
    data.keyLength = 0;
    data.Iv = nullptr;
    data.ivLength = 0;
  }

  inline std::string toHexString(const unsigned char *pUChar, size_t length) {
    if (pUChar == nullptr) {
      throw std::invalid_argument("Null pointer passed to toHexString");
    }

    std::ostringstream oss;
    for (size_t i = 0; i < length; ++i) {
      oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(pUChar[i]);
    }
    return oss.str();
  }

  inline void debugFileData(const FileData &data) {
    std::cout << "-------------------------------------------------------------" << std::endl;
    std::cout << "FileData Debug: " << std::endl;

    try {
      std::cout << "FileID: "
                << (data.FileID != nullptr ? toHexString(data.FileID, data.fileIDLength) : "null") << std::endl;
    } catch (const std::exception &e) {
      std::cerr << "Error converting FileID to hex string: " << e.what() << std::endl;
    }

    std::wcout << L"OriginalFilePath: "
               << (data.OriginalFilePath != nullptr ? data.OriginalFilePath : L"null") << std::endl;
    std::wcout << L"AlgorithmenType: "
               << (data.AlgorithmenType != nullptr ? data.AlgorithmenType : L"null") << std::endl;
    std::wcout << L"EncryptedFilePath: "
               << (data.EncryptedFilePath != nullptr ? data.EncryptedFilePath : L"null") << std::endl;
    std::wcout << L"DecryptedFilePath: "
               << (data.DecryptedFilePath != nullptr ? data.DecryptedFilePath : L"null") << std::endl;

    try {
      std::cout << "Key: "
                << (data.Key != nullptr ? toHexString(data.Key, data.keyLength) : "null") << std::endl;
    } catch (const std::exception &e) {
      std::cerr << "Error converting Key to hex string: " << e.what() << std::endl;
    }

    try {
      std::cout << "Iv: "
                << (data.Iv != nullptr ? toHexString(data.Iv, data.ivLength) : "null") << std::endl;
    } catch (const std::exception &e) {
      std::cerr << "Error converting Iv to hex string: " << e.what() << std::endl;
    }

    std::cout << "-------------------------------------------------------------" << std::endl;
  }

  // enums
  enum AlgorithmType {
    AES128,
    AES256
  };
}
#endif // GLOBALDEFINITIONS_H