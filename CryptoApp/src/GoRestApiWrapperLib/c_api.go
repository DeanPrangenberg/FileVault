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
	"unsafe"
)

//export DeleteEntry
func DeleteEntry(data *C.FileDataDB, result *C.bool) {
	fileID := wcharToString(data.FileID)

	jsonData, err := json.Marshal(map[string]string{"FileID": fileID})
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

	jsonData, err := json.Marshal(map[string]string{"FileID": fileID})
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
func GetAllFileIDsAndEncryptedPaths() **C.FileDataDB {
	resp, err := http.Get("http://localhost:8000/api/getAllFileIDsAndEncryptedPaths")
	if err != nil {
		return nil
	}
	defer resp.Body.Close()

	body, _ := ioutil.ReadAll(resp.Body)
	var goData []GoFileData
	err = json.Unmarshal(body, &goData)
	if err != nil {
		return nil
	}

	cData := make([]C.FileDataDB, len(goData))
	for i, data := range goData {
		cData[i] = C.FileDataDB{
			FileID:            stringToWcharT(data.FileID),
			AlgorithmenType:   stringToWcharT(data.AlgorithmenType),
			OriginalFilePath:  stringToWcharT(data.OriginalFilePath),
			EncryptedFilePath: stringToWcharT(data.EncryptedFilePath),
			DecryptedFilePath: stringToWcharT(data.DecryptedFilePath),
			Key:               stringToWcharT(data.Key),
			Iv:                stringToWcharT(data.Iv),
		}
	}

	return (**C.FileDataDB)(unsafe.Pointer(&cData[0]))
}

//export ReplaceEntry
func ReplaceEntry(data *C.FileDataDB, result *C.bool) {
	*result = *makeRequest("http://localhost:8000/api/replace", data)
}

//export InsertEntry
func InsertEntry(data *C.FileDataDB, result *C.bool) {
	*result = *makeRequest("http://localhost:8000/api/insert", data)
}
