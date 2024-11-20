package main

type GoFileData struct {
	FileID            []byte `json:"FileID"`
	FileIDLength      int    `json:"fileIDLength"`
	AlgorithmenType   string `json:"AlgorithmenType"`
	OriginalFilePath  string `json:"OriginalFilePath"`
	EncryptedFilePath string `json:"EncryptedFilePath"`
	DecryptedFilePath string `json:"DecryptedFilePath"`
	Key               []byte `json:"Key"`
	KeyLength         int    `json:"keyLength"`
	Iv                []byte `json:"Iv"`
	IvLength          int    `json:"ivLength"`
}
