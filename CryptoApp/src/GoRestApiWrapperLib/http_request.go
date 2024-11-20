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

func makeRequest(url string, data *C.FileData) *C.bool {
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

func getAllFileIDsAndEncryptedPaths() **C.FileData {
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

	cData := make([]C.FileData, len(goData))
	for i, data := range goData {
		cData[i] = C.FileData{
			FileID:            (*C.uchar)(C.CBytes(data.FileID)),
			fileIDLength:      C.size_t(data.FileIDLength),
			AlgorithmenType:   stringToWcharT(data.AlgorithmenType),
			OriginalFilePath:  stringToWcharT(data.OriginalFilePath),
			EncryptedFilePath: stringToWcharT(data.EncryptedFilePath),
			DecryptedFilePath: stringToWcharT(data.DecryptedFilePath),
			Key:               (*C.uchar)(C.CBytes(data.Key)),
			keyLength:         C.size_t(data.KeyLength),
			Iv:                (*C.uchar)(C.CBytes(data.Iv)),
			ivLength:          C.size_t(data.IvLength),
		}
	}

	return (**C.FileData)(unsafe.Pointer(&cData[0]))
}
