// BackendTester.h
#ifndef BACKENDTESTER_H
#define BACKENDTESTER_H

#include "../HelperUtils/HelperUtils.h"
#include "../StructUtils/StructUtils.h"
#include "../DLLUtils/FileScannerDLL.h"
#include "../DLLUtils/CryptoDLL.h"
#include "RestAPI/RestApiDLL.h"
#include "../DLLUtils/FileMarkDLL.h"
#include "chrono"
#include <vector>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

class BackendTester {
public:
    BackendTester(int testRuns);

private:
    void scanAndBuildMetaData();
    void encryptAndSaveFiles();
    void repairAndReloadFiles();
    void decryptAndDeleteFiles();
    void removeFailedEntries(std::vector<FileData> &fileDataVec, std::vector<bool> &results);
    void printDiv();

    FileScannerDLL fileScannerDll;
    CryptoDLL cryptoDll;
    RestApiDLL restApiDll;
    FileMarkDLL fileMarkDll;
    HelperUtils helperUtils;
    std::vector<FileData> fileDataVec;
    const std::string pathToCrypt = "S:\\clips\\cut";
    const bool printDebug = false;
};

#endif // BACKENDTESTER_H