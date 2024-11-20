package main

/*
#include <stdlib.h>
#include "filedata.h"
*/
import "C"
import (
  "fmt"
  "runtime"
  "strconv"
  "strings"
  "unsafe"
)

func wcharToString(wstr *C.wchar_t) string {
  var runes []rune
  for {
    r := *wstr
    if r == 0 {
      break
    }
    runes = append(runes, rune(r))
    wstr = (*C.wchar_t)(unsafe.Pointer(uintptr(unsafe.Pointer(wstr)) + unsafe.Sizeof(*wstr)))
  }
  return string(runes)
}

func convertToGoFileData(data *C.FileData) GoFileData {
  fileID, err := ConvertFileIDToBinaryString(data.FileID, data.fileIDLength)
  if err != nil {
    // handle error
  }
  key, err := ConvertFileIDToBinaryString(data.Key, data.keyLength)
  if err != nil {
    // handle error
  }
  iv, err := ConvertFileIDToBinaryString(data.Iv, data.ivLength)
  if err != nil {
    // handle error
  }

  return GoFileData{
    FileID:            fileID,
    FileIDLength:      int(data.fileIDLength),
    AlgorithmenType:   wcharToString(data.AlgorithmenType),
    OriginalFilePath:  wcharToString(data.OriginalFilePath),
    EncryptedFilePath: wcharToString(data.EncryptedFilePath),
    DecryptedFilePath: wcharToString(data.DecryptedFilePath),
    Key:               key,
    KeyLength:         int(data.keyLength),
    Iv:                iv,
    IvLength:          int(data.ivLength),
  }
}

func stringToWcharT(s string) *C.wchar_t {
  runes := []rune(s)
  wcharArray := make([]C.wchar_t, len(runes)+1)
  for i, r := range runes {
    wcharArray[i] = C.wchar_t(r)
  }
  wcharArray[len(runes)] = 0

  runtime.KeepAlive(wcharArray) // Ensure data is not garbage collected
  return &wcharArray[0]
}

// ConvertFileIDToBinaryString converts a C unsigned char array to a binary string
func ConvertFileIDToBinaryString(fileID *C.uchar, length C.size_t) (string, error) {
  if fileID == nil || length == 0 {
    return "", fmt.Errorf("invalid input")
  }

  var binaryString strings.Builder
  for i := 0; i < int(length); i++ {
    byteValue := *(*C.uchar)(unsafe.Pointer(uintptr(unsafe.Pointer(fileID)) + uintptr(i)))
    binaryString.WriteString(fmt.Sprintf("%08b", byteValue))
  }

  return binaryString.String(), nil
}

func convertToCFileData(data GoFileData) *C.FileData {
  fileID, fileIDLength, _ := ConvertBinaryStringToFileID(data.FileID)
  key, keyLength, _ := ConvertBinaryStringToFileID(data.Key)
  iv, ivLength, _ := ConvertBinaryStringToFileID(data.Iv)

  return &C.FileData{
    FileID:            fileID,
    fileIDLength:      fileIDLength,
    AlgorithmenType:   stringToWcharT(data.AlgorithmenType),
    OriginalFilePath:  stringToWcharT(data.OriginalFilePath),
    EncryptedFilePath: stringToWcharT(data.EncryptedFilePath),
    DecryptedFilePath: stringToWcharT(data.DecryptedFilePath),
    Key:               key,
    keyLength:         keyLength,
    Iv:                iv,
    ivLength:          ivLength,
  }
}

// ConvertBinaryStringToFileID converts a binary string to a C unsigned char array
func ConvertBinaryStringToFileID(binaryString string) (*C.uchar, C.size_t, error) {
  if len(binaryString)%8 != 0 {
    return nil, 0, fmt.Errorf("invalid binary string length")
  }
  fileIDLength := len(binaryString) / 8
  fileID := C.malloc(C.size_t(fileIDLength))
  if fileID == nil {
    return nil, 0, fmt.Errorf("memory allocation failed")
  }

  for i := 0; i < len(binaryString); i += 8 {
    byteValue, err := strconv.ParseUint(binaryString[i:i+8], 2, 8)
    if err != nil {
      C.free(unsafe.Pointer(fileID))
      return nil, 0, err
    }
    *(*C.uchar)(unsafe.Pointer(uintptr(fileID) + uintptr(i/8))) = C.uchar(byteValue)
  }
  return (*C.uchar)(fileID), C.size_t(fileIDLength), nil
}
