package main

type GoFileData struct {
	FileID            string `json:"FileID"`
	AlgorithmenType   string `json:"AlgorithmenType"`
	OriginalFilePath  string `json:"OriginalFilePath"`
	EncryptedFilePath string `json:"EncryptedFilePath"`
	DecryptedFilePath string `json:"DecryptedFilePath"`
	Key               string `json:"Key"`
	Iv                string `json:"Iv"`
}
