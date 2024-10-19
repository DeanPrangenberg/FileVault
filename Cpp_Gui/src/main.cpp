#include <iostream>
#include <windows.h>

typedef void (*GenerateKeyIvFunc)(size_t, unsigned char*, unsigned char*);
typedef bool (*EncryptFileAes128Func)(const wchar_t*, const wchar_t*, unsigned char*, size_t, unsigned char*, size_t);
typedef bool (*DecryptFileAes128Func)(const wchar_t*, const wchar_t*, unsigned char*, size_t, unsigned char*, size_t);

void logError(const std::string& message) {
    std::cerr << message << std::endl;
}

int main() {
    // Load the C++ DLL
    HMODULE hDll = LoadLibrary(L"D:\\MyRepo\\FileVault\\cmake-build-release\\bin\\libCryptograhpy.dll");
    if (!hDll) {
        logError("Failed to load C++ DLL");
        return 1;
    }

    // Get function pointers for key and IV generation
    auto GenerateKeyIv = (GenerateKeyIvFunc)GetProcAddress(hDll, "GenerateKeyIv");

    if (!GenerateKeyIv) {
        logError("Failed to get function addresses for key and IV generation");
        FreeLibrary(hDll);
        return 1;
    }

    // Buffers for key and IV
    unsigned char key[32]; // Example key size (256-bit)
    unsigned char iv[16];  // Example IV size (128-bit)

    // Generate key and IV
    GenerateKeyIv(sizeof(key), key, iv);

    // Print the generated key and IV
    std::cout << "Generated Key: ";
    for (size_t i = 0; i < sizeof(key); ++i) {
        std::cout << std::hex << (int)key[i];
    }
    std::cout << std::endl;

    std::cout << "Generated IV: ";
    for (size_t i = 0; i < sizeof(iv); ++i) {
        std::cout << std::hex << (int)iv[i];
    }
    std::cout << std::endl;

    // Load the Go DLL
    HMODULE hGoDll = LoadLibrary(L"D:\\MyRepo\\FileVault\\cmake-build-release\\bin\\cryptoLib.dll");
    if (!hGoDll) {
        logError("Failed to load Go DLL");
        FreeLibrary(hDll);
        return 1;
    }

    // Get function pointers for encryption and decryption
    auto EncryptFileAes128 = (EncryptFileAes128Func)GetProcAddress(hGoDll, "EncryptFileAes128");
    auto DecryptFileAes128 = (DecryptFileAes128Func)GetProcAddress(hGoDll, "DecryptFileAes128");

    if (!EncryptFileAes128 || !DecryptFileAes128) {
        logError("Failed to get function addresses for encryption and decryption");
        FreeLibrary(hDll);
        FreeLibrary(hGoDll);
        return 1;
    }

    // Test data
    const wchar_t* originalFilePath = L"original.txt";
    const wchar_t* encryptedFilePath = L"encrypted.txt";
    const wchar_t* decryptedFilePath = L"decrypted.txt";

    // Call the encryption function
    bool encryptResult = EncryptFileAes128(originalFilePath, encryptedFilePath, key, sizeof(key), iv, sizeof(iv));
    if (encryptResult) {
        std::cout << "Encryption successful" << std::endl;
    } else {
        logError("Encryption failed");
    }

    // Call the decryption function
    bool decryptResult = DecryptFileAes128(encryptedFilePath, decryptedFilePath, key, sizeof(key), iv, sizeof(iv));
    if (decryptResult) {
        std::cout << "Decryption successful" << std::endl;
    } else {
        logError("Decryption failed");
    }

    // Free the DLLs
    FreeLibrary(hDll);
    FreeLibrary(hGoDll);
    return 0;
}