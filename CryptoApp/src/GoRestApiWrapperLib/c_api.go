package main

/*
#include <stdlib.h>
#include "filedata.h"
*/
import "C"
import (
  "bytes"
  "encoding/json"
  "io/ioutil"
  "net/http"
  "unsafe"
)

//export InsertEntry
func InsertEntry(data *C.FileData, result *C.bool) {
  result = makeRequest("http://localhost:8000/api/insert", data)
}

//export DeleteEntry
func DeleteEntry(data *C.FileData, result **C.bool) {
  fileID := C.GoBytes(unsafe.Pointer(data.FileID), C.int(data.fileIDLength))
  jsonData, err := json.Marshal(map[string][]byte{"FileID": fileID})
  if err != nil {
    falseVal := C.bool(false)
    *result = &falseVal
    return
  }

  resp, err := http.Post("http://localhost:8000/api/delete", "application/json", bytes.NewBuffer(jsonData))
  if err != nil {
    falseVal := C.bool(false)
    *result = &falseVal
    return
  }
  defer resp.Body.Close()

  body, _ := ioutil.ReadAll(resp.Body)
  if string(body) == "true" {
    trueVal := C.bool(true)
    *result = &trueVal
    return
  }

  falseVal := C.bool(false)
  *result = &falseVal
}

//export SearchEntry
func SearchEntry(data *C.FileData) *C.FileData {
  fileID := C.GoBytes(unsafe.Pointer(data.FileID), C.int(data.fileIDLength))
  jsonData, err := json.Marshal(map[string][]byte{"FileID": fileID})
  if err != nil {
    return nil
  }

  resp, err := http.Post("http://localhost:8000/api/search", "application/json", bytes.NewBuffer(jsonData))
  if err != nil {
    return nil
  }
  defer resp.Body.Close()

  body, _ := ioutil.ReadAll(resp.Body)
  var result GoFileData
  err = json.Unmarshal(body, &result)
  if err != nil {
    return nil
  }

  return convertToCFileData(result)
}

//export ReplaceEntry
func ReplaceEntry(data *C.FileData, result *C.bool) {
  result = makeRequest("http://localhost:8000/api/replace", data)
}

//export GetAllFileIDsAndEncryptedPaths
func GetAllFileIDsAndEncryptedPaths(fileDataList ***C.FileData) {
  *fileDataList = getAllFileIDsAndEncryptedPaths()
}
