
/** ------------- DateServerMultiThread.java -------------------------------
 * HW 3: 4.26
 *
 * Author : Robert Rochlin
 * Course: GCSDD CSS 503 Systems Programming
 *
 * Creation Date: 2026-04-22
 * Date of Last Modification: 2026-04-22
 * -------------------------------------------------------------------------
 * [Purpose]
 * This program accepts client requests servicing them each with a new Thread
 * -------------------------------------------------------------------------
 */

import java.net.*;
import java.io.*;

public class DateServerMultiThread {
    public static void main(String[] args) {
        try {
            ServerSocket sock = new ServerSocket(6013);
            while (true) {
                Socket client = sock.accept();
                Thread thread = new Thread(new ServiceRequest(client));
                thread.start();
            }
        } catch (IOException ie) {
        }
    }
}

class ServiceRequest implements Runnable {
    Socket client;

    ServiceRequest(Socket client) {
        this.client = client;
    }

    public void run() {
        try {
            PrintWriter pout = new PrintWriter(this.client.getOutputStream(), true);
            pout.println(new java.util.Date().toString());
            this.client.close();
        } catch (IOException e) {

            e.printStackTrace();
        }
    }
}
