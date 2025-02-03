#ifndef ROOTPROJECT_STRUCTUTILS_H
#define ROOTPROJECT_STRUCTUTILS_H

#include <filesystem>
#include <stdexcept>
#include "../DLLUtils/CryptoDLL.h"
#include "../../../shared/src/GlobalDefinitions.h"
#include "../../../shared/src/FileData.h"

namespace fs = std::filesystem;

/**
 * @class AttributeUtils
 * @brief Utility class for handling file data structures and conversions.
 */
class AttributeUtils {
public:
    /**
     * @brief Creates a FileData structure for the given algorithm type and original file path.
     * @param algorithmenType The algorithm type to be used for encryption.
     * @param originalFilePath The path to the original file.
     * @return A FileData structure containing the file's metadata and encryption information.
     * @throws std::invalid_argument if the algorithm type is unsupported.
     */
    void createFileDataStruct(const globalDefinitions::AlgorithmType &algorithmenType, const fs::path &originalFilePath, FileData &fileData);

private:
    /**
     * @brief Converts an AlgorithmType enum to a string representation.
     * @param type The AlgorithmType enum value.
     * @return A string representing the algorithm type.
     */
    static std::string AlgorithmTypeToString(globalDefinitions::AlgorithmType type);
};

#endif //ROOTPROJECT_STRUCTUTILS_H