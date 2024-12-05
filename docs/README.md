# FileVault - C++/Qt File Encryption Tool (English)

FileVault is a versatile and secure file encryption tool that allows users to encrypt files using either AES-128 or
AES-256 encryption. With an intuitive interface, FileVault makes it simple to protect sensitive files by using a
password to derive the encryption key, ensuring strong data protection.

## Features

- **AES-128 and AES-256 Encryption**: Provides the flexibility to choose between two encryption standards depending on
  your security requirements.
- **Password-based Key Derivation**: The encryption key is derived from a user-provided password, enhancing the security
  of your encrypted data.
- **User-Friendly GUI**: The graphical user interface (GUI) makes encryption and decryption accessible to users without
  technical knowledge.
- **File and Directory Scanning**: Users can scan entire directories to search for specific file types or manually
  select files for encryption or decryption.
- **Key, IV, and Metadata Storage in JSON**: For each encrypted file, a 512-bit unique file-specific ID, key, and IV are
  stored in a database to manage encryption and decryption.
- **Automated Decryption**: The software can scan the system for all previously encrypted files and decrypt them in bulk
  or individually.

## How It Works

1. **Select Encryption Strength**: Choose between AES-128 or AES-256 based on the level of security required.
2. **Select Files or Directories**: Use the built-in file explorer to select files or entire directories for encryption.
3. **Enter Password**: Input a password that will be used to derive the encryption key.
4. **Encrypt**: Encrypt the selected files using the chosen AES method. The original files will be deleted after
   successful encryption.
5. **Decrypt**: To restore files, select the encrypted files or directories, enter the password, and decrypt them using
   the correct AES method.
6. **File Scanning for Decryption**: Scan the system to find all encrypted files and decrypt them in one batch.

## Prerequisites

- OpenSSL library (for cryptographic operations)
- C++17 or later
- Qt for the graphical user interface (GUI)
- CMake (for building the project)
- Docker (for managing database containers)
   