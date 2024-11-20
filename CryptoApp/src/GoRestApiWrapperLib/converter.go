package main

/*
#include <stdlib.h>
#include "filedata.h"
*/
import "C"
import "unsafe"

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

func convertToGoFileData(data *C.FileData) GoFileData {
	return GoFileData{
		FileID:            C.GoBytes(unsafe.Pointer(data.FileID), C.int(data.fileIDLength)),
		FileIDLength:      int(data.fileIDLength),
		AlgorithmenType:   wcharToString(data.AlgorithmenType),
		OriginalFilePath:  wcharToString(data.OriginalFilePath),
		EncryptedFilePath: wcharToString(data.EncryptedFilePath),
		DecryptedFilePath: wcharToString(data.DecryptedFilePath),
		Key:               C.GoBytes(unsafe.Pointer(data.Key), C.int(data.keyLength)),
		KeyLength:         int(data.keyLength),
		Iv:                C.GoBytes(unsafe.Pointer(data.Iv), C.int(data.ivLength)),
		IvLength:          int(data.ivLength),
	}
}

func stringToWcharT(s string) *C.wchar_t {
	runes := []rune(s)
	wcharArray := make([]C.wchar_t, len(runes)+1)
	for i, r := range runes {
		wcharArray[i] = C.wchar_t(r)
	}
	wcharArray[len(runes)] = 0
	return &wcharArray[0]
}

func convertToCFileData(data GoFileData) *C.FileData {
	return &C.FileData{
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
