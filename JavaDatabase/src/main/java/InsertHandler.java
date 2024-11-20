import com.fasterxml.jackson.databind.ObjectMapper;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpExchange;
import java.io.IOException;
import java.io.OutputStream;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;

public class InsertHandler implements HttpHandler {
    private Database db;

    public InsertHandler(Database db) {
        this.db = db;
    }

    @Override
    public void handle(HttpExchange t) throws IOException {
        InputStream is = t.getRequestBody();
        byte[] inputBytes = is.readAllBytes();
        String input = new String(inputBytes, StandardCharsets.UTF_8);

        // Parse input to create Database.FileData object (assuming JSON input)
        Database.FileData data = parseFileData(input);

        try {
            db.insertEntry(data);
            String response = "true";
            t.sendResponseHeaders(200, response.length());
            OutputStream os = t.getResponseBody();
            os.write(response.getBytes());
            os.close();
            System.out.println("Inserted entry with FileID: " + new String(data.FileID, StandardCharsets.UTF_8));
        } catch (Exception e) {
            e.printStackTrace();
            String response = "false";
            t.sendResponseHeaders(500, response.length());
            OutputStream os = t.getResponseBody();
            os.write(response.getBytes());
            os.close();
            System.out.println("Failed to insert entry with FileID: " + new String(data.FileID, StandardCharsets.UTF_8));
        }
    }

    private Database.FileData parseFileData(String input) {
        ObjectMapper mapper = new ObjectMapper();
        try {
            return mapper.readValue(input, Database.FileData.class);
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
    }
}