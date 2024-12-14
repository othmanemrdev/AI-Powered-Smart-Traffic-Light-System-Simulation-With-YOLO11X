#include <ESP8266WiFi.h>

// Configuration WiFi
const char* ssid = "";         // Remplace avec ton SSID
const char* password = ""; // Remplace avec ton mot de passe WiFi

WiFiServer server(80); // Serveur HTTP sur le port 80

// Pins pour les LEDs
const int ledRed1 = D4;   // D9 on arduini Wifi 
const int ledGreen1 = D2; // D4
const int ledOrange1 = D3; // D8
const int ledRed2 = D5; //  D5
const int ledGreen2 = D7; // D7
const int ledOrange2 = D6; // D6

const int defaultGreenTime=5;
// const int orangeTime=5;
int greenTimeRoad1 = defaultGreenTime;
int greenTimeRoad2 = defaultGreenTime;

void setup() {
  // Initialisation des LEDs
  pinMode(ledRed1, OUTPUT);
  pinMode(ledGreen1, OUTPUT);
  pinMode(ledOrange1, OUTPUT);
  pinMode(ledRed2, OUTPUT);
  pinMode(ledGreen2, OUTPUT);
  pinMode(ledOrange2, OUTPUT);

  // Éteindre toutes les LEDs au départ
  digitalWrite(ledRed1, LOW);
  digitalWrite(ledGreen1, LOW);
  digitalWrite(ledOrange1, LOW);
  digitalWrite(ledRed2, LOW);
  digitalWrite(ledGreen2, LOW);
  digitalWrite(ledOrange2, LOW);

  // Connexion au WiFi
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(10000);
    Serial.println(WiFi.status());
    Serial.print(".");
  }
  Serial.println("Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Démarrage du serveur
  server.begin();
  Serial.println("Server started.");
}

void loop() {
  // Vérifier si un client est connecté
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  Serial.println("Client connected.");
  String request = client.readStringUntil('\r');
  Serial.println("Request: " + request);

  // Parse des données (format attendu : /update?road1=10&road2=5)
  if (request.indexOf("GET /update") >= 0) {
    int index1 = request.indexOf("road1=") + 6;
    int index2 = request.indexOf("&road2=");
    int road1 = request.substring(index1, index2).toInt();
    int road2 = request.substring(index2 + 7).toInt();
    Serial.printf("Road 1: %d, Road 2: %d\n", road1, road2);

if (abs(road1 - road2) <= 3) {
    // Trafic équilibré
    greenTimeRoad1 = defaultGreenTime;
    greenTimeRoad2 = defaultGreenTime;
    Serial.printf("Balanced traffic condition \n");
  } else if (road1 > road2) {
    // Plus de trafic sur route 1
    greenTimeRoad1 = defaultGreenTime + min(60, (road1 - road2) * 2);
    greenTimeRoad2 = defaultGreenTime;
    Serial.printf("Road 1 has more traffic. Green time = {%d} \n", greenTimeRoad1 );
  } else {
    // Plus de trafic sur route 2
    greenTimeRoad1 = defaultGreenTime;
    greenTimeRoad2 = defaultGreenTime + min(60, (road2 - road1) * 2);
    Serial.printf("Road 2 has more traffic. Green time = {%d} \n", greenTimeRoad2 );
  }


  
  digitalWrite(ledRed1, HIGH);   // Allumer rouge pour Feu 1
  digitalWrite(ledGreen2, HIGH); // Allumer vert pour Feu 2
  delay(greenTimeRoad2 * 1000 );               // Durée verte (20 secondes)
  
  // Transition Feu 2 à orange
  digitalWrite(ledGreen2, LOW);  // Éteindre vert pour Feu 2
  for (int i = 0; i < 5; i++) { // Clignoter orange 5 fois
    digitalWrite(ledOrange2, HIGH);
    delay(1000);
    digitalWrite(ledOrange2, LOW);
    delay(500);
  }
  digitalWrite(ledRed2, HIGH);   // Allumer rouge pour Feu 2

  // Transition Feu 1 à vert
  digitalWrite(ledRed1, LOW);    // Éteindre rouge pour Feu 1
  for (int i = 0; i < 5; i++) { // Clignoter orange 5 fois
    digitalWrite(ledOrange1, HIGH);
    delay(1000);
    digitalWrite(ledOrange1, LOW);
    delay(500);
  }
  digitalWrite(ledGreen1, HIGH); // Allumer vert pour Feu 1
    delay(greenTimeRoad1 * 1000 );                     // Durée verte (20 secondes)

  // Transition Feu 1 à orange
  digitalWrite(ledGreen1, LOW);  // Éteindre vert pour Feu 1
  for (int i = 0; i < 5; i++) { // Clignoter orange 5 fois
    digitalWrite(ledOrange1, HIGH);
    delay(1000);
    digitalWrite(ledOrange1, LOW);
    delay(500);
  }
  digitalWrite(ledRed1, HIGH);   // Allumer rouge pour Feu 1

  // Transition Feu 2 à vert
  digitalWrite(ledRed2, LOW);    // Éteindre rouge pour Feu 2
  for (int i = 0; i < 5; i++) { // Clignoter orange 5 fois
    digitalWrite(ledOrange2, HIGH);
    delay(1000);
    digitalWrite(ledOrange2, LOW);
    delay(500);
  }
  digitalWrite(ledGreen2, HIGH); // Allumer vert pour Feu 2



  // Réponse au client
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println("Traffic lights updated!");
  client.stop();
}
}