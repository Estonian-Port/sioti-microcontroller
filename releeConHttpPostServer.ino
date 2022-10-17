// --------------------------------------------------
  Author: rdzsebastian
  Template by: Makerbro
  Platforms: ESP8266
  Language: C++/Arduino
// --------------------------------------------------

// Setup the server to receive data over WiFi
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Configuration parameters for Access Point
char * ssid = "Galicia";
char * password = "Holaquetal12341";

// Set up the server object
ESP8266WebServer server; // Port 80

// Global variables
bool state = false;
int relayId = 0;

// IO configure
#define Relay1    16    //D0
#define Relay2    5     //D1

// Struct of Relay
struct relay {
  int id;
  int relay;
};

// Map of Relay with Id
relay relay1 = { 1, Relay1};
relay relay2 = { 2, Relay2};

relay relayMap[] = { relay1, relay2};

// Setup of conection server
void setup() {
  WiFi.begin(ssid,password);
  Serial.begin(115200);

	// Wait for connection
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	// Print connection IP upon success
	Serial.println("");
	Serial.println("WiFi connected");  
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());

  // ----- Configure the server's routes ----
  // Route to home
  server.on("/", home);

  // Route to get state of the Relay
  server.on("/getState", HTTP_GET, getState);

  // Route to set Relay On or Off
  server.on("/onOff", HTTP_GET, onOff);

  // ---------------------------------------

  // Enable CORS and start server
  server.enableCORS(true);
  server.begin();
  Serial.println("Server started");

  // Configure pin mode of Relays
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);

}

// Loop code of ESP8266
void loop() {
  server.handleClient();
}

// Home server message
void home() {
  serverPrint("SIOTI");
}

// Get state of the Relay
void getState() {
  relayId = server.arg("relay").toInt();

  int relayElegido = 0;

  if(relayMap[0].id == relayId){
    relayElegido = relayMap[0].relay;
  }

  if(relayMap[1].id == relayId){
    relayElegido = relayMap[1].relay;
  }
  serverPrint(String(digitalRead(relayElegido)));
}

// To set Relay On or Off
void onOff() {
  state = server.arg("estado").toInt();
  relayId = server.arg("relay").toInt();

  Serial.println(state);
  Serial.println(relayId);
  Serial.println();
  Serial.println("--------RELAY 1--------");
  Serial.println(relayMap[0].id);
  Serial.println(relayMap[0].relay);
  Serial.println();
  Serial.println("--------RELAY 2--------");
  Serial.println(relayMap[1].id);
  Serial.println(relayMap[1].relay);

  int relayElegido = 0;

  if(relayMap[0].id == relayId){
    relayElegido = relayMap[0].relay;
  }

  if(relayMap[1].id == relayId){
    relayElegido = relayMap[1].relay;
  }

  digitalWrite(relayElegido, state);
  serverPrint(String(state));
}

void serverPrint(String menssage){
  server.send(200,"text/plain", menssage);
}