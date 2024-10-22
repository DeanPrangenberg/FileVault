package main

/*
#cgo LDFLAGS: -LD:/MyRepo/FileVault/cmake-build-release/bin -lCppCryptoLib
#include <stdlib.h>
#include <wchar.h>
#include <stdbool.h>

struct FileData {
  unsigned char *FileID;
  int fileIDLength;
  const wchar_t *AlgorithmenType;
  const wchar_t *OriginalFilePath;
  const wchar_t *EncryptedFilePath;
  const wchar_t *DecryptedFilePath;
  unsigned char *Key;
  int keyLength;
  unsigned char *Iv;
  int ivLength;
};

extern bool EncryptFileAes128(const wchar_t *originalFilePath, const wchar_t *encryptedFilePath,
                                     unsigned char *key, size_t keySize, unsigned char *iv, size_t ivSize);

extern bool DecryptFileAes128(const wchar_t *encryptedFilePath, const wchar_t *decryptedFilePath,
                                     unsigned char *key, size_t keySize, unsigned char *iv, size_t ivSize);

extern bool EncryptFileAes256(const wchar_t *originalFilePath, const wchar_t *encryptedFilePath,
                                     unsigned char *key, size_t keySize, unsigned char *iv, size_t ivSize);

extern bool DecryptFileAes256(const wchar_t *encryptedFilePath, const wchar_t *decryptedFilePath,
                                     unsigned char *key, size_t keySize, unsigned char *iv, size_t ivSize);
*/
import "C"
import (
 "fmt"
 "sync"
 "unsafe"
)

// Convert wchar_t* to Go string
func wcharToString(wstr *C.wchar_t) string {
 var result string
 for *wstr != 0 {
  result += string(*wstr)
  wstr = (*C.wchar_t)(unsafe.Pointer(uintptr(unsafe.Pointer(wstr)) + unsafe.Sizeof(*wstr)))
 }
 return result
}

// EncryptFiles encrypts files based on FileData
func EncryptFiles(fileInfos []C.struct_FileData) {
 var wg sync.WaitGroup

 for _, file := range fileInfos {
  wg.Add(1)
  go func(file C.struct_FileData) {
   defer wg.Done()

   var result C.bool
   if file.keyLength == 16 { // AES-128
    result = C.EncryptFileAes128(
     file.OriginalFilePath,
     file.EncryptedFilePath,
     file.Key,
     C.size_t(file.keyLength),
     file.Iv,
     C.size_t(file.ivLength),
    )
   } else if file.keyLength == 32 { // AES-256
    result = C.EncryptFileAes256(
     file.OriginalFilePath,
     file.EncryptedFilePath,
     file.Key,
     C.size_t(file.keyLength),
     file.Iv,
     C.size_t(file.ivLength),
    )
   }

   if result == C.bool(false) {
    fmt.Printf("Failed to encrypt file: %s\n", wcharToString(file.OriginalFilePath))
   } else {
    fmt.Printf("Successfully encrypted file: %s\n", wcharToString(file.OriginalFilePath))
   }
  }(file)
 }

 wg.Wait()
}

// DecryptFiles decrypts files based on FileData
func DecryptFiles(fileInfos []C.struct_FileData) {
 var wg sync.WaitGroup

 for _, file := range fileInfos {
  wg.Add(1)
  go func(file C.struct_FileData) {
   defer wg.Done()

   var result C.bool
   if file.keyLength == 16 { // AES-128
    result = C.DecryptFileAes128(
     file.EncryptedFilePath,
     file.DecryptedFilePath,
     file.Key,
     C.size_t(file.keyLength),
     file.Iv,
     C.size_t(file.ivLength),
    )
   } else if file.keyLength == 32 { // AES-256
    result = C.DecryptFileAes256(
     file.EncryptedFilePath,
     file.DecryptedFilePath,
     file.Key,
     C.size_t(file.keyLength),
     file.Iv,
     C.size_t(file.ivLength),
    )
   }

   if result == C.bool(false) {
    fmt.Printf("Failed to decrypt file: %s\n", wcharToString(file.EncryptedFilePath))
   } else {
    fmt.Printf("Successfully decrypted file: %s\n", wcharToString(file.EncryptedFilePath))
   }
  }(file)
 }

 wg.Wait()
}

//export EncryptFilesWrapper
func EncryptFilesWrapper(filesPtr *C.struct_FileData, numFiles C.int) {
 fileInfos := make([]C.struct_FileData, numFiles)
 for i := 0; i < int(numFiles); i++ {
  fileInfos[i] = *(*C.struct_FileData)(unsafe.Pointer(uintptr(unsafe.Pointer(filesPtr)) + uintptr(i)*unsafe.Sizeof(C.struct_FileData{})))
 }
 EncryptFiles(fileInfos)
}

//export DecryptFilesWrapper
func DecryptFilesWrapper(filesPtr *C.struct_FileData, numFiles C.int) {
 fileInfos := make([]C.struct_FileData, numFiles)
 for i := 0; i < int(numFiles); i++ {
  fileInfos[i] = *(*C.struct_FileData)(unsafe.Pointer(uintptr(unsafe.Pointer(filesPtr)) + uintptr(i)*unsafe.Sizeof(C.struct_FileData{})))
 }
 DecryptFiles(fileInfos)
}

func main() {}