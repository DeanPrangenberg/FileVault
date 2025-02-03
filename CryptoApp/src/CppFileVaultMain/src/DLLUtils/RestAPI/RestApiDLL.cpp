#include "RestApiDLL.h"
#include "../../../../shared/src/FileData.h"
#include <windows.h>

/**
 * @brief Exports the database using the GoRestApiWrapperLib DLL.
 *
 * @return A map containing the exported database data and key.
 */
std::unordered_map<std::string, std::string> RestApiDLL::ExportDatabase() {
  HMODULE hDll = loadDll(L"GoRestApiWrapperLib.dll");
  if (!hDll) {
    Logs::writeToErrorLog("RestApiDLL-Export: Failed to load DLL");
    return {};
  }

  auto func = (ExportDatabaseFunc)GetProcAddress(hDll, "ExportDatabase");
  if (!func) {
    Logs::writeToErrorLog("RestApiDLL-Export: Failed to get function address for ExportDatabase");
    unloadDll(hDll);
    return {};
  }

  char *data = nullptr;
  char *key = nullptr;
  int dataSize = 0;
  int keySize = 0;
  int result = 0;

  func(&data, &dataSize, &key, &keySize, &result);

  switch (result) {
    case -1:
      Logs::writeToInfoLog("RestApiDLL-Export: Successfully exported database");
      break;
    case 1:
      Logs::writeToErrorLog("RestApiDLL-Export: Couldn't reach db container");
      break;
    case 2:
      Logs::writeToErrorLog("RestApiDLL-Export: Response body could not be unmarshaled");
      break;
    case 3:
      Logs::writeToErrorLog("RestApiDLL-Export: Key and Data fields are not present in Json response");
      break;
    default:
      Logs::writeToErrorLog("RestApiDLL-Export: Unknown error code");
      break;
  }

  if (dataSize == 0 || keySize == 0 || data == nullptr || key == nullptr) {
    Logs::writeToErrorLog("RestApiDLL-Export: Database is empty");
    free(data);
    free(key);
    unloadDll(hDll);
    return {};
  }

  std::string dataStr(data, dataSize);
  std::string keyStr(key, keySize);

  Logs::writeToInfoLog("Data: " + dataStr);
  Logs::writeToInfoLog("Key: " + keyStr);

  std::unordered_map<std::string, std::string> exportResult;
  exportResult["Data"] = dataStr;
  exportResult["Key"] = keyStr;
  Logs::writeToInfoLog("RestApiDLL-Export: Successfully converted database to std::unordered_map");

  free(data);
  free(key);

  unloadDll(hDll);
  return exportResult;
}

/**
 * @brief Inserts data into the database using the GoRestApiWrapperLib DLL.
 *
 * @param key The key for the data.
 * @param data The data to insert.
 * @return True if the insertion was successful, false otherwise.
 */
bool RestApiDLL::InsertDatabase(const std::string &key, const std::string &data) {
  HMODULE hDll = loadDll(L"GoRestApiWrapperLib.dll");
  if (!hDll) {
    Logs::writeToErrorLog("RestApiDLL-InsertDB: Failed to load DLL");
    return false;
  }

  auto func = (InsertDatabaseFunc) GetProcAddress(hDll, "InsertDatabase");
  if (!func) {
    Logs::writeToErrorLog("RestApiDLL-InsertDB: Failed to get function address for InsertDatabase");
    unloadDll(hDll);
    return false;
  }

  bool result = false;
  func(data.c_str(), key.c_str(), &result);

  unloadDll(hDll);
  return result;
}

/**
 * @brief Replaces data in the database using the GoRestApiWrapperLib DLL.
 *
 * @param key The key for the data.
 * @param data The data to replace.
 * @return True if the replacement was successful, false otherwise.
 */
bool RestApiDLL::ReplaceDatabase(const std::string &key, const std::string &data) {
  HMODULE hDll = loadDll(L"GoRestApiWrapperLib.dll");
  if (!hDll) {
    Logs::writeToErrorLog("RestApiDLL-ReplaceDB: Failed to load DLL");
    return false;
  }

  auto func = (ReplaceDatabaseFunc) GetProcAddress(hDll, "ReplaceDatabase");
  if (!func) {
    Logs::writeToErrorLog("RestApiDLL-ReplaceDB: Failed to get function address for ReplaceDatabase");
    unloadDll(hDll);
    return false;
  }

  bool result = false;

  Logs::writeToInfoLog("RestApiDLL-ReplaceDB: Replacing database with key: " + key);
  Logs::writeToInfoLog("RestApiDLL-ReplaceDB: Replacing database with data: " + data);

  func(data.c_str(), key.c_str(), &result);

  unloadDll(hDll);
  return result;
}

/**
 * @brief Resets the database using the GoRestApiWrapperLib DLL.
 *
 * @return True if the reset was successful, false otherwise.
 */
bool RestApiDLL::ResetDatabase() {
  HMODULE hDll = loadDll(L"GoRestApiWrapperLib.dll");
  if (!hDll) {
    Logs::writeToErrorLog("RestApiDLL-ResetDB: Failed to load DLL");
    return false;
  }

  auto func = (ResetDatabaseFunc) GetProcAddress(hDll, "ResetDatabase");
  if (!func) {
    Logs::writeToErrorLog("RestApiDLL-ResetDB: Failed to get function address for ResetDatabase");
    unloadDll(hDll);
    return false;
  }

  bool result = false;
  func(&result);

  unloadDll(hDll);
  return result;
}

/**
 * @brief Inserts an entry into the database using the GoRestApiWrapperLib DLL.
 *
 * @param data The FileData object to insert.
 * @return True if the insertion was successful, false otherwise.
 */
bool RestApiDLL::InsertEntry(const FileData &data) {
  HMODULE hDll = loadDll(L"GoRestApiWrapperLib.dll");
  if (!hDll) {
    Logs::writeToErrorLog("RestApiDLL-Insert: Failed to load DLL");
    return false;
  }

  auto func = (InsertEntryFunc) GetProcAddress(hDll, "InsertEntry");
  if (!func) {
    Logs::writeToErrorLog("RestApiDLL-Insert: Failed to get function address for InsertEntry");
    unloadDll(hDll);
    return false;
  }

  bool result = false;
  auto dbStruct = convertFileDataToDBStruct(data);
  func(&dbStruct, &result);
  if (result) {
    Logs::writeToInfoLog("RestApiDLL-Insert: Successfully inserted FileData struct into the database");
  } else {
    Logs::writeToErrorLog("RestApiDLL-Insert: Failed to insert FileData struct into the database");
  }

  delete dbStruct.AlgorithmenType;

  unloadDll(hDll);
  return result;
}

/**
 * @brief Deletes an entry from the database using the GoRestApiWrapperLib DLL.
 *
 * @param data The FileData object to delete.
 * @return True if the deletion was successful, false otherwise.
 */
bool RestApiDLL::DeleteEntry(const FileData &data) {
  HMODULE hDll = loadDll(L"GoRestApiWrapperLib.dll");
  if (!hDll) {
    Logs::writeToErrorLog("RestApiDLL-Delete: Failed to load DLL");
    return false;
  }

  auto func = (DeleteEntryFunc) GetProcAddress(hDll, "DeleteEntry");
  if (!func) {
    Logs::writeToErrorLog("RestApiDLL-Delete: Failed to get function address for DeleteEntry");
    unloadDll(hDll);
    return false;
  }

  bool result = false;
  auto dbStruct = convertFileDataToDBStruct(data);

  try {
    func(&dbStruct, &result);
  } catch (const std::exception &e) {
    Logs::writeToErrorLog(std::string("RestApiDLL-Delete: Exception caught: ") + e.what());
    result = false;
  }

  if (!result) {
    Logs::writeToErrorLog("RestApiDLL-Delete: Failed to delete Entry in the database: "
    + data.EncryptedFilePath->string());
  }

  delete dbStruct.AlgorithmenType;

  unloadDll(hDll);
  return result;
}

/**
 * @brief Searches for an entry in the database using the GoRestApiWrapperLib DLL.
 *
 * @param data The FileData object to search for.
 * @return True if the search was successful, false otherwise.
 */
bool RestApiDLL::SearchEntry(FileData &data) {
  if (printDebug) std::cout << "RestApiDLL-Search: Searching for FileData struct in the database" << std::endl;
  HMODULE hDll = loadDll(L"GoRestApiWrapperLib.dll");
  if (!hDll) {
    Logs::writeToErrorLog("RestApiDLL-Search: Failed to load DLL");
    return false;
  }

  auto func = (SearchEntryFunc) GetProcAddress(hDll, "SearchEntry");
  if (!func) {
    Logs::writeToErrorLog("RestApiDLL-Search: Failed to get function address for SearchEntry");
    unloadDll(hDll);
    return false;
  }

  bool result = false;
  auto dbStruct = convertFileDataForSearch(data);
  if (printDebug) std::cout << "RestApiDLL-Search: Translated FileData struct for search" << std::endl;

  func(&dbStruct, &result);

  std::cout.flush();

  if (!result) {
    Logs::writeToErrorLog("RestApiDLL-Search: Failed to get FileData struct from the database");
    unloadDll(hDll);
    return false;
  }

  if (printDebug) std::cout << "RestApiDLL-Search: Successfully got FileData struct from the database" << std::endl;
  if (printConverterDebug) debugFileDataDB(dbStruct);
  if (printDebug) std::cout << "RestApiDLL-Search: Converting FileDataDB struct to FileData struct" << std::endl;

  data = convertDBStructToFileData(dbStruct);

  if (printConverterDebug) data.debugFileData();

  unloadDll(hDll);
  return true;
}

/**
 * @brief Replaces an entry in the database using the GoRestApiWrapperLib DLL.
 *
 * @param data The FileData object to replace.
 * @return True if the replacement was successful, false otherwise.
 */
bool RestApiDLL::ReplaceEntry(const FileData &data) {
  HMODULE hDll = loadDll(L"GoRestApiWrapperLib.dll");
  if (!hDll) {
    Logs::writeToErrorLog("RestApiDLL-Replace: Failed to load DLL");
    return false;
  }

  auto func = (ReplaceEntryFunc) GetProcAddress(hDll, "ReplaceEntry");
  if (!func) {
    Logs::writeToErrorLog("RestApiDLL-Replace: Failed to get function address for ReplaceEntry");
    unloadDll(hDll);
    return false;
  }

  bool result = false;
  auto dbStruct = convertFileDataToDBStruct(data);
  func(&dbStruct, &result);

  if (result) {
    Logs::writeToInfoLog("RestApiDLL-Replace: Successfully replaced FileData struct into the database");
  } else {
    Logs::writeToErrorLog("RestApiDLL-Replace: Failed to replace FileData struct into the database");
  }

  delete dbStruct.AlgorithmenType;

  unloadDll(hDll);
  return result;
}

/**
 * @brief Retrieves all FileData objects from the database using the GoRestApiWrapperLib DLL.
 *
 * @return A vector containing all FileData objects.
 */
std::vector<FileData> RestApiDLL::GetAllFileIDsAndEncryptedPaths() {
  HMODULE hDll = loadDll(L"GoRestApiWrapperLib.dll");
  if (!hDll) {
    Logs::writeToErrorLog("RestApiDLL-GetAll: Failed to load DLL");
    return {};
  }

  auto func = (GetAllFileIDsAndEncryptedPathsFunc) GetProcAddress(hDll, "GetAllFileIDsAndEncryptedPaths");
  if (!func) {
    Logs::writeToErrorLog("RestApiDLL-GetAll: Failed to get function address for GetAllFileIDsAndEncryptedPaths");
    unloadDll(hDll);
    return {};
  }

  FileDataDB* pFileDataList = nullptr;
  int listSize = 0;
  Logs::writeToInfoLog("RestApiDLL-GetAll: Getting all FileData structs from the database");
  func(&pFileDataList, &listSize);
  unloadDll(hDll);

  if (listSize == 0 || pFileDataList == nullptr) {
    Logs::writeToErrorLog("RestApiDLL-GetAll: No FileData structs found in the database");
    return {};
  }

  std::vector<FileData> FileDataList;
  Logs::writeToInfoLog("RestApiDLL-GetAll: Converting FileDataDB structs to FileData structs");
  for (int i = 0; i < listSize; i++) {
    FileData fileData = convertDBStructToFileData(pFileDataList[i]);
    Logs::writeToInfoLog("RestApiDLL-GetAll: Converted entry for file: " + fileData.EncryptedFilePath->string());
    FileDataList.push_back(fileData);
  }

  Logs::writeToInfoLog("RestApiDLL-GetAll: Completed converting FileDataDB structs to FileData structs");
  return FileDataList;
}

/**
 * @brief Gets the size of the database file using the GoRestApiWrapperLib DLL.
 *
 * @return The size of the database file, or -1 if an error occurred.
 */
int RestApiDLL::getDatabaseFileSize() {
  HMODULE hDll = loadDll(L"GoRestApiWrapperLib.dll");
  if (!hDll) {
    Logs::writeToErrorLog("RestApiDLL-GetFileSize: Failed to load DLL");
    return -1;
  }

  auto func = (GetDatabaseFileSizeFunc) GetProcAddress(hDll, "GetDatabaseFileSize");
  if (!func) {
    Logs::writeToErrorLog("RestApiDLL-GetFileSize: Failed to get function address for GetDatabaseFileSize");
    unloadDll(hDll);
    return -1;
  }

  int fileSize = 0;

  func(&fileSize);

  unloadDll(hDll);
  return fileSize;
}