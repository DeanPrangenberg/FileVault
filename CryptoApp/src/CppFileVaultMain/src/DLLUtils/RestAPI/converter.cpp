//
// Created by prang on 01.01.2025.
//

#include "RestApiDLL.h"

wchar_t* RestApiDLL::convertToHexWChar(const unsigned char* input, size_t size) {
  if (input == nullptr || size == 0) {
    std::cerr << "convertToHexWChar: Input is null or size is zero" << std::endl;
    return nullptr;
  }

  // Stream für die Hex-Darstellung
  std::wstringstream wss;
  for (size_t i = 0; i < size; ++i) {
    wss << std::setw(2) << std::setfill(L'0') << std::hex << static_cast<int>(input[i]);
  }

  // Hex-Darstellung als wstring speichern
  std::wstring hexStr = wss.str();

  // Speicher für wchar_t* (inklusive Null-Terminierung)
  wchar_t* result = new wchar_t[hexStr.size() + 1];
  std::wmemcpy(result, hexStr.c_str(), hexStr.size() + 1);  // Null-Terminierung beachten

  return result;
}

unsigned char* RestApiDLL::convertFromHexWChar(const wchar_t* input, size_t& size) {
  if (input == nullptr) {
    std::cerr << "convertFromHexWChar: Input is null" << std::endl;
    size = 0;
    return nullptr;
  }

  size_t len = std::wcslen(input);
  if (len % 2 != 0) {
    std::cerr << "convertFromHexWChar: Invalid hex string length" << std::endl;
    size = 0;
    return nullptr;
  }

  size = len / 2;  // Jede zwei Hex-Ziffern entsprechen einem Byte
  unsigned char* result = new unsigned char[size];

  for (size_t i = 0; i < size; ++i) {
    unsigned int byte;
    std::swscanf(input + 2 * i, L"%2x", &byte);
    result[i] = static_cast<unsigned char>(byte);
  }

  return result;
}

RestApiDLL::FileDataDB RestApiDLL::convertFileDataForSearch(const FileData& data) {
  RestApiDLL::FileDataDB dbStruct{};

  // Konvertiere nur FileID
  dbStruct.FileID = convertToHexWChar(data.getFileId(), data.getFileIdLength());
  dbStruct.EncryptedID = convertToHexWChar(data.getEncryptionId(), data.getEncryptionIdLength());

  return dbStruct;
}

RestApiDLL::FileDataDB RestApiDLL::convertFileDataToDBStruct(const FileData &data) {
  RestApiDLL::FileDataDB dbStruct{};

  dbStruct.FileID = convertToHexWChar(data.getFileId(), data.getFileIdLength());
  dbStruct.FileIDLength = data.getFileIdLength();
  dbStruct.EncryptedID = convertToHexWChar(data.getEncryptionId(), data.getEncryptionIdLength());
  dbStruct.EncryptedIDLength = data.getEncryptionIdLength();
  dbStruct.LastUpdateID = convertToHexWChar(data.getLastUpdateId(), data.getLastUpdateIdLength());
  dbStruct.LastUpdateIDLength = data.getLastUpdateIdLength();
  dbStruct.AlgorithmenType = data.getAlgorithmenType();
  dbStruct.OriginalFilePath = data.getOriginalFilePath();
  dbStruct.EncryptedFilePath = data.getEncryptedFilePath();
  dbStruct.DecryptedFilePath = data.getDecryptedFilePath();
  dbStruct.Key = convertToHexWChar(data.getKey(), data.getKeyLength());
  dbStruct.KeyLength = data.getKeyLength();
  dbStruct.Iv = convertToHexWChar(data.getIv(), data.getIvLength());
  dbStruct.IvLength = data.getIvLength();

  return dbStruct;
}

FileData RestApiDLL::convertDBStructToFileData(const FileDataDB &data) {
  FileData fileData{};

  size_t fileIdLength = 0;
  auto fileId = convertFromHexWChar(data.FileID, fileIdLength);
  fileData.setFileId(fileId);
  fileData.setFileIdLength(data.FileIDLength);

  size_t encryptedIdLength = 0;
  auto encryptedId = convertFromHexWChar(data.EncryptedID, encryptedIdLength);
  fileData.setEncryptionId(encryptedId);
  fileData.setEncryptionIdLength(data.EncryptedIDLength);

  size_t lastUpdateIdLength = 0;
  auto lastUpdateId = convertFromHexWChar(data.LastUpdateID, lastUpdateIdLength);
  fileData.setLastUpdateId(lastUpdateId);
  fileData.setLastUpdateIdLength(data.LastUpdateIDLength);

  size_t keyLength = 0;
  auto key = convertFromHexWChar(data.Key, keyLength);
  fileData.setKey(key);
  fileData.setKeyLength(data.KeyLength);

  size_t ivLength = 0;
  auto iv = convertFromHexWChar(data.Iv, ivLength);
  fileData.setIv(iv);
  fileData.setIvLength(data.IvLength);

  fileData.setEncryptedFilePath(data.EncryptedFilePath);
  fileData.setOriginalFilePath(data.OriginalFilePath);
  fileData.setAlgorithmenType(data.AlgorithmenType);
  fileData.setDecryptedFilePath(data.DecryptedFilePath);

  return fileData;
}

void RestApiDLL::debugFileDataDB(const FileDataDB &data) {
  std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
  std::cout << "SearchRestAPI: FileIDLength -> " << data.FileIDLength << "; KeyLength -> " << data.KeyLength << "; IvLength -> " << data.IvLength << std::endl;
  std::wcout << L"+ FileDataDB: " << std::endl;

  if (data.FileID != nullptr) {
    try {
      std::wcout << L"+ FileID: " << data.FileID << std::endl;
    } catch (const std::exception &e) {
      std::wcerr << L"+ Error accessing FileID: " << e.what() << std::endl;
    }
  } else {
    std::wcout << L"+ FileID: null" << std::endl;
  }

  std::cout <<  "+ FileIDLength: " << data.FileIDLength << std::endl;

  if (data.AlgorithmenType != nullptr) {
    try {
      std::wcout << L"+ AlgorithmenType: " << data.AlgorithmenType << std::endl;
    } catch (const std::exception &e) {
      std::wcerr << L"+ Error accessing AlgorithmenType: " << e.what() << std::endl;
    }
  } else {
    std::wcout << L"+ AlgorithmenType: null" << std::endl;
  }

  if (data.OriginalFilePath != nullptr) {
    try {
      std::wcout << L"+ OriginalFilePath: " << data.OriginalFilePath << std::endl;
    } catch (const std::exception &e) {
      std::wcerr << L"+ Error accessing OriginalFilePath: " << e.what() << std::endl;
    }
  } else {
    std::wcout << L"+ OriginalFilePath: null" << std::endl;
  }

  if (data.EncryptedFilePath != nullptr) {
    try {
      std::wcout << L"+ EncryptedFilePath: " << data.EncryptedFilePath << std::endl;
    } catch (const std::exception &e) {
      std::wcerr << L"+ Error accessing EncryptedFilePath: " << e.what() << std::endl;
    }
  } else {
    std::wcout << L"+ EncryptedFilePath: null" << std::endl;
  }

  if (data.DecryptedFilePath != nullptr) {
    try {
      std::wcout << L"+ DecryptedFilePath: " << data.DecryptedFilePath << std::endl;
    } catch (const std::exception &e) {
      std::wcerr << L"+ Error accessing DecryptedFilePath: " << e.what() << std::endl;
    }
  } else {
    std::wcout << L"+ DecryptedFilePath: null" << std::endl;
  }

  if (data.Key != nullptr) {
    try {
      std::wcout << L"+ Key: " << data.Key << std::endl;
    } catch (const std::exception &e) {
      std::wcerr << L"+ Error accessing Key: " << e.what() << std::endl;
    }
  } else {
    std::wcout << L"+ Key: null" << std::endl;
  }

  std::cout << "+ KeyLength: " << data.KeyLength << std::endl;

  if (data.Iv != nullptr) {
    try {
      std::wcout << L"+ Iv: " << data.Iv << std::endl;
    } catch (const std::exception &e) {
      std::wcerr << L"+ Error accessing Iv: " << e.what() << std::endl;
    }
  } else {
    std::wcout << L"+ Iv: null" << std::endl;
  }

  std::cout << "+ IvLength: " << data.IvLength << std::endl;

  std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
}
