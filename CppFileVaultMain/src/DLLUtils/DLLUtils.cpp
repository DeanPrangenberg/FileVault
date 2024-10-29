#include "DLLUtils.h"
#include <iostream>
#include <sstream>
#include "../StructUtils/StructUtils.h"

void DLLUtils::logError(const std::string &message) {
  std::cerr << message << std::endl;
}

HMODULE DLLUtils::loadDll(const wchar_t *dllName) {
  HMODULE hDll = LoadLibraryW(dllName);
  if (!hDll) {
    std::wcerr << L"Failed to load DLL: " << dllName << std::endl;
  }
  return hDll;
}

void DLLUtils::unloadDll(HMODULE hDll) {
  if (hDll) {
    FreeLibrary(hDll);
  }
}

void DLLUtils::GenerateKeyIv(size_t size, unsigned char *key, unsigned char *iv) {
  HMODULE hCryptographyDll = loadDll(L"CppCryptoLib.dll");
  if (!hCryptographyDll) {
    return;
  }

  auto generateKeyIvFunc = (GenerateKeyIvFunc) GetProcAddress(hCryptographyDll, "GenerateKeyIv");
  if (!generateKeyIvFunc) {
    logError("Failed to get function address for GenerateKeyIv");
    unloadDll(hCryptographyDll);
    return;
  }

  generateKeyIvFunc(size, key, iv);
  unloadDll(hCryptographyDll);
}

void DLLUtils::EncryptFiles(struct FileData *fileData, int count, std::vector<bool> &results) {
  HMODULE hMultiThreadCryptoLib = loadDll(L"GoMultiThreadCryptoLib.dll");
  if (!hMultiThreadCryptoLib) {
    return;
  }

  auto encryptFilesFunc = (EncryptFilesFunc) GetProcAddress(hMultiThreadCryptoLib, "EncryptFilesWrapper");
  if (!encryptFilesFunc) {
    logError("Failed to get function address for EncryptFilesWrapper");
    unloadDll(hMultiThreadCryptoLib);
    return;
  }

  bool *resultArray = encryptFilesFunc(fileData, count);
  results.assign(resultArray, resultArray + count);

  unloadDll(hMultiThreadCryptoLib);
}

void DLLUtils::DecryptFiles(struct FileData *fileData, int count, std::vector<bool> &results) {
  HMODULE hMultiThreadCryptoLib = loadDll(L"GoMultiThreadCryptoLib.dll");
  if (!hMultiThreadCryptoLib) {
    return;
  }

  auto decryptFilesFunc = (DecryptFilesFunc) GetProcAddress(hMultiThreadCryptoLib, "DecryptFilesWrapper");
  if (!decryptFilesFunc) {
    logError("Failed to get function address for DecryptFilesWrapper");
    unloadDll(hMultiThreadCryptoLib);
    return;
  }

  bool *resultArray = decryptFilesFunc(fileData, count);
  results.assign(resultArray, resultArray + count);

  unloadDll(hMultiThreadCryptoLib);
}

void DLLUtils::GenerateFileID(const wchar_t *filePath, unsigned char *fileID) {
  if (!filePath || !fileID) {
    logError("Invalid file path or fileID buffer");
    return;
  }

  HMODULE hCryptographyDll = loadDll(L"CppCryptoLib.dll");
  if (!hCryptographyDll) {
    return;
  }

  auto generateFileIDFunc = (GenerateFileIDFunc) GetProcAddress(hCryptographyDll, "GenerateFileID");
  if (!generateFileIDFunc) {
    logError("Failed to get function address for GenerateFileID");
    unloadDll(hCryptographyDll);
    return;
  }

  generateFileIDFunc(filePath, fileID);
  unloadDll(hCryptographyDll);
}

void DLLUtils::WriteFileDataToJson(const std::vector<FileData>& fileDataList) {
  HMODULE hSaveFileOperationsDll = loadDll(L"CppSaveFileLib.dll");
  if (!hSaveFileOperationsDll) {
    return;
  }

  auto writeFileDataToJsonFunc = (WriteFileDataToJsonFunc) GetProcAddress(hSaveFileOperationsDll, "WriteFileDataToJson");
  if (!writeFileDataToJsonFunc) {
    logError("Failed to get function address for WriteFileDataToJson");
    unloadDll(hSaveFileOperationsDll);
    return;
  }

  writeFileDataToJsonFunc(&fileDataList[0], fileDataList.size(), globalDefinitions::jsonFileName);
  unloadDll(hSaveFileOperationsDll);
}

bool DLLUtils::FindAndCompleteStruct(FileData *partialStruct, const wchar_t *filePath) {
  HMODULE hSaveFileOperationsDll = loadDll(L"CppSaveFileLib.dll");
  if (!hSaveFileOperationsDll) {
    return false;
  }

  auto findAndCompleteStructFunc = (FindAndCompleteStructFunc) GetProcAddress(hSaveFileOperationsDll, "FindAndCompleteStruct");
  if (!findAndCompleteStructFunc) {
    logError("Failed to get function address for FindAndCompleteStruct");
    unloadDll(hSaveFileOperationsDll);
    return false;
  }

  bool result = findAndCompleteStructFunc(partialStruct, filePath);
  unloadDll(hSaveFileOperationsDll);
  return result;
}

bool DLLUtils::DeleteStructFromJson(const FileData *targetStruct, const wchar_t *filePath) {
  HMODULE hSaveFileOperationsDll = loadDll(L"CppSaveFileLib.dll");
  if (!hSaveFileOperationsDll) {
    return false;
  }

  auto deleteStructFromJsonFunc = (DeleteStructFromJsonFunc) GetProcAddress(hSaveFileOperationsDll, "DeleteStructFromJson");
  if (!deleteStructFromJsonFunc) {
    logError("Failed to get function address for DeleteStructFromJson");
    unloadDll(hSaveFileOperationsDll);
    return false;
  }

  bool result = deleteStructFromJsonFunc(targetStruct, filePath);
  unloadDll(hSaveFileOperationsDll);
  return result;
}

void DLLUtils::ExtractFileIDFromFile() {

}

std::vector<fs::path> DLLUtils::ScanDirectory(const fs::path& directory, const bool searchOnlyForDecryptedFiles) {
  HMODULE hDirectoryScannerDll = loadDll(L"CppDirectoryScannerLib.dll");
  if (!hDirectoryScannerDll) {
    return {};
  }

  auto scanDirectoryFunc = (ScanDirectoryFunc) GetProcAddress(hDirectoryScannerDll, "ScanForFilesInDirectory");
  if (!scanDirectoryFunc) {
    logError("Failed to get function address for GenerateFileID");
    unloadDll(hDirectoryScannerDll);
    return {};
  }

  const wchar_t **filePaths = scanDirectoryFunc(directory.wstring().c_str(), searchOnlyForDecryptedFiles);

  std::vector<fs::path> paths;
  for (size_t i = 0; filePaths[i] != nullptr; ++i) {
    paths.push_back(fs::path(filePaths[i]));
  }

  unloadDll(hDirectoryScannerDll);
  return paths;
}