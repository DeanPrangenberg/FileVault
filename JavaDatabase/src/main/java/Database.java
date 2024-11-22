import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class Database {
    private Connection conn;

    public static class FileData {
        public String FileID; // Changed to String to store binary string
        public String AlgorithmenType;
        public String OriginalFilePath;
        public String EncryptedFilePath;
        public String DecryptedFilePath;
        public String Key; // Changed to String to store binary string
        public String Iv; // Changed to String to store binary string
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
                "AlgorithmenType TEXT, " +
                "OriginalFilePath TEXT, " +
                "EncryptedFilePath TEXT, " +
                "DecryptedFilePath TEXT, " +
                "Key TEXT, " +
                "Iv TEXT)";
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
        String sql = "INSERT OR REPLACE INTO file_data (FileID, AlgorithmenType, OriginalFilePath, EncryptedFilePath, DecryptedFilePath, Key, Iv) VALUES (?, ?, ?, ?, ?, ?, ?)";
        try (PreparedStatement pstmt = conn.prepareStatement(sql)) {
            pstmt.setString(1, data.FileID);
            pstmt.setString(2, data.AlgorithmenType);
            pstmt.setString(3, data.OriginalFilePath);
            pstmt.setString(4, data.EncryptedFilePath);
            pstmt.setString(5, data.DecryptedFilePath);
            pstmt.setString(6, data.Key);
            pstmt.setString(7, data.Iv);
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
                    data.AlgorithmenType = rs.getString("AlgorithmenType");
                    data.OriginalFilePath = rs.getString("OriginalFilePath");
                    data.EncryptedFilePath = rs.getString("EncryptedFilePath");
                    data.DecryptedFilePath = rs.getString("DecryptedFilePath");
                    data.Key = rs.getString("Key");
                    data.Iv = rs.getString("Iv");
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
}