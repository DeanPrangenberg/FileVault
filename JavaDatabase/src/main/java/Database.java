import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class Database {
    private Connection conn;

    public String getDatabasePath() {
        return "filedata.db";
    }

    public static class GoFileData {
        public String FileID;
        public int FileIDLength;
        public String EncryptionID;
        public int EncryptionIDLength;
        public String LastUpdateID;
        public int LastUpdateIDLength;
        public String AlgorithmenType;
        public String OriginalFilePath;
        public String EncryptedFilePath;
        public String DecryptedFilePath;
        public String Key;
        public int KeyLength;
        public String Iv;
        public int IvLength;
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
            "FileID TEXT, " +
            "FileIDLength INTEGER, " +
            "EncryptionID TEXT, " +
            "EncryptionIDLength INTEGER, " +
            "LastUpdateID TEXT, " +
            "LastUpdateIDLength INTEGER, " +
            "AlgorithmenType TEXT, " +
            "OriginalFilePath TEXT, " +
            "EncryptedFilePath TEXT, " +
            "DecryptedFilePath TEXT, " +
            "Key TEXT, " +
            "KeyLength INTEGER, " +
            "Iv TEXT, "+
            "IvLength INTEGER, " +
            "PRIMARY KEY (FileID, EncryptionID))";
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

    public void insertEntry(GoFileData data) {
        String sql = "INSERT OR REPLACE INTO file_data (" +
            "FileID, " +
            "FileIDLength, " +
            "EncryptionID, " +
            "EncryptionIDLength, " +
            "LastUpdateID, " +
            "LastUpdateIDLength, " +
            "AlgorithmenType, " +
            "OriginalFilePath, " +
            "EncryptedFilePath, " +
            "DecryptedFilePath, " +
            "Key, " +
            "KeyLength, " +
            "Iv, " +
            "IvLength) " +
            "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
        try (PreparedStatement pstmt = conn.prepareStatement(sql)) {
            pstmt.setString(1, data.FileID);
            pstmt.setInt(2, data.FileIDLength);
            pstmt.setString(3, data.EncryptionID);
            pstmt.setInt(4, data.EncryptionIDLength);
            pstmt.setString(5, data.LastUpdateID);
            pstmt.setInt(6, data.LastUpdateIDLength);
            pstmt.setString(7, data.AlgorithmenType);
            pstmt.setString(8, data.OriginalFilePath);
            pstmt.setString(9, data.EncryptedFilePath);
            pstmt.setString(10, data.DecryptedFilePath);
            pstmt.setString(11, data.Key);
            pstmt.setInt(12, data.KeyLength);
            pstmt.setString(13, data.Iv);
            pstmt.setInt(14, data.IvLength);
            pstmt.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public void replaceEntry(GoFileData data) {
        GoFileData searchData = searchEntry(data.FileID, data.EncryptionID);
        if (searchData.LastUpdateID != data.LastUpdateID) {
            deleteEntry(data.FileID, data.EncryptionID);
            insertEntry(data);
        }
    }

    public void deleteEntry(String fileID, String encryptionID) {
        String sql = "DELETE FROM file_data WHERE FileID = ? AND EncryptionID = ?";
        try (PreparedStatement pstmt = conn.prepareStatement(sql)) {
            pstmt.setString(1, fileID);
            pstmt.setString(2, encryptionID);
            pstmt.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public GoFileData searchEntry(String fileID, String encryptionID) {
        String sql = "SELECT * FROM file_data WHERE FileID = ? AND EncryptionID = ?";
        try (PreparedStatement pstmt = conn.prepareStatement(sql)) {
            pstmt.setString(1, fileID);
            pstmt.setString(2, encryptionID);
            try (ResultSet rs = pstmt.executeQuery()) {
                if (rs.next()) {
                    GoFileData data = new GoFileData();
                    data.FileID = rs.getString("FileID");
                    data.FileIDLength = rs.getInt("FileIDLength");
                    data.EncryptionID = rs.getString("EncryptionID");
                    data.EncryptionIDLength = rs.getInt("EncryptionIDLength");
                    data.LastUpdateID = rs.getString("LastUpdateID");
                    data.LastUpdateIDLength = rs.getInt("LastUpdateIDLength");
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

    public List<GoFileData> getAllFileIDsAndEncryptedPaths() {
        List<GoFileData> fileList = new ArrayList<>();
        String sql = "SELECT FileID, FileIDLength, EncryptionID, EncryptionIDLength, EncryptedFilePath FROM file_data";
        try (Statement stmt = conn.createStatement();
             ResultSet rs = stmt.executeQuery(sql)) {
            while (rs.next()) {
                GoFileData data = new GoFileData();
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