package main

/*
#include <stdlib.h>
#include "filedata.h"
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

//export DeleteEntry
func DeleteEntry(data *C.FileData, result *C.bool) {
	fileID, err := ConvertFileIDToBinaryString(data.FileID, data.fileIDLength)
	if err != nil {
		falseVal := C.bool(false)
		*result = falseVal
		return
	}
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
func SearchEntry(data *C.FileData, result *C.bool) {
	if data == nil {
		returnBool := C.bool(false)
		runtime.KeepAlive(returnBool)
		*result = returnBool
		return
	}

	fileID, err := ConvertFileIDToBinaryString(data.FileID, data.fileIDLength)
	if err != nil {
		returnBool := C.bool(false)
		runtime.KeepAlive(returnBool)
		*result = returnBool
		return
	}
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
func GetAllFileIDsAndEncryptedPaths(fileDataList **C.FileData) {
	fileDataList = getAllFileIDsAndEncryptedPaths()
}

//export ReplaceEntry
func ReplaceEntry(data *C.FileData, result *C.bool) {
	result = makeRequest("http://localhost:8000/api/replace", data)
}

//export InsertEntry
func InsertEntry(data *C.FileData, result *C.bool) {
	result = makeRequest("http://localhost:8000/api/insert", data)
}
