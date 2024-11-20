#ifndef FILEDATA_H
#define FILEDATA_H

#include <stddef.h>
#include <stdbool.h>

typedef struct {
    unsigned char *FileID;
    size_t fileIDLength;
    const wchar_t *AlgorithmenType;
    const wchar_t *OriginalFilePath;
    const wchar_t *EncryptedFilePath;
    const wchar_t *DecryptedFilePath;
    unsigned char *Key;
    size_t keyLength;
    unsigned char *Iv;
    size_t ivLength;
} FileData;

#endif // FILEDATA_H