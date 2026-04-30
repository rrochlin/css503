
/** ------------- DateServerThreadPool.java -------------------------------
 * HW 3: 4.26
 *
 * Author : Robert Rochlin
 * Course: GCSDD CSS 503 Systems Programming
 *
 * Creation Date: 2026-04-22
 * Date of Last Modification: 2026-04-22
 * -------------------------------------------------------------------------
 * [Purpose]
 * This program accepts client requests using a Thread Pool
 * -------------------------------------------------------------------------
 */

import java.net.*;
import java.io.*;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class DateServerThreadPool {
    public static void main(String[] args) {
        ExecutorService executor = Executors.newFixedThreadPool(10);
        try {
            ServerSocket sock = new ServerSocket(6013);
            // 10 threads because why not
            while (true) {
                Socket client = sock.accept();
                executor.submit(() -> handleRequest(client));
            }
        } catch (IOException ie) {
        } finally {
            executor.shutdown();
        }
    }

    private static void handleRequest(Socket client) {
        try (client) {
            PrintWriter pout = new PrintWriter(client.getOutputStream(), true);
            pout.println(new java.util.Date().toString());
            client.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
