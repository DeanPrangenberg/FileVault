import com.fasterxml.jackson.databind.ObjectMapper;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpExchange;

import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;
import javax.crypto.spec.SecretKeySpec;
import java.io.*;
import java.nio.charset.StandardCharsets;
import java.security.SecureRandom;
import java.util.Base64;
import java.util.List;

/**
 * Handles HTTP requests for exporting database entries.
 */
public class DBMexportHandler implements HttpHandler {
  private Database db;

  /**
   * Constructs a new DBMexportHandler with the specified database.
   *
   * @param db the database to export entries from
   */
  public DBMexportHandler(Database db) {
    this.db = db;
  }

  /**
   * Handles an HTTP request by exporting all database entries as encrypted JSON.
   *
   * @param t the HttpExchange containing the request and response
   * @throws IOException if an I/O error occurs
   */
  @Override
  public void handle(HttpExchange t) throws IOException {
    try {
      // Read all entries from the database
      List<Database.GoFileData> dataEntries = db.getAllEntries();

      // Convert the data entries to JSON
      ObjectMapper mapper = new ObjectMapper();
      String jsonData = mapper.writeValueAsString(dataEntries);

      // Generate encryption key
      KeyGenerator keyGen = KeyGenerator.getInstance("AES");
      keyGen.init(256, new SecureRandom());
      SecretKey secretKey = keyGen.generateKey();
      byte[] keyBytes = secretKey.getEncoded();

      // Encrypt the JSON data
      Cipher cipher = Cipher.getInstance("AES");
      cipher.init(Cipher.ENCRYPT_MODE, new SecretKeySpec(keyBytes, "AES"));
      byte[] encryptedBytes = cipher.doFinal(jsonData.getBytes(StandardCharsets.UTF_8));

      // Prepare the response
      String encodedKey = Base64.getEncoder().encodeToString(keyBytes);
      String encodedData = Base64.getEncoder().encodeToString(encryptedBytes);
      String response = "{\"Key\":\"" + encodedKey + "\", \"Data\":\"" + encodedData + "\"}";

      System.out.println("Exported database data: " + response);

      // Send the response
      t.sendResponseHeaders(200, response.length());
      OutputStream os = t.getResponseBody();
      os.write(response.getBytes());
      os.close();
    } catch (Exception e) {
      e.printStackTrace();
      String response = "false";
      t.sendResponseHeaders(500, response.length());
      OutputStream os = t.getResponseBody();
      os.write(response.getBytes());
      os.close();
    }
  }
}