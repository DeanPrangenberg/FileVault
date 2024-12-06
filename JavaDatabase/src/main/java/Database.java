import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class Database {
    private Connection conn;

    public static class FileData {
        public String FileID; // Changed to String to store binary string
        public int FileIDLength; // Added to store integer value of FileID
        public String AlgorithmenType;
        public String OriginalFilePath;
        public String EncryptedFilePath;
        public String DecryptedFilePath;
        public String Key; // Changed to String to store binary string
        public int KeyLength; // Added to store integer value of Key
        public String Iv; // Changed to String to store binary string
        public int IvLength; // Added to store integer value of Iv
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

    public void dropTable() {
        connect();
        String sql = "DROP TABLE IF EXISTS file_data";
        try (Statement stmt = conn.createStatement()) {
            stmt.execute(sql);
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    private void initializeTable() throws SQLException {
        String sql = "CREATE TABLE IF NOT EXISTS file_data (" +
                "FileID TEXT PRIMARY KEY, " +
                "FileIDLength INTEGER, " +
                "AlgorithmenType TEXT, " +
                "OriginalFilePath TEXT, " +
                "EncryptedFilePath TEXT, " +
                "DecryptedFilePath TEXT, " +
                "Key TEXT, " +
                "KeyLength INTEGER, " +
                "Iv TEXT, "+
                "IvLength INTEGER)";
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
        String sql = "INSERT OR REPLACE INTO file_data (FileID, FileIDLength, AlgorithmenType, OriginalFilePath, EncryptedFilePath, DecryptedFilePath, Key, KeyLength, Iv, IvLength) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
        try (PreparedStatement pstmt = conn.prepareStatement(sql)) {
            pstmt.setString(1, data.FileID);
            pstmt.setInt(2, data.FileIDLength);
            pstmt.setString(3, data.AlgorithmenType);
            pstmt.setString(4, data.OriginalFilePath);
            pstmt.setString(5, data.EncryptedFilePath);
            pstmt.setString(6, data.DecryptedFilePath);
            pstmt.setString(7, data.Key);
            pstmt.setInt(8, data.KeyLength);
            pstmt.setString(9, data.Iv);
            pstmt.setInt(10, data.IvLength);
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
                    data.FileIDLength = rs.getInt("FileIDLength");
                    data.AlgorithmenType = rs.getString("AlgorithmenType");
                    data.OriginalFilePath = rs.getString("OriginalFilePath");
                    data.EncryptedFilePath = rs.getString("EncryptedFilePath");
                    data.DecryptedFilePath = rs.getString("DecryptedFilePath");
                    data.Key = rs.getString("Key");
                    data.KeyLength = rs.getInt("KeyLength");
                    data.Iv = rs.getString("Iv");
                    data.IvLength = rs.getInt("IvLength");
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
        String sql = "SELECT FileID, FileIDLength, EncryptedFilePath FROM file_data";
        try (Statement stmt = conn.createStatement();
             ResultSet rs = stmt.executeQuery(sql)) {
            while (rs.next()) {
                FileData data = new FileData();
                data.FileID = rs.getString("FileID");
                data.FileIDLength = rs.getInt("FileIDLength");
                data.EncryptedFilePath = rs.getString("EncryptedFilePath");
                fileList.add(data);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return fileList;
    }
}