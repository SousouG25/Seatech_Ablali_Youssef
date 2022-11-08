#include <xc.h>
#include "timer.h"
#include "IO.h"
#include "PWM.h"
#include "ADC.h"
#include "main.h"
unsigned char toggle = 0;

//Initialisation d?un timer 32 bits
void InitTimer23(void) {
T3CONbits.TON = 0; // Stop any 16-bit Timer3 operation
T2CONbits.TON = 0; // Stop any 16/32-bit Timer3 operation
T2CONbits.T32 = 1; // Enable 32-bit Timer mode
T2CONbits.TCS = 0; // Select internal instruction cycle clock
T2CONbits.TCKPS = 0b00; // Select 1:1 Prescaler
TMR3 = 0x00; // Clear 32-bit Timer (msw)
TMR2 = 0x00; // Clear 32-bit Timer (lsw)
PR3 = 0xB400; // Load 32-bit period value (msw)
PR2 = 0x04C4; // Load 32-bit period value (lsw)
IPC2bits.T3IP = 0x01; // Set Timer3 Interrupt Priority Level
IFS0bits.T3IF = 0; // Clear Timer3 Interrupt Flag
IEC0bits.T3IE = 1; // Enable Timer3 interrupt
T2CONbits.TON = 1; // Start 32-bit Timer
/* Example code for Timer3 ISR */
}


///=======================/MODIFIED/////================================//

//Interruption du timer 32 bits sur 2-3
void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void) {

IFS0bits.T3IF = 0; // Clear Timer3 Interrupt Flag
/*
if(toggle == 0){
    PWMSetSpeedConsigne(30, MOTEUR_DROIT);
    PWMSetSpeedConsigne(30, MOTEUR_GAUCHE);
    toggle = 1;
} else {
    PWMSetSpeedConsigne(-30, MOTEUR_DROIT);
    PWMSetSpeedConsigne(-30, MOTEUR_GAUCHE);
    toggle = 0;
}*/
    LED_ORANGE = !LED_ORANGE;
}
/////================================================================////

//Initialisation d?un timer 16 bits
void InitTimer1(float freq)
{
//Timer1 pour horodater les mesures (1ms)
T1CONbits.TON = 0; // Disable Timer
T1CONbits.TCKPS = 0b01; //Prescaler
//11 = 1:256 prescale value
//10 = 1:64 prescale value
//01 = 1:8 prescale value
//00 = 1:1 prescale value
T1CONbits.TCS = 0; //clock source = internal clock
//PR1 = 40000000/64/50;

SetFreqTimer1(freq);
IFS0bits.T1IF = 0; // Clear Timer Interrupt Flag
IEC0bits.T1IE = 1; // Enable Timer interrupt
T1CONbits.TON = 1; // Enable Timer



};


//INIT TIMER 4

void InitTimer4(float freq) {
    //Timer1 pour h o r o d a t e r l e s mesures ( 1ms)
    T4CONbits .TON = 0; // Di s a bl e Timer
    //T1CONbits .TCKPS = 0b01; // P r e s c a l e r                         //////////a été commentée au 3.de la partie 1
    // 11 = 1: 2 5 6 p r e s c a l e v al u e
    // 10 = 1: 6 4 p r e s c a l e v al u e
    // 01 = 1: 8 p r e s c a l e v al u e
    // 00 = 1: 1 p r e s c a l e v al u e


    T4CONbits .TCS = 0; // cl o c k s o u r c e = i n t e r n a l cl o c k
    //PR1 = 40000000/64/50;                                                //////////a été commentée au 3.de la partie 1
    
    SetFreqTimer4(freq);
    IFS1bits.T4IF = 0; // Cle a r Timer I n t e r r u p t Flag
    IEC1bits.T4IE = 1; // Enable Timer i n t e r r u p t
    T4CONbits.TON = 1; // Enable Timer
     
}

// I n t e r r u p t i o n du time r 1

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {
    IFS0bits . T1IF = 0;
    //LED_BLANCHE = !LED_BLANCHE;
    PWMUpdateSpeed();
    //InitADC1();
    //ADC1StartConversionSequence(); 
    //LED_BLEUE=!LED_BLEUE;
}

// SET FREQ TIMER 1

void SetFreqTimer1(float freq) {
    T1CONbits.TCKPS = 0b00; //00 = 1:1 prescaler value
    if (FCY / freq > 65535) {
        T1CONbits.TCKPS = 0b01; //01 = 1:8 prescaler value
        if (FCY / freq / 8 > 65535) {
            T1CONbits.TCKPS = 0b10; //10 = 1:64 prescaler value
            if (FCY / freq / 64 > 65535) {
                T1CONbits.TCKPS = 0b11; //11 = 1:256 prescaler value
                PR1 = (int) (FCY / freq / 256);
            } else
                PR1 = (int) (FCY / freq / 64);
        } else
            PR1 = (int) (FCY / freq / 8);
    } else
        PR1 = (int) (FCY / freq);
}


// I n t e r r u p t i o n du time r 4

void __attribute__((interrupt, no_auto_psv)) _T4Interrupt(void) {
    IFS1bits . T4IF = 0;
    //LED_BLANCHE = !LED_BLANCHE;
    PWMUpdateSpeed();
    //InitADC1();
    //ADC1StartConversionSequence(); 
    LED_BLEUE=!LED_BLEUE;
    timestamp = timestamp + 1; //Increm
    OperatingSystemLoop();
}

void SetFreqTimer4(float freq) {
    T1CONbits.TCKPS = 0b00; //00 = 1:1 prescaler value
    if (FCY / freq > 65535) {
        T4CONbits.TCKPS = 0b01; //01 = 1:8 prescaler value
        if (FCY / freq / 8 > 65535) {
            T4CONbits.TCKPS = 0b10; //10 = 1:64 prescaler value
            if (FCY / freq / 64 > 65535) {
                T4CONbits.TCKPS = 0b11; //11 = 1:256 prescaler value
                PR1 = (int) (FCY / freq / 256);
            } else
                PR4 = (int) (FCY / freq / 64);
        } else
            PR4 = (int) (FCY / freq / 8);
    } else
        PR4 = (int) (FCY / freq);
}

