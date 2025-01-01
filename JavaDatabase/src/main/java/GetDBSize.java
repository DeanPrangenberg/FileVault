import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;

import java.io.File;
import java.io.IOException;
import java.io.OutputStream;

public class GetDBSize implements HttpHandler {
  private Database db;

  public GetDBSize(Database db) {
    this.db = db;
  }

  @Override
  public void handle(HttpExchange t) throws IOException {
    File dbFile = new File(db.getDatabasePath());
    long fileSizeInBytes = dbFile.length();
    long fileSizeInKB = fileSizeInBytes / 1024;

    String response = String.valueOf(fileSizeInKB);
    t.sendResponseHeaders(200, response.length());
    OutputStream os = t.getResponseBody();
    os.write(response.getBytes());
    os.close();
  }
}