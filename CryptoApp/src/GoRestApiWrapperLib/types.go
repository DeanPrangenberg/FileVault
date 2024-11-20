package main

type GoFileData struct {
	FileID            string `json:"FileID"`
	FileIDLength      int    `json:"fileIDLength"`
	AlgorithmenType   string `json:"AlgorithmenType"`
	OriginalFilePath  string `json:"OriginalFilePath"`
	EncryptedFilePath string `json:"EncryptedFilePath"`
	DecryptedFilePath string `json:"DecryptedFilePath"`
	Key               string `json:"Key"`
	KeyLength         int    `json:"keyLength"`
	Iv                string `json:"Iv"`
	IvLength          int    `json:"ivLength"`
}
