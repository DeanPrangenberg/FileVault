public class FileData {
    public String FileID;
    public int fileIDLength;
    public String AlgorithmenType;
    public String OriginalFilePath;
    public String EncryptedFilePath;
    public String DecryptedFilePath;
    public String Key;
    public int keyLength;
    public String Iv;
    public int ivLength;

    public String getFileID() {
        return FileID;
    }

    public void setFileID(String fileID) {
        FileID = fileID;
    }

    public int getFileIDLength() {
        return fileIDLength;
    }

    public void setFileIDLength(int fileIDLength) {
        this.fileIDLength = fileIDLength;
    }

    public String getAlgorithmenType() {
        return AlgorithmenType;
    }

    public void setAlgorithmenType(String algorithmenType) {
        AlgorithmenType = algorithmenType;
    }

    public String getOriginalFilePath() {
        return OriginalFilePath;
    }

    public void setOriginalFilePath(String originalFilePath) {
        OriginalFilePath = originalFilePath;
    }

    public String getEncryptedFilePath() {
        return EncryptedFilePath;
    }

    public void setEncryptedFilePath(String encryptedFilePath) {
        EncryptedFilePath = encryptedFilePath;
    }

    public String getDecryptedFilePath() {
        return DecryptedFilePath;
    }

    public void setDecryptedFilePath(String decryptedFilePath) {
        DecryptedFilePath = decryptedFilePath;
    }

    public String getKey() {
        return Key;
    }

    public void setKey(String key) {
        Key = key;
    }

    public int getKeyLength() {
        return keyLength;
    }

    public void setKeyLength(int keyLength) {
        this.keyLength = keyLength;
    }

    public String getIv() {
        return Iv;
    }

    public void setIv(String iv) {
        Iv = iv;
    }

    public int getIvLength() {
        return ivLength;
    }

    public void setIvLength(int ivLength) {
        this.ivLength = ivLength;
    }
}