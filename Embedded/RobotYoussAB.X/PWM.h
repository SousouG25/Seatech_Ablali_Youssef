
#ifndef PWM_H
#define	PWM_H

#define PI 3.141592654
#define MOTEUR_DROIT 1 
#define MOTEUR_GAUCHE 0

void InitPWM(void);
//void PWMSetSpeed(float vitesseEnPourcents, int numeroMoteur);
void PWMUpdateSpeed(void);
void PWMSetSpeedConsigne(float vitesseEnPourcents, char numeroMoteur);

#endif	/* PWM_H */
