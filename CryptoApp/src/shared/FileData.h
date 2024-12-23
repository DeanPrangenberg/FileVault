//
// Created by prang on 06.12.2024.
//

#ifndef FILEVAULTROOT_FILEDATA_H
#define FILEVAULTROOT_FILEDATA_H

#include <iostream>
#include <iomanip>
#include <sstream>

class FileData {
public:
  void cleanupFileData();
  void debugFileData();

  unsigned char *getFileId() const;
  void setFileId(unsigned char *fileId);
  size_t getFileIdLength() const;
  void setFileIdLength(size_t fileIdLength);
  unsigned char *getEncryptionId() const;
  void setEncryptionId(unsigned char *encryptionId);
  size_t getEncryptionIdLength() const;
  void setEncryptionIdLength(size_t encryptionIdLength);
  unsigned char *getLastUpdateId() const;
  void setLastUpdateId(unsigned char *lastUpdateId);
  size_t getLastUpdateIdLength() const;
  void setLastUpdateIdLength(size_t lastUpdateIdLength);
  wchar_t *getAlgorithmenType() const;
  void setAlgorithmenType(wchar_t *algorithmenType);
  wchar_t *getOriginalFilePath() const;
  void setOriginalFilePath(wchar_t *originalFilePath);
  wchar_t *getEncryptedFilePath() const;
  void setEncryptedFilePath(wchar_t *encryptedFilePath);
  wchar_t *getDecryptedFilePath() const;
  void setDecryptedFilePath(wchar_t *decryptedFilePath);
  unsigned char *getKey() const;
  void setKey(unsigned char *key);
  size_t getKeyLength() const;
  void setKeyLength(size_t keyLength);
  unsigned char *getIv() const;
  void setIv(unsigned char *iv);
  size_t getIvLength() const;
  void setIvLength(size_t ivLength);

  bool operator==(const FileData& other) const;

private:
  bool printDebug = false;
  void safeDeleteUCharArray(unsigned char *&ptr, size_t &length);
  void safeDeleteWChar(wchar_t *&ptr);
  std::string toHexString(const unsigned char *pUChar, size_t length);

  unsigned char *FileID = nullptr;
  size_t fileIDLength = 0;
  unsigned char *EncryptionID = nullptr;
  size_t EncryptionIDLength = 0;
  unsigned char *LastUpdateID = nullptr;
  size_t LastUpdateIDLength = 0;
  wchar_t *AlgorithmenType = nullptr;
  wchar_t *OriginalFilePath = nullptr;
  wchar_t *EncryptedFilePath = nullptr;
  wchar_t *DecryptedFilePath = nullptr;
  unsigned char *Key = nullptr;
  size_t keyLength = 0;
  unsigned char *Iv = nullptr;
  size_t ivLength = 0;
};


#endif //FILEVAULTROOT_FILEDATA_H
