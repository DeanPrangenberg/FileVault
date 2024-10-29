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

extern bool EncryptFileWrapper(const struct FileData *fileData);
extern bool DecryptFileWrapper(const struct FileData *fileData);
*/
import "C"
import (
 "fmt"
 "sync"
 "unsafe"
 "runtime"
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

// Convert []bool to []byte
func boolsToBytes(bools []bool) []byte {
    bytes := make([]byte, len(bools))
    for i, b := range bools {
        if b {
            bytes[i] = 1
        } else {
            bytes[i] = 0
        }
    }
    return bytes
}

// EncryptFiles encrypts files based on FileData and returns a slice of booleans indicating success or failure
func EncryptFiles(fileInfos []C.struct_FileData) []bool {
    var wg sync.WaitGroup
    results := make([]bool, len(fileInfos))

    for i, file := range fileInfos {
        wg.Add(1)
        go func(i int, file C.struct_FileData) {
            defer wg.Done()
            runtime.LockOSThread()
            defer runtime.UnlockOSThread()

            result := C.EncryptFileWrapper(&file)

            if result == C.bool(false) {
                fmt.Printf("Go: Failed to encrypt file: %s\n", wcharToString(file.OriginalFilePath))
                results[i] = false
            } else {
                fmt.Printf("Go: Successfully encrypted file: %s\n", wcharToString(file.OriginalFilePath))
                results[i] = true
            }
        }(i, file)
    }

    wg.Wait()
    return results
}

// DecryptFiles decrypts files based on FileData and returns a slice of booleans indicating success or failure
func DecryptFiles(fileInfos []C.struct_FileData) []bool {
    var wg sync.WaitGroup
    results := make([]bool, len(fileInfos))

    for i, file := range fileInfos {
        wg.Add(1)
        go func(i int, file C.struct_FileData) {
            defer wg.Done()
            runtime.LockOSThread()
            defer runtime.UnlockOSThread()

            result := C.DecryptFileWrapper(&file)

            if result == C.bool(false) {
                fmt.Printf("Go: Failed to decrypt file: %s\n", wcharToString(file.EncryptedFilePath))
                results[i] = false
            } else {
                fmt.Printf("Go: Successfully decrypted file: %s\n", wcharToString(file.EncryptedFilePath))
                results[i] = true
            }
        }(i, file)
    }

    wg.Wait()
    return results
}

//export EncryptFilesWrapper
func EncryptFilesWrapper(filesPtr *C.struct_FileData, numFiles C.int) *C.bool {
    fileInfos := make([]C.struct_FileData, numFiles)
    for i := 0; i < int(numFiles); i++ {
        fileInfos[i] = *(*C.struct_FileData)(unsafe.Pointer(uintptr(unsafe.Pointer(filesPtr)) + uintptr(i)*unsafe.Sizeof(C.struct_FileData{})))
    }
    results := EncryptFiles(fileInfos)
    resultBytes := boolsToBytes(results)
    return (*C.bool)(C.CBytes(resultBytes))
}

//export DecryptFilesWrapper
func DecryptFilesWrapper(filesPtr *C.struct_FileData, numFiles C.int) *C.bool {
    fileInfos := make([]C.struct_FileData, numFiles)
    for i := 0; i < int(numFiles); i++ {
        fileInfos[i] = *(*C.struct_FileData)(unsafe.Pointer(uintptr(unsafe.Pointer(filesPtr)) + uintptr(i)*unsafe.Sizeof(C.struct_FileData{})))
    }
    results := DecryptFiles(fileInfos)
    resultBytes := boolsToBytes(results)
    return (*C.bool)(C.CBytes(resultBytes))
}

func main() {}