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
    /*unsigned char *EncryptionID = nullptr;
    size_t EncryptionIDLength = 0;
    unsigned char *LastUpdateID = nullptr;
    size_t LastUpdateIDLength = 0;*/
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
          /*&& EncryptionIDLength == other.EncryptionIDLength
          && LastUpdateIDLength == other.LastUpdateIDLength*/
          && keyLength == other.keyLength
          && ivLength == other.ivLength
          && std::equal(FileID, FileID + fileIDLength, other.FileID)
          /*&& std::equal(EncryptionID, EncryptionID + EncryptionIDLength, other.EncryptionID)
          && std::equal(LastUpdateID, LastUpdateID + LastUpdateIDLength, other.LastUpdateID)*/
          && wcscmp(AlgorithmenType, other.AlgorithmenType) == 0
          && wcscmp(OriginalFilePath, other.OriginalFilePath) == 0
          && wcscmp(EncryptedFilePath, other.EncryptedFilePath) == 0
          && wcscmp(DecryptedFilePath, other.DecryptedFilePath) == 0;
    }
  };

  inline void cleanupFileData(FileData &data) {
    // Hilfsfunktion zum sicheren Löschen von unsigned char Arrays
    auto safeDeleteUCharArray = [](unsigned char *&ptr, size_t &length) {
      if (ptr != nullptr) {
        delete[] ptr;
        ptr = nullptr;
        length = 0;
      }
    };

    // Hilfsfunktion zum sicheren Löschen von wchar_t Arrays
    auto safeDeleteWChar = [](wchar_t *&ptr) {
      if (ptr != nullptr) {
        delete[] ptr;
        ptr = nullptr;
      }
    };

    // Sicheres Löschen aller dynamisch allokierten Felder
    safeDeleteUCharArray(data.FileID, data.fileIDLength);
    safeDeleteWChar(data.AlgorithmenType);
    safeDeleteWChar(data.OriginalFilePath);
    safeDeleteWChar(data.EncryptedFilePath);
    safeDeleteWChar(data.DecryptedFilePath);
    safeDeleteUCharArray(data.Key, data.keyLength);
    safeDeleteUCharArray(data.Iv, data.ivLength);

    // Zurücksetzen der Struktur auf Standardwerte
    data = FileData();
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
    std::wcout.flush();
    std::cout << "-------------------------------------------------------------" << std::endl;
    std::cout << "FileData Debug from debugFileData fn: " << std::endl;

    try {
      std::cout << "FileID: "
                << (data.FileID != nullptr ? toHexString(data.FileID, data.fileIDLength) : "null") << std::endl;
    } catch (const std::exception &e) {
      std::cerr << "Error converting FileID to hex string: " << e.what() << std::endl;
    }

    std::cout << "EncryptedFilePath: ";
    if (data.EncryptedFilePath && *data.EncryptedFilePath != L'\0') {
      std::cout << "not null: ";
      std::wcout << data.EncryptedFilePath;
    } else {
      std::wcout << L"null";
    }
    std::wcout << std::endl;

    std::cout << "OriginalFilePath: ";
    if (data.OriginalFilePath && *data.OriginalFilePath != L'\0') {
      std::cout << "not null: ";
      std::wcout << data.OriginalFilePath;
    } else {
      std::wcout << L"null";
    }
    std::wcout << std::endl;

    std::cout << "DecryptedFilePath: ";
    if (data.DecryptedFilePath && *data.DecryptedFilePath != L'\0') {
      std::cout << "not null: ";
      std::wcout << data.DecryptedFilePath;
    } else {
      std::wcout << L"null";
    }
    std::wcout << std::endl;

    std::cout << L"AlgorithmenType: ";
    if (data.AlgorithmenType && *data.AlgorithmenType != L'\0') {
      std::cout << "not null: ";
      std::wcout << data.AlgorithmenType;
    } else {
      std::wcout << L"null";
    }
    std::wcout << std::endl;

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