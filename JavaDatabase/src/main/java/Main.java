import com.sun.net.httpserver.HttpServer;

import java.io.IOException;
import java.net.InetSocketAddress;

public class Main {
  /**
   * The main method to start the HTTP server and initialize the database.
   *
   * @param args Command line arguments (not used).
   * @throws IOException If an I/O error occurs.
   */
  public static void main(String[] args) throws IOException {
    // Initialize the database
    Database db = new Database();
    db.dropTable();
    db.connect();

    // Create and configure the HTTP server
    HttpServer server = HttpServer.create(new InetSocketAddress(8000), 0);

    // Set up the context handlers for various API endpoints
    server.createContext("/api/insert", new InsertHandler(db));
    server.createContext("/api/delete", new DeleteHandler(db));
    server.createContext("/api/search", new SearchHandler(db));
    server.createContext("/api/replace", new ReplaceHandler(db));
    server.createContext("/api/getAllFileIDsAndEncryptedPaths", new GetAllFileIDsAndEncryptedPathsHandler(db));
    server.createContext("/api/getDBSize", new GetDBSizeHandler(db));

    // Set up the context handlers for DBM operations
    server.createContext("/api/DBM/export", new DBMexportHandler(db));
    server.createContext("/api/DBM/insert", new DBMinsertHandler(db));
    server.createContext("/api/DBM/replace", new DBMreplaceHandler(db));
    server.createContext("/api/DBM/reset", new DBMresetHandler(db));

    // Set the executor to null, which means the default executor will be used
    server.setExecutor(null);

    // Start the server
    server.start();

    // Print a message indicating the server has started
    System.out.println("Server started on port 8000");
  }
}