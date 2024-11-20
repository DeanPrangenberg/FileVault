#include "RestApiDLL.h"
#include <windows.h>

bool RestApiDLL::InsertEntry(const FileData &data) {
  HMODULE hDll = loadDll(L"GoRestApiWrapperLib.dll");
  if (!hDll) return false;

  auto func = (InsertEntryFunc)GetProcAddress(hDll, "InsertEntry");
  if (!func) {
    logError("Failed to get function address for InsertEntry");
    unloadDll(hDll);
    return false;
  }

  bool result = false;
  func(&data, &result);
  unloadDll(hDll);
  return result;
}

bool RestApiDLL::DeleteEntry(const FileData &data) {
  HMODULE hDll = loadDll(L"GoRestApiWrapperLib.dll");
  if (!hDll) return false;

  auto func = (DeleteEntryFunc)GetProcAddress(hDll, "DeleteEntry");
  if (!func) {
    logError("Failed to get function address for DeleteEntry");
    unloadDll(hDll);
    return false;
  }

  bool result = false;
  func(&data, &result);
  unloadDll(hDll);
  return result;
}

bool RestApiDLL::SearchEntry(FileData &data) {
  HMODULE hDll = loadDll(L"GoRestApiWrapperLib.dll");
  if (!hDll) {
    return false;
  }

  auto func = (SearchEntryFunc)GetProcAddress(hDll, "SearchEntry");
  if (!func) {
    logError("Failed to get function address for SearchEntry");
    unloadDll(hDll);
    return false;
  }

  bool result = false;
  func(&data, &result);
  if (!result) {
    logError("Failed to get FileData struct from the database");
    unloadDll(hDll);
    return false;
  }

  unloadDll(hDll);
  return true;
}

bool RestApiDLL::ReplaceEntry(const FileData &data) {
  HMODULE hDll = loadDll(L"GoRestApiWrapperLib.dll");
  if (!hDll) return false;

  auto func = (ReplaceEntryFunc)GetProcAddress(hDll, "ReplaceEntry");
  if (!func) {
    logError("Failed to get function address for ReplaceEntry");
    unloadDll(hDll);
    return false;
  }

  bool result = false;
  func(&data, &result);
  unloadDll(hDll);
  return result;
}

std::vector<FileData> RestApiDLL::GetAllFileIDsAndEncryptedPaths() {
  HMODULE hDll = loadDll(L"GoRestApiWrapperLib.dll");
  if (!hDll) return {};

  auto func = (GetAllFileIDsAndEncryptedPathsFunc)GetProcAddress(hDll, "GetAllFileIDsAndEncryptedPaths");
  if (!func) {
    logError("Failed to get function address for GetAllFileIDsAndEncryptedPaths");
    unloadDll(hDll);
    return {};
  }

  bool result = false;
  FileData** pFileDataList;
  func(pFileDataList, &result);
  unloadDll(hDll);

  if (!result) {
    logError("Failed to get all FileData structs from the database");
    return {};
  }

  std::vector<FileData> FileDataList;

  for (int i = 0; pFileDataList[i] != nullptr; i++) {
    pFileDataList[i]->FileID = new unsigned char[64];
    std::copy(pFileDataList[i]->FileID, pFileDataList[i]->FileID + pFileDataList[i]->fileIDLength, FileDataList[i].FileID);

  }

  return FileDataList;
}