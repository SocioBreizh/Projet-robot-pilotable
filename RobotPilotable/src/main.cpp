#include <iostream>
#include <Arduino.h>
#include <hardware/pwm.h>

#define AVANT_DROITE_PWM_PIN 10 
#define AVANT_GAUCHE_PWM_PIN 12
#define ARRIERE_DROITE_PWM_PIN 11
#define ARRIERE_GAUCHE_PWM_PIN 13
#define LED_PIN_ERROR 28 // Led pour signaler un défaut
#define DIVISEUR 40  
#define WRAP 62500


//declaration des fonctions
void commandeVitesse(uint16_t vitesseAvtD, uint16_t vitesseAvtG, uint16_t vitesseArrD, uint16_t vitesseArrG);
uint16_t pourcentageToValeur(int pourcentage);
void init_PWM_PIN(uint pin);
void avanceDroite(uint16_t vitesse);
void avanceGauche(uint16_t vitesse);
void reculeDroite(uint16_t vitesse);
void reculeGauche(uint16_t vitesse);



void setup() {

    // Configuration des signaux pwm
    init_PWM_PIN(AVANT_DROITE_PWM_PIN);
    init_PWM_PIN(AVANT_GAUCHE_PWM_PIN);
    init_PWM_PIN(ARRIERE_DROITE_PWM_PIN);
    init_PWM_PIN(ARRIERE_GAUCHE_PWM_PIN);  

    

}

void loop() {
    // Utilisation des fonctions avancer/reculer par une future commande quelconque
    commandeVitesse(0,0,0,0);
    delay(5000);
    commandeVitesse(24000,24000,0,0);
    delay(1000);
    commandeVitesse(45000,45000,0,0);
    delay(1000);
    commandeVitesse(50000,50000,0,0);
    delay(1000);
    commandeVitesse(50000,0,0,50000);
    delay(1000);
    commandeVitesse(0,0,0,0);
    delay(5000);
    commandeVitesse(0,50000,50000,0);
    delay(1000);


    
}

void init_PWM_PIN(uint pin){
gpio_set_function(pin, GPIO_FUNC_PWM);
pwm_set_clkdiv(pwm_gpio_to_slice_num(pin),DIVISEUR); // On divide la fréquence de 125Mhz
pwm_set_wrap(pwm_gpio_to_slice_num(pin),WRAP); //On règle la valeur TOP pour définir la fréquence
pwm_set_chan_level(pwm_gpio_to_slice_num(pin),pwm_gpio_to_channel(pin),0); //On met le comparateur à 0, équivaut au duty_cycle
pwm_set_enabled(pwm_gpio_to_slice_num(pin),true); //on active le pwm sur la slice correspondante

}

// Fonctions de pilotage unique
void avanceDroite(uint16_t vitesse) {
  pwm_set_chan_level(pwm_gpio_to_slice_num(AVANT_DROITE_PWM_PIN),pwm_gpio_to_channel(AVANT_DROITE_PWM_PIN),vitesse);
    
}

void avanceGauche(uint16_t vitesse) {
  pwm_set_chan_level(pwm_gpio_to_slice_num(AVANT_GAUCHE_PWM_PIN),pwm_gpio_to_channel(AVANT_GAUCHE_PWM_PIN),vitesse);   
}

void reculeDroite(uint16_t vitesse) {
  pwm_set_chan_level(pwm_gpio_to_slice_num(ARRIERE_DROITE_PWM_PIN),pwm_gpio_to_channel(ARRIERE_DROITE_PWM_PIN),vitesse); 
}

void reculeGauche(uint16_t vitesse) {
  pwm_set_chan_level(pwm_gpio_to_slice_num(ARRIERE_GAUCHE_PWM_PIN),pwm_gpio_to_channel(ARRIERE_GAUCHE_PWM_PIN),vitesse);
}
    

// Fonction de commande des moteurs, avec test pour ignorer les commandes contradictoires
void commandeVitesse(uint16_t vitesseAvtD, uint16_t vitesseAvtG, uint16_t vitesseArrD, uint16_t vitesseArrG) {
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

// Conversion de la vitesse en % vers la valeur du duty_cycle
uint16_t pourcentageToValeur(int pourcentage) {
    // Gérer le cas spécial de 0% = pas de signal
    if (pourcentage == 0) {
        return 0;
    }

    // Calculer la valeur linéaire entre 24000 et 62500 (24000 valeur mini pour fonctionnement moteur, 62500 valeur max du wrap)
    return static_cast<uint16_t>(24000 + (pourcentage / 100.0) * (WRAP - 24000));
}