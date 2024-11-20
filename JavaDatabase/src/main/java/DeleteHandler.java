import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpExchange;
import java.io.IOException;
import java.io.OutputStream;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;

public class DeleteHandler implements HttpHandler {
    private Database db;

    public DeleteHandler(Database db) {
        this.db = db;
    }

    @Override
    public void handle(HttpExchange t) throws IOException {
        InputStream is = t.getRequestBody();
        byte[] inputBytes = is.readAllBytes();
        String input = new String(inputBytes, StandardCharsets.UTF_8);

        // Convert input to byte array (assuming input is a hex string)
        byte[] fileID = hexStringToByteArray(input);

        try {
            db.deleteEntry(fileID);
            String response = "true";
            t.sendResponseHeaders(200, response.length());
            OutputStream os = t.getResponseBody();
            os.write(response.getBytes());
            os.close();
            System.out.println("Deleted entry with FileID: " + input);
        } catch (Exception e) {
            e.printStackTrace();
            String response = "false";
            t.sendResponseHeaders(500, response.length());
            OutputStream os = t.getResponseBody();
            os.write(response.getBytes());
            os.close();
            System.out.println("Failed to delete entry with FileID: " + input);
        }
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