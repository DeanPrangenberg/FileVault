import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpExchange;

import java.io.IOException;
import java.io.OutputStream;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;

/**
 * Handles HTTP requests for searching database entries.
 */
public class SearchHandler implements HttpHandler {
  private Database db;
  private ObjectMapper objectMapper = new ObjectMapper();

  /**
   * Constructs a new SearchHandler with the specified database.
   *
   * @param db the database to search entries in
   */
  public SearchHandler(Database db) {
    this.db = db;
  }

  /**
   * Handles an HTTP request by searching for a database entry based on the provided JSON data.
   *
   * @param t the HttpExchange containing the request and response
   * @throws IOException if an I/O error occurs
   */
  @Override
  public void handle(HttpExchange t) throws IOException {
    InputStream is = t.getRequestBody();
    byte[] inputBytes = is.readAllBytes();
    String input = new String(inputBytes, StandardCharsets.UTF_8);
    System.out.println("Received input for search: " + input);

    ObjectMapper mapper = new ObjectMapper();
    JsonNode jsonNode = mapper.readTree(input);
    String fileID = jsonNode.get("FileID").asText();
    String encryptionID = jsonNode.get("EncryptionID").asText();

    Database.GoFileData data = db.searchEntry(fileID, encryptionID);

    String response;
    if (data != null) {
      response = objectMapper.writeValueAsString(data);
      t.sendResponseHeaders(200, response.getBytes().length);
      System.out.println("Found entry with FileID: " + fileID + " and EncryptionID: " + encryptionID);
    } else {
      response = "false";
      t.sendResponseHeaders(404, response.getBytes().length);
      System.out.println("Entry not found for FileID: " + fileID + " and EncryptionID: " + encryptionID);
    }

    OutputStream os = t.getResponseBody();
    os.write(response.getBytes());
    os.close();
  }
}