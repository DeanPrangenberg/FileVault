#include "RestApiDLL.h"
#include "../../../../shared/FileData.h"
#include <windows.h>

std::unordered_map<std::string, std::string> RestApiDLL::ExportDatabase() {
  HMODULE hDll = loadDll(L"GoRestApiWrapperLib.dll");
  if (!hDll) {
    logError("RestApiDLL-Export: Failed to load DLL");
    return {};
  }

  auto func = (ExportDatabaseFunc) GetProcAddress(hDll, "ExportDatabase");
  if (!func) {
    logError("RestApiDLL-Export: Failed to get function address for ExportDatabase");
    unloadDll(hDll);
    return {};
  }

  char *data = nullptr;
  int dataSize = 0;
  char *key = nullptr;
  int keySize = 0;

  func(&data, &dataSize, &key, &keySize);
  logInfo("RestApiDLL-Export: Successfully exported database");

  std::string dataStr;
  for (int i = 0; i < dataSize; ++i) {
    dataStr.push_back(data[i]);
  }

  std::string keyStr;
  for (int i = 0; i < keySize; ++i) {
    keyStr.push_back(key[i]);
  }

  logInfo("Data: " + dataStr);
  logInfo("Key: " + keyStr);

  std::unordered_map<std::string, std::string> exportResult;
  exportResult["Data"] = dataStr;
  exportResult["Key"] = keyStr;
  logInfo("RestApiDLL-Export: Successfully converted database to std::unordered_map");

  unloadDll(hDll);
  return exportResult;
}

bool RestApiDLL::InsertDatabase(const std::string &key, const std::string &data) {
  HMODULE hDll = loadDll(L"GoRestApiWrapperLib.dll");
  if (!hDll) {
    logError("RestApiDLL-InsertDB: Failed to load DLL");
    return false;
  }

  auto func = (InsertDatabaseFunc) GetProcAddress(hDll, "InsertDatabase");
  if (!func) {
    logError("RestApiDLL-InsertDB: Failed to get function address for InsertDatabase");
    unloadDll(hDll);
    return false;
  }

  bool result = false;
  func(data.c_str(), key.c_str(), &result);

  unloadDll(hDll);
  return result;
}

bool RestApiDLL::ReplaceDatabase(const std::string &key, const std::string &data) {
  HMODULE hDll = loadDll(L"GoRestApiWrapperLib.dll");
  if (!hDll) {
    logError("RestApiDLL-ReplaceDB: Failed to load DLL");
    return false;
  }

  auto func = (ReplaceDatabaseFunc) GetProcAddress(hDll, "ReplaceDatabase");
  if (!func) {
    logError("RestApiDLL-ReplaceDB: Failed to get function address for ReplaceDatabase");
    unloadDll(hDll);
    return false;
  }

  bool result = false;

  logInfo("RestApiDLL-ReplaceDB: Replacing database with key: " + key);
  logInfo("RestApiDLL-ReplaceDB: Replacing database with data: " + data);

  func(data.c_str(), key.c_str(), &result);

  unloadDll(hDll);
  return result;
}

bool RestApiDLL::ResetDatabase() {
  HMODULE hDll = loadDll(L"GoRestApiWrapperLib.dll");
  if (!hDll) {
    logError("RestApiDLL-ResetDB: Failed to load DLL");
    return false;
  }

  auto func = (ResetDatabaseFunc) GetProcAddress(hDll, "ResetDatabase");
  if (!func) {
    logError("RestApiDLL-ResetDB: Failed to get function address for ResetDatabase");
    unloadDll(hDll);
    return false;
  }

  bool result = false;
  func(&result);

  unloadDll(hDll);
  return result;
}

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

  FileDataDB* pFileDataList = nullptr;
  int listSize = 0;
  logInfo("RestApiDLL-GetAll: Getting all FileData structs from the database");
  func(&pFileDataList, &listSize);
  unloadDll(hDll);

  if (listSize == 0 || pFileDataList == nullptr) {
    logError("RestApiDLL-GetAll: No FileData structs found in the database");
    return {};
  }

  std::vector<FileData> FileDataList;
  logInfo("RestApiDLL-GetAll: Converting FileDataDB structs to FileData structs");
  for (int i = 0; i < listSize; i++) {
    FileData fileData = convertDBStructToFileData(pFileDataList[i]);
    FileDataList.push_back(fileData);
  }

  logInfo("RestApiDLL-GetAll: Completed converting FileDataDB structs to FileData structs");
  return FileDataList;
}

int RestApiDLL::getDatabaseFileSize() {
  HMODULE hDll = loadDll(L"GoRestApiWrapperLib.dll");
  if (!hDll) {
    logError("RestApiDLL-GetFileSize: Failed to load DLL");
    return -1;
  }

  auto func = (GetDatabaseFileSizeFunc) GetProcAddress(hDll, "GetDatabaseFileSize");
  if (!func) {
    logError("RestApiDLL-GetFileSize: Failed to get function address for GetDatabaseFileSize");
    unloadDll(hDll);
    return -1;
  }

  int fileSize = 0;

  func(&fileSize);

  unloadDll(hDll);
  return fileSize;
}
