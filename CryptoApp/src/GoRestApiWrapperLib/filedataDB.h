#ifndef FILEDATA_H
#define FILEDATA_H

#include <stddef.h>
#include <stdbool.h>

typedef struct {
    const wchar_t *FileID;
    size_t FileIDLength;
    const wchar_t *AlgorithmenType;
    const wchar_t *OriginalFilePath;
    const wchar_t *EncryptedFilePath;
    const wchar_t *DecryptedFilePath;
    const wchar_t *Key;
    size_t KeyLength;
    const wchar_t *Iv;
    size_t IvLength;
} FileDataDB;

#endif // FILEDATA_H