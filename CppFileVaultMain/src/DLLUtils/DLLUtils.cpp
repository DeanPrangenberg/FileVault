#include "DLLUtils.h"
#include <iostream>

std::vector<std::string> DLLUtils::SplitString(const std::string& str, char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  for (char ch : str) {
    if (ch == delimiter) {
      if (!token.empty()) {
        tokens.push_back(token);
      }
      token.clear();
    } else {
      token += ch;
    }
  }
  if (!token.empty()) {
    tokens.push_back(token);
  }
  return tokens;
}

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

void DLLUtils::EncryptFiles(struct FileData *fileData, int count) {
  HMODULE hRustDll = loadDll(L"GoMultiThreadCryptoLib.dll");
  if (!hRustDll) {
    return;
  }

  auto encryptFilesFunc = (EncryptFilesFunc) GetProcAddress(hRustDll, "EncryptFilesWrapper");
  if (!encryptFilesFunc) {
    logError("Failed to get function address for EncryptFilesWrapper");
    unloadDll(hRustDll);
    return;
  }

  encryptFilesFunc(fileData, count);
  unloadDll(hRustDll);
}

void DLLUtils::DecryptFiles(struct FileData *fileData, int count) {
  HMODULE hRustDll = loadDll(L"GoMultiThreadCryptoLib.dll");
  if (!hRustDll) {
    return;
  }

  auto decryptFilesFunc = (DecryptFilesFunc) GetProcAddress(hRustDll, "DecryptFilesWrapper");
  if (!decryptFilesFunc) {
    logError("Failed to get function address for DecryptFilesWrapper");
    unloadDll(hRustDll);
    return;
  }

  decryptFilesFunc(fileData, count);
  unloadDll(hRustDll);
}

void DLLUtils::GenerateFileID(const wchar_t *filePath, unsigned char **fileID, int *fileIDLength) {
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

  generateFileIDFunc(filePath, fileID, fileIDLength);
  unloadDll(hCryptographyDll);
}

void DLLUtils::WriteFileDataToJson(const std::vector<FileData>& fileDataList) {
  HMODULE hSaveFileOperationsDll = loadDll(L"CppSaveFileLib.dll");
  if (!hSaveFileOperationsDll) {
    return;
  }

  auto writeFileDataToJsonFunc = (WriteFileDataToJsonFunc) GetProcAddress(hSaveFileOperationsDll, "writeFileDataToJson");
  if (!writeFileDataToJsonFunc) {
    logError("Failed to get function address for WriteFileDataToJson");
    unloadDll(hSaveFileOperationsDll);
    return;
  }

  writeFileDataToJsonFunc(&fileDataList[0], fileDataList.size(), globalDefinitions::jsonFileName);
  unloadDll(hSaveFileOperationsDll);
}

std::vector<std::string> DLLUtils::ProcessFileWithCsScanner(const char *directoryPath) {
  HMODULE hCsScannerDll = loadDll(L"FileScanner.dll");
  if (!hCsScannerDll) {
    logError("Failed to open FileScanner.dll");
    return {"ERROR"};
  }

  auto fileScannerFunc = (ScanDirectoryFunc) GetProcAddress(hCsScannerDll, "ScanDirectory");
  auto freeFileScannerMemoryFunc = (FreeMemoryFunc) GetProcAddress(hCsScannerDll, "FreeMemory");

  if (!fileScannerFunc && !freeFileScannerMemoryFunc) {
    logError("Failed to get function pointer for FileScanner and FreeFileScannerMemory");
    FreeLibrary(hCsScannerDll);
    return {"ERROR"};
  }

  if (!fileScannerFunc) {
    logError("Failed to get function pointer for FileScanner");
    FreeLibrary(hCsScannerDll);
    return {"ERROR"};
  }

  if (!freeFileScannerMemoryFunc) {
    logError("Failed to get function pointer for FreeFileScannerMemory");
    FreeLibrary(hCsScannerDll);
    return {"ERROR"};
  }

  const char* result = fileScannerFunc(directoryPath);

  if (result) {
    // Split the result string by the delimiter ('|') to get file list
    std::string filesStr(result);
    std::vector<std::string> files = SplitString(filesStr, '|');

    /*// Free the memory allocated by the C# function
    freeFileScannerMemoryFunc(result);*/

    return files;
  } else {
    std::cout << "No files found or error occurred." << std::endl;
    return {"NO FILES FOUND"};
  }

  unloadDll(hCsScannerDll);
}