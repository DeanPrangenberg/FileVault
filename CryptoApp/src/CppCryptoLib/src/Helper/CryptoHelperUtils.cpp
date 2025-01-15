//
// Created by prang on 23.10.2024.
//

#include "CryptoHelperUtils.h"

void CryptoHelperUtils::printError(const std::wstring &msg) {
  std::wcerr << msg << L" Error: " << ERR_reason_error_string(ERR_get_error()) << std::endl;
}

void CryptoHelperUtils::deleteFile(const fs::path &filePath) {
  try {
    if (filePath.extension() == globalDefinitions::encFileSuffix && globalDefinitions::deleteFileAfterDecryption) {
      std::cout << "Deleting original file after encryption: " << filePath << std::endl;
      fs::remove(filePath);
    } else if (globalDefinitions::deleteFileAfterDecryption) {
      std::cout << "Deleting encrypted file after decryption: " << filePath << std::endl;
      fs::remove(filePath);
    } else {
      std::cout << "Skipped file deletion for: " << filePath << std::endl;
      std::cout << (filePath.extension() == globalDefinitions::encFileSuffix ? "Encrypted File" : "Decrypted File") << std::endl;
      std::cout << "Delete after decryption: " << globalDefinitions::deleteFileAfterDecryption << std::endl;
      std::cout << "Delete after encryption: " << globalDefinitions::deleteFileAfterEncryption << std::endl;
    }
  } catch (const fs::filesystem_error &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

void CryptoHelperUtils::MarkFile(const class FileData *fileData) {
  HMODULE hFileMarkDll = loadDll(L"CppFileMarkLib.dll");
  if (!hFileMarkDll) {
    return;
  }

  auto markFileFunc = (MarkFileFunc) GetProcAddress(hFileMarkDll, "markFile");
  if (!markFileFunc) {
    logError("Failed to get function address for markFile");
    unloadDll(hFileMarkDll);
    return;
  }

  markFileFunc(fileData);
  unloadDll(hFileMarkDll);
}

bool CryptoHelperUtils::UnmarkFile(const class FileData *fileData) {
  HMODULE hFileMarkDll = loadDll(L"CppFileMarkLib.dll");
  if (!hFileMarkDll) {
    return false;
  }

  auto unmarkFileFunc = (UnmarkFileFunc) GetProcAddress(hFileMarkDll, "unmarkFile");
  if (!unmarkFileFunc) {
    logError("Failed to get function address for unmarkFile");
    unloadDll(hFileMarkDll);
    return false;
  }

  bool result = unmarkFileFunc(fileData);

  unloadDll(hFileMarkDll);

  return result;
}

std::vector<unsigned char> CryptoHelperUtils::getCurrentTime() {
  auto now = std::chrono::system_clock::now();
  auto duration = now.time_since_epoch();
  auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

  std::string ms_string = std::to_string(milliseconds);

  return std::vector<unsigned char>(ms_string.begin(), ms_string.end());
}

void CryptoHelperUtils::logError(const std::string &message) {
  std::cerr << message << std::endl;
}

HMODULE CryptoHelperUtils::loadDll(const wchar_t *dllName) {
  HMODULE hDll = LoadLibraryW(dllName);
  if (!hDll) {
    std::wcerr << L"Failed to load DLL: " << dllName << std::endl;
  }
  return hDll;
}

void CryptoHelperUtils::unloadDll(HMODULE hDll) {
  if (hDll) {
    FreeLibrary(hDll);
  }
}
