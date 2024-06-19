#include "pilotage.h"
#include <hardware/pwm.h>
#include <Arduino.h>

#define AVANT_DROITE_PWM_PIN 12 
#define AVANT_GAUCHE_PWM_PIN 10
#define ARRIERE_DROITE_PWM_PIN 13
#define ARRIERE_GAUCHE_PWM_PIN 11
#define DIVISEUR 10 
#define WRAP 62500

Pilotage::Pilotage() {}

// Configuration des signaux pwm
void Pilotage::setup() {
    
    init_PWM_PIN(AVANT_DROITE_PWM_PIN);
    init_PWM_PIN(AVANT_GAUCHE_PWM_PIN);
    init_PWM_PIN(ARRIERE_DROITE_PWM_PIN);
    init_PWM_PIN(ARRIERE_GAUCHE_PWM_PIN);
}
// test de marche avec commandes simple
void Pilotage::essai() {
    
    commandeVitesse(0,0,0,0);
    delay(5000);
    commandeVitesse(24000,24000,0,0);
    delay(1000);
    commandeVitesse(45000,24000,0,0);
    delay(1000);
    commandeVitesse(45000,0,0,0);
    delay(1000);
    commandeVitesse(45000,0,0,45000);
    delay(1000);
    commandeVitesse(60000,0,0,60000);
    delay(5000);
    commandeVitesse(60000,0,0,24000);
    delay(5000);
}

void Pilotage::init_PWM_PIN(uint pin){
    gpio_set_function(pin, GPIO_FUNC_PWM);
    pwm_set_clkdiv(pwm_gpio_to_slice_num(pin),DIVISEUR); // On divide la fréquence de 125Mhz
    pwm_set_wrap(pwm_gpio_to_slice_num(pin),WRAP); //On règle la valeur TOP pour définir la fréquence
    pwm_set_chan_level(pwm_gpio_to_slice_num(pin),pwm_gpio_to_channel(pin),0); //On met le comparateur à 0, équivaut au duty_cycle
    pwm_set_enabled(pwm_gpio_to_slice_num(pin),true); //on active le pwm sur la slice correspondante
}

void Pilotage::commandeVitesse(uint16_t vitesseAvtD, uint16_t vitesseAvtG, uint16_t vitesseArrD, uint16_t vitesseArrG) {
    if (!(vitesseAvtD > 0 && vitesseArrD > 0)) {
        // Pas de commande de marche avant et arrière droit simultanée autorisée
        avanceDroite(vitesseAvtD);
        reculeDroite(vitesseArrD);
    }

    if (!(vitesseAvtG > 0 && vitesseArrG > 0)) {
        // Pas de commande de marche avant et arrière gauche simultanée autorisée
        avanceGauche(vitesseAvtG);
        reculeGauche(vitesseArrG);
    }
}

void Pilotage::avanceDroite(uint16_t vitesse) {
    pwm_set_chan_level(pwm_gpio_to_slice_num(AVANT_DROITE_PWM_PIN),pwm_gpio_to_channel(AVANT_DROITE_PWM_PIN),vitesse);
}

void Pilotage::avanceGauche(uint16_t vitesse) {
    pwm_set_chan_level(pwm_gpio_to_slice_num(AVANT_GAUCHE_PWM_PIN),pwm_gpio_to_channel(AVANT_GAUCHE_PWM_PIN),vitesse);   
}

void Pilotage::reculeDroite(uint16_t vitesse) {
    pwm_set_chan_level(pwm_gpio_to_slice_num(ARRIERE_DROITE_PWM_PIN),pwm_gpio_to_channel(ARRIERE_DROITE_PWM_PIN),vitesse); 
}

void Pilotage::reculeGauche(uint16_t vitesse) {
    pwm_set_chan_level(pwm_gpio_to_slice_num(ARRIERE_GAUCHE_PWM_PIN),pwm_gpio_to_channel(ARRIERE_GAUCHE_PWM_PIN),vitesse);
}

uint16_t Pilotage::pourcentageToValeur(int pourcentage) {
    // Gérer le cas spécial de 0% = pas de signal
    if (pourcentage == 0) {
        return 0;
    }

    // Calculer la valeur linéaire entre 24000 et 62500 (24000 valeur mini pour fonctionnement moteur, 62500 valeur max du wrap)
    return static_cast<uint16_t>(24000 + (pourcentage / 100.0) * (WRAP - 24000));
}

void Pilotage::commande(int x,int y){ // découpage par zone de joystick, à optimiser
//zone morte si curseur trop proche du centre
 if (abs(x) < 20) {
        x = 0;
 }
 if (abs(y) < 20) {
        y = 0;
 }
 if (x==0 && y==0){//a l'arrêt
    commandeVitesse(0,0,0,0);
 }
 else if (x==0 && y>20){//avance tout droit
    commandeVitesse(60000,60000,0,0);
 }
 else if (x==0 && y<-20){//recule tout droit
    commandeVitesse(0,0,60000,60000);
 }
 else if (x>=20 && y==0){//tourne sur soi-même vers la droite
    commandeVitesse(0,60000,60000,0);
 }
 else if (x<=-20 && y==0){//tourne sur soi-même vers la gauche
    commandeVitesse(60000,0,0,60000);
 }
 else if (x>=20 && y>=20){//tourne vers la droite en avançant
    commandeVitesse(25000,60000,0,0);
 }
 else if (x<=-20 && y>=20){//tourne vers la gauche en avançant
    commandeVitesse(60000,25000,0,0);
 }
 else if (x>=20 && y<=-20){//tourne vers la droite en reculant
    commandeVitesse(0,0,0,60000);
 }
 else if (x<=-20 && y<=-20){//tourne vers la droite en reculant
    commandeVitesse(0,0,60000,0);
 }
}