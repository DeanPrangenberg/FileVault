import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import com.fasterxml.jackson.databind.ObjectMapper;

import java.io.IOException;
import java.io.OutputStream;
import java.util.List;

public class GetAllFileIDsAndEncryptedPathsHandler implements HttpHandler {
    private final Database db;

    public GetAllFileIDsAndEncryptedPathsHandler(Database db) {
        this.db = db;
    }

    @Override
    public void handle(HttpExchange exchange) throws IOException {
        if ("GET".equals(exchange.getRequestMethod())) {
            List<Database.GoFileData> fileList = db.getAllFileIDsAndEncryptedPaths();
            ObjectMapper mapper = new ObjectMapper();
            String response = mapper.writeValueAsString(fileList);

            exchange.sendResponseHeaders(200, response.getBytes().length);
            OutputStream os = exchange.getResponseBody();
            os.write(response.getBytes());
            os.close();

            System.out.println("Retrieved all file IDs and encrypted paths");
        } else {
            exchange.sendResponseHeaders(405, -1); // Method Not Allowed
        }
    }
}