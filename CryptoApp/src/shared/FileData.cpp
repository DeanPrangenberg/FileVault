//
// Created by prang on 06.12.2024.
//

#include "FileData.h"

bool FileData::operator==(const FileData& other) const {
  if (fileIDLength != other.fileIDLength ||
      EncryptionIDLength != other.EncryptionIDLength ||
      LastUpdateIDLength != other.LastUpdateIDLength ||
      keyLength != other.keyLength ||
      ivLength != other.ivLength) {
    return false;
  }

  if (memcmp(FileID, other.FileID, fileIDLength) != 0 ||
      memcmp(EncryptionID, other.EncryptionID, EncryptionIDLength) != 0 ||
      memcmp(LastUpdateID, other.LastUpdateID, LastUpdateIDLength) != 0 ||
      memcmp(Key, other.Key, keyLength) != 0 ||
      memcmp(Iv, other.Iv, ivLength) != 0) {
    return false;
  }

  if (wcscmp(AlgorithmenType, other.AlgorithmenType) != 0 ||
      wcscmp(OriginalFilePath, other.OriginalFilePath) != 0 ||
      wcscmp(EncryptedFilePath, other.EncryptedFilePath) != 0 ||
      wcscmp(DecryptedFilePath, other.DecryptedFilePath) != 0) {
    return false;
  }

  return true;
}

void FileData::safeDeleteUCharArray(unsigned char *&ptr, size_t &length) {
  try {
    if (ptr != nullptr) {
      std::memset(ptr, 0, length);
      delete[] ptr;
      ptr = nullptr;
      length = 0;
    } else {
      std::cout << "Attempted to delete a nullptr (unsigned char array)" << std::endl;
    }
  } catch (const std::exception &e) {
    std::cerr << "Exception during deletion of unsigned char array: " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Unknown exception during deletion of unsigned char array!" << std::endl;
  }
}

void FileData::safeDeleteWChar(wchar_t *&ptr) {
  try {
    if (ptr != nullptr) {
      delete[] ptr;
      ptr = nullptr;
    } else {
      std::cout << "Attempted to delete a nullptr (wchar_t array)" << std::endl;
    }
  } catch (const std::exception &e) {
    std::cerr << "Exception during deletion of wchar_t array: " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Unknown exception during deletion of wchar_t array!" << std::endl;
  }
}

void FileData::cleanupFileData() {
  this->debugFileData();
  std::cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << std::endl;
  std::cout << "$ Global-cleanupFileData: Cleaning up FileData" << std::endl;
  std::cout << "$ Global-cleanupFileData: Cleaning up FileID" << std::endl;
  safeDeleteUCharArray(this->FileID, this->fileIDLength);
  std::cout << "$ Global-cleanupFileData: Cleaning up AlgorithmenType" << std::endl;
  safeDeleteWChar(this->AlgorithmenType);
  std::cout << "$ Global-cleanupFileData: Cleaning up OriginalFilePath" << std::endl;
  safeDeleteWChar(this->OriginalFilePath);
  std::cout << "$ Global-cleanupFileData: Cleaning up EncryptedFilePath" << std::endl;
  safeDeleteWChar(this->EncryptedFilePath);
  std::cout << "$ Global-cleanupFileData: Cleaning up DecryptedFilePath" << std::endl;
  safeDeleteWChar(this->DecryptedFilePath);
  std::cout << "$ Global-cleanupFileData: Cleaning up Key" << std::endl;
  safeDeleteUCharArray(this->Key, this->keyLength);
  std::cout << "$ Global-cleanupFileData: Cleaning up Iv" << std::endl;
  safeDeleteUCharArray(this->Iv, this->ivLength);

  this->FileID = nullptr;
  this->fileIDLength = 0;
  this->EncryptionID = nullptr;
  this->EncryptionIDLength = 0;
  this->LastUpdateID = nullptr;
  this->LastUpdateIDLength = 0;
  this->AlgorithmenType = nullptr;
  this->OriginalFilePath = nullptr;
  this->EncryptedFilePath = nullptr;
  this->DecryptedFilePath = nullptr;
  this->Key = nullptr;
  this->keyLength = 0;
  this->Iv = nullptr;
  this->ivLength = 0;

  std::cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << std::endl;
}

std::string FileData::toHexString(const unsigned char *pUChar, size_t length) {
  if (pUChar == nullptr) {
    throw std::invalid_argument("Null pointer passed to toHexString");
  }

  // Optimierte String-Speicherreservierung
  std::ostringstream oss;
  oss << std::hex << std::setw(2) << std::setfill('0');

  for (size_t i = 0; i < length; ++i) {
    oss << static_cast<int>(pUChar[i]);
  }
  return oss.str();
}

void FileData::debugFileData() {
  std::cout << "Global-debugFileData: Converting FileData to hex strings" << std::endl;

  std::string fileIDstr;
  std::string keyStr;
  std::string ivStr;

  try {
    fileIDstr = toHexString(this->FileID, this->fileIDLength);
    keyStr = toHexString(this->Key, this->keyLength);
    ivStr = toHexString(this->Iv, this->ivLength);
  } catch (const std::invalid_argument &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  std::wstring fileIDwstr(fileIDstr.begin(), fileIDstr.end());
  std::wstring keyWstr(keyStr.begin(), keyStr.end());
  std::wstring ivWstr(ivStr.begin(), ivStr.end());

  std::wcout << L"-------------------------------------------------------------\n";
  std::wcout << L"- FileData Debug:\n";

  try {
    std::wcout << L"- FileID: " << (this->FileID != nullptr ? fileIDwstr : L"null") << "\n";
  } catch (const std::exception &e) {
    std::wcerr << "- Error converting FileID to hex string: " << e.what() << "\n";
  }

  std::cout << "- FileIDLength: " << this->fileIDLength << std::endl;

  std::wcout << L"- EncryptedFilePath: "
             << (this->EncryptedFilePath && *this->EncryptedFilePath != L'\0' ? this->EncryptedFilePath : L"null")
             << "\n";
  std::wcout << L"- OriginalFilePath: "
             << (this->OriginalFilePath && *this->OriginalFilePath != L'\0' ? this->OriginalFilePath : L"null") << "\n";
  std::wcout << L"- DecryptedFilePath: "
             << (this->DecryptedFilePath && *this->DecryptedFilePath != L'\0' ? this->DecryptedFilePath : L"null")
             << "\n";
  std::wcout << L"- AlgorithmenType: "
             << (this->AlgorithmenType && *this->AlgorithmenType != L'\0' ? this->AlgorithmenType : L"null") << "\n";

  try {
    std::wcout << L"- Key: " << (this->Key != nullptr ? keyWstr : L"null") << "\n";
  } catch (const std::exception &e) {
    std::wcerr << "- Error converting Key to hex string: " << e.what() << "\n";
  }

  std::cout << "- KeyLength: " << this->keyLength << std::endl;

  try {
    std::wcout << L"- Iv: " << (this->Iv != nullptr ? ivWstr : L"null") << "\n";
  } catch (const std::exception &e) {
    std::wcerr << "- Error converting Iv to hex string: " << e.what() << "\n";
  }

  std::cout << "- IvLength: " << this->ivLength << std::endl;

  std::wcout << L"-------------------------------------------------------------\n";
}

unsigned char *FileData::getFileId() const {
  return FileID;
}

void FileData::setFileId(unsigned char *fileId) {
  FileID = fileId;
}

size_t FileData::getFileIdLength() const {
  return fileIDLength;
}

void FileData::setFileIdLength(size_t fileIdLength) {
  fileIDLength = fileIdLength;
}

unsigned char *FileData::getEncryptionId() const {
  return EncryptionID;
}

void FileData::setEncryptionId(unsigned char *encryptionId) {
  EncryptionID = encryptionId;
}

size_t FileData::getEncryptionIdLength() const {
  return EncryptionIDLength;
}

void FileData::setEncryptionIdLength(size_t encryptionIdLength) {
  EncryptionIDLength = encryptionIdLength;
}

unsigned char *FileData::getLastUpdateId() const {
  return LastUpdateID;
}

void FileData::setLastUpdateId(unsigned char *lastUpdateId) {
  LastUpdateID = lastUpdateId;
}

size_t FileData::getLastUpdateIdLength() const {
  return LastUpdateIDLength;
}

void FileData::setLastUpdateIdLength(size_t lastUpdateIdLength) {
  LastUpdateIDLength = lastUpdateIdLength;
}

wchar_t *FileData::getAlgorithmenType() const {
  return AlgorithmenType;
}

void FileData::setAlgorithmenType(wchar_t *algorithmenType) {
  AlgorithmenType = algorithmenType;
}

wchar_t *FileData::getOriginalFilePath() const {
  return OriginalFilePath;
}

void FileData::setOriginalFilePath(wchar_t *originalFilePath) {
  OriginalFilePath = originalFilePath;
}

wchar_t *FileData::getEncryptedFilePath() const {
  return EncryptedFilePath;
}

void FileData::setEncryptedFilePath(wchar_t *encryptedFilePath) {
  EncryptedFilePath = encryptedFilePath;
}

wchar_t *FileData::getDecryptedFilePath() const {
  return DecryptedFilePath;
}

void FileData::setDecryptedFilePath(wchar_t *decryptedFilePath) {
  DecryptedFilePath = decryptedFilePath;
}

unsigned char *FileData::getKey() const {
  return Key;
}

void FileData::setKey(unsigned char *key) {
  Key = key;
}

size_t FileData::getKeyLength() const {
  return keyLength;
}

void FileData::setKeyLength(size_t keyLength) {
  FileData::keyLength = keyLength;
}

unsigned char *FileData::getIv() const {
  return Iv;
}

void FileData::setIv(unsigned char *iv) {
  Iv = iv;
}

size_t FileData::getIvLength() const {
  return ivLength;
}

void FileData::setIvLength(size_t ivLength) {
  FileData::ivLength = ivLength;
}
