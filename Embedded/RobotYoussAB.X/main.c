/* 
 * File:   newmain.c
 * Author: Youssef ABLALI
 *
 * Created on 21 septembre 2022, 16:41
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"


int main (void){
/***************************************************************************************************/
//Initialisation de l?oscillateur
/****************************************************************************************************/
InitOscillator();
InitTimer1();
InitTimer23();
/****************************************************************************************************/
// Configuration des entrées sorties
/****************************************************************************************************/
InitIO();

LED_BLANCHE = 1;
LED_BLEUE = 1;
LED_ORANGE = 1;

InitPWM();


//PWMSetSpeed(20,MOTEUR_GAUCHE);
//PWMSetSpeed(20,MOTEUR_DROIT);


/****************************************************************************************************/
// Boucle Principale
/****************************************************************************************************/
while (1){
    
} // fin main
}
