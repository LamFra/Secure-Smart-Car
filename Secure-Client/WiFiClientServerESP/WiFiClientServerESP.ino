#include <WiFiClientSecure.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

const char* ssid     = "";     // network SSID (name of wifi network)
const char* password = ""; // network password

const char*  server = "";  // Server URL

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;
unsigned long message_counter = 0;

const int MPU=0x68; 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

WiFiClientSecure client;

const char* test_root_ca= \
"-----BEGIN CERTIFICATE-----\n" \

"-----END CERTIFICATE-----\n" ;

const char* test_client_key = \
"-----BEGIN PRIVATE KEY-----\n" \

"-----END PRIVATE KEY-----\n" ;


const char* test_client_cert =  \
"-----BEGIN CERTIFICATE-----\n" \

"-----END CERTIFICATE-----\n" ;

void setup() 
{
  client.setInsecure();
  Serial.begin(115200);
  delay(100);
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    // wait 1 second for re-trying
    delay(1000);
  }
  //Amoled writing 
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
  { 
    Serial.println(F("SSD1306 allocation failed"));
  }
  else
  {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println("Connected to wifi:");
    display.println(ssid);
    display.display();
    delay(3000);
  }
  
  //Gyroscope initialization
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  Serial.print("Connected to ");
  Serial.println(ssid);

  if(!client.setCACert((const uint8_t*)test_root_ca, sizeof(test_root_ca)))
    Serial.println("Failed loading CA");
  if(!client.setCertificate((const uint8_t*)test_client_cert, sizeof(test_client_cert))) // for client verification
    Serial.println("Failed loading client cert");
  if(!client.setPrivateKey((const uint8_t*)test_client_key, sizeof(test_client_key)))	// for client verification
    Serial.println("Failed client key");

  Serial.println("\nStarting connection to server...");
  if (!client.connect(server, 8333)){
    Serial.println("Connection failed!");
    while(true);
  }
  
  Serial.println("Connected to server!");
}

void loop() 
{
  if ((millis() - lastTime) > timerDelay) 
  {
    if(WiFi.status() == WL_CONNECTED){
      message_counter++;
      //Gettig measures from gyroscope
      int AX, AY, AZ, GX, GY, GZ, Tmp;
      Wire.beginTransmission(MPU);
      Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
      Wire.endTransmission(false);
      Wire.requestFrom(MPU,14,true); // request a total of 14 registers
      AX=Wire.read()<<8|Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
      AY=Wire.read()<<8|Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
      AZ=Wire.read()<<8|Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
      Tmp=Wire.read()<<8|Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
      GX=Wire.read()<<8|Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
      GY=Wire.read()<<8|Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
      GZ=Wire.read()<<8|Wire.read(); // 0x47 (GYRO_YOUT_H) & 0x46 (GYRO_ZOUT_L)
      String messageOled = "Axel: " + String(AX) +" " + String(AY) + " " + String(AZ) + "\n\nGyro: " + String(GX) + " " + String(GY) + " " + String(GZ); 
      
      //Displaying data on amoled 
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println(messageOled);
      display.display();

      //Sending data to the server
      String jsondata = String("{\n\"mnumber\": \"")+String(message_counter)+String("\",\n\"gyrox\": \"")+String(GX)+String("\",\n\"gyroy\": \"")+String(GY)+String("\",\n\"gyroz\": \"")+String(GZ)+String("\",\n\"accx\": \"")+String(AX)+String("\",\n\"accy\": \"")+String(AY)+String("\",\n\"accz\": \"")+String(AZ)+String("\"\n}");
      
      client.println(jsondata);
      Serial.println(jsondata);
    }
    else 
    {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}