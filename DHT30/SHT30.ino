#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <WEMOS_SHT3X.h>

SHT3X sht30(0x45);

const char* ssid = "SSID"; // SETUP: Set this (WiFi SSID).
const char* password = "Password"; // SETUP: Set this (WiFi Password).

byte grhost[] = {1, 3, 3, 7}; // SETUP: Set this (InfluxDB IP).

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(1000);
  if(sht30.get()==0){
    float h = sht30.humidity;
    float t = sht30.cTemp;
    float f = sht30.fTemp;

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print(f);
    Serial.print(" *F\t");

    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Reconnecting to WiFi");
    }
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.println("connecting to grafana");
    WiFiClient client;
    if (!client.connect(grhost, 8086)) {
      Serial.println("connection failed");
      return;
    }

    String url = "/write?u=username&p=password&db=database"; // SETUP: Set this (u= is username, p= is password, db= is database).

    String grcontent = "devicename humidity="+String(h)+",celcius="+String(t)+",fahrenheit="+String(f);  // SETUP: Set this (replace devicename with whatever you want to call it).
    Serial.print("Requesting URL: ");
    Serial.println(url);
    Serial.print("Requesting content: ");
    Serial.println(grcontent);

    client.print(String("POST ") + url + " HTTP/1.1\r\n" +
     "Host: 1.3.3.7\r\n" // SETUP: Set this (insert InfluxDB IP).
     "Content-Type: application/x-www-form-urlencoded\r\n" + 
     "Content-Length: "+grcontent.length()+"\r\n" + 
     "Connection: close\r\n\r\n"+
     grcontent);
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }

  // Read all the lines of the reply from server and print them to Serial
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }

    Serial.println();
    Serial.println("closing connection");
  }
  else
  {
    Serial.println("Error reading sensor!");
  }
}