#include "RestApiDLL.h"
#include "../../../shared/FileData.h"
#include <windows.h>

bool RestApiDLL::InsertEntry(const FileData &data) {
  HMODULE hDll = loadDll(L"GoRestApiWrapperLib.dll");
  if (!hDll) {
    logError("RestApiDLL-Insert: Failed to load DLL");
    return false;
  }

  auto func = (InsertEntryFunc) GetProcAddress(hDll, "InsertEntry");
  if (!func) {
    logError("RestApiDLL-Insert: Failed to get function address for InsertEntry");
    unloadDll(hDll);
    return false;
  }

  bool result = false;
  auto dbStruct = convertFileDataToDBStruct(data);
  func(&dbStruct, &result);
  if (result) {
    logInfo("RestApiDLL-Insert: Successfully inserted FileData struct into the database");
  } else {
    logError("RestApiDLL-Insert: Failed to insert FileData struct into the database");
  }

  unloadDll(hDll);
  return result;
}

bool RestApiDLL::DeleteEntry(const FileData &data) {
  HMODULE hDll = loadDll(L"GoRestApiWrapperLib.dll");
  if (!hDll) {
    logError("RestApiDLL-Delete: Failed to load DLL");
    return false;
  }

  auto func = (DeleteEntryFunc) GetProcAddress(hDll, "DeleteEntry");
  if (!func) {
    logError("RestApiDLL-Delete: Failed to get function address for DeleteEntry");
    unloadDll(hDll);
    return false;
  }

  bool result = false;
  auto dbStruct = convertFileDataToDBStruct(data);

  try {
    func(&dbStruct, &result);
  } catch (const std::exception &e) {
    logError(std::string("RestApiDLL-Delete: Exception caught: ") + e.what());
    result = false;
  }

  if (result) {
    logInfo("RestApiDLL-Delete: Successfully deleted FileData struct from the database");
  } else {
    logError("RestApiDLL-Delete: Failed to delete FileData struct from the database");
  }

  unloadDll(hDll);
  return result;
}

bool RestApiDLL::SearchEntry(FileData &data) {
  if (printDebug) std::cout << "RestApiDLL-Search: Searching for FileData struct in the database" << std::endl;
  HMODULE hDll = loadDll(L"GoRestApiWrapperLib.dll");
  if (!hDll) {
    logError("RestApiDLL-Search: Failed to load DLL");
    return false;
  }

  auto func = (SearchEntryFunc) GetProcAddress(hDll, "SearchEntry");
  if (!func) {
    logError("RestApiDLL-Search: Failed to get function address for SearchEntry");
    unloadDll(hDll);
    return false;
  }

  bool result = false;
  auto dbStruct = convertFileDataForSearch(data);
  if (printDebug) std::cout << "RestApiDLL-Search: Translated FileData struct for search" << std::endl;

  func(&dbStruct, &result);

  std::cout.flush();

  if (!result) {
    logError("RestApiDLL-Search: Failed to get FileData struct from the database");
    unloadDll(hDll);
    return false;
  }

  if (printDebug) std::cout << "RestApiDLL-Search: Successfully got FileData struct from the database" << std::endl;
  if (printConverterDebug) debugFileDataDB(dbStruct);
  if (printDebug) std::cout << "RestApiDLL-Search: Converting FileDataDB struct to FileData struct" << std::endl;

  std::cout << "SearchRestAPI: FileIDLength -> " << dbStruct.FileIDLength << "; KeyLength -> " << dbStruct.KeyLength << "; IvLength -> " << dbStruct.IvLength << std::endl;

  data = convertDBStructToFileData(dbStruct);

  if (printConverterDebug) data.debugFileData();

  unloadDll(hDll);
  return true;
}

bool RestApiDLL::ReplaceEntry(const FileData &data) {
  HMODULE hDll = loadDll(L"GoRestApiWrapperLib.dll");
  if (!hDll) {
    logError("RestApiDLL-Replace: Failed to load DLL");
    return false;
  }

  auto func = (ReplaceEntryFunc) GetProcAddress(hDll, "ReplaceEntry");
  if (!func) {
    logError("RestApiDLL-Replace: Failed to get function address for ReplaceEntry");
    unloadDll(hDll);
    return false;
  }

  bool result = false;
  auto dbStruct = convertFileDataToDBStruct(data);
  func(&dbStruct, &result);

  if (result) {
    logInfo("RestApiDLL-Replace: Successfully replaced FileData struct into the database");
  } else {
    logError("RestApiDLL-Replace: Failed to replace FileData struct into the database");
  }

  unloadDll(hDll);
  return result;
}

std::vector<FileData> RestApiDLL::GetAllFileIDsAndEncryptedPaths() {
  HMODULE hDll = loadDll(L"GoRestApiWrapperLib.dll");
  if (!hDll) {
    logError("RestApiDLL-GetAll: Failed to load DLL");
    return {};
  }

  auto func = (GetAllFileIDsAndEncryptedPathsFunc) GetProcAddress(hDll, "GetAllFileIDsAndEncryptedPaths");
  if (!func) {
    logError("RestApiDLL-GetAll: Failed to get function address for GetAllFileIDsAndEncryptedPaths");
    unloadDll(hDll);
    return {};
  }

  bool result = false;
  FileDataDB **pFileDataList = nullptr;
  logInfo("RestApiDLL-GetAll: Getting all FileData structs from the database");
  func(pFileDataList, &result);
  unloadDll(hDll);

  if (!result) {
    logError("RestApiDLL-GetAll: Failed to get all FileData structs from the database");
    return {};
  }

  std::vector<FileData> FileDataList;
  logInfo("RestApiDLL-GetAll: Converting FileDataDB structs to FileData structs");
  for (int i = 0; pFileDataList[i] != nullptr; i++) {
    FileData fileData = convertDBStructToFileData(*pFileDataList[i]);
    FileDataList.push_back(fileData);
  }

  logInfo("RestApiDLL-GetAll: Completed converting FileDataDB structs to FileData structs");
  return FileDataList;
}


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


RestApiDLL::FileDataDB RestApiDLL::convertFileDataToDBStruct(const FileData& data) {
  RestApiDLL::FileDataDB dbStruct{};

  dbStruct.FileID = convertToHexWChar(data.getFileId(), data.getFileIdLength());
  dbStruct.FileIDLength = data.getFileIdLength();
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


RestApiDLL::FileDataDB RestApiDLL::convertFileDataForSearch(const FileData& data) {
  RestApiDLL::FileDataDB dbStruct{};

  // Konvertiere nur FileID
  dbStruct.FileID = convertToHexWChar(data.getFileId(), data.getFileIdLength());
  dbStruct.FileIDLength = 0;
  dbStruct.AlgorithmenType = nullptr;
  dbStruct.OriginalFilePath = nullptr;
  dbStruct.EncryptedFilePath = nullptr;
  dbStruct.DecryptedFilePath = nullptr;
  dbStruct.Key = nullptr;
  dbStruct.KeyLength = 0;
  dbStruct.Iv = nullptr;
  dbStruct.IvLength = 0;

  return dbStruct;
}

FileData RestApiDLL::convertDBStructToFileData(const FileDataDB& data) {
  FileData fileData{};

  // Konvertiere FileID und setze die Länge
  size_t fileIdLength = 0;
  auto fileId = convertFromHexWChar(data.FileID, fileIdLength);
  fileData.setFileId(fileId);
  fileData.setFileIdLength(data.FileIDLength);

  std::cout << "fileIdLength -> " << fileData.getFileIdLength() << std::endl;
  std::cout << "FileId Len " << data.FileIDLength << " -> " << fileIdLength << " -> " << fileData.getFileIdLength() << std::endl;

  // Konvertiere Key und setze die Länge
  size_t keyLength = 0;
  auto key = convertFromHexWChar(data.Key, keyLength);
  fileData.setKey(key);
  fileData.setKeyLength(data.KeyLength);

  std::cout << "Key Len " << data.KeyLength << " -> " << keyLength << " -> " << fileData.getKeyLength() << std::endl;

  // Konvertiere IV und setze die Länge
  size_t ivLength = 0;
  auto iv = convertFromHexWChar(data.Iv, ivLength);
  fileData.setIv(iv);
  fileData.setIvLength(data.IvLength);

  std::cout << "Iv Len " << data.IvLength << " -> " << ivLength << " -> " << fileData.getIvLength() << std::endl;

  // Setze die Dateipfade und den Algorithmus
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