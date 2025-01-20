package main

// #include "filedataDB.h"
import "C"
import (
  "bytes"
  "encoding/json"
  "fmt"
  "io/ioutil"
  "net/http"
)

/**
 * Makes an HTTP request to the specified URL with the given FileDataDB data.
 *
 * @param url The URL to send the request to.
 * @param data Pointer to the C FileDataDB struct containing the data to send.
 * @return Pointer to a C bool indicating the success of the request.
 */
func makeRequest(url string, data *C.FileDataDB) *C.bool {
  goData := convertToGoFileData(data)
  jsonData, err := json.Marshal(goData)
  if err != nil {
    fmt.Print("Error: " + err.Error())
    falseVal := C.bool(false)
    return &falseVal
  }

  resp, err := http.Post(url, "application/json", bytes.NewBuffer(jsonData))
  if err != nil {
    fmt.Print("Error: " + err.Error())
    falseVal := C.bool(false)
    return &falseVal
  }
  defer resp.Body.Close()

  body, _ := ioutil.ReadAll(resp.Body)

  if string(body) == "true" {
    trueVal := C.bool(true)
    return &trueVal
  }

  falseVal := C.bool(false)
  return &falseVal
}