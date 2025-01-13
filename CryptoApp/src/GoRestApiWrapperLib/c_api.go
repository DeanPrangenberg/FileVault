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

//export ReplaceEntry
func ReplaceEntry(data *C.FileDataDB, result *C.bool) {
	*result = *makeRequest("http://localhost:8000/api/replace", data)
}

//export InsertEntry
func InsertEntry(data *C.FileDataDB, result *C.bool) {
	*result = *makeRequest("http://localhost:8000/api/insert", data)
}
