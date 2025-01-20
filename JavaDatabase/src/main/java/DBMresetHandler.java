import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpExchange;

import java.io.IOException;
import java.io.OutputStream;

/**
 * Handles HTTP requests for resetting the database.
 */
public class DBMresetHandler implements HttpHandler {
  private Database db;

  /**
   * Constructs a new DBMresetHandler with the specified database.
   *
   * @param db the database to reset
   */
  public DBMresetHandler(Database db) {
    this.db = db;
  }

  /**
   * Handles an HTTP request by dropping the database table and sending a response.
   *
   * @param t the HttpExchange containing the request and response
   * @throws IOException if an I/O error occurs
   */
  @Override
  public void handle(HttpExchange t) throws IOException {
    try {
      // Drop the database table
      db.resetTable();
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