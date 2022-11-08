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
#include "Robot.h"
#include "main.h"

unsigned int ADCValue0;
unsigned int ADCValue1;
unsigned int ADCValue2;
float vitesseChoisie = 10;



int main (void){
/***************************************************************************************************/
//Initialisation de l?oscillateur
/****************************************************************************************************/
InitOscillator();
InitTimer1(2.5);
InitTimer23();
InitTimer4(1000);
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
    /*if (ADCIsConversionFinished() == 1){
        ADCClearConversionFinishedFlag();
        unsigned int * result = ADCGetResult();
        float volts = ((float) result[0])*3.3 / 4096 * 3.2;
        robotState.distanceTelemetreDroit = 34 / volts - 5;
        volts = ((float) result[1])*3.3 / 4096 * 3.2;
        robotState.distanceTelemetreCentre = 34 / volts - 5;
        volts = ((float) result[2])*3.3 / 4096 * 3.2;
        robotState.distanceTelemetreGauche = 34 / volts - 5;
    }*/
    
    if (robotState.distanceTelemetreDroit < 30){
        LED_ORANGE = 1;
    } else {
        LED_ORANGE = 0;
    }
    if (robotState.distanceTelemetreCentre < 30){
        LED_BLEUE = 1;
    } else {
        LED_BLEUE = 0;
    }
    if (robotState.distanceTelemetreGauche < 30){
        LED_BLANCHE = 1;
    } else {
        LED_BLANCHE = 0;
    }
} // fin main
    
    unsigned char stateRobot;
    
    void OperatingSystemLoop(void) {
    switch (stateRobot) {
        case STATE_ATTENTE:
            
            timestamp = 0;
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;

        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 1000)
                stateRobot = STATE_AVANCE;
            break;

        case STATE_AVANCE:
            PWMSetSpeedConsigne(vitesseChoisie, MOTEUR_DROIT);
            PWMSetSpeedConsigne(vitesseChoisie, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_EN_COURS;
            break;
        case STATE_AVANCE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(vitesseChoisie, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_DROITE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(vitesseChoisie, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            PWMSetSpeedConsigne(vitesseChoisie/2, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-vitesseChoisie/2, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_DROITE:
            PWMSetSpeedConsigne(-vitesseChoisie/2, MOTEUR_DROIT);
            PWMSetSpeedConsigne(vitesseChoisie/2, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        default:
            stateRobot = STATE_ATTENTE;
            break;
    }
    }
    
    
    unsigned char nextStateRobot = 0;

    void SetNextRobotStateInAutomaticMode() {
    
    unsigned char positionObstacle = PAS_D_OBSTACLE;
    
    //Détermination de la position des obstacles en fonction des télémètres
    if ((robotState.distanceTelemetreDroit < 30 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche > 30)            ||     (robotState.distanceTelemetreExtremiteDroite < 30 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche > 30))  //Obstacle à droite ou a l'extrmite droite
        positionObstacle = OBSTACLE_A_DROITE;
    else if ((robotState.distanceTelemetreDroit > 30 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche < 30)            ||     (robotState.distanceTelemetreDroit > 30 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreExtremiteGauche < 30)) //Obstacle à gauche ou al'extremite gauche)
        
        positionObstacle = OBSTACLE_A_GAUCHE;
    else if (robotState.distanceTelemetreCentre < 20) //Obstacle en face
        positionObstacle = OBSTACLE_EN_FACE;
    else if (robotState.distanceTelemetreDroit > 30 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche > 30) //pas d?obstacle
        positionObstacle = PAS_D_OBSTACLE;

    //Détermination de l?état à venir du robot
    if (positionObstacle == PAS_D_OBSTACLE)
        nextStateRobot = STATE_AVANCE;
    else if (positionObstacle == OBSTACLE_A_DROITE)
        nextStateRobot = STATE_TOURNE_GAUCHE;
    else if (positionObstacle == OBSTACLE_A_GAUCHE)
        nextStateRobot = STATE_TOURNE_DROITE;
    else if (positionObstacle == OBSTACLE_EN_FACE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;

    //Si l?on n?est pas dans la transition de l?étape en cours
    if (nextStateRobot != stateRobot - 1){
        stateRobot = nextStateRobot;
    }
    
}
}
