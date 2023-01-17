import javax.net.ssl.*;
import java.io.*;
import java.net.InetAddress;
import java.security.*;
import java.security.cert.CertificateException;

public class SSLServer {
    public static void main(String[] args) {
        try
        {
            KeyStore keyStore = KeyStore.getInstance("PKCS12");
            String password = "abcdefg";
            InputStream inputStream = ClassLoader.getSystemClassLoader().getResourceAsStream("certificates/server/certificate-server.p12");
            keyStore.load(inputStream, password.toCharArray());

            String password2 = "aabbcc";
            KeyStore trustStore = KeyStore.getInstance("PKCS12");
            TrustManagerFactory trustManagerFactory = TrustManagerFactory.getInstance("PKIX", "SunJSSE");
            InputStream inputStream1 = ClassLoader.getSystemClassLoader().getResourceAsStream("certificates/client/certificate-client.p12");
            trustStore.load(inputStream1, password2.toCharArray());
            trustManagerFactory.init(trustStore);
            X509TrustManager x509TrustManager = null;
            for (TrustManager trustManager : trustManagerFactory.getTrustManagers()) {
                if (trustManager instanceof X509TrustManager) {
                    x509TrustManager = (X509TrustManager) trustManager;
                    break;
                }
            }
            if (x509TrustManager == null) throw new NullPointerException();

            KeyManagerFactory keyManagerFactory = KeyManagerFactory.getInstance("SunX509", "SunJSSE");
            keyManagerFactory.init(keyStore, password.toCharArray());
            X509KeyManager x509KeyManager = null;
            for (KeyManager keyManager : keyManagerFactory.getKeyManagers()) {
                if (keyManager instanceof X509KeyManager) {
                    x509KeyManager = (X509KeyManager) keyManager;
                    break;
                }
            }
            if (x509KeyManager == null) throw new NullPointerException();

            SSLContext sslContext = SSLContext.getInstance("TLS");
            sslContext.init(new KeyManager[]{x509KeyManager}, new TrustManager[]{x509TrustManager}, null);

            SSLServerSocketFactory serverSocketFactory = sslContext.getServerSocketFactory();
            SSLServerSocket serverSocket = (SSLServerSocket) serverSocketFactory.createServerSocket(8333, 8333, InetAddress.getByName("192.168.1.10"));
            serverSocket.setNeedClientAuth(false);

            serverSocket.setEnabledProtocols(new String[]{"TLSv1.2"});
            System.out.println("Server started on: " + serverSocket.getInetAddress().toString() + ":8333");
            SSLSocket socket = (SSLSocket) serverSocket.accept();

            BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));

            String inputLine;

            System.out.println("Start of the Message: ");
            while ((inputLine = in.readLine()) != null)
                System.out.println(inputLine);
        }
        catch (IOException | CertificateException | NoSuchAlgorithmException | UnrecoverableKeyException |
               NoSuchProviderException | KeyStoreException | KeyManagementException e){
            e.printStackTrace();
        }
    }
}
