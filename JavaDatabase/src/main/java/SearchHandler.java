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

        // Convert input to byte array (assuming input is a hex string)
        byte[] fileID = hexStringToByteArray(input);
        Database.FileData data = db.searchEntry(fileID);

        String response;
        if (data != null) {
            response = objectMapper.writeValueAsString(data);
            t.sendResponseHeaders(200, response.getBytes().length);
            System.out.println("Found entry with FileID: " + input);
        } else {
            response = "{\"message\": \"File not found\"}";
            t.sendResponseHeaders(404, response.getBytes().length);
            System.out.println("Entry not found for FileID: " + input);
        }

        OutputStream os = t.getResponseBody();
        os.write(response.getBytes());
        os.close();
    }

    private byte[] hexStringToByteArray(String s) {
        int len = s.length();
        byte[] data = new byte[len / 2];
        for (int i = 0; i < len; i += 2) {
            data[i / 2] = (byte) ((Character.digit(s.charAt(i), 16) << 4)
                    + Character.digit(s.charAt(i+1), 16));
        }
        return data;
    }
}