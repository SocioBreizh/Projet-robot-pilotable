
// #include "Arduino.h"
// #include <WiFi.h>
// #include <WiFiClient.h>
// #include <WebServer.h>
// #include <iostream>
// #include <string>
// #include "hardware/timer.h"
// #include "HTTPServer.h"
#include <LEAmDNS.h>
#include "GestionServeur.hpp"
#include "pilotage.h"

GestionServeur gestionServeur(80); //déclaration de l'objet gestionServeur en global
Pilotage myPilotage;

void setup()
{

    delay(1000);

    Serial.begin(115200); // Démarrage de la liaison série (ttyACMx sur linux) à 115200 bauds

    WiFi.beginAP("PICOAP", "12345678"); // Démarrage du point d'accès SSID = PICOAP, pass = 12345678

    pinMode(D19, OUTPUT); // Client(s) connecté
    pinMode(D18, OUTPUT); // Pas de client connecté
    pinMode(D6, OUTPUT); // page not found
    pinMode(D5, OUTPUT); // x > 0
    pinMode(D4, OUTPUT); // x < 0
    pinMode(D3, OUTPUT); // y > 0
    pinMode(D2, OUTPUT); // y < 0

    Serial.println(WiFi.localIP()); // envoi de l'IP de l'AP via laison série (192.168.42.1 par défaut)

    if (MDNS.begin("picow")) // résolution DNS 
    {
        Serial.println("MDNS responder started");
    }

    gestionServeur.begin(); // init route http
    myPilotage.setup();
}

void fctTime() // fonctio contenant les fonctions appellées périodiquement 
{
    static const unsigned long REFRESH_INTERVAL = 1000; // ms
    static const unsigned long REFRESH_INTERVAL_XY = 500;
    static unsigned long lastRefreshTime = 0;
    static unsigned long lastRefreshTime_xy = 0;

    if (millis() - lastRefreshTime >= REFRESH_INTERVAL)
    {
        lastRefreshTime += REFRESH_INTERVAL;
        gestionServeur.isconnected();
    }
    if (millis() - lastRefreshTime_xy >= REFRESH_INTERVAL_XY)
    {
        lastRefreshTime_xy += REFRESH_INTERVAL_XY;
        gestionServeur.updateLEDXY();
        Serial.printf("X = %d ",gestionServeur.getjoyX());
        Serial.printf("Y = %d",gestionServeur.getjoyY());
        Serial.println();
    }
}


void loop(void)
{
    
    //gestionServeur.handleClient(); // Gère les requêtes client
   // MDNS.update(); // Gère la résolution DNS
    //fctTime(); // Gère les appels périodique
    //myPilotage.commande(gestionServeur.getjoyX(),gestionServeur.getjoyY());
    myPilotage.essai();
}

