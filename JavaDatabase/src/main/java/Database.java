import java.sql.*;
import java.util.ArrayList;
import java.util.List;

/**
 * The Database class provides methods to connect to an SQLite database,
 * initialize tables, and perform CRUD operations on the `file_data` table.
 */
public class Database {
    private Connection conn;

    /**
     * Returns the path to the SQLite database file.
     *
     * @return the database file path as a String.
     */
    public String getDatabasePath() {
        return "filedata.db";
    }

    /**
     * Connects to the SQLite database. If the connection is not already established,
     * it initializes the `file_data` table.
     */
    public void connect() {
        try {
            if (conn == null || conn.isClosed()) {
Class.forName("org.sqlite.JDBC");
conn = DriverManager.getConnection("jdbc:sqlite:" + getDatabasePath());
initializeTable();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * Disconnects from the SQLite database if the connection is open.
     */
    public void disconnect() {
        try {
            if (conn != null && !conn.isClosed()) {
conn.close();
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    /**
     * Initializes the `file_data` table if it does not already exist.
     *
     * @throws SQLException if a database access error occurs.
     */
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

    /**
     * Resets the `file_data` table by deleting all its entries.
     */
    public void resetTable() {
        connect();
        String checkTableExists = "SELECT name FROM sqlite_master WHERE type='table' AND name='file_data'";
        String deleteData = "DELETE FROM file_data";
        try (Statement stmt = conn.createStatement();
             ResultSet rs = stmt.executeQuery(checkTableExists)) {
            if (rs.next()) {
stmt.execute(deleteData);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        disconnect();
    }

    /**
     * Inserts or replaces an entry in the `file_data` table.
     *
     * @param data the GoFileData object containing the data to be inserted.
     */
    public void insertEntry(GoFileData data) {
        connect();
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
        disconnect();
    }

    /**
     * Replaces an entry in the `file_data` table if the LastUpdateID has changed.
     *
     * @param data the GoFileData object containing the data to be replaced.
     */
    public void replaceEntry(GoFileData data) {
        connect();
        GoFileData searchData = searchEntry(data.FileID, data.EncryptionID);
        if (searchData.LastUpdateID != data.LastUpdateID) {
            deleteEntry(data.FileID, data.EncryptionID);
            insertEntry(data);
        }
        disconnect();
    }

    /**
     * Deletes an entry from the `file_data` table.
     *
     * @param fileID the FileID of the entry to be deleted.
     * @param encryptionID the EncryptionID of the entry to be deleted.
     */
    public void deleteEntry(String fileID, String encryptionID) {
        connect();
        String sql = "DELETE FROM file_data WHERE FileID = ? AND EncryptionID = ?";
        try (PreparedStatement pstmt = conn.prepareStatement(sql)) {
            pstmt.setString(1, fileID);
            pstmt.setString(2, encryptionID);
            pstmt.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        }
        disconnect();
    }

    /**
     * Searches for an entry in the `file_data` table.
     *
     * @param fileID the FileID of the entry to be searched.
     * @param encryptionID the EncryptionID of the entry to be searched.
     * @return a GoFileData object containing the found data, or null if not found.
     */
    public GoFileData searchEntry(String fileID, String encryptionID) {
        connect();
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
        disconnect();
        return null;
    }

    /**
     * Retrieves all entries from the `file_data` table.
     *
     * @return a List of GoFileData objects containing all entries.
     */
    public List<GoFileData> getAllEntries() {
        connect();
        List<GoFileData> entries = new ArrayList<>();
        String sql = "SELECT * FROM file_data";
        try (Statement stmt = conn.createStatement();
             ResultSet rs = stmt.executeQuery(sql)) {
            while (rs.next()) {
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
entries.add(data);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        disconnect();
        return entries;
    }

    /**
     * Retrieves all FileIDs and EncryptedFilePaths from the `file_data` table.
     *
     * @return a List of GoFileData objects containing FileIDs and EncryptedFilePaths.
     */
    public List<GoFileData> getAllFileIDsAndEncryptedPaths() {
        connect();
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
        disconnect();
        return fileList;
    }

    /**
     * The GoFileData class represents the data structure for entries in the `file_data` table.
     */
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
}