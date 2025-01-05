import com.sun.net.httpserver.HttpServer;

import java.io.IOException;
import java.net.InetSocketAddress;

public class Main {
  public static void main(String[] args) throws IOException {
    Database db = new Database();
    db.dropTable();
    db.connect();

    HttpServer server = HttpServer.create(new InetSocketAddress(8000), 0);
    server.createContext("/api/insert", new InsertHandler(db));
    server.createContext("/api/delete", new DeleteHandler(db));
    server.createContext("/api/search", new SearchHandler(db));
    server.createContext("/api/replace", new ReplaceHandler(db));
    server.createContext("/api/getAllFileIDsAndEncryptedPaths", new GetAllFileIDsAndEncryptedPathsHandler(db));
    server.createContext("/api/getDBSize", new GetDBSizeHandler(db));

    server.createContext("/api/DBM/export", new DBMexportHandler(db));
    server.createContext("/api/DBM/insert", new DBMinsertHandler(db));
    server.createContext("/api/DBM/replace", new DBMreplaceHandler(db));
    server.createContext("/api/DBM/reset", new DBMresetHandler(db));
    server.setExecutor(null);
    server.start();

    System.out.println("Server started on port 8000");
  }
}