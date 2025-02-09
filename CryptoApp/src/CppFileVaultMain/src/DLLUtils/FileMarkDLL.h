//
// Created by prang on 20.11.2024.
//

#ifndef FILEVAULTROOT_FILEMARKDLL_H
#define FILEVAULTROOT_FILEMARKDLL_H

#include "MasterDLLClass.h"

/**
 * @class FileMarkDLL
 * @brief A class for extracting FileID and EncryptionID from files using the CppFileMarkLib DLL.
 */
class FileMarkDLL : MasterDLLClass {
public:
  /**
   * @brief Extracts FileID and EncryptionID from a file using the CppFileMarkLib DLL.
   *
   * @param filePath The path of the file to extract IDs from.
   * @param FileID Buffer to store the extracted FileID.
   * @param EncryptionID Buffer to store the extracted EncryptionID.
   * @return True if the extraction was successful, false otherwise.
   */
  bool extractIDsFromFile(const fs::path* filePath, std::vector<unsigned char> *FileID,
                          std::vector<unsigned char> *EncryptionID);

private:
  /**
   * @brief Function pointer type for extracting IDs from a file.
   */
  typedef bool (*ExtractIDsFromFileFunc)(const fs::path*, std::vector<unsigned char> *, std::vector<unsigned char> *);
};

#endif //FILEVAULTROOT_FILEMARKDLL_H