import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpExchange;

import java.io.IOException;
import java.io.OutputStream;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;

/**
 * Handles HTTP requests for deleting database entries.
 */
public class DeleteHandler implements HttpHandler {
  private Database db;

  /**
   * Constructs a new DeleteHandler with the specified database.
   *
   * @param db the database to delete entries from
   */
  public DeleteHandler(Database db) {
    this.db = db;
  }

  /**
   * Handles an HTTP request by deleting a database entry based on the provided JSON data.
   *
   * @param t the HttpExchange containing the request and response
   * @throws IOException if an I/O error occurs
   */
  @Override
  public void handle(HttpExchange t) throws IOException {
    InputStream is = t.getRequestBody();
    byte[] inputBytes = is.readAllBytes();
    String input = new String(inputBytes, StandardCharsets.UTF_8);
    System.out.println("Received input for delete: " + input);

    ObjectMapper mapper = new ObjectMapper();
    JsonNode jsonNode = mapper.readTree(input);
    String fileID = jsonNode.get("FileID").asText();
    String encryptionID = jsonNode.get("EncryptionID").asText();

    try {
      // Delete the entry from the database
      db.deleteEntry(fileID, encryptionID);
      String response = "true";
      t.sendResponseHeaders(200, response.length());
      OutputStream os = t.getResponseBody();
      os.write(response.getBytes());
      os.close();
      System.out.println("Deleted entry with FileID: " + fileID + " and EncryptionID: " + encryptionID);
    } catch (Exception e) {
      e.printStackTrace();
      String response = "false";
      t.sendResponseHeaders(500, response.length());
      OutputStream os = t.getResponseBody();
      os.write(response.getBytes());
      os.close();
      System.out.println("Failed to delete entry with FileID: " + fileID + " and EncryptionID: " + encryptionID);
    }
  }
}