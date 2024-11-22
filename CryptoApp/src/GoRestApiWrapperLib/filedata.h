#ifndef FILEDATA_H
#define FILEDATA_H

#include <stddef.h>
#include <stdbool.h>

struct FileDataDB {
    const wchar_t *FileID;
    const wchar_t *AlgorithmenType;
    const wchar_t *OriginalFilePath;
    const wchar_t *EncryptedFilePath;
    const wchar_t *DecryptedFilePath;
    const wchar_t *Key;
    const wchar_t *Iv;
};

#endif // FILEDATA_H