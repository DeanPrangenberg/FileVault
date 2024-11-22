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

wchar_t* RestApiDLL::convertToWChar(const unsigned char* input, size_t size) {
  if (input == nullptr || size == 0) {
    return nullptr;
  }

  std::vector<wchar_t> buffer(size + 1, 0); // +1 for null terminator
  for (size_t i = 0; i < size; ++i) {
    buffer[i] = static_cast<wchar_t>(input[i]);
  }

  wchar_t* result = new wchar_t[buffer.size()];
  std::copy(buffer.begin(), buffer.end(), result);
  return result;
}

unsigned char* RestApiDLL::convertToUnsignedChar(const wchar_t* input, size_t size) {
  if (input == nullptr || size == 0) {
    return nullptr;
  }

  std::vector<unsigned char> buffer(size + 1, 0); // +1 for null terminator
  for (size_t i = 0; i < size; ++i) {
    buffer[i] = static_cast<unsigned char>(input[i]);
  }

  unsigned char* result = new unsigned char[buffer.size()];
  std::copy(buffer.begin(), buffer.end(), result);
  return result;
}

wchar_t* RestApiDLL::wcharToBinaryWChar(const wchar_t* input, size_t size) {
  if (input == nullptr || size == 0) {
    return nullptr;
  }

  std::wstring binaryString;
  for (size_t i = 0; i < size; ++i) {
    std::bitset<sizeof(wchar_t) * 8> bits(input[i]);
    binaryString += std::wstring(bits.to_string().begin(), bits.to_string().end());
  }

  wchar_t* result = new wchar_t[binaryString.size() + 1];
  std::copy(binaryString.begin(), binaryString.end(), result);
  result[binaryString.size()] = L'\0'; // null terminator
  return result;
}

wchar_t* RestApiDLL::binaryWCharToWChar(const wchar_t* input, size_t size) {
  if (input == nullptr || size == 0 || size % (sizeof(wchar_t) * 8) != 0) {
    return nullptr;
  }

  size_t numChars = size / (sizeof(wchar_t) * 8);
  wchar_t* result = new wchar_t[numChars + 1]; // +1 for null terminator

  for (size_t i = 0; i < numChars; ++i) {
    std::bitset<sizeof(wchar_t) * 8> bits;
    for (size_t j = 0; j < sizeof(wchar_t) * 8; ++j) {
      bits[j] = (input[i * sizeof(wchar_t) * 8 + j] == L'1');
    }
    result[i] = static_cast<wchar_t>(bits.to_ulong());
  }

  result[numChars] = L'\0'; // null terminator
  return result;
}

RestApiDLL::FileDataDB RestApiDLL::convertFileDataToDBStruct(const FileData &data) {
  RestApiDLL::FileDataDB dbStruct;
  dbStruct.FileID = wcharToBinaryWChar(convertToWChar(data.FileID, data.fileIDLength), data.fileIDLength);
  dbStruct.EncryptedFilePath = data.EncryptedFilePath;
  dbStruct.OriginalFilePath = data.OriginalFilePath;
  dbStruct.AlgorithmenType = data.AlgorithmenType;
  dbStruct.DecryptedFilePath = data.DecryptedFilePath;
  dbStruct.Key = wcharToBinaryWChar(convertToWChar(data.Key, data.keyLength), data.keyLength);
  dbStruct.Iv = wcharToBinaryWChar(convertToWChar(data.Iv, data.ivLength), data.ivLength);

  return dbStruct;
}

FileData RestApiDLL::convertDBStructToFileData(const FileDataDB &data) {
  auto normalFileIDwChar = binaryWCharToWChar(data.FileID, std::wcslen(data.FileID));
  auto normalKEYChar = binaryWCharToWChar(data.Key, std::wcslen(data.Key));
  auto normalIVwChar = binaryWCharToWChar(data.Iv, std::wcslen(data.Iv));

  FileData fileData;
  fileData.FileID = convertToUnsignedChar(normalFileIDwChar,std::wcslen(normalFileIDwChar));
  fileData.EncryptedFilePath = data.EncryptedFilePath;
  fileData.OriginalFilePath = data.OriginalFilePath;
  fileData.AlgorithmenType = data.AlgorithmenType;
  fileData.DecryptedFilePath = data.DecryptedFilePath;
  fileData.Key = convertToUnsignedChar(normalKEYChar,std::wcslen(normalKEYChar));
  fileData.Iv = convertToUnsignedChar(normalIVwChar,std::wcslen(normalIVwChar));

  return fileData;
}