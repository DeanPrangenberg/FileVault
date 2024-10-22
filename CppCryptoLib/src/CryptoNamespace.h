#ifndef CRYPTOLIB_H
#define CRYPTOLIB_H

#include <string>
#include <vector>
#include <filesystem>

#ifdef CRYPTOLIBRARY_EXPORTS
#define CRYPTOLIB_API __declspec(dllexport)
#else
#define CRYPTOLIB_API __declspec(dllimport)
#endif

namespace fs = std::filesystem;

namespace easyOpenSSL {
  // AES-128 encryption class
  class CRYPTOLIB_API AES128 {
  public:
    static bool encryptFile(const fs::path &originalFile, const fs::path &encryptedFile,
                            const std::vector<unsigned char> &KEY, const std::vector<unsigned char> &IV);

    static bool decryptFile(const fs::path &encryptedFile, const fs::path &decryptedFile,
                            const std::vector<unsigned char> &KEY, const std::vector<unsigned char> &IV);

  private:
    static void printError(const std::wstring &msg);
  };

  // AES-256 encryption class
  class CRYPTOLIB_API AES256 {
  public:
    static bool encryptFile(const fs::path &originalFile, const fs::path &encryptedFile,
                            const std::vector<unsigned char> &KEY, const std::vector<unsigned char> &IV);

    static bool decryptFile(const fs::path &encryptedFile, const fs::path &decryptedFile,
                            const std::vector<unsigned char> &KEY, const std::vector<unsigned char> &IV);

  private:
    static void printError(const std::wstring &msg);
  };

  class CRYPTOLIB_API SHA256 {
  public:
    static void hashFile(const wchar_t *filePath, unsigned char **fileID, int *fileIDLength);
  };

  // Key generation class for generating keys and initialization vectors (IV)
  class CRYPTOLIB_API KeyGen {
  public:
    static void generateKeyIv(const size_t &keySize, std::vector<unsigned char> &KEY, std::vector<unsigned char> &IV);
  };
}

// Wrapper functions for C compatibility
extern "C" {
struct FileData {
  unsigned char *FileID;
  int fileIDLength;
  std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>> AlgorithmenType;
  std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>> OriginalFilePath;
  wchar_t *EncryptedFilePath;
  wchar_t *DecryptedFilePath;
  unsigned char *Key;
  int keyLength;
  unsigned char *Iv;
  int ivLength;
};
CRYPTOLIB_API bool EncryptFileAes128(const wchar_t *originalFilePath, const wchar_t *encryptedFilePath,
                                     unsigned char *key, size_t keySize, unsigned char *iv, size_t ivSize);

CRYPTOLIB_API bool DecryptFileAes128(const wchar_t *encryptedFilePath, const wchar_t *decryptedFilePath,
                                     unsigned char *key, size_t keySize, unsigned char *iv, size_t ivSize);

CRYPTOLIB_API bool EncryptFileAes256(const wchar_t *originalFilePath, const wchar_t *encryptedFilePath,
                                     unsigned char *key, size_t keySize, unsigned char *iv, size_t ivSize);

CRYPTOLIB_API bool DecryptFileAes256(const wchar_t *encryptedFilePath, const wchar_t *decryptedFilePath,
                                     unsigned char *key, size_t keySize, unsigned char *iv, size_t ivSize);

CRYPTOLIB_API void GenerateKeyIv(size_t keySize, unsigned char *key, unsigned char *iv);

CRYPTOLIB_API void GenerateFileID(const wchar_t *filePath, unsigned char **fileID, int *fileIDLength);

}

#endif //CRYPTOLIB_H
