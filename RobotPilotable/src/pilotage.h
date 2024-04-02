#ifndef PILOTAGE_H
#define PILOTAGE_H

#include <hardware/pwm.h>
#include <Arduino.h>

class Pilotage {
private:
    uint16_t pourcentageToValeur(int pourcentage);
    void init_PWM_PIN(uint pin);
    void avanceDroite(uint16_t vitesse);
    void avanceGauche(uint16_t vitesse);
    void reculeDroite(uint16_t vitesse);
    void reculeGauche(uint16_t vitesse);

public:
    Pilotage();
    void setup();
    void loop();
    void commandeVitesse(uint16_t vitesseAvtD, uint16_t vitesseAvtG, uint16_t vitesseArrD, uint16_t vitesseArrG);
};

#endif // PILOTAGE_H
