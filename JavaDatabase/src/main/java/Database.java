import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class Database {
    private Connection conn;

    public static class FileData {
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
                "FileID BLOB PRIMARY KEY, " +
                "fileIDLength INTEGER, " +
                "AlgorithmenType TEXT, " +
                "OriginalFilePath TEXT, " +
                "EncryptedFilePath TEXT, " +
                "DecryptedFilePath TEXT, " +
                "Key BLOB, " +
                "keyLength INTEGER, " +
                "Iv BLOB, " +
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
            pstmt.setBytes(1, data.FileID);
            pstmt.setInt(2, data.fileIDLength);
            pstmt.setString(3, data.AlgorithmenType);
            pstmt.setString(4, data.OriginalFilePath);
            pstmt.setString(5, data.EncryptedFilePath);
            pstmt.setString(6, data.DecryptedFilePath);
            pstmt.setBytes(7, data.Key);
            pstmt.setInt(8, data.keyLength);
            pstmt.setBytes(9, data.Iv);
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

    public void deleteEntry(byte[] fileID) {
        String sql = "DELETE FROM file_data WHERE FileID = ?";
        try (PreparedStatement pstmt = conn.prepareStatement(sql)) {
            pstmt.setBytes(1, fileID);
            pstmt.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public FileData searchEntry(byte[] fileID) {
        String sql = "SELECT * FROM file_data WHERE FileID = ?";
        try (PreparedStatement pstmt = conn.prepareStatement(sql)) {
            pstmt.setBytes(1, fileID);
            try (ResultSet rs = pstmt.executeQuery()) {
                if (rs.next()) {
                    FileData data = new FileData();
                    data.FileID = rs.getBytes("FileID");
                    data.fileIDLength = rs.getInt("fileIDLength");
                    data.AlgorithmenType = rs.getString("AlgorithmenType");
                    data.OriginalFilePath = rs.getString("OriginalFilePath");
                    data.EncryptedFilePath = rs.getString("EncryptedFilePath");
                    data.DecryptedFilePath = rs.getString("DecryptedFilePath");
                    data.Key = rs.getBytes("Key");
                    data.keyLength = rs.getInt("keyLength");
                    data.Iv = rs.getBytes("Iv");
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
                data.FileID = rs.getBytes("FileID");
                data.EncryptedFilePath = rs.getString("EncryptedFilePath");
                fileList.add(data);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return fileList;
    }
}