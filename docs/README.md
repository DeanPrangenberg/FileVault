# **FileVault: Frontend and Backend Solution with C++ and Qt**

## **Frontend in C++ and Qt**

This project features a modern user interface designed in dark purple and with **4 tabs** for different functionalities:

### **1. Settings**
- **Language Selection**: Choose between English (ENG) and German (DE).
- **Database Export**: Export the database, either **encrypted** (using a key and IV) or **unencrypted**.
- **Default Algorithm**: Select the encryption algorithm (**AES-128** or **AES-256**).
- **File Deletion**: Option to delete files after encryption or decryption.
- **Log Save Folder**: Choose a location to save log files.
- **Central Storage**: Move all encrypted or decrypted files to a single directory.

### **2. Statistics**
- **Circular Diagram**: Visualize how many files have been decrypted using AES-128 or AES-256.
- **Directory Status**: Display whether files are moved to another directory (outdated database) or remain in their proper location (up-to-date database).
- **Decrypted File Counter**: Count how many files are currently decrypted.
- **Encryption/Decryption Counters**: Show the total number of encrypted and decrypted files.

### **3. Encryption**
- **Algorithm Info**: Display the selected encryption algorithm (**AES-128** or **AES-256**).
- **Add/Remove Directory**: Buttons to manage directories for encryption.
- **Statistics**:
    - Percentage of encryption by algorithm.
    - Number of files to be encrypted.
- **Custom File Picker**: Configure files for encryption.
- **Start Button**: Begin the encryption process.

### **4. Decryption**
- **Decrypt All Button**: Option to decrypt all encrypted files.
- **Quick Selection Menu**: Easily select encrypted files for decryption.
- **Add/Remove Directory**: Buttons to manage directories for decryption.
- **Statistics**:
    - Percentage of decryption by algorithm.
    - Number of files to be decrypted.
- **Custom File Picker**: Configure files for decryption.
- **Start Button**: Begin the decryption process.

---

## **Backend Logic in C++ and Golang**

The core logic of the program is divided into **4 modules**:

### **1. CppCryptoLib (C++)**
A custom DLL that uses **OpenSSL** to enable various cryptographic features:
- **Key & IV Generator**: Generates cryptographically secure random keys and IVs for encryption and decryption.
- **FileID Generator**: Uses file metadata (content and path) to generate a **512-bit hash** using OpenSSL's SHA-512.
- **Time-Hash**: Creates a hash of the current timestamp using SHA-512.
- **Encryption/Decryption**: Implements AES-128-CBC and AES-256-CBC for encryption and decryption with **multithreading** support. During encryption, an **EncryptionID** is generated with the Time-Hash.

### **2. CppDirectoryScanner (C++)**
A custom DLL that scans directories to find files based on specific criteria.

### **3. FileMarking (C++)**
This custom DLL marks files with a specific prefix and suffix after encryption, allowing files to be moved while still being identifiable in the database.
- **Marking Example**:  
  `<FileVault>512-bit FileID<FileVault>512-bit EncryptionID<FileVault>`
- **Features**:
    - Add or remove file markings.
    - Maintain database consistency even after file movement.

### **4. REST API Connector (Go)**
A DLL written in Go that connects to the backend database container via HTTP to:
- Add new entries.
- Search for existing entries using **FileID** and **EncryptionID**.

---

## **Backend Database Container in Java and Docker**

### **Docker**
Docker ensures the database runs in a lightweight, isolated **container**. Each container provides a consistent environment, making the application deployment seamless.

### **Database**
The database stores all metadata required for file handling:

1. FileID               (Unique File-Hash identifier)
2. EncryptionID         (Unique Time-Hash identifier)
3. LastUpdateID         (Unique Time-Hash identifier)
4. AlgorithmType        (The selected encryption algorithm)
5. OriginalPath         (Path to the original file)
6. EncryptedPath        (Path to the encrypted file)
7. DecryptedPath        (Path to the decrypted file)
8. KEY                  (Encryption key)
9. IV                   (Initialization vector)


**Database Functions**:
1. **InsertEntry**: Add a new record to the database.
2. **ReplaceEntry**: Replace an existing record using the **FileID** and **EncryptionID**.
3. **DeleteEntry**: Remove an entry based on **FileID** and **EncryptionID**.
4. **SearchEntry**: Search for an entry using **FileID** and **EncryptionID**.
5. **GetAllEntries**: Retrieve all primary keys and paths.

---

## **Backend Testing in Rust**

A Rust-based module to stress test the backend system:
- Generates test directories containing a specified number of files.
- Runs the files through the entire backend workflow multiple times.

---

## **Backend Utilities and Frontend-Backend Connector in C++**

- **Utility Functions**: Provides helper functions to connect the custom DLLs and update lost files in the database.
- **C-API Integration**: Enables communication between the DLLs and the main application.
- **Qt Integration**: Connects the GUI (Qt) with the backend logic, allowing seamless data exchange between the frontend and backend.

---

## **Technologies Used**
- **Frontend**: C++ with Qt Framework
- **Core Logic**: C++ and Go
- **Database**: Java and Docker
- **Testing**: Rust
- **Cryptography**: OpenSSL

---

## **Key Features**
- Fully modular architecture.
- Multithreaded encryption and decryption (AES-128/AES-256).
- Robust database management with Docker.
- GUI with customizable settings and live statistics.
- File scanning, marking, and movement capabilities.

---

## **How to Run**
1. Clone the repository.
2. Ensure all dependencies (Qt, OpenSSL, Go, Rust, Docker) are installed.
3. Build the project using your preferred IDE or CMake.
4. Launch the application and configure the settings.
5. Use the **Encryption** or **Decryption** tabs to process files.

## **License**
This project is licensed under the MIT License.

## **Contact**
For questions, feedback, or collaboration, feel free to reach out to me via GitHub.
