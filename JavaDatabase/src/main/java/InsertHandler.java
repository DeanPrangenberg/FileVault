import com.fasterxml.jackson.databind.ObjectMapper;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpExchange;

import java.io.IOException;
import java.io.OutputStream;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;

/**
 * Handles HTTP requests for inserting database entries.
 */
public class InsertHandler implements HttpHandler {
  private Database db;

  /**
   * Constructs a new InsertHandler with the specified database.
   *
   * @param db the database to insert entries into
   */
  public InsertHandler(Database db) {
    this.db = db;
  }

  /**
   * Handles an HTTP request by inserting a database entry based on the provided JSON data.
   *
   * @param t the HttpExchange containing the request and response
   * @throws IOException if an I/O error occurs
   */
  @Override
  public void handle(HttpExchange t) throws IOException {
    InputStream is = t.getRequestBody();
    byte[] inputBytes = is.readAllBytes();
    String input = new String(inputBytes, StandardCharsets.UTF_8);
    System.out.println("Received input for insert: " + input);

    // Parse input to create Database.GoFileData object (assuming JSON input)
    Database.GoFileData data = parseFileData(input);

    try {
      db.insertEntry(data);
      String response = "true";
      t.sendResponseHeaders(200, response.length());
      OutputStream os = t.getResponseBody();
      os.write(response.getBytes());
      os.close();
      System.out.println("Inserted entry with FileID: " + data.FileID + " and EncryptionID: " + data.EncryptionID);
    } catch (Exception e) {
      e.printStackTrace();
      String response = "false";
      t.sendResponseHeaders(500, response.length());
      OutputStream os = t.getResponseBody();
      os.write(response.getBytes());
      os.close();
      System.out.println("Failed to insert entry with FileID: " + data.FileID + " and EncryptionID: " + data.EncryptionID);
    }
  }

  /**
   * Parses the input JSON string to create a Database.GoFileData object.
   *
   * @param input the JSON string representing the file data
   * @return the parsed Database.GoFileData object
   */
  private Database.GoFileData parseFileData(String input) {
    ObjectMapper mapper = new ObjectMapper();
    try {
      return mapper.readValue(input, Database.GoFileData.class);
    } catch (IOException e) {
      e.printStackTrace();
      return null;
    }
  }
}