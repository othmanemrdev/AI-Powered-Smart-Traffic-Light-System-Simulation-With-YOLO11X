#include <ESP8266WiFi.h>

// Configuration WiFi
const char* ssid = "Othmane";         // Remplace avec ton SSID
const char* password = "JDKD82JD72J83@"; // Remplace avec ton mot de passe WiFi

WiFiServer server(80); // Serveur HTTP sur le port 80

// Pins pour les LEDs
const int ledRed1 = D4;
const int ledGreen1 = D2;
const int ledOrange1 = D3; // LED orange pour la route 1
const int ledRed2 = D5;
const int ledGreen2 = D7;
const int ledOrange2 = D6; // LED orange pour la route 2

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



/*

// Définition de la pin où est connectée la LED
const int r1 = D4; //d9
const int y1_pin = D3; // Renommée pour éviter le conflit //d8
const int g1 = D2; //d4

const int r2 = D5; //d5
const int y2_pin = D6; //d6
const int g2 = D7; //d7

void setup() {
  // Configuration de la pin LED en sortie
  pinMode(r1, OUTPUT);
  pinMode(y1_pin, OUTPUT);
  pinMode(g1, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(y2_pin, OUTPUT);
  pinMode(g2, OUTPUT);

  // Initialisation
  Serial.begin(115200);
  Serial.println("Test de la LED démarré");
}

void loop() {
  // Allumer la LED
  digitalWrite(r1, HIGH);
  Serial.println("LED rouge 1 allumée");
  delay(1000);

  // Éteindre la LED
  digitalWrite(r1, LOW);
  Serial.println("LED rouge 1 éteinte");
  delay(1000);

  digitalWrite(y1_pin, HIGH); // Utiliser le nouveau nom
  Serial.println("LED jaune 1 allumée");
  delay(1000);

  // Éteindre la LED
  digitalWrite(y1_pin, LOW); // Utiliser le nouveau nom
  Serial.println("LED jaune 1 éteinte");
  delay(1000);

  digitalWrite(g1, HIGH);
  Serial.println("LED verte 1 allumée");
  delay(1000);

  // Éteindre la LED
  digitalWrite(g1, LOW);
  Serial.println("LED verte 1 éteinte");
  delay(1000);

    // Allumer la LED
  digitalWrite(r2, HIGH);
  Serial.println("LED rouge 1 allumée");
  delay(1000);

  // Éteindre la LED
  digitalWrite(r2, LOW);
  Serial.println("LED rouge 1 éteinte");
  delay(1000);


  digitalWrite(y2_pin, HIGH); // Utiliser le nouveau nom
  Serial.println("LED jaune 1 allumée");
  delay(1000);

  // Éteindre la LED
  digitalWrite(y2_pin, LOW); // Utiliser le nouveau nom
  Serial.println("LED jaune 1 éteinte");
  delay(1000);
 
  digitalWrite(g2, HIGH);
  Serial.println("LED verte 1 allumée");
  delay(1000);

  // Éteindre la LED
  digitalWrite(g2, LOW);
  Serial.println("LED verte 1 éteinte");
  delay(1000);

}
 */