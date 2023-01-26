# Secure Smart Robot Car with Arduino 

Basically, in this project the idea is to add a feature to the one in this repository [here](https://github.com/LamFra/Smart-Robot-Car-Arduino) to make communication between client and server more secure by the use of TLS/SSL. 

In a TSL/SSL communication:
1. The client sends all the necessary information (SSL version number, compression method, session-specific data ) to the server to communicate
2. The server chooses a cipher and a compression method that support both and tells the client its choice
3.  The server submits its certificate
4. The client presents a certificate signed by one of the CAs and sends the pre-master (based on the cipher) encrypted with the server's public key to the server
5. The server accepts the certificate presented by the client. The server uses its private key to decrypt the pre-master's secret. Both client and server perform the steps to generate the master secret with the agreed cipher
6. Both client and server exchange messages to inform each other that future messages will be encrypted

The idea is to transmit the captured information of the GY521 accelerometer gyroscope mounted on the smart machine to the WiFi kit 8 module that interfaces with the server with a TLS/SSL communication.

![diagram](https://github.com/LamFra/Secure-Smart-Car/blob/main/idea.png?raw=true)

## Developed with

  - [Arduino IDE](https://www.arduino.cc/en/software)
  - [IntelliJ IDEA](https://www.jetbrains.com/idea/download)
  - Robot Car kit
  - SMRAZA compatible with Arduino Uno
  - Motor Driver Controller L298N
  - Gyro Accelerometer GY-521
  - WiFi Kit 8 ESP8266

## Authors

  - **Francesca La Manna** - f.lamanna5@studenti.unisa.it
  - **Gioacchino Caliendo** - g.caliendo16@studenti.unisa.it
