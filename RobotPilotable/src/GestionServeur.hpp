#ifndef GESTION_SERVEUR_H
#define GESTION_SERVEUR_H

#include <WebServer.h>



class GestionServeur {
private:
    WebServer server;

    const int led = D6; // led indiquant une adresse non trouvé
    int joyX = 0; 
    int joyY = 0;

    
    void handleRoot(); // Page web Racine 192.168.42.1
    void handleNotFound(); // Page not found 
    void handleJoystick(); // Page web du joystick 192.168.42.1/play
    void handleCoordinates(); // HTML_POST des coordonnées du joystick


public:

    GestionServeur(int port); // constructeur et le numéro du port du serveur (port 80 = http)
    void handleClient();  // fonction qui check si un client envoi une requete httpGET à une adresse défini, puis appelle la fonction correspondante
    void begin(); // initialisation des routes( ou adresses) auquel le serveur répondra
    bool isconnected(); // fait clignoter la LED 19 si un client est connecté, ou 18 sinon
    void updateLEDXY(); // faity clignoter les LED 2 3 4 et 5 en fonction des variables joyX et joyY

    int getjoyX(); // renvoi la valeur de joyX
    int getjoyY(); // renvoi la valeur de joyY


};

#endif // GESTION_SERVEUR_H

