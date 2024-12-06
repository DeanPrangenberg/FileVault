package main

/*
#include <stdlib.h>
#include "filedataDB.h"
*/
import "C"
import (
  "runtime"
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

func convertToGoFileData(data *C.FileDataDB) GoFileData {
  var fileID, algorithmenType, originalFilePath, encryptedFilePath, decryptedFilePath, key, iv string
  var fileIDLength, keyLength, ivLength int

  if data != nil {
    if data.FileID != nil {
      fileID = wcharToString(data.FileID)
    }

    fileIDLength = int(data.FileIDLength)

    if data.AlgorithmenType != nil {
      algorithmenType = wcharToString(data.AlgorithmenType)
    }
    if data.OriginalFilePath != nil {
      originalFilePath = wcharToString(data.OriginalFilePath)
    }
    if data.EncryptedFilePath != nil {
      encryptedFilePath = wcharToString(data.EncryptedFilePath)
    }
    if data.DecryptedFilePath != nil {
      decryptedFilePath = wcharToString(data.DecryptedFilePath)
    }
    if data.Key != nil {
      key = wcharToString(data.Key)
    }

    keyLength = int(data.KeyLength)

    if data.Iv != nil {
      iv = wcharToString(data.Iv)
    }

    ivLength = int(data.IvLength)
  }

  return GoFileData{
    FileID:            fileID,
    FileIDLength:      fileIDLength,
    AlgorithmenType:   algorithmenType,
    OriginalFilePath:  originalFilePath,
    EncryptedFilePath: encryptedFilePath,
    DecryptedFilePath: decryptedFilePath,
    Key:               key,
    KeyLength:         keyLength,
    Iv:                iv,
    IvLength:          ivLength,
  }
}

func convertToCFileData(data GoFileData) *C.FileDataDB {
  return &C.FileDataDB{
    FileID:            stringToWcharT(data.FileID),
    FileIDLength:      C.size_t(data.FileIDLength),
    AlgorithmenType:   stringToWcharT(data.AlgorithmenType),
    OriginalFilePath:  stringToWcharT(data.OriginalFilePath),
    EncryptedFilePath: stringToWcharT(data.EncryptedFilePath),
    DecryptedFilePath: stringToWcharT(data.DecryptedFilePath),
    Key:               stringToWcharT(data.Key),
    KeyLength:         C.size_t(data.KeyLength),
    Iv:                stringToWcharT(data.Iv),
    IvLength:          C.size_t(data.IvLength),
  }
}
