import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import com.fasterxml.jackson.databind.ObjectMapper;

import java.io.IOException;
import java.io.OutputStream;
import java.util.List;

/**
 * Handles HTTP requests for retrieving all file IDs and their encrypted paths.
 */
public class GetAllFileIDsAndEncryptedPathsHandler implements HttpHandler {
    private final Database db;

    /**
     * Constructs a new GetAllFileIDsAndEncryptedPathsHandler with the specified database.
     *
     * @param db the database to retrieve file IDs and encrypted paths from
     */
    public GetAllFileIDsAndEncryptedPathsHandler(Database db) {
        this.db = db;
    }

    /**
     * Handles an HTTP request by retrieving all file IDs and their encrypted paths from the database.
     *
     * @param exchange the HttpExchange containing the request and response
     * @throws IOException if an I/O error occurs
     */
    @Override
    public void handle(HttpExchange exchange) throws IOException {
        if ("GET".equals(exchange.getRequestMethod())) {
            // Retrieve all file IDs and encrypted paths from the database
            List<Database.GoFileData> fileList = db.getAllFileIDsAndEncryptedPaths();
            ObjectMapper mapper = new ObjectMapper();
            String response = mapper.writeValueAsString(fileList);

            // Send the response
            exchange.sendResponseHeaders(200, response.getBytes().length);
            OutputStream os = exchange.getResponseBody();
            os.write(response.getBytes());
            os.close();

            System.out.println("Retrieved all file IDs and encrypted paths");
        } else {
            // Method not allowed
            exchange.sendResponseHeaders(405, -1); // Method Not Allowed
        }
    }
}