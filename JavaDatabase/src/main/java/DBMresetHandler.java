import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpExchange;

import java.io.IOException;
import java.io.OutputStream;

public class DBMresetHandler implements HttpHandler {
  private Database db;

  public DBMresetHandler(Database db) {
    this.db = db;
  }

  @Override
  public void handle(HttpExchange t) throws IOException {
    try {
      db.dropTable();
      String response = "true";
      t.sendResponseHeaders(200, response.length());
      OutputStream os = t.getResponseBody();
      os.write(response.getBytes());
      os.close();
      System.out.println("Database reset successfully.");
    } catch (Exception e) {
      e.printStackTrace();
      String response = "false";
      t.sendResponseHeaders(500, response.length());
      OutputStream os = t.getResponseBody();
      os.write(response.getBytes());
      os.close();
      System.out.println("Failed to reset database.");
    }
  }
}