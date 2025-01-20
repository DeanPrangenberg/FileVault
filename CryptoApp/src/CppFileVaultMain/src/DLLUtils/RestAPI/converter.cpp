//
// Created by prang on 01.01.2025.
//

#include <codecvt>
#include "RestApiDLL.h"

/**
 * @brief Converts an unsigned char array to a hex wchar_t string.
 *
 * @param input Pointer to the unsigned char array.
 * @param size Size of the array.
 * @return Pointer to the resulting hex wchar_t string.
 */
wchar_t *RestApiDLL::convertToHexWChar(const unsigned char *input, size_t size) {
  if (input == nullptr || size == 0) {
    std::cerr << "convertToHexWChar: Input is null or size is zero" << std::endl;
    return nullptr;
  }

  // Stream for hex representation
  std::wstringstream wss;
  for (size_t i = 0; i < size; ++i) {
    wss << std::setw(2) << std::setfill(L'0') << std::hex << static_cast<int>(input[i]);
  }

  // Save hex representation as wstring
  std::wstring hexStr = wss.str();

  // Allocate memory for wchar_t* (including null termination)
  wchar_t *result = new wchar_t[hexStr.size() + 1];
  std::wmemcpy(result, hexStr.c_str(), hexStr.size() + 1);  // Include null termination

  return result;
}

/**
 * @brief Converts a hex wchar_t string to an unsigned char array.
 *
 * @param input Pointer to the hex wchar_t string.
 * @param size Reference to the size of the resulting array.
 * @return Pointer to the resulting unsigned char array.
 */
std::shared_ptr<std::vector<unsigned char>> RestApiDLL::convertFromHexWChar(const wchar_t *input) {
  if (input == nullptr) {
    std::cerr << "convertFromHexWChar: Input is null" << std::endl;
    return nullptr;
  }

  size_t len = std::wcslen(input);
  if (len % 2 != 0) {
    std::cerr << "convertFromHexWChar: Invalid hex string length" << std::endl;
    return nullptr;
  }

  auto result = std::make_shared<std::vector<unsigned char>>(len / 2);  // Every two hex digits correspond to one byte

  for (size_t i = 0; i < result->size(); ++i) {
    unsigned int byte;
    std::swscanf(input + 2 * i, L"%2x", &byte);
    (*result)[i] = static_cast<unsigned char>(byte);
  }

  return result;
}

/**
 * @brief Converts FileData to a FileDataDB structure for search purposes.
 *
 * @param data The FileData object to convert.
 * @return The resulting FileDataDB structure.
 */
RestApiDLL::FileDataDB RestApiDLL::convertFileDataForSearch(const FileData &data) {
  RestApiDLL::FileDataDB dbStruct{};

  // Convert only FileID
  dbStruct.FileID = convertToHexWChar(data.FileID->data(), data.FileID->size());
  dbStruct.EncryptedID = convertToHexWChar(data.EncryptionID->data(), data.EncryptionID->size());

  return dbStruct;
}

wchar_t *RestApiDLL::convertStringToWChar(const std::string &str) {
  // Determine the required buffer size
  size_t size = mbstowcs(nullptr, str.c_str(), 0) + 1;
  std::vector<wchar_t> buffer(size);

  // Perform the conversion
  mbstowcs(buffer.data(), str.c_str(), size);

  // Allocate memory for the wchar_t* and copy the buffer
  auto result = new wchar_t[size];
  std::wmemcpy(result, buffer.data(), size);

  return result;
}

std::string RestApiDLL::wcharToString(const wchar_t *wstr) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
  return converter.to_bytes(wstr);
}

RestApiDLL::FileDataDB RestApiDLL::convertFileDataToDBStruct(const FileData &data) {
  // Convert and allocate memory for file paths
  auto originalFilePath = new wchar_t[data.OriginalFilePath->wstring().size() + 1];
  std::wcscpy(originalFilePath, data.OriginalFilePath->wstring().c_str());

  auto encryptedFilePath = new wchar_t[data.EncryptedFilePath->wstring().size() + 1];
  std::wcscpy(encryptedFilePath, data.EncryptedFilePath->wstring().c_str());

  auto decryptedFilePath = new wchar_t[data.DecryptedFilePath->wstring().size() + 1];
  std::wcscpy(decryptedFilePath, data.DecryptedFilePath->wstring().c_str());

  // Convert and allocate memory for other fields
  if (printDebug) std::cout << "Converting FileData to FileDataDB" << std::endl;
  RestApiDLL::FileDataDB dbStruct{};
  if (printDebug) std::cout << "Created FileDataDB" << std::endl;
  dbStruct.FileID = convertToHexWChar(data.FileID->data(), data.FileID->size());
  if (printDebug) std::cout << "Converted FileID" << std::endl;
  dbStruct.FileIDLength = data.FileID->size();
  if (printDebug) std::cout << "Set FileIDLength" << std::endl;
  dbStruct.EncryptedID = convertToHexWChar(data.EncryptionID->data(), data.EncryptionID->size());
  if (printDebug) std::cout << "Converted EncryptedID" << std::endl;
  dbStruct.EncryptedIDLength = data.EncryptionID->size();
  if (printDebug) std::cout << "Set EncryptedIDLength" << std::endl;
  dbStruct.LastUpdateID = convertToHexWChar(data.LastUpdateID->data(), data.LastUpdateID->size());
  if (printDebug) std::cout << "Converted LastUpdateID" << std::endl;
  dbStruct.LastUpdateIDLength = data.LastUpdateID->size();
  if (printDebug) std::cout << "Set LastUpdateIDLength" << std::endl;
  dbStruct.AlgorithmenType = convertStringToWChar(*data.AlgorithmenType);
  if (printDebug) std::cout << "Converted AlgorithmenType" << std::endl;
  dbStruct.OriginalFilePath = originalFilePath;
  if (printDebug) std::cout << "Set OriginalFilePath" << std::endl;
  dbStruct.EncryptedFilePath = encryptedFilePath;
  if (printDebug) std::cout << "Set EncryptedFilePath" << std::endl;
  dbStruct.DecryptedFilePath = decryptedFilePath;
  if (printDebug) std::cout << "Set DecryptedFilePath" << std::endl;
  dbStruct.Key = convertToHexWChar(data.Key->data(), data.Key->size());
  if (printDebug) std::cout << "Converted Key" << std::endl;
  dbStruct.KeyLength = data.Key->size();
  if (printDebug) std::cout << "Set KeyLength" << std::endl;
  dbStruct.Iv = convertToHexWChar(data.Iv->data(), data.Iv->size());
  if (printDebug) std::cout << "Converted Iv" << std::endl;
  dbStruct.IvLength = data.Iv->size();
  if (printDebug) std::cout << "Set IvLength" << std::endl;

  if (printConverterDebug) debugFileDataDB(dbStruct);

  return dbStruct;
}

/**
 * @brief Converts a FileDataDB structure to a FileData object.
 *
 * @param data The FileDataDB structure to convert.
 * @return The resulting FileData object.
 */
FileData RestApiDLL::convertDBStructToFileData(FileDataDB &data) {
  FileData fileData;

  if (printDebug) std::cout << "convertDBStructToFileData: Converting FileDataDB to FileData" << std::endl;

  try {
    if (data.AlgorithmenType == nullptr) {
      throw std::invalid_argument("convertDBStructToFileData: AlgorithmenType is null");
    }

    if (printDebug) std::cout << "convertDBStructToFileData: Converting AlgorithmenType" << std::endl;
    auto algotype = wcharToString(data.AlgorithmenType);

    if (algotype == "AES128") {
      if (printDebug) std::cout << "convertDBStructToFileData: AES128" << std::endl;
      fileData.Key->resize(16);
      fileData.Iv->resize(16);
    } else if (algotype == "AES256") {
      if (printDebug) std::cout << "convertDBStructToFileData: AES256" << std::endl;
      fileData.Key->resize(32);
      fileData.Iv->resize(16);
    } else {
      throw std::invalid_argument("convertDBStructToFileData: Unsupported algorithm type: " + algotype);
    }

    if (data.FileID == nullptr) {
      throw std::invalid_argument("convertDBStructToFileData: FileID is null");
    }

    if (printDebug) std::cout << "convertDBStructToFileData: Converting FileID" << std::endl;
    fileData.FileID = convertFromHexWChar(data.FileID);
    if (fileData.FileID->size() != data.FileIDLength) {
      throw std::runtime_error("convertDBStructToFileData: FileID length mismatch");
    }

    if (data.EncryptedID == nullptr) {
      throw std::invalid_argument("convertDBStructToFileData: EncryptedID is null");
    }

    if (printDebug) std::cout << "convertDBStructToFileData: Converting EncryptedID" << std::endl;
    fileData.EncryptionID = convertFromHexWChar(data.EncryptedID);
    if (fileData.EncryptionID->size() != data.EncryptedIDLength) {
      throw std::runtime_error("convertDBStructToFileData: EncryptedID length mismatch");
    }

    if (data.LastUpdateID == nullptr) {
      throw std::invalid_argument("convertDBStructToFileData: LastUpdateID is null");
    }

    if (printDebug) std::cout << "convertDBStructToFileData: Converting LastUpdateID" << std::endl;
    fileData.LastUpdateID = convertFromHexWChar(data.LastUpdateID);
    if (fileData.LastUpdateID->size() != data.LastUpdateIDLength) {
      throw std::runtime_error("convertDBStructToFileData: LastUpdateID length mismatch");
    }

    if (data.Key == nullptr) {
      throw std::invalid_argument("convertDBStructToFileData: Key is null");
    }

    if (printDebug) std::cout << "convertDBStructToFileData: Converting Key" << std::endl;
    fileData.Key = convertFromHexWChar(data.Key);
    if (fileData.Key->size() != data.KeyLength) {
      throw std::runtime_error("convertDBStructToFileData: Key length mismatch");
    }

    if (data.Iv == nullptr) {
      throw std::invalid_argument("convertDBStructToFileData: Iv is null");
    }

    if (printDebug) std::cout << "convertDBStructToFileData: Converting Iv" << std::endl;
    fileData.Iv = convertFromHexWChar(data.Iv);
    if (fileData.Iv->size() != data.IvLength) {
      throw std::runtime_error("convertDBStructToFileData: Iv length mismatch");
    }

    if (printDebug) std::cout << "convertDBStructToFileData: Converting other fields" << std::endl;
    fileData.AlgorithmenType->assign(algotype);
    fileData.EncryptedFilePath->assign(fs::path(data.EncryptedFilePath));
    fileData.OriginalFilePath->assign(fs::path(data.OriginalFilePath));
    fileData.DecryptedFilePath->assign(fs::path(data.DecryptedFilePath));

  } catch (const std::exception &e) {
    std::cerr << "convertDBStructToFileData: " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "convertDBStructToFileData: Unknown error" << std::endl;
  }

  if (printDebug) std::cout << "convertDBStructToFileData: Converted FileDataDB to FileData" << std::endl;

  return fileData;
}

/**
 * @brief Prints debug information for a FileDataDB structure.
 *
 * @param data The FileDataDB structure to debug.
 */
void RestApiDLL::debugFileDataDB(const FileDataDB &data) {
  if (printDebug) {
    std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
    std::wcout << L"+ FileDataDB: " << std::endl;

    if (data.FileID != nullptr) {
      try {
        std::wcout << L"+ FileID: " << data.FileID << std::endl;
      } catch (const std::exception &e) {
        std::wcerr << L"+ Error accessing FileID: " << e.what() << std::endl;
      }
    } else {
      std::wcout << L"+ FileID: null" << std::endl;
    }

    std::cout << "+ FileIDLength: " << data.FileIDLength << std::endl;

    if (data.EncryptedID != nullptr) {
      try {
        std::wcout << L"+ EncryptedID: " << data.EncryptedID << std::endl;
      } catch (const std::exception &e) {
        std::wcerr << L"+ Error accessing EncryptedID: " << e.what() << std::endl;
      }
    } else {
      std::wcout << L"+ EncryptedID: null" << std::endl;
    }
    std::cout << "+ EncryptedIDLength: " << data.EncryptedIDLength << std::endl;

    if (data.LastUpdateID != nullptr) {
      try {
        std::wcout << L"+ LastUpdateID: " << data.LastUpdateID << std::endl;
      } catch (const std::exception &e) {
        std::wcerr << L"+ Error accessing LastUpdateID: " << e.what() << std::endl;
      }
    } else {
      std::wcout << L"+ LastUpdateID: null" << std::endl;
    }

    if (data.AlgorithmenType != nullptr) {
      try {
        std::wcout << L"+ AlgorithmenType: " << data.AlgorithmenType << std::endl;
      } catch (const std::exception &e) {
        std::wcerr << L"+ Error accessing AlgorithmenType: " << e.what() << std::endl;
      }
    } else {
      std::wcout << L"+ AlgorithmenType: null" << std::endl;
    }

    if (data.OriginalFilePath != nullptr) {
      try {
        std::wcout << L"+ OriginalFilePath: " << data.OriginalFilePath << std::endl;
      } catch (const std::exception &e) {
        std::wcerr << L"+ Error accessing OriginalFilePath: " << e.what() << std::endl;
      }
    } else {
      std::wcout << L"+ OriginalFilePath: null" << std::endl;
    }

    if (data.EncryptedFilePath != nullptr) {
      try {
        std::wcout << L"+ EncryptedFilePath: " << data.EncryptedFilePath << std::endl;
      } catch (const std::exception &e) {
        std::wcerr << L"+ Error accessing EncryptedFilePath: " << e.what() << std::endl;
      }
    } else {
      std::wcout << L"+ EncryptedFilePath: null" << std::endl;
    }

    if (data.DecryptedFilePath != nullptr) {
      try {
        std::wcout << L"+ DecryptedFilePath: " << data.DecryptedFilePath << std::endl;
      } catch (const std::exception &e) {
        std::wcerr << L"+ Error accessing DecryptedFilePath: " << e.what() << std::endl;
      }
    } else {
      std::wcout << L"+ DecryptedFilePath: null" << std::endl;
    }

    if (data.Key != nullptr) {
      try {
        std::wcout << L"+ Key: " << data.Key << std::endl;
      } catch (const std::exception &e) {
        std::wcerr << L"+ Error accessing Key: " << e.what() << std::endl;
      }
    } else {
      std::wcout << L"+ Key: null" << std::endl;
    }

    std::cout << "+ KeyLength: " << data.KeyLength << std::endl;

    if (data.Iv != nullptr) {
      try {
        std::wcout << L"+ Iv: " << data.Iv << std::endl;
      } catch (const std::exception &e) {
        std::wcerr << L"+ Error accessing Iv: " << e.what() << std::endl;
      }
    } else {
      std::wcout << L"+ Iv: null" << std::endl;
    }

    std::cout << "+ IvLength: " << data.IvLength << std::endl;

    std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
  }
}