
# FileVault - C++/Qt File Encryption Tool (English) 

FileVault is a versatile and secure file encryption tool that allows users to encrypt files using either AES-128 or AES-256 encryption. With an intuitive interface, FileVault makes it simple to protect sensitive files by using a password to derive the encryption key, ensuring strong data protection.

## Features

- **AES-128 and AES-256 Encryption**: Provides the flexibility to choose between two encryption standards depending on your security requirements.
- **Password-based Key Derivation**: The encryption key is derived from a user-provided password, enhancing the security of your encrypted data.
- **User-Friendly GUI**: The graphical user interface (GUI) makes encryption and decryption accessible to users without technical knowledge.
- **File and Directory Scanning**: Users can scan entire directories to search for specific file types or manually select files for encryption or decryption.
- **Key, IV, and Metadata Storage in JSON**: For each encrypted file, a 512-bit unique file-specific ID, key, and IV are stored in a database to manage encryption and decryption.
- **Automated Decryption**: The software can scan the system for all previously encrypted files and decrypt them in bulk or individually.

## How It Works

1. **Select Encryption Strength**: Choose between AES-128 or AES-256 based on the level of security required.
2. **Select Files or Directories**: Use the built-in file explorer to select files or entire directories for encryption.
3. **Enter Password**: Input a password that will be used to derive the encryption key.
4. **Encrypt**: Encrypt the selected files using the chosen AES method. The original files will be deleted after successful encryption.
5. **Decrypt**: To restore files, select the encrypted files or directories, enter the password, and decrypt them using the correct AES method.
6. **File Scanning for Decryption**: Scan the system to find all encrypted files and decrypt them in one batch.

## Installation

### Prerequisites

- OpenSSL library (for cryptographic operations)
- C++17 or later
- Qt for the graphical user interface (GUI)
- CMake (for building the project)

### Build Instructions

1. Clone the repository:
```bash
git clone https://github.com/DeanPrangenberg/FileVault.git
```

2Navigate to the project directory:
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
3. Choose the files or directories you wish to encrypt.
4. Enter a password and click "Encrypt". The files will be encrypted and the keys and metadata will be stored in a database.
5. To decrypt files, select the encrypted files or scan the system for all encrypted files, choose the correct AES method, enter the password, and click "Decrypt".

## License
This project is licensed under the MIT License. See the LICENSE file for more details.

## Acknowledgements
Special thanks to the OpenSSL and Qt communities for providing robust libraries and support.

---

# FileVault - C++/Qt Dateiverschlüsselungstool (Deutsch)

FileVault ist ein vielseitiges und sicheres Tool zur Dateiverschlüsselung, das es Benutzern ermöglicht, Dateien mit entweder AES-128- oder AES-256-Verschlüsselung zu schützen. Mit einer intuitiven Benutzeroberfläche macht es FileVault einfach, sensible Dateien durch die Verwendung eines Passworts zu verschlüsseln, um den Verschlüsselungsschlüssel abzuleiten und somit den Schutz der Daten zu gewährleisten.

## Funktionen

- **AES-128 und AES-256 Verschlüsselung**: Bietet die Flexibilität, je nach Sicherheitsanforderungen zwischen zwei Verschlüsselungsstandards zu wählen.
- **Passwortbasierte Schlüsselderivation**: Der Verschlüsselungsschlüssel wird von einem benutzerdefinierten Passwort abgeleitet, was die Sicherheit der verschlüsselten Daten verbessert.
- **Benutzerfreundliche GUI**: Die grafische Benutzeroberfläche (GUI) ermöglicht die Nutzung von Verschlüsselung und Entschlüsselung auch ohne technisches Vorwissen.
- **Datei- und Verzeichnis-Scan**: Benutzer können ganze Verzeichnisse scannen, um nach bestimmten Dateitypen zu suchen oder manuell Dateien für die Verschlüsselung oder Entschlüsselung auszuwählen.
- **Schlüssel, IV und Metadaten in JSON-Datei gespeichert**: Für jede verschlüsselte Datei wird eine 512-Bit lange eindeutige ID, der Schlüssel und der IV in einer JSON-Datei gespeichert, um die Verschlüsselung und Entschlüsselung zu verwalten.
- **Automatisierte Entschlüsselung**: Die Software kann das System nach allen zuvor verschlüsselten Dateien durchsuchen und sie einzeln oder im Stapel entschlüsseln.

## Funktionsweise

1. **Verschlüsselungsmethode wählen**: Wählen Sie zwischen AES-128 oder AES-256 basierend auf Ihren Sicherheitsanforderungen.
2. **Dateien oder Verzeichnisse auswählen**: Verwenden Sie den integrierten Datei-Explorer, um Dateien oder ganze Verzeichnisse zur Verschlüsselung auszuwählen.
3. **Passwort eingeben**: Geben Sie ein Passwort ein, das zur Ableitung des Verschlüsselungsschlüssels verwendet wird.
4. **Verschlüsseln**: Die ausgewählten Dateien werden mit der gewählten AES-Methode verschlüsselt. Die Originaldateien werden nach erfolgreicher Verschlüsselung gelöscht.
5. **Entschlüsseln**: Um die Dateien wiederherzustellen, wählen Sie die verschlüsselten Dateien oder Verzeichnisse aus, geben Sie das Passwort ein und entschlüsseln Sie sie mit der richtigen AES-Methode.
6. **System-Scan für Entschlüsselung**: Scannen Sie das System, um alle verschlüsselten Dateien zu finden und entschlüsseln Sie sie im Stapel.

## Installation

### Voraussetzungen

- OpenSSL-Bibliothek (für kryptographische Operationen)
- C++17 oder neuer
- Qt für die grafische Benutzeroberfläche (GUI)
- CMake (für den Bau des Projekts)

### Build-Anleitung

1. Repository klonen:
```bash
git clone https://github.com/DeanPrangenberg/FileVault.git
```

2. In das Projektverzeichnis wechseln:
```bash
cd FileVault
```

3. Projekt mit CMake konfigurieren:
```bash
cmake -B build
```

4. Projekt bauen:
```bash
cmake --build build
```

## Nutzung

1. Starten Sie die Anwendung.
2. Wählen Sie aus, ob Sie AES-128 oder AES-256 für die Verschlüsselung verwenden möchten.
3. Wählen Sie die Dateien oder Verzeichnisse aus, die Sie verschlüsseln möchten.
4. Geben Sie ein Passwort ein und klicken Sie auf "Verschlüsseln". Die Dateien werden verschlüsselt und die Schlüssel sowie Metadaten in einer JSON-Datei gespeichert.
5. Um Dateien zu entschlüsseln, wählen Sie die verschlüsselten Dateien aus oder scannen Sie das System nach allen verschlüsselten Dateien, wählen Sie die richtige AES-Methode, geben Sie das Passwort ein und klicken Sie auf "Entschlüsseln".

## Lizenz
Dieses Projekt ist unter der MIT-Lizenz lizenziert. Siehe die LICENSE-Datei für weitere Details.

## Danksagungen
Besonderer Dank gilt den OpenSSL- und Qt-Communities für ihre robusten Bibliotheken und ihre Unterstützung.
