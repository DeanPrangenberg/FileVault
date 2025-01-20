// BackendTester.cpp
#include "BackendTester.h"

BackendTester::BackendTester(int testRuns) {
  int totalFilesScanned = 0;
  int totalFilesEncrypted = 0;
  int totalFilesDecrypted = 0;

  auto start = std::chrono::system_clock::now();

  for (int i = 0; i < testRuns; ++i) {
    system(".\\RustFileCopy.exe");
    scanAndBuildMetaData();
    totalFilesScanned += fileDataVec.size();

    encryptAndSaveFiles();
    totalFilesEncrypted += fileDataVec.size();

    repairAndReloadFiles();

    decryptAndDeleteFiles();
    totalFilesDecrypted += fileDataVec.size();
    fileDataVec.clear();

    if (printDebug) std::cout << "++ Test Run " << i + 1 << " completed ++" << std::endl;
  }

  auto end = std::chrono::system_clock::now();
  auto timeDiff = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

  printDiv();
  std::cout << std::endl << "Backend Test Results:" << std::endl;
  std::cout << "Total files scanned: " << totalFilesScanned << std::endl;
  std::cout << "Succeeded encryptions: " << totalFilesEncrypted << " (" << (totalFilesEncrypted * 100 / totalFilesScanned) << "%)" << std::endl;
  std::cout << "Succeeded decryptions: " << totalFilesDecrypted << " (" << (totalFilesDecrypted * 100 / totalFilesScanned) << "%)" << std::endl;
  std::cout << "The test took " << timeDiff << " seconds." << std::endl;
  std::cout << std::endl;
  printDiv();
}

void BackendTester::scanAndBuildMetaData() {
    printDiv();
    if (printDebug) std::cout << "--Starting file scan--" << std::endl;
    std::vector<fs::path> pathList;
    fileScannerDll.ScanDirectory(pathToCrypt, false, pathList, [](const fs::path) {});
    if (printDebug) std::cout << "++Scan completed: found " << pathList.size() << " files!++" << std::endl;

    if (!pathList.empty()) {
        if (printDebug) std::cout << "--Building Structs--" << std::endl;
        for (const auto &filePath: pathList) {
            auto filePathStr = filePath.wstring();
            if (filePathStr.empty() || filePathStr.find(L'\\') == std::wstring::npos) continue;

            if (printDebug) std::wcout << L"++ Generating Struct for path: " << filePathStr << " ++" << std::endl;
            fileDataVec.push_back(
                StructUtils::createFileDataStruct(globalDefinitions::AlgorithmType::AES256, filePath));
        }
    }
    printDiv();
}

void BackendTester::encryptAndSaveFiles() {
    if (fileDataVec.empty()) return;

    if (printDebug) std::cout << "--Encrypting Files--" << std::endl;
    std::vector<fs::path> encrypPaths;

    for (const auto &fileData: fileDataVec) {
      encrypPaths.push_back(fileData.OriginalFilePath->wstring());
    }
    std::vector<int> results = helperUtils.encryptFiles(encrypPaths, {"AES-256"});

    if (printDebug) std::cout << "++" << fileDataVec.size() << " Files Encrypted++" << std::endl;
}

void BackendTester::repairAndReloadFiles() {
    printDiv();
    if (printDebug) std::cout << "--Repairing Lost Encrypted File Structs--" << std::endl;
    std::vector<fs::path> paths = {pathToCrypt};
    helperUtils.repairLostEncFileStructs(paths);

    fileDataVec.clear();
    if (printDebug) std::cout << "--Rescanning Directory for encrypted files--" << std::endl;
    std::vector<fs::path> pathList;
    fileScannerDll.ScanDirectory(pathToCrypt, true, pathList, [](const fs::path) {});
    if (printDebug) std::cout << "++Scan completed: found " << pathList.size() << " files!++" << std::endl;

    if (printDebug) std::cout << "--Reloading File Data--" << std::endl;
    for (const auto &filePath: pathList) {
        FileData partialStruct;
        partialStruct.EncryptedFilePath->assign(filePath);

        if (fileMarkDll.extractIDsFromFile(partialStruct.EncryptedFilePath.get(), partialStruct.FileID.get(),
                                           partialStruct.EncryptionID.get())) {
            if (printDebug)
                std::cout << "++Extracted FileID: "
                          << globalDefinitions::toHexString(partialStruct.FileID->data(), partialStruct.FileID->size())
                          << " from: "
                          << filePath << " ++" << std::endl;
            if (printDebug) std::cout << "++Extracted EncryptionID: "
                      << globalDefinitions::toHexString(partialStruct.EncryptionID->data(),
                                                        partialStruct.EncryptionID->size())
                      << " from: "
                      << filePath << " ++" << std::endl;
        } else {
            std::wcerr << L"++ Failed to extract File ID from: " << filePath << " ++" << std::endl;
            continue;
        }

        if (restApiDll.SearchEntry(partialStruct)) {
            fileDataVec.push_back(partialStruct);
            if (printDebug) std::wcout << L"++ Found and completed struct for file: " << filePath << " ++" << std::endl;
        } else {
            std::wcerr << L"++ Could not find struct for file: " << filePath << " in database++" << std::endl;
        }
    }
}

void BackendTester::decryptAndDeleteFiles() {
    printDiv();
    if (fileDataVec.empty()) return;

    if (printDebug) std::cout << "--Decrypting Files--" << std::endl;

    std::vector<fs::path> decrypPaths;

    for (const auto &fileData: fileDataVec) {
        if (printDebug) std::cout << "++ EncryptionID: " << fileData.toHexString(fileData.EncryptionID) << " ++" << std::endl;
        decrypPaths.push_back(fileData.EncryptedFilePath->wstring());
    }

    std::unordered_map<std::string, int> results = helperUtils.decryptFiles(decrypPaths);

    if (printDebug) std::cout << "++" << fileDataVec.size() << " Files Decrypted++" << std::endl;

    if (printDebug) std::cout << "--Deleting Entrys--" << std::endl;
    restApiDll.ResetDatabase();
    if (printDebug) std::cout << "###############################################################################################" << std::endl;
}

void BackendTester::printDiv() {
    for (int k = 0; k < 3; ++k) {
        for (int i = 0; i < 90; ++i) {
            if (printDebug) std::cout << "#";
        }
        if (printDebug) std::cout << std::endl;
    }
}