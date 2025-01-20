#include "FileData.h"

FileData::FileData() {
  FileID = std::make_shared<std::vector<unsigned char>>(64);
  EncryptionID = std::make_shared<std::vector<unsigned char>>(64);
  LastUpdateID = std::make_shared<std::vector<unsigned char>>(64);
  AlgorithmenType = std::make_shared<std::string>();
  OriginalFilePath = std::make_shared<fs::path>();
  EncryptedFilePath = std::make_shared<fs::path>();
  DecryptedFilePath = std::make_shared<fs::path>();
  Key = std::make_shared<std::vector<unsigned char>>();
  Iv = std::make_shared<std::vector<unsigned char>>();
}

bool FileData::operator==(const FileData &other) const {
  if (*FileID != *other.FileID ||
      *EncryptionID != *other.EncryptionID) {
    return false;
  }

  return true;
}

std::string FileData::toHexString(const std::shared_ptr<std::vector<unsigned char>> &data) {
  if (!data || data->empty()) {
    throw std::invalid_argument("Null or empty data passed to toHexString");
  }

  std::ostringstream oss;
  oss << std::hex << std::setw(2) << std::setfill('0');

  for (const auto &byte: *data) {
    oss << static_cast<int>(byte);
  }
  return oss.str();
}

void FileData::debugFileData() {
  if (printDebug) {
    std::cout << "Global-debugFileData: Converting FileData to hex strings" << std::endl;

    std::string fileIDstr;
    std::string encryptionIDstr;
    std::string lastUpdateIDstr;
    std::string keyStr;
    std::string ivStr;

    try {
      fileIDstr = toHexString(this->FileID);
      encryptionIDstr = toHexString(this->EncryptionID);
      lastUpdateIDstr = toHexString(this->LastUpdateID);
      keyStr = toHexString(this->Key);
      ivStr = toHexString(this->Iv);
    } catch (const std::invalid_argument &e) {
      std::cerr << "Error: " << e.what() << std::endl;
    }

    std::wstring fileIDwstr(fileIDstr.begin(), fileIDstr.end());
    std::wstring encryptionIDwstr(encryptionIDstr.begin(), encryptionIDstr.end());
    std::wstring lastUpdateIDwstr(lastUpdateIDstr.begin(), lastUpdateIDstr.end());
    std::wstring keyWstr(keyStr.begin(), keyStr.end());
    std::wstring ivWstr(ivStr.begin(), ivStr.end());

    std::wcout << L"-------------------------------------------------------------" << std::endl;
    std::wcout << L"- FileData Debug:" << std::endl;
    std::wcout << L"- FileID: " << (this->FileID ? fileIDwstr : L"null") << std::endl;
    std::wcout << L"- EncryptionID: " << (this->EncryptionID ? encryptionIDwstr : L"null") << std::endl;
    std::wcout << L"- LastUpdateID: " << (this->LastUpdateID ? lastUpdateIDwstr : L"null") << std::endl;
    if (!this->EncryptedFilePath->empty()) std::wcout << this->EncryptedFilePath->wstring() << std::endl;
    if (!this->OriginalFilePath->empty()) std::wcout << this->OriginalFilePath->wstring() << std::endl;
    if (!this->DecryptedFilePath->empty()) std::wcout << this->DecryptedFilePath->wstring() << std::endl;
    if (!this->AlgorithmenType->empty()) std::cout << this->AlgorithmenType << std::endl;
    std::wcout << L"- Key: " << (this->Key ? keyWstr : L"null") << std::endl;
    std::wcout << L"- Iv: " << (this->Iv ? ivWstr : L"null") << std::endl;
    std::wcout << L"------------------------------------------------------------" << std::endl;
  }
}
