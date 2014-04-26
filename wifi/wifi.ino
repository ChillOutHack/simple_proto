#include <SPI.h>
#include <WiFi.h>

// Wifi shield setup
char ssid[] = "FBI Surveillance Van #42";     // the name of your network
char pass[] = "44xzp44xzp";

int status = WL_IDLE_STATUS;     // the Wifi radio's status
byte postTo[] = {192,168,43,207};
char host[] = "192.168.43.207";

byte postTo2[] = {192,168,43,247};
char host2[] = "192.168.43.247";

WiFiClient postClient;
WiFiServer server(80);

// Pelier and potentiometer constants
int peltier = 3; //The N-Channel MOSFET is on digital pin 3
int power = 0; //Power level fro 0 to 99%
int peltier_level = map(power, 0, 99, 0, 255); //This is a value from 0 to 255 that actually controls the MOSFET
int const pot_pin = A0; //The pot input is Analog 0
int const temp_pin = A5;
int pot_val; // This is the value of the pot, with a range of 0 - 1023

void setup() {
  // initialize serial:
  Serial.begin(9600);
  while (status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, pass);
  
    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.print("Connected to the network");
  printWifiStatus();
  
  server.begin();
}

void loop(){
    // Code for pelier and potentiometer
  char option;

  pot_val = analogRead(pot_pin);
  power = map(pot_val, 0, 1023, 0, 99);
  peltier_level = map(power, 0, 99, 0, 200);
  
  // Code to get temp
  float voltage, degreesC, degreesF;
  float rawVoltage = analogRead(temp_pin);
  voltage = rawVoltage * 3.3;
  voltage/= 1024.0;
  degreesC = (voltage - 0.33) * 100.0;
  degreesF = degreesC * (9.0/5.0) + 32.0;
  Serial.print("Raw voltage: ");
  Serial.print(rawVoltage);
  Serial.print(" Voltage: ");
  Serial.print(voltage);
  Serial.print(" degreesC: ");
  Serial.print(degreesC);
  Serial.print(" degreesF: ");
  Serial.println(degreesF);
  
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.print("Pentiometer setting: ");
          client.println(pot_val);
          client.print("<br/>");
          client.print("Peltier level is ");
          client.println(peltier_level);
          client.print("<br/>");
          client.print("Temperature is: ");
          client.println(degreesF);
          client.print("<br/>");
          client.println("</html>");
           break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
  
  delay(5);
  postToServer(peltier_level, pot_val, degreesF, host, postTo);
  postToServer(peltier_level, pot_val, degreesF, host2, postTo2);
  
  analogWrite(peltier, peltier_level); //Write this new value out to the port
}

void printFirmwareVersion(){
  Serial.print("Firmware version: ");
  Serial.println(WiFi.firmwareVersion());
}

void postToServer(float peltierVoltage, float potentiometerLevel, float temperature, char host[], byte postTo[]){
   for (int i = 0; i < 1; i++)
   {
      if (postClient.connect(postTo, 5000)) {
          Serial.println("connected");
          // Make a HTTP request:
          postClient.print("GET /api/data?peltier=");
          postClient.print(peltierVoltage);
          postClient.print("&potentiometer=");
          postClient.print(potentiometerLevel);
          postClient.print("&temperature=");
          postClient.print(temperature);
          postClient.println(" HTTP/1.1");
          postClient.print("Host: ");
          postClient.println(host);
          postClient.println("Connection: close");
          postClient.println();
      } else {
        Serial.println("Could not get data from server");
      }
   }
}

void printMacAddress() {
  // the MAC address of your Wifi shield
  byte mac[6];                     

  // print your MAC address:
  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  Serial.print(mac[5],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.println(mac[0],HEX);
}

void listNetworks() {
  // scan for nearby networks:
  Serial.println("** Scan Networks **");
  byte numSsid = WiFi.scanNetworks();

  // print the list of networks seen:
  Serial.print("number of available networks:");
  Serial.println(numSsid);

  // print the network number and name for each network found:
  for (int thisNet = 0; thisNet<numSsid; thisNet++) {
    Serial.print(thisNet);
    Serial.print(") ");
    Serial.print(WiFi.SSID(thisNet));
    Serial.print("\tSignal: ");
    Serial.print(WiFi.RSSI(thisNet));
    Serial.print(" dBm");
    Serial.print("\tEncryption: ");
    Serial.println(WiFi.encryptionType(thisNet));
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
