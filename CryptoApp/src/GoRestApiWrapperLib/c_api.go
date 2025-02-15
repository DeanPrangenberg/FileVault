package main

/*
#include <stdlib.h>
#include "filedataDB.h"
*/
import "C"
import (
  "bytes"
  "encoding/json"
  "fmt"
  "io/ioutil"
  "net/http"
  "runtime"
)

/**
 * ExportDatabase exports the database to a remote server and retrieves the data and key.
 *
 * @param data Pointer to a pointer to a C char where the data will be stored.
 * @param dataSize Pointer to a C int where the size of the data will be stored.
 * @param key Pointer to a pointer to a C char where the key will be stored.
 * @param keySize Pointer to a C int where the size of the key will be stored.
 * @param result Pointer to a C int where the result code will be stored.
 */
//export ExportDatabase
func ExportDatabase(data **C.char, dataSize *C.int, key **C.char, keySize *C.int, result *C.int) {
  resp, err := http.Get("http://localhost:8000/api/DBM/export")
  if err != nil {
    *data = nil
    *key = nil
    *result = C.int(1)
    return
  }
  defer resp.Body.Close()

  body, _ := ioutil.ReadAll(resp.Body)
  var jsonResponse map[string]string
  err = json.Unmarshal(body, &jsonResponse)
  if err != nil {
    *data = nil
    *key = nil
    *result = C.int(2)
    return
  }

  keyStr, keyExists := jsonResponse["Key"]
  dataStr, dataExists := jsonResponse["Data"]
  if !keyExists || !dataExists {
    *data = nil
    *key = nil
    *result = C.int(3)
    return
  }

  fmt.Println("Go body:", string(body))
  fmt.Println("Go Key:", keyStr)
  fmt.Println("Go Data:", dataStr)

  *data = C.CString(dataStr)
  *dataSize = C.int(len(dataStr))
  *key = C.CString(keyStr)
  *keySize = C.int(len(keyStr))

  runtime.KeepAlive(data)
  runtime.KeepAlive(dataSize)
  runtime.KeepAlive(key)
  runtime.KeepAlive(keySize)
  runtime.KeepAlive(result)

  *result = C.int(-1)
}

/**
 * InsertDatabase inserts data into the database on a remote server.
 *
 * @param data Pointer to a C char containing the data to insert.
 * @param key Pointer to a C char containing the key associated with the data.
 * @param result Pointer to a C bool where the result of the operation will be stored.
 */
//export InsertDatabase
func InsertDatabase(data *C.char, key *C.char, result *C.bool) {
  jsonData, err := json.Marshal(map[string]string{"Key": C.GoString(key), "Data": C.GoString(data)})
  if err != nil {
    falseVal := C.bool(false)
    *result = falseVal
    return
  }

  resp, err := http.Post("http://localhost:8000/api/DBM/insert", "application/json", bytes.NewBuffer(jsonData))
  if err != nil {
    falseVal := C.bool(false)
    *result = falseVal
    return
  }
  defer resp.Body.Close()

  body, _ := ioutil.ReadAll(resp.Body)
  if string(body) == "true" {
    trueVal := C.bool(true)
    *result = trueVal
  } else {
    falseVal := C.bool(false)
    *result = falseVal
  }

  runtime.KeepAlive(data)
  runtime.KeepAlive(key)
  runtime.KeepAlive(result)
}

/**
 * ReplaceDatabase replaces data in the database on a remote server.
 *
 * @param data Pointer to a C char containing the data to replace.
 * @param key Pointer to a C char containing the key associated with the data.
 * @param result Pointer to a C bool where the result of the operation will be stored.
 */
//export ReplaceDatabase
func ReplaceDatabase(data *C.char, key *C.char, result *C.bool) {
  jsonData, err := json.Marshal(map[string]string{"Key": C.GoString(key), "Data": C.GoString(data)})
  if err != nil {
    falseVal := C.bool(false)
    *result = falseVal
    return
  }

  resp, err := http.Post("http://localhost:8000/api/DBM/replace", "application/json", bytes.NewBuffer(jsonData))
  if err != nil {
    falseVal := C.bool(false)
    *result = falseVal
    return
  }
  defer resp.Body.Close()

  body, _ := ioutil.ReadAll(resp.Body)
  if string(body) == "true" {
    trueVal := C.bool(true)
    *result = trueVal
  } else {
    falseVal := C.bool(false)
    *result = falseVal
  }

  runtime.KeepAlive(data)
  runtime.KeepAlive(key)
  runtime.KeepAlive(result)
}

/**
 * ResetDatabase resets the database on a remote server.
 *
 * @param result Pointer to a C bool where the result of the operation will be stored.
 */
//export ResetDatabase
func ResetDatabase(result *C.bool) {
  resp, err := http.Get("http://localhost:8000/api/DBM/reset")
  if err != nil {
    falseVal := C.bool(false)
    *result = falseVal
    return
  }
  defer resp.Body.Close()

  body, _ := ioutil.ReadAll(resp.Body)
  if string(body) == "true" {
    trueVal := C.bool(true)
    *result = trueVal
  } else {
    falseVal := C.bool(false)
    *result = falseVal
  }

  runtime.KeepAlive(result)
}

/**
 * GetDatabaseFileSize retrieves the size of the database file from a remote server.
 *
 * @param result Pointer to a C int where the size of the database file will be stored.
 */
//export GetDatabaseFileSize
func GetDatabaseFileSize(result *C.int) {
  resp, err := http.Get("http://localhost:8000/api/getDBSize")
  if err != nil {
    *result = -1
    return
  }
  defer resp.Body.Close()

  body, _ := ioutil.ReadAll(resp.Body)
  var sizeInKB int
  err = json.Unmarshal(body, &sizeInKB)
  if err != nil {
    *result = -1
    return
  }

  *result = C.int(sizeInKB)
}

/**
 * DeleteEntry deletes an entry from the database on a remote server.
 *
 * @param data Pointer to a C FileDataDB struct containing the data to delete.
 * @param result Pointer to a C bool where the result of the operation will be stored.
 */
//export DeleteEntry
func DeleteEntry(data *C.FileDataDB, result *C.bool) {
  fileID := wcharToString(data.FileID)
  encryptionID := wcharToString(data.EncryptionID)

  jsonData, err := json.Marshal(map[string]string{"FileID": fileID, "EncryptionID": encryptionID})
  if err != nil {
    falseVal := C.bool(false)
    *result = falseVal
    return
  }

  resp, err := http.Post("http://localhost:8000/api/delete", "application/json", bytes.NewBuffer(jsonData))
  if err != nil {
    falseVal := C.bool(false)
    *result = falseVal
    return
  }
  defer resp.Body.Close()

  body, _ := ioutil.ReadAll(resp.Body)
  if string(body) == "true" {
    trueVal := C.bool(true)
    *result = trueVal
    return
  }

  falseVal := C.bool(false)
  *result = falseVal
}

/**
 * SearchEntry searches for an entry in the database on a remote server.
 *
 * @param data Pointer to a C FileDataDB struct containing the data to search for.
 * @param result Pointer to a C bool where the result of the operation will be stored.
 */
//export SearchEntry
func SearchEntry(data *C.FileDataDB, result *C.bool) {
  if data == nil {
    returnBool := C.bool(false)
    runtime.KeepAlive(returnBool)
    *result = returnBool
    return
  }

  fileID := wcharToString(data.FileID)
  encryptionID := wcharToString(data.EncryptionID)

  jsonData, err := json.Marshal(map[string]string{"FileID": fileID, "EncryptionID": encryptionID})
  if err != nil {
    returnBool := C.bool(false)
    runtime.KeepAlive(returnBool)
    *result = returnBool
    return
  }

  resp, err := http.Post("http://localhost:8000/api/search", "application/json", bytes.NewBuffer(jsonData))
  if err != nil {
    returnBool := C.bool(false)
    runtime.KeepAlive(returnBool)
    *result = returnBool
    return
  }
  defer resp.Body.Close()

  body, _ := ioutil.ReadAll(resp.Body)
  if string(body) == "false" {
    returnBool := C.bool(false)
    runtime.KeepAlive(returnBool)
    *result = returnBool
    fmt.Println("Entry not found for FileID:", fileID)
    return
  }

  var resultOutput GoFileData
  err = json.Unmarshal(body, &resultOutput)
  if err != nil {
    returnBool := C.bool(false)
    runtime.KeepAlive(returnBool)
    *result = returnBool
    return
  }

  if resultOutput.FileID != "" {
    cData := convertToCFileData(resultOutput)
    if cData == nil {
      returnBool := C.bool(false)
      runtime.KeepAlive(returnBool)
      *result = returnBool
      return
    }

    *data = *cData

    runtime.KeepAlive(cData)
    returnBool := C.bool(true)
    runtime.KeepAlive(returnBool)
    *result = returnBool
    fmt.Println("Found entry with FileID:", fileID)
  } else {
    returnBool := C.bool(false)
    runtime.KeepAlive(returnBool)
    *result = returnBool
    fmt.Println("Entry not found for FileID:", fileID)
  }
}

/**
 * GetAllFileIDsAndEncryptedPaths retrieves all file IDs and encrypted paths from the database on a remote server.
 *
 * @param result Pointer to a pointer to a C FileDataDB struct where the data will be stored.
 * @param count Pointer to a C int where the number of entries will be stored.
 */
//export GetAllFileIDsAndEncryptedPaths
func GetAllFileIDsAndEncryptedPaths(result **C.FileDataDB, count *C.int) {
  resp, err := http.Get("http://localhost:8000/api/getAllFileIDsAndEncryptedPaths")
  if err != nil {
    *result = nil
    *count = 0
    return
  }
  defer resp.Body.Close()

  body, _ := ioutil.ReadAll(resp.Body)
  var goData []GoFileData
  err = json.Unmarshal(body, &goData)
  if err != nil {
    *result = nil
    *count = 0
    return
  }

  cData := make([]C.FileDataDB, len(goData))
  for i, data := range goData {
    cData[i] = C.FileDataDB{
      FileID:            stringToWcharT(data.FileID),
      EncryptionID:      stringToWcharT(data.EncryptionID),
      EncryptedFilePath: stringToWcharT(data.EncryptedFilePath),
    }
  }

  // Pin the cData slice to prevent it from being moved by the garbage collector
  runtime.KeepAlive(cData)
  *result = &cData[0]
  *count = C.int(len(goData))
}

/**
 * ReplaceEntry replaces an entry in the database on a remote server.
 *
 * @param data Pointer to a C FileDataDB struct containing the data to replace.
 * @param result Pointer to a C bool where the result of the operation will be stored.
 */
//export ReplaceEntry
func ReplaceEntry(data *C.FileDataDB, result *C.bool) {
  *result = *makeRequest("http://localhost:8000/api/replace", data)
}

/**
 * InsertEntry inserts an entry into the database on a remote server.
 *
 * @param data Pointer to a C FileDataDB struct containing the data to insert.
 * @param result Pointer to a C bool where the result of the operation will be stored.
 */
//export InsertEntry
func InsertEntry(data *C.FileDataDB, result *C.bool) {
  *result = *makeRequest("http://localhost:8000/api/insert", data)
}
