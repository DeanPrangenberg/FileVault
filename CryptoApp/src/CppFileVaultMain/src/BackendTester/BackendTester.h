// BackendTester.h
#ifndef BACKENDTESTER_H
#define BACKENDTESTER_H

#include "../HelperUtils/HelperUtils.h"
#include "../AttributeUtils/AttributeUtils.h"
#include "../DLLUtils/FileScannerDLL.h"
#include "../DLLUtils/CryptoDLL.h"
#include "RestAPI/RestApiDLL.h"
#include "../DLLUtils/FileMarkDLL.h"
#include "chrono"
#include <vector>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

/**
 * @class BackendTester
 * @brief Class for testing backend functionalities including file scanning, encryption, decryption, and metadata handling.
 */
class BackendTester {
public:
    /**
     * @brief Constructs a BackendTester object and runs the specified number of test runs.
     * @param testRuns The number of test runs to perform.
     */
    BackendTester(int testRuns);

private:
    /**
     * @brief Scans the directory for files and builds metadata for each file.
     */
    void scanAndBuildMetaData();

    /**
     * @brief Encrypts and saves the files.
     */
    void encryptAndSaveFiles();

    /**
     * @brief Repairs lost encrypted file structures and reloads the files.
     */
    void repairAndReloadFiles();

    /**
     * @brief Decrypts and deletes the files.
     */
    void decryptAndDeleteFiles();

    /**
     * @brief Removes failed entries from the file data vector.
     * @param fileDataVec Vector of file data structures.
     * @param results Vector of boolean results indicating success or failure.
     */
    void removeFailedEntries(std::vector<FileData> &fileDataVec, std::vector<bool> &results);

    /**
     * @brief Prints a divider line for console output.
     */
    void printDiv();

    FileScannerDLL fileScannerDll; /**< DLL for scanning files. */
    CryptoDLL cryptoDll; /**< DLL for cryptographic operations. */
    RestApiDLL restApiDll; /**< DLL for REST API operations. */
    FileMarkDLL fileMarkDll; /**< DLL for file marking operations. */
    HelperUtils helperUtils; /**< Utility functions for helper operations. */
    std::vector<FileData> fileDataVec; /**< Vector of file data structures. */
    const std::string pathToCrypt = "S:\\clips\\cut"; /**< Path to the directory to be encrypted. */
    const bool printDebug = true; /**< Flag to enable or disable debug printing. */
};

#endif // BACKENDTESTER_H