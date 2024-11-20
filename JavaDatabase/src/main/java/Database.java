import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class Database {
    private Connection conn;

    public static class FileData {
        public String FileID; // Changed to String to store binary string
        public int fileIDLength;
        public String AlgorithmenType;
        public String OriginalFilePath;
        public String EncryptedFilePath;
        public String DecryptedFilePath;
        public String Key; // Changed to String to store binary string
        public int keyLength;
        public String Iv; // Changed to String to store binary string
        public int ivLength;
    }

    public void connect() {
        try {
            Class.forName("org.sqlite.JDBC");
            conn = DriverManager.getConnection("jdbc:sqlite:filedata.db");
            initializeTable();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void initializeTable() throws SQLException {
        String sql = "CREATE TABLE IF NOT EXISTS file_data (" +
                "FileID TEXT PRIMARY KEY, " +
                "fileIDLength INTEGER, " +
                "AlgorithmenType TEXT, " +
                "OriginalFilePath TEXT, " +
                "EncryptedFilePath TEXT, " +
                "DecryptedFilePath TEXT, " +
                "Key TEXT, " +
                "keyLength INTEGER, " +
                "Iv TEXT, " +
                "ivLength INTEGER)";
        try (Statement stmt = conn.createStatement()) {
            stmt.execute(sql);
        }
    }

    public void disconnect() {
        try {
            if (conn != null) {
                conn.close();
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public void insertEntry(FileData data) {
        String sql = "INSERT OR REPLACE INTO file_data (FileID, fileIDLength, AlgorithmenType, OriginalFilePath, EncryptedFilePath, DecryptedFilePath, Key, keyLength, Iv, ivLength) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
        try (PreparedStatement pstmt = conn.prepareStatement(sql)) {
            pstmt.setString(1, data.FileID);
            pstmt.setInt(2, data.fileIDLength);
            pstmt.setString(3, data.AlgorithmenType);
            pstmt.setString(4, data.OriginalFilePath);
            pstmt.setString(5, data.EncryptedFilePath);
            pstmt.setString(6, data.DecryptedFilePath);
            pstmt.setString(7, data.Key);
            pstmt.setInt(8, data.keyLength);
            pstmt.setString(9, data.Iv);
            pstmt.setInt(10, data.ivLength);
            pstmt.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public void replaceEntry(FileData data) {
        deleteEntry(data.FileID);
        insertEntry(data);
    }

    public void deleteEntry(String fileID) {
        String sql = "DELETE FROM file_data WHERE FileID = ?";
        try (PreparedStatement pstmt = conn.prepareStatement(sql)) {
            pstmt.setString(1, fileID);
            pstmt.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public FileData searchEntry(String fileID) {
        String sql = "SELECT * FROM file_data WHERE FileID = ?";
        try (PreparedStatement pstmt = conn.prepareStatement(sql)) {
            pstmt.setString(1, fileID);
            try (ResultSet rs = pstmt.executeQuery()) {
                if (rs.next()) {
                    FileData data = new FileData();
                    data.FileID = rs.getString("FileID");
                    data.fileIDLength = rs.getInt("fileIDLength");
                    data.AlgorithmenType = rs.getString("AlgorithmenType");
                    data.OriginalFilePath = rs.getString("OriginalFilePath");
                    data.EncryptedFilePath = rs.getString("EncryptedFilePath");
                    data.DecryptedFilePath = rs.getString("DecryptedFilePath");
                    data.Key = rs.getString("Key");
                    data.keyLength = rs.getInt("keyLength");
                    data.Iv = rs.getString("Iv");
                    data.ivLength = rs.getInt("ivLength");
                    return data;
                }
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return null;
    }

    public List<FileData> getAllFileIDsAndEncryptedPaths() {
        List<FileData> fileList = new ArrayList<>();
        String sql = "SELECT FileID, EncryptedFilePath FROM file_data";
        try (Statement stmt = conn.createStatement();
             ResultSet rs = stmt.executeQuery(sql)) {
            while (rs.next()) {
                FileData data = new FileData();
                data.FileID = rs.getString("FileID");
                data.EncryptedFilePath = rs.getString("EncryptedFilePath");
                fileList.add(data);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return fileList;
    }

    private String byteArrayToBinaryString(byte[] bytes) {
        StringBuilder binaryString = new StringBuilder();
        for (byte b : bytes) {
            binaryString.append(String.format("%8s", Integer.toBinaryString(b & 0xFF)).replace(' ', '0'));
        }
        return binaryString.toString();
    }

    private byte[] binaryStringToByteArray(String binaryString) {
        int byteCount = binaryString.length() / 8;
        byte[] bytes = new byte[byteCount];
        for (int i = 0; i < byteCount; i++) {
            String byteString = binaryString.substring(8 * i, 8 * (i + 1));
            bytes[i] = (byte) Integer.parseInt(byteString, 2);
        }
        return bytes;
    }
}