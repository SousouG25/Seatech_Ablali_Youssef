//Partie PWM
#include <xc.h> // library xc.h inclut tous les uC
#include "IO.h"  
#include "PWM.h" 
#include "Robot.h"   
#include "ToolBox.h"

#define PWMPER 40.0

float acceleration = 1.0;

void InitPWM(void) {
    PTCON2bits.PCLKDIV = 0b000; //Divide by 1
    PTPER = 100 * PWMPER; //Période en pourcentage

    //Réglage PWM moteur 1 sur hacheur 1
    IOCON1bits.POLH = 1; //High = 1 and active on low =0
    IOCON1bits.POLL = 1; //High = 1
    IOCON1bits.PMOD = 0b01; //Set PWM Mode to Redundant
    FCLCON1 = 0x0003; //Désactive la gestion des faults

    //Reglage PWM moteur 2 sur hacheur 6
    IOCON6bits.POLH = 1; //High = 1
    IOCON6bits.POLL = 1; //High = 1
    IOCON6bits.PMOD = 0b01; //Set PWM Mode to Redundant
    FCLCON6 = 0x0003; //Désactive la gestion des faults

    /* Enable PWM Module */
    PTCONbits.PTEN = 1;
}

/*void PWMSetSpeed(float vitesseEnPourcents, int numeroMoteur)
{ //Moteur Gauche ou 0
if (numeroMoteur == 0){
    if (vitesseEnPourcents>=0){
    robotState.vitesseGaucheCommandeCourante = vitesseEnPourcents;
    MOTEUR_GAUCHE_L_PWM_ENABLE = 0; //Pilotage de la pin en mode IO
    MOTEUR_GAUCHE_L_IO_OUTPUT = 1; //Mise à 1 de la pin
    MOTEUR_GAUCHE_H_PWM_ENABLE = 1; //Pilotage de la pin en mode PWM
    MOTEUR_GAUCHE_DUTY_CYCLE = Abs(robotState.vitesseGaucheCommandeCourante*PWMPER);   
} else {
    robotState.vitesseGaucheCommandeCourante = vitesseEnPourcents;
    MOTEUR_GAUCHE_L_PWM_ENABLE = 1; //Pilotage de la pin en mode IO
    MOTEUR_GAUCHE_H_IO_OUTPUT = 1; //Mise à 1 de la pin
    MOTEUR_GAUCHE_H_PWM_ENABLE = 0; //Pilotage de la pin en mode PWM
    MOTEUR_GAUCHE_DUTY_CYCLE = Abs(robotState.vitesseGaucheCommandeCourante*PWMPER);
}
    //Moteur droit ou 1
} else if (numeroMoteur == 1 ){
    if (vitesseEnPourcents>=0){
    robotState.vitesseGaucheCommandeCourante = vitesseEnPourcents;
    MOTEUR_DROIT_L_PWM_ENABLE = 0; //Pilotage de la pin en mode IO
    MOTEUR_DROIT_L_IO_OUTPUT = 1; //Mise à 1 de la pin
    MOTEUR_DROIT_H_PWM_ENABLE = 1; //Pilotage de la pin en mode PWM
    MOTEUR_DROIT_DUTY_CYCLE = Abs(robotState.vitesseGaucheCommandeCourante*PWMPER);   
    } else {
    robotState.vitesseGaucheCommandeCourante = vitesseEnPourcents;
    MOTEUR_DROIT_L_PWM_ENABLE = 1; //Pilotage de la pin en mode IO
    MOTEUR_DROIT_H_IO_OUTPUT = 1; //Mise à 1 de la pin
    MOTEUR_DROIT_H_PWM_ENABLE = 0; //Pilotage de la pin en mode PWM
    MOTEUR_DROIT_DUTY_CYCLE = Abs(robotState.vitesseGaucheCommandeCourante*PWMPER);
}

    }

}*/



void PWMUpdateSpeed() {

    // Cette fonction est appelée sur timer et permet de suivre des rampes d?accélération
    if (robotState.vitesseDroiteCommandeCourante < robotState.vitesseDroiteConsigne) {
        robotState.vitesseDroiteCommandeCourante = Min(
                robotState.vitesseDroiteCommandeCourante + acceleration,
                robotState.vitesseDroiteConsigne);
    } else if (robotState.vitesseDroiteCommandeCourante > robotState.vitesseDroiteConsigne) {
        robotState.vitesseDroiteCommandeCourante = Max(
                robotState.vitesseDroiteCommandeCourante - acceleration,
                robotState.vitesseDroiteConsigne);
    } else if (robotState.vitesseDroiteCommandeCourante > 0) {
        MOTEUR_DROIT_L_PWM_ENABLE = 0; //pilotage de la pin en mode IO
        MOTEUR_DROIT_L_IO_OUTPUT = 1; //Mise à 1 de la pin
        MOTEUR_DROIT_H_PWM_ENABLE = 1; //Pilotage de la pin en mode PWM
    } else {
        MOTEUR_DROIT_H_PWM_ENABLE = 0; //pilotage de la pin en mode IO
        MOTEUR_DROIT_H_IO_OUTPUT = 1; //Mise à 1 de la pin
        MOTEUR_DROIT_L_PWM_ENABLE = 1; //Pilotage de la pin en mode PWM
    }
    MOTEUR_DROIT_DUTY_CYCLE = Abs(robotState.vitesseDroiteCommandeCourante) * PWMPER;

    if (robotState.vitesseGaucheCommandeCourante < robotState.vitesseGaucheConsigne)
    {
        robotState.vitesseGaucheCommandeCourante = Min(
                robotState.vitesseGaucheCommandeCourante + acceleration,
                robotState.vitesseGaucheConsigne);
    }
    else if (robotState.vitesseGaucheCommandeCourante > robotState.vitesseGaucheConsigne)
    {
        robotState.vitesseGaucheCommandeCourante = Max(
                robotState.vitesseGaucheCommandeCourante - acceleration,
                robotState.vitesseGaucheConsigne);
    } else if (robotState.vitesseGaucheCommandeCourante > 0) {
        MOTEUR_GAUCHE_L_PWM_ENABLE = 0; //pilotage de la pin en mode IO
        MOTEUR_GAUCHE_L_IO_OUTPUT = 1; //Mise à 1 de la pin
        MOTEUR_GAUCHE_H_PWM_ENABLE = 1; //Pilotage de la pin en mode PWM
    } else {
        MOTEUR_GAUCHE_H_PWM_ENABLE = 0; //pilotage de la pin en mode IO
        MOTEUR_GAUCHE_H_IO_OUTPUT = 1; //Mise à 1 de la pin
        MOTEUR_GAUCHE_L_PWM_ENABLE = 1; //Pilotage de la pin en mode PWM
    }
    MOTEUR_GAUCHE_DUTY_CYCLE = Abs(robotState.vitesseGaucheCommandeCourante) * PWMPER;
}

void PWMSetSpeedConsigne(float vitesseEnPourcents, char numeroMoteur) {
    if (numeroMoteur == MOTEUR_GAUCHE) {
        robotState.vitesseGaucheCommandeCourante = vitesseEnPourcents;

    } else if (numeroMoteur == MOTEUR_DROIT) {
        robotState.vitesseDroiteCommandeCourante = vitesseEnPourcents;
    }
}