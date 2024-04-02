#include "GestionServeur.hpp"



extern const char *html_joystick;

GestionServeur::GestionServeur(int port) : server(port) {
    

}

void GestionServeur::begin() {
    server.on("/", std::bind(&GestionServeur::handleRoot, this));
    server.on("/play", std::bind(&GestionServeur::handleJoystick, this));
    server.on("/update_coordinates", std::bind(&GestionServeur::handleCoordinates, this));
    server.onNotFound(std::bind(&GestionServeur::handleNotFound, this));

    server.begin();

    Serial.println("HTTP server started");
}

void GestionServeur::handleClient() {

    
    server.handleClient();
}

void GestionServeur::handleRoot() {
    server.send(200, "text/plain", "ça marche");
}

void GestionServeur::handleNotFound() {
    digitalWrite(led, 1);
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++)
    {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
    digitalWrite(led, 0);
}

void GestionServeur::handleJoystick() {
    server.send(200, "text/html", html_joystick);
}

void GestionServeur::handleCoordinates() {
    String x = server.arg("x");
    String y = server.arg("y");

    joyX = x.toInt();
    joyY = y.toInt();

    Serial.println("Lecture des informations");

// Répondre au client avec un statut 200 (OK)
    server.send(200, "text/plain", "Coordonnées reçues avec succès");

/*
    Serial.printf("X =  %d ",joyX);
    Serial.printf("Y =  %d",joyY);
    Serial.println();
*/
}

bool GestionServeur::isconnected()
{
    int nbClient = WiFi.softAPgetStationNum();
    ;

    Serial.printf("Client connected : %d \n \r", nbClient);
    Serial.println(WiFi.localIP());

    if (nbClient >= 1)
    {
        if (digitalRead(D19) == LOW)
        {
            digitalWrite(D19, HIGH);
        }
        else
            digitalWrite(D19, LOW);
        // delay(1000);
        Serial.println("Client is connected\n\r");
        digitalWrite(D18, LOW);
        return true;
    }
    else
    {
        if (digitalRead(D18) == LOW)
        {
            digitalWrite(D18, HIGH);
        }
        else
            digitalWrite(D18, LOW);

        // delay(1000);
        Serial.println("No client is connected\n\r");
        digitalWrite(D19, LOW);
        return false;
    }
    return true;
}

void GestionServeur::updateLEDXY()
{

    if (joyX > 0)
    {
        if (digitalRead(D5) == LOW)
            digitalWrite(D5, HIGH);
        else
            digitalWrite(D5, LOW);
        digitalWrite(D4, LOW);
    }
    else if (joyX < 0)
    {
        if (digitalRead(D4) == LOW)
            digitalWrite(D4, HIGH);
        else
            digitalWrite(D4, LOW);
        digitalWrite(D5, LOW);
    }
    else
    {
        digitalWrite(D5, LOW);
        digitalWrite(D4, LOW);
    }

    if (joyY > 0)
    {
        if (digitalRead(D3) == LOW)
            digitalWrite(D3, HIGH);
        else
            digitalWrite(D3, LOW);
        digitalWrite(D2, LOW);
    }
    else if (joyY < 0)
    {
        if (digitalRead(D2) == LOW)
            digitalWrite(D2, HIGH);
        else
            digitalWrite(D2, LOW);
        digitalWrite(D3, LOW);
    }
    else
    {
        digitalWrite(D3, LOW);
        digitalWrite(D2, LOW);
    }
}

int GestionServeur::getjoyX() {return joyX;}
int GestionServeur::getjoyY() {return joyY;}