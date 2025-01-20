package main

/**
 * GoFileData represents the structure of file data used in the application.
 */
type GoFileData struct {
  /**
   * The File ID.
   */
  FileID string `json:"FileID"`

  /**
   * The length of the File ID.
   */
  FileIDLength int `json:"FileIDLength"`

  /**
   * The Encryption ID.
   */
  EncryptionID string `json:"EncryptionID"`

  /**
   * The length of the Encryption ID.
   */
  EncryptionIDLength int `json:"EncryptionIDLength"`

  /**
   * The Last Update ID.
   */
  LastUpdateID string `json:"LastUpdateID"`

  /**
   * The length of the Last Update ID.
   */
  LastUpdateIDLength int `json:"LastUpdateIDLength"`

  /**
   * The type of algorithm used.
   */
  AlgorithmenType string `json:"AlgorithmenType"`

  /**
   * The original file path.
   */
  OriginalFilePath string `json:"OriginalFilePath"`

  /**
   * The encrypted file path.
   */
  EncryptedFilePath string `json:"EncryptedFilePath"`

  /**
   * The decrypted file path.
   */
  DecryptedFilePath string `json:"DecryptedFilePath"`

  /**
   * The encryption key.
   */
  Key string `json:"Key"`

  /**
   * The length of the encryption key.
   */
  KeyLength int `json:"KeyLength"`

  /**
   * The initialization vector.
   */
  Iv string `json:"Iv"`

  /**
   * The length of the initialization vector.
   */
  IvLength int `json:"IvLength"`
}