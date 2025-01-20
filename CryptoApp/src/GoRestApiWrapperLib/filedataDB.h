#ifndef FILEDATA_H
#define FILEDATA_H

#include <stddef.h>
#include <stdbool.h>

/**
 * Struct representing file data in the database.
 */
typedef struct {
    /**
     * The File ID.
     */
    const wchar_t *FileID;

    /**
     * The length of the File ID.
     */
    size_t FileIDLength;

    /**
     * The Encryption ID.
     */
    const wchar_t *EncryptionID;

    /**
     * The length of the Encryption ID.
     */
    size_t EncryptionIDLength;

    /**
     * The Last Update ID.
     */
    const wchar_t *LastUpdateID;

    /**
     * The length of the Last Update ID.
     */
    size_t LastUpdateIDLength;

    /**
     * The type of algorithm used.
     */
    const wchar_t *AlgorithmenType;

    /**
     * The original file path.
     */
    const wchar_t *OriginalFilePath;

    /**
     * The encrypted file path.
     */
    const wchar_t *EncryptedFilePath;

    /**
     * The decrypted file path.
     */
    const wchar_t *DecryptedFilePath;

    /**
     * The encryption key.
     */
    const wchar_t *Key;

    /**
     * The length of the encryption key.
     */
    size_t KeyLength;

    /**
     * The initialization vector.
     */
    const wchar_t *Iv;

    /**
     * The length of the initialization vector.
     */
    size_t IvLength;
} FileDataDB;

#endif // FILEDATA_H