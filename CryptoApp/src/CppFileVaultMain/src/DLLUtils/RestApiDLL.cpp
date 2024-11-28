#include "RestApiDLL.h"
#include <windows.h>

bool RestApiDLL::InsertEntry(const FileData &data) {
  HMODULE hDll = loadDll(L"GoRestApiWrapperLib.dll");
  if (!hDll) {
    logError("RestApiDLL-Insert: Failed to load DLL");
    return false;
  }

  globalDefinitions::debugFileData(data);

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
  func(&dbStruct, &result);

  if (result) {
    logInfo("RestApiDLL-Delete: Successfully deleted FileData struct into the database");
  } else {
    logError("RestApiDLL-Delete: Failed to delete FileData struct into the database");
  }

  unloadDll(hDll);
  return result;
}

bool RestApiDLL::SearchEntry(FileData &data) {
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

  globalDefinitions::debugFileData(data);

  bool result = false;
  auto dbStruct = convertFileDataForSearch(data);

  func(&dbStruct, &result);
  if (!result) {
    logError("RestApiDLL-Search: Failed to get FileData struct from the database");
    unloadDll(hDll);
    return false;
  }

  std::cout << "RestApiDLL-Search: Successfully got FileData struct from the database" << std::endl;
  debugFileDataDB(dbStruct);
  std::cout << "RestApiDLL-Search: Converting FileDataDB struct to FileData struct" << std::endl;

  globalDefinitions::cleanupFileData(data);

  data = convertDBStructToFileData(dbStruct);

  globalDefinitions::debugFileData(data);

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

wchar_t *RestApiDLL::convertToWChar(const unsigned char *input, size_t size) {
  if (input == nullptr || size == 0) {
    std::cerr << "convertToWChar: Input is null or size is zero" << std::endl;
    return nullptr;
  }

  // Allocate memory for the wchar_t array
  auto result = new wchar_t[size + 1]; // +1 for null terminator

  // Use memcpy to copy the data
  std::memcpy(result, input, size * sizeof(unsigned char));

  // Add the null terminator
  result[size] = L'\0';

  return result;
}

unsigned char* RestApiDLL::convertToUnsignedChar(const wchar_t* input, size_t size) {
  if (input == nullptr || size == 0) {
    std::cerr << "convertToUnsignedChar: Input ist null oder Größe ist null" << std::endl;
    return nullptr;
  }

  // Berechne die Größe in Bytes
  size_t byteSize = size * sizeof(wchar_t);

  // Allokiere Speicher für das Ergebnis
  unsigned char* result = new (std::nothrow) unsigned char[byteSize];
  if (result == nullptr) {
    std::cerr << "convertToUnsignedChar: Speicherallokation fehlgeschlagen" << std::endl;
    return nullptr;
  }

  // Kopiere die rohen Binärdaten
  std::memcpy(result, input, byteSize);

  return result;
}

wchar_t *RestApiDLL::convertToHexWChar(const unsigned char *input, size_t size) {
  if (input == nullptr || size == 0) {
    std::cerr << "convertToHexWChar: Input is null or size is zero" << std::endl;
    return nullptr;
  }

  std::wstringstream wss;
  for (size_t i = 0; i < size; ++i) {
    wss << std::setw(2) << std::setfill(L'0') << std::hex << static_cast<int>(input[i]);
  }

  std::wstring hexStr = wss.str();
  wchar_t *result = new wchar_t[hexStr.size() + 1];
  std::wmemcpy(result, hexStr.c_str(), hexStr.size() + 1);

  return result;
}

unsigned char* RestApiDLL::convertFromHexWChar(const wchar_t* input, const size_t size) {
  if (input == nullptr || size == 0 || size % 4 != 0) {
    std::wcerr << L"Invalid input" << std::endl;
    return nullptr;
  }

  size_t outputSize = size / 2;
  std::cout << outputSize << std::endl;
  std::cout << size << std::endl;
  unsigned char* result = new unsigned char[outputSize];

  for (size_t i = 0; i < outputSize; ++i) {
    wchar_t hexPair[3] = {input[2*i], input[2*i+1], L'\0'};
    result[i] = (unsigned char)std::wcstoul(hexPair, nullptr, 16);
  }

  return result;
}

RestApiDLL::FileDataDB RestApiDLL::convertFileDataToDBStruct(const FileData &data) {
  RestApiDLL::FileDataDB dbStruct;

  dbStruct.FileID = convertToHexWChar(data.FileID, data.fileIDLength);
  dbStruct.AlgorithmenType = data.AlgorithmenType;
  dbStruct.OriginalFilePath = data.OriginalFilePath;
  dbStruct.EncryptedFilePath = data.EncryptedFilePath;
  dbStruct.DecryptedFilePath = data.DecryptedFilePath;
  dbStruct.Key = convertToHexWChar(data.Key, data.keyLength);
  dbStruct.Iv = convertToHexWChar(data.Iv, data.ivLength);

  if (dbStruct.FileID == nullptr
      || dbStruct.AlgorithmenType == nullptr
      || dbStruct.EncryptedFilePath == nullptr
      || dbStruct.OriginalFilePath == nullptr
      || dbStruct.DecryptedFilePath == nullptr
      || dbStruct.Key == nullptr
      || dbStruct.Iv == nullptr) {
    logError("Failed to convert FileData struct to FileDataDB struct");
  } else {
    debugFileDataDB(dbStruct);
  }

  return dbStruct;
}

RestApiDLL::FileDataDB RestApiDLL::convertFileDataForSearch(const FileData &data) {
  RestApiDLL::FileDataDB dbStruct{};

  dbStruct.FileID = convertToHexWChar(data.FileID, data.fileIDLength);
  dbStruct.AlgorithmenType = nullptr;
  dbStruct.OriginalFilePath = nullptr;
  dbStruct.EncryptedFilePath = nullptr;
  dbStruct.DecryptedFilePath = nullptr;
  dbStruct.Key = nullptr;
  dbStruct.Iv = nullptr;

  if (dbStruct.FileID == nullptr) {
    logError("Failed to convert FileData struct to FileDataDB struct");
  }
  debugFileDataDB(dbStruct);
  return dbStruct;
}

FileData RestApiDLL::convertDBStructToFileData(const FileDataDB &data) {
  FileData fileData;
  fileData.FileID = convertFromHexWChar(data.FileID, std::wcslen(data.FileID));
  fileData.fileIDLength = std::wcslen(data.FileID) / 2;
  fileData.EncryptedFilePath = data.EncryptedFilePath;
  fileData.OriginalFilePath = data.OriginalFilePath;
  fileData.AlgorithmenType = data.AlgorithmenType;
  fileData.DecryptedFilePath = data.DecryptedFilePath;
  fileData.Key = convertFromHexWChar(data.Key, std::wcslen(data.Key));
  fileData.keyLength = std::wcslen(data.Key) / 2;
  fileData.Iv = convertFromHexWChar(data.Iv, std::wcslen(data.Iv));
  fileData.ivLength = std::wcslen(data.Iv) / 2;

  return fileData;
}

void RestApiDLL::debugFileDataDB(const FileDataDB &data) {
  std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
  std::wcout << L"FileDataDB: " << std::endl;

  if (data.FileID != nullptr) {
    try {
      std::wcout << L"FileID: " << data.FileID << std::endl;
    } catch (const std::exception &e) {
      std::wcerr << L"Error accessing FileID: " << e.what() << std::endl;
    }
  } else {
    std::wcout << L"FileID: null" << std::endl;
  }

  if (data.AlgorithmenType != nullptr) {
    try {
      std::wcout << L"AlgorithmenType: " << data.AlgorithmenType << std::endl;
    } catch (const std::exception &e) {
      std::wcerr << L"Error accessing AlgorithmenType: " << e.what() << std::endl;
    }
  } else {
    std::wcout << L"AlgorithmenType: null" << std::endl;
  }

  if (data.OriginalFilePath != nullptr) {
    try {
      std::wcout << L"OriginalFilePath: " << data.OriginalFilePath << std::endl;
    } catch (const std::exception &e) {
      std::wcerr << L"Error accessing OriginalFilePath: " << e.what() << std::endl;
    }
  } else {
    std::wcout << L"OriginalFilePath: null" << std::endl;
  }

  if (data.EncryptedFilePath != nullptr) {
    try {
      std::wcout << L"EncryptedFilePath: " << data.EncryptedFilePath << std::endl;
    } catch (const std::exception &e) {
      std::wcerr << L"Error accessing EncryptedFilePath: " << e.what() << std::endl;
    }
  } else {
    std::wcout << L"EncryptedFilePath: null" << std::endl;
  }

  if (data.DecryptedFilePath != nullptr) {
    try {
      std::wcout << L"DecryptedFilePath: " << data.DecryptedFilePath << std::endl;
    } catch (const std::exception &e) {
      std::wcerr << L"Error accessing DecryptedFilePath: " << e.what() << std::endl;
    }
  } else {
    std::wcout << L"DecryptedFilePath: null" << std::endl;
  }

  if (data.Key != nullptr) {
    try {
      std::wcout << L"Key: " << data.Key << std::endl;
    } catch (const std::exception &e) {
      std::wcerr << L"Error accessing Key: " << e.what() << std::endl;
    }
  } else {
    std::wcout << L"Key: null" << std::endl;
  }

  if (data.Iv != nullptr) {
    try {
      std::wcout << L"Iv: " << data.Iv << std::endl;
    } catch (const std::exception &e) {
      std::wcerr << L"Error accessing Iv: " << e.what() << std::endl;
    }
  } else {
    std::wcout << L"Iv: null" << std::endl;
  }

  std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
}