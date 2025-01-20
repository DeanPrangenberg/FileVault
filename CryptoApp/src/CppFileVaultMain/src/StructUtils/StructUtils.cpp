#include "StructUtils.h"

namespace fs = std::filesystem;

/**
 * @brief Converts an AlgorithmType enum to a string representation.
 * @param type The AlgorithmType enum value.
 * @return A string representing the algorithm type.
 */
std::string StructUtils::AlgorithmTypeToString(globalDefinitions::AlgorithmType type) {
  switch (type) {
    case globalDefinitions::AlgorithmType::AES128:
      return "AES128";
    case globalDefinitions::AlgorithmType::AES256:
      return "AES256";
    default:
      return "Unknown";
  }
}

/**
 * @brief Creates a FileData structure for the given algorithm type and original file path.
 * @param algorithmenType The algorithm type to be used for encryption.
 * @param originalFilePath The path to the original file.
 * @return A FileData structure containing the file's metadata and encryption information.
 * @throws std::invalid_argument if the algorithm type is unsupported.
 */
FileData StructUtils::createFileDataStruct(const globalDefinitions::AlgorithmType &algorithmenType, const fs::path &originalFilePath) {
  CryptoDLL cryptoDll;
  FileData fileData;

  int keyLength = 0;
  int ivLength = 0;

  switch (algorithmenType) {
    case globalDefinitions::AlgorithmType::AES128:
      keyLength = 16;
      ivLength = 16;
      break;
    case globalDefinitions::AlgorithmType::AES256:
      keyLength = 32;
      ivLength = 16;
      break;
    default:
      throw std::invalid_argument("Unsupported algorithm type");
  }

  fileData.Key->resize(keyLength);
  fileData.Iv->resize(ivLength);

  std::wstring encryptedFilePath = originalFilePath.wstring() + globalDefinitions::encFileSuffix;
  std::string algorithmenTypeStr = AlgorithmTypeToString(algorithmenType);

  cryptoDll.GenerateFileID(originalFilePath.wstring().c_str(), fileData.FileID->data());

  // encryptionID gets generated in the DLL
  cryptoDll.getCurrentTimeHash(fileData.LastUpdateID->data());
  fileData.AlgorithmenType->assign(algorithmenTypeStr);
  fileData.OriginalFilePath->assign(originalFilePath);
  fileData.EncryptedFilePath->assign(fs::path (encryptedFilePath));
  fileData.DecryptedFilePath->assign(originalFilePath);
  cryptoDll.GenerateKeyIv(keyLength, ivLength, fileData.Key->data(), fileData.Iv->data());

  fileData.debugFileData();

  return fileData;
}