public class FileData {
    public byte[] FileID;
    public int fileIDLength;
    public String AlgorithmenType;
    public String OriginalFilePath;
    public String EncryptedFilePath;
    public String DecryptedFilePath;
    public byte[] Key;
    public int keyLength;
    public byte[] Iv;
    public int ivLength;

    public byte[] getFileID() {
        return FileID;
    }

    public void setFileID(byte[] fileID) {
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

    public byte[] getKey() {
        return Key;
    }

    public void setKey(byte[] key) {
        Key = key;
    }

    public int getKeyLength() {
        return keyLength;
    }

    public void setKeyLength(int keyLength) {
        this.keyLength = keyLength;
    }

    public byte[] getIv() {
        return Iv;
    }

    public void setIv(byte[] iv) {
        Iv = iv;
    }

    public int getIvLength() {
        return ivLength;
    }

    public void setIvLength(int ivLength) {
        this.ivLength = ivLength;
    }
}