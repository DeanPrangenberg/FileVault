package main

// #include "filedata.h"
import "C"
import (
	"bytes"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"net/http"
	"unsafe"
)

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

func getAllFileIDsAndEncryptedPaths() **C.FileDataDB {
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
