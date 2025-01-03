#include "RestApiDLL.h"
#include "../../../../shared/FileData.h"
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
