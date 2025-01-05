import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpExchange;

import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;
import javax.crypto.spec.SecretKeySpec;
import java.io.*;
import java.nio.file.Files;
import java.security.SecureRandom;
import java.util.Base64;

public class DBMexportHandler implements HttpHandler {
  private Database db;

  public DBMexportHandler(Database db) {
    this.db = db;
  }

  @Override
  public void handle(HttpExchange t) throws IOException {
    try {
      // Read the database file
      File dbFile = new File(db.getDatabasePath());
      byte[] fileBytes = Files.readAllBytes(dbFile.toPath());

      // Generate encryption key
      KeyGenerator keyGen = KeyGenerator.getInstance("AES");
      keyGen.init(256, new SecureRandom());
      SecretKey secretKey = keyGen.generateKey();
      byte[] keyBytes = secretKey.getEncoded();

      // Encrypt the file
      Cipher cipher = Cipher.getInstance("AES");
      cipher.init(Cipher.ENCRYPT_MODE, new SecretKeySpec(keyBytes, "AES"));
      byte[] encryptedBytes = cipher.doFinal(fileBytes);

      // Prepare the response
      String encodedKey = Base64.getEncoder().encodeToString(keyBytes);
      String encodedData = Base64.getEncoder().encodeToString(encryptedBytes);
      String response = "Key: " + encodedKey + "\nData: " + encodedData;

      System.out.println("Exported database file: " + response);

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