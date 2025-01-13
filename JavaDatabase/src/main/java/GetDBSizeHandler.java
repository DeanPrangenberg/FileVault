import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;

import java.io.File;
import java.io.IOException;
import java.io.OutputStream;

/**
 * Handles HTTP requests for retrieving the size of the database.
 */
public class GetDBSizeHandler implements HttpHandler {
  private Database db;

  /**
   * Constructs a new GetDBSizeHandler with the specified database.
   *
   * @param db the database to get the size of
   */
  public GetDBSizeHandler(Database db) {
    this.db = db;
  }

  /**
   * Handles an HTTP request by retrieving the size of the database file and sending it as a response.
   *
   * @param t the HttpExchange containing the request and response
   * @throws IOException if an I/O error occurs
   */
  @Override
  public void handle(HttpExchange t) throws IOException {
    // Get the database file
    File dbFile = new File(db.getDatabasePath());
    // Calculate the file size in kilobytes
    long fileSizeInBytes = dbFile.length();
    long fileSizeInKB = fileSizeInBytes / 1024;

    // Send the file size as the response
    String response = String.valueOf(fileSizeInKB);
    t.sendResponseHeaders(200, response.length());
    OutputStream os = t.getResponseBody();
    os.write(response.getBytes());
    os.close();
  }
}