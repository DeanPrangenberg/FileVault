# FileVault

FileVault is a versatile and secure file encryption tool that gives users the option to encrypt files using either AES-128 or AES-256 encryption. With an intuitive interface, FileVault makes it simple to protect sensitive files by using a password to derive the encryption key, ensuring strong protection of your data.

## Features

- **AES-128 and AES-256 Encryption**: Choose between two encryption standards, offering flexibility in the level of security.
- **Password-based Key Derivation**: The encryption key is derived from a user-provided password.
- **User-Friendly GUI**: The graphical user interface (GUI) makes encryption and decryption accessible to all users.

## How It Works

1. **Select Encryption Strength**: Choose between AES-128 or AES-256 depending on your security requirements.
2. **Select Files**: Choose the files you want to encrypt through the file selection dialog.
3. **Enter Password**: Input a password to derive the encryption key.
4. **Encrypt**: Your files will be securely encrypted with the chosen AES standard.
5. **Decrypt**: To restore your encrypted files, simply use the same password and encryption method.

## Installation

### Prerequisites

- OpenSSL library (for cryptographic operations)
- C++17 or later
- Qt for the graphical user interface (GUI)

### Build Instructions

1. Clone the repository:
```bash
git clone https://github.com/yourusername/FileVault.git
```

2. Navigate to the project directory:
```bash
cd FileVault
```

3. Configure the project with CMake:
```bash
cmake -B build
```

4. Build the project:
```bash
cmake --build build
```

## Usage
1. Launch the application.
2. Select whether to use AES-128 or AES-256 for encryption.
3. Select the files you wish to encrypt.
4. Enter a password and click "Encrypt".
5. To decrypt files, select the encrypted files, choose the correct encryption method, enter the password, and click "Decrypt".

## License
This project is licensed under the MIT License. See the LICENSE file for more details.

## Acknowledgements
Thanks to the OpenSSL and Qt communities for providing robust libraries and support.
