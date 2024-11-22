package main

/*
#include <stdlib.h>
#include "filedata.h"
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
	return GoFileData{
		FileID:            wcharToString(data.FileID),
		AlgorithmenType:   wcharToString(data.AlgorithmenType),
		OriginalFilePath:  wcharToString(data.OriginalFilePath),
		EncryptedFilePath: wcharToString(data.EncryptedFilePath),
		DecryptedFilePath: wcharToString(data.DecryptedFilePath),
		Key:               wcharToString(data.Key),
		Iv:                wcharToString(data.Iv),
	}
}

func convertToCFileData(data GoFileData) *C.FileDataDB {
	return &C.FileDataDB{
		FileID:            stringToWcharT(data.AlgorithmenType),
		AlgorithmenType:   stringToWcharT(data.AlgorithmenType),
		OriginalFilePath:  stringToWcharT(data.OriginalFilePath),
		EncryptedFilePath: stringToWcharT(data.EncryptedFilePath),
		DecryptedFilePath: stringToWcharT(data.DecryptedFilePath),
		Key:               stringToWcharT(data.AlgorithmenType),
		Iv:                stringToWcharT(data.AlgorithmenType),
	}
}
