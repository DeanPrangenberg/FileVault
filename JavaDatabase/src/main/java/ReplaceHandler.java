import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import com.fasterxml.jackson.databind.ObjectMapper;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.charset.StandardCharsets;

/**
 * Handles HTTP requests for replacing database entries.
 */
public class ReplaceHandler implements HttpHandler {
  private final Database db;

  /**
   * Constructs a new ReplaceHandler with the specified database.
   *
   * @param db the database to replace entries in
   */
  public ReplaceHandler(Database db) {
    this.db = db;
  }

  /**
   * Handles an HTTP request by replacing a database entry based on the provided JSON data.
   *
   * @param exchange the HttpExchange containing the request and response
   * @throws IOException if an I/O error occurs
   */
  @Override
  public void handle(HttpExchange exchange) throws IOException {
    if ("POST".equals(exchange.getRequestMethod())) {
      InputStream is = exchange.getRequestBody();
      byte[] inputBytes = is.readAllBytes();
      String input = new String(inputBytes, StandardCharsets.UTF_8);
      System.out.println("Received input for replace: " + input);

      // Parse input to create Database.GoFileData object (assuming JSON input)
      ObjectMapper mapper = new ObjectMapper();
      Database.GoFileData data = mapper.readValue(input, Database.GoFileData.class);

      try {
        // Replace the entry in the database
        db.replaceEntry(data);
        String response = "true";
        exchange.sendResponseHeaders(200, response.getBytes().length);
        OutputStream os = exchange.getResponseBody();
        os.write(response.getBytes());
        os.close();
        System.out.println("Replaced entry with FileID: " + data.FileID + " and EncryptionID: " + data.EncryptionID);
      } catch (Exception e) {
        e.printStackTrace();
        String response = "false";
        exchange.sendResponseHeaders(500, response.length());
        OutputStream os = exchange.getResponseBody();
        os.write(response.getBytes());
        os.close();
        System.out.println("Failed to replace entry with FileID: " + data.FileID + " and EncryptionID: " + data.EncryptionID);
      }
    } else {
      exchange.sendResponseHeaders(405, -1); // Method Not Allowed
    }
  }
}