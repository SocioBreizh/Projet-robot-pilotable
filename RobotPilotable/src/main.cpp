#include <Arduino.h>
#include "hardware/pwm.h"

// put function declarations here:
void avancer();

void setup() {
  //gp10 et 12 pour avancer
  //gp11 et 13 pour reculer
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
    const uint pwm_pin_droite = 10; 
  gpio_set_function(pwm_pin_droite, GPIO_FUNC_PWM);
  const uint sliceDroite = pwm_gpio_to_slice_num(pwm_pin_droite); // slice associée à GP10
  const uint channelDroite = pwm_gpio_to_channel(pwm_pin_droite);  // channel associée à GP10
  pwm_set_clkdiv(sliceDroite, 256);
  pwm_set_wrap(sliceDroite,9764);
  pwm_set_chan_level(sliceDroite, channelDroite, 9000);

      const uint pwm_pin_gauche = 12; 
  gpio_set_function(pwm_pin_gauche, GPIO_FUNC_PWM);
  const uint sliceGauche = pwm_gpio_to_slice_num(pwm_pin_gauche); // slice associée à GP10
  const uint channelGauche = pwm_gpio_to_channel(pwm_pin_gauche);  // channel associée à GP10
  pwm_set_clkdiv(sliceGauche, 256);
  pwm_set_wrap(sliceGauche,9764);
  pwm_set_chan_level(sliceGauche, channelGauche, 0);

pwm_set_enabled(sliceGauche,true);
  pwm_set_enabled(sliceDroite,true);
  delay (5000);
  pwm_set_enabled(sliceDroite,false);
  pwm_set_enabled(sliceGauche,false);
  delay(5000);
}

// put function definitions here:
void myFunction() {
}