// DBMreplaceHandler.java
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpExchange;

import javax.crypto.Cipher;
import javax.crypto.spec.SecretKeySpec;
import java.io.IOException;
import java.io.OutputStream;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.util.Base64;

public class DBMreplaceHandler implements HttpHandler {
  private Database db;

  public DBMreplaceHandler(Database db) {
    this.db = db;
  }

  @Override
  public void handle(HttpExchange t) throws IOException {
    InputStream is = t.getRequestBody();
    byte[] inputBytes = is.readAllBytes();
    String input = new String(inputBytes, StandardCharsets.UTF_8);

    ObjectMapper mapper = new ObjectMapper();
    JsonNode jsonNode = mapper.readTree(input);
    String encodedKey = jsonNode.get("Key").asText();
    String encodedData = jsonNode.get("Data").asText();

    try {
      // Decrypt the data
      byte[] keyBytes = Base64.getDecoder().decode(encodedKey);
      byte[] encryptedBytes = Base64.getDecoder().decode(encodedData);
      Cipher cipher = Cipher.getInstance("AES");
      cipher.init(Cipher.DECRYPT_MODE, new SecretKeySpec(keyBytes, "AES"));
      byte[] decryptedBytes = cipher.doFinal(encryptedBytes);
      String decryptedData = new String(decryptedBytes, StandardCharsets.UTF_8);

      // Parse the decrypted data as a list of entries
      Database.GoFileData[] dataEntries = mapper.readValue(decryptedData, Database.GoFileData[].class);

      // Drop the existing table and reinitialize it
      db.dropTable();
      db.connect();

      // Insert all entries into the database
      for (Database.GoFileData data : dataEntries) {
        db.insertEntry(data);
      }

      String response = "true";
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