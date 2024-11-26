import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpExchange;

import java.io.IOException;
import java.io.OutputStream;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;

public class SearchHandler implements HttpHandler {
  private Database db;
  private ObjectMapper objectMapper = new ObjectMapper();

  public SearchHandler(Database db) {
    this.db = db;
  }

  @Override
  public void handle(HttpExchange t) throws IOException {
    InputStream is = t.getRequestBody();
    byte[] inputBytes = is.readAllBytes();
    String input = new String(inputBytes, StandardCharsets.UTF_8);
    System.out.println("Received input: " + input + " for search");

    ObjectMapper mapper = new ObjectMapper();
    JsonNode jsonNode = mapper.readTree(input);
    String fileID = jsonNode.get("FileID").asText();

    Database.FileData data = db.searchEntry(fileID);

    String response;
    if (data != null) {
      response = objectMapper.writeValueAsString(data);
      t.sendResponseHeaders(200, response.getBytes().length);
      System.out.println("Found entry with FileID: " + fileID);
    } else {
      response = "false";
      t.sendResponseHeaders(404, response.getBytes().length);
      System.out.println("Entry not found for FileID: " + fileID);
    }

    OutputStream os = t.getResponseBody();
    os.write(response.getBytes());
    os.close();
  }
}