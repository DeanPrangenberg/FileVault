use std::fs;
use std::path::Path;

fn main() {
    let source_dir = "D:\\save\\clips\\cut";
    let destination_dir = "S:\\clips\\cut";

    // Create the destination directory if it doesn't exist
    fs::create_dir_all(destination_dir).expect("Failed to create destination directory");

    // Remove all files in the destination directory
    for entry in fs::read_dir(destination_dir).expect("Failed to read destination directory") {
        let entry = entry.expect("Failed to get directory entry");
        let path = entry.path();

        if path.is_file() {
            fs::remove_file(&path).expect("Failed to remove file");
        }
    }

    // Iterate over the files in the source directory
    for entry in fs::read_dir(source_dir).expect("Failed to read source directory") {
        let entry = entry.expect("Failed to get directory entry");
        let path = entry.path();

        if path.is_file() {
            // Construct the destination path
            let file_name = path.file_name().expect("Failed to get file name");
            let dest_path = Path::new(destination_dir).join(file_name);

            // Copy the file to the destination directory
            fs::copy(&path, &dest_path).expect("Failed to copy file");
            println!("Copied {:?} to {:?}", path, dest_path);
        }
    }
}