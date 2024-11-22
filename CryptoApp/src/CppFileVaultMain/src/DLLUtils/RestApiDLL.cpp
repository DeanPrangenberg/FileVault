#include "RestApiDLL.h"
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

  bool result = false;
  auto dbStruct = convertFileDataForSearch(data);
  func(&dbStruct, &result);
  if (!result) {
    logError("RestApiDLL-Search: Failed to get FileData struct from the database");
    unloadDll(hDll);
    return false;
  }

  data = convertDBStructToFileData(dbStruct);

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

wchar_t* RestApiDLL::convertToWChar(const unsigned char* input, size_t size) {
  if (input == nullptr || size == 0) {
    std::cerr << "convertToWChar: Input is null or size is zero" << std::endl;
    return nullptr;
  }

  wchar_t* result = new wchar_t[size + 1]; // +1 for null terminator
  for (size_t i = 0; i < size; ++i) {
    result[i] = static_cast<wchar_t>(input[i]);
  }
  result[size] = L'\0'; // null terminator
  return result;
}

unsigned char* RestApiDLL::convertToUnsignedChar(const wchar_t* input, size_t size) {
  if (input == nullptr || size == 0) {
    std::cerr << "convertToUnsignedChar: Input is null or size is zero" << std::endl;
    return nullptr;
  }

  unsigned char* result = new unsigned char[size + 1]; // +1 for null terminator
  for (size_t i = 0; i < size; ++i) {
    result[i] = static_cast<unsigned char>(input[i]);
  }
  result[size] = '\0'; // null terminator
  return result;
}

RestApiDLL::FileDataDB RestApiDLL::convertFileDataToDBStruct(const FileData &data) {
  RestApiDLL::FileDataDB dbStruct;

  dbStruct.FileID = convertToWChar(data.FileID, data.fileIDLength);
  dbStruct.AlgorithmenType = data.AlgorithmenType;
  dbStruct.OriginalFilePath = data.OriginalFilePath;
  dbStruct.EncryptedFilePath = data.EncryptedFilePath;
  dbStruct.DecryptedFilePath = data.DecryptedFilePath;
  dbStruct.Key = convertToWChar(data.Key, data.keyLength);
  dbStruct.Iv = convertToWChar(data.Iv, data.ivLength);

  if (dbStruct.FileID == nullptr
      || dbStruct.AlgorithmenType == nullptr
      || dbStruct.EncryptedFilePath == nullptr
      || dbStruct.OriginalFilePath == nullptr
      || dbStruct.DecryptedFilePath == nullptr
      || dbStruct.Key == nullptr
      || dbStruct.Iv == nullptr) {
    logError("Failed to convert FileData struct to FileDataDB struct");
  }

  return dbStruct;
}

RestApiDLL::FileDataDB RestApiDLL::convertFileDataForSearch(const FileData &data) {
  RestApiDLL::FileDataDB dbStruct;
  dbStruct.FileID = convertToWChar(data.FileID, data.fileIDLength);
  dbStruct.EncryptedFilePath = data.EncryptedFilePath;

  if (dbStruct.FileID == nullptr || dbStruct.EncryptedFilePath == nullptr) {
    logError("Failed to convert FileData struct to FileDataDB struct");
  }

  return dbStruct;
}

FileData RestApiDLL::convertDBStructToFileData(const FileDataDB &data) {
  FileData fileData;
  fileData.FileID = convertToUnsignedChar(data.FileID, std::wcslen(data.FileID));
  fileData.EncryptedFilePath = data.EncryptedFilePath;
  fileData.OriginalFilePath = data.OriginalFilePath;
  fileData.AlgorithmenType = data.AlgorithmenType;
  fileData.DecryptedFilePath = data.DecryptedFilePath;
  fileData.Key = convertToUnsignedChar(data.Key, std::wcslen(data.Key));
  fileData.Iv = convertToUnsignedChar(data.Iv, std::wcslen(data.Iv));

  return fileData;
}