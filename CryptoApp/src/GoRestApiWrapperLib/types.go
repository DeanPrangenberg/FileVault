package main

type GoFileData struct {
  FileID             string `json:"FileID"`
 	FileIDLength       int    `json:"FileIDLength"`
 	EncryptionID       string `json:"EncryptionID"`
 	EncryptionIDLength int    `json:"EncryptionIDLength"`
 	LastUpdateID       string `json:"LastUpdateID"`
 	LastUpdateIDLength int    `json:"LastUpdateIDLength"`
 	AlgorithmenType    string `json:"AlgorithmenType"`
 	OriginalFilePath   string `json:"OriginalFilePath"`
 	EncryptedFilePath  string `json:"EncryptedFilePath"`
 	DecryptedFilePath  string `json:"DecryptedFilePath"`
 	Key                string `json:"Key"`
 	KeyLength          int    `json:"KeyLength"`
 	Iv                 string `json:"Iv"`
 	IvLength           int    `json:"IvLength"`
}
