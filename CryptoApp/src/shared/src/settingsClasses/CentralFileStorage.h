// CentralFileStorage.h
#ifndef CENTRALSTORAGE_H
#define CENTRALSTORAGE_H

#include <filesystem>

namespace fs = std::filesystem;

class CentralFileStorage {
public:
  /**
   * @brief Gets the singleton instance of CentralFileStorage.
   * @return The singleton instance.
   */
  static CentralFileStorage& getInstance() {
    static CentralFileStorage instance;
    return instance;
  }

  // Delete copy constructor and assignment operator to prevent copies
  CentralFileStorage(const CentralFileStorage&) = delete;
  void operator=(const CentralFileStorage&) = delete;

  /**
   * @brief Indicates if the central encrypted file storage is enabled.
   */
  static bool centralEncFileStorage;

  /**
   * @brief Indicates if the central decrypted file storage is enabled.
   */
  static bool centralDecFileStorage;

  /**
   * @brief Path to the central file storage.
   */
  static fs::path fileStoragePath;

  /**
   * @brief Moves a file to the central storage.
   *
   * @param filePath The path of the file to be moved.
   * @param isEncrypted A boolean indicating whether the file is encrypted.
   * @return True if the file was successfully moved, false otherwise.
   */
  bool moveFileToCentralStorage(const fs::path &filePath, bool isEncrypted);

private:
  /**
   * @brief Default constructor for CentralFileStorage.
   */
  CentralFileStorage() = default;

  /**
   * @brief Checks and creates the central file storage path and its subdirectories if they do not exist.
   */
  static void checkCentralFileStoragePath();

  /**
   * @brief Counter for the number of encrypted files.
   */
  static unsigned long long encFileCounter;

  /**
   * @brief Counter for the number of decrypted files.
   */
  static unsigned long long decFileCounter;
};

#endif //CENTRALSTORAGE_H