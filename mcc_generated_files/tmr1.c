/**
  TMR1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    tmr1.c

  @Summary
    This is the generated driver implementation file for the TMR1 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This source file provides APIs for TMR1.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.65.2
        Device            :  PIC16F18877
        Driver Version    :  2.01
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.45
        MPLAB 	          :  MPLAB X 4.15
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

/**
  Section: Included Files
*/

#include <xc.h>
#include "tmr1.h"
#include "../config.h"
/**
  Section: Global Variables Definitions
*/
volatile uint16_t timer1ReloadVal;
void (*TMR1_InterruptHandler)(void);

/**
  Section: TMR1 APIs
*/

void TMR1_Initialize(void)
{
    //Set the Timer to the options selected in the GUI

    //T1GE disabled; T1GTM disabled; T1GPOL low; T1GGO done; T1GSPM disabled; 
    T1GCON = 0x00;

    //GSS T1G_pin; 
    T1GATE = 0x00;

    //CS FOSC/4; 
    T1CLK = 0x01;

    //TMR1H 240; 
    TMR1H = 0xF0;

    //TMR1L 96; 
    TMR1L = 0x60;

    // Load the TMR value to reload variable
    timer1ReloadVal=(uint16_t)((TMR1H << 8) | TMR1L);

    // Clearing IF flag before enabling the interrupt.
    PIR4bits.TMR1IF = 0;

    // Enabling TMR1 interrupt.
    PIE4bits.TMR1IE = 1;

    // Set Default Interrupt Handler
    TMR1_SetInterruptHandler(TMR1_DefaultInterruptHandler);

    // CKPS 1:1; nT1SYNC synchronize; TMR1ON enabled; T1RD16 disabled; 
    T1CON = 0x01;
}

void TMR1_StartTimer(void)
{
    // Start the Timer by writing to TMRxON bit
    T1CONbits.TMR1ON = 1;
}

void TMR1_StopTimer(void)
{
    // Stop the Timer by writing to TMRxON bit
    T1CONbits.TMR1ON = 0;
}

uint16_t TMR1_ReadTimer(void)
{
    uint16_t readVal;
    uint8_t readValHigh;
    uint8_t readValLow;
    
    T1CONbits.T1RD16 = 1;
	
    readValLow = TMR1L;
    readValHigh = TMR1H;
    
    readVal = ((uint16_t)readValHigh << 8) | readValLow;

    return readVal;
}

void TMR1_WriteTimer(uint16_t timerVal)
{
    if (T1CONbits.nT1SYNC == 1)
    {
        // Stop the Timer by writing to TMRxON bit
        T1CONbits.TMR1ON = 0;

        // Write to the Timer1 register
        TMR1H = (timerVal >> 8);
        TMR1L = timerVal;

        // Start the Timer after writing to the register
        T1CONbits.TMR1ON =1;
    }
    else
    {
        // Write to the Timer1 register
        TMR1H = (timerVal >> 8);
        TMR1L = timerVal;
    }
}

void TMR1_Reload(void)
{
    TMR1_WriteTimer(timer1ReloadVal);
}

void TMR1_StartSinglePulseAcquisition(void)
{
    T1GCONbits.T1GGO = 1;
}

uint8_t TMR1_CheckGateValueStatus(void)
{
    return (T1GCONbits.T1GVAL);
}

void TMR1_ISR(void)
{

    // Clear the TMR1 interrupt flag
    PIR4bits.TMR1IF = LOW;
    TMR1_WriteTimer(timer1ReloadVal);

    if(TMR1_InterruptHandler)
    {
        TMR1_InterruptHandler();
    }
}


void TMR1_SetInterruptHandler(void (* InterruptHandler)(void)){
    TMR1_InterruptHandler = InterruptHandler;
}

/*
 *                  a_
 *                 f|g|b
 *                 e|_|c
 *                   d 
 */
void dataToDisplay(uint8_t value)
{
    
    //value = 1;
    
    switch(value)
    {
        case 1:
            RD7_DATA_LINE_A  = LOW;
            RD6_DATA_LINE_B  = HIGH;
            RD5_DATA_LINE_C  = HIGH;
            RD4_DATA_LINE_D  = LOW;
            RC0_DATA_LINE_E  = LOW;
            RC1_DATA_LINE_F  = LOW;
            RC2_DATA_LINE_G  = LOW;
            RA6_DATA_LINE_H  = LOW;              
            break;
        case 2:
            RD7_DATA_LINE_A  = HIGH;
            RD6_DATA_LINE_B  = HIGH;
            RD5_DATA_LINE_C  = LOW;
            RD4_DATA_LINE_D  = HIGH;
            RC0_DATA_LINE_E  = HIGH;
            RC1_DATA_LINE_F  = LOW;
            RC2_DATA_LINE_G  = HIGH;
            RA6_DATA_LINE_H  = LOW;             
            break;
        case 3:
            RD7_DATA_LINE_A  = HIGH;
            RD6_DATA_LINE_B  = HIGH;
            RD5_DATA_LINE_C  = HIGH;
            RD4_DATA_LINE_D  = HIGH;
            RC0_DATA_LINE_E  = LOW;
            RC1_DATA_LINE_F  = LOW;
            RC2_DATA_LINE_G  = HIGH;
            RA6_DATA_LINE_H  = LOW;             
            break;
        case 4:
            RD7_DATA_LINE_A  = LOW;
            RD6_DATA_LINE_B  = HIGH;
            RD5_DATA_LINE_C  = HIGH;
            RD4_DATA_LINE_D  = LOW;
            RC0_DATA_LINE_E  = LOW;
            RC1_DATA_LINE_F  = HIGH;
            RC2_DATA_LINE_G  = HIGH;
            RA6_DATA_LINE_H  = LOW;             
            break;
        case 5:
            RD7_DATA_LINE_A  = HIGH;
            RD6_DATA_LINE_B  = LOW;
            RD5_DATA_LINE_C  = HIGH;
            RD4_DATA_LINE_D  = HIGH;
            RC0_DATA_LINE_E  = LOW;
            RC1_DATA_LINE_F  = HIGH;
            RC2_DATA_LINE_G  = HIGH;
            RA6_DATA_LINE_H  = LOW;             
            break;
        case 6:
            RD7_DATA_LINE_A  = HIGH;
            RD6_DATA_LINE_B  = LOW;
            RD5_DATA_LINE_C  = HIGH;
            RD4_DATA_LINE_D  = HIGH;
            RC0_DATA_LINE_E  = HIGH;
            RC1_DATA_LINE_F  = HIGH;
            RC2_DATA_LINE_G  = HIGH;
            RA6_DATA_LINE_H  = LOW;             
            break;
        case 7:
            RD7_DATA_LINE_A  = HIGH;
            RD6_DATA_LINE_B  = HIGH;
            RD5_DATA_LINE_C  = HIGH;
            RD4_DATA_LINE_D  = LOW;
            RC0_DATA_LINE_E  = LOW;
            RC1_DATA_LINE_F  = LOW;
            RC2_DATA_LINE_G  = LOW;
            RA6_DATA_LINE_H  = LOW;             
            break;
        case 8:
            RD7_DATA_LINE_A  = HIGH;
            RD6_DATA_LINE_B  = HIGH;
            RD5_DATA_LINE_C  = HIGH;
            RD4_DATA_LINE_D  = HIGH;
            RC0_DATA_LINE_E  = HIGH;
            RC1_DATA_LINE_F  = HIGH;
            RC2_DATA_LINE_G  = HIGH;
            RA6_DATA_LINE_H  = LOW;            
            break;
        case 9:
            RD7_DATA_LINE_A  = HIGH;
            RD6_DATA_LINE_B  = HIGH;
            RD5_DATA_LINE_C  = HIGH;
            RD4_DATA_LINE_D  = HIGH;
            RC0_DATA_LINE_E  = LOW;
            RC1_DATA_LINE_F  = HIGH;
            RC2_DATA_LINE_G  = HIGH;
            RA6_DATA_LINE_H  = LOW;             
            break;
        case 0:
            RD7_DATA_LINE_A  = HIGH;
            RD6_DATA_LINE_B  = HIGH;
            RD5_DATA_LINE_C  = HIGH;
            RD4_DATA_LINE_D  = HIGH;
            RC0_DATA_LINE_E  = HIGH;
            RC1_DATA_LINE_F  = HIGH;
            RC2_DATA_LINE_G  = LOW;
            RA6_DATA_LINE_H  = LOW;              
            break;
        case 10:
            RD7_DATA_LINE_A  = HIGH;
            RD6_DATA_LINE_B  = LOW;
            RD5_DATA_LINE_C  = LOW;
            RD4_DATA_LINE_D  = LOW;
            RC0_DATA_LINE_E  = LOW;
            RC1_DATA_LINE_F  = LOW;
            RC2_DATA_LINE_G  = LOW;
            RA6_DATA_LINE_H  = LOW;              
            break;            
        case 11:
            RD7_DATA_LINE_A  = LOW;
            RD6_DATA_LINE_B  = HIGH;
            RD5_DATA_LINE_C  = LOW;
            RD4_DATA_LINE_D  = LOW;
            RC0_DATA_LINE_E  = LOW;
            RC1_DATA_LINE_F  = LOW;
            RC2_DATA_LINE_G  = LOW;
            RA6_DATA_LINE_H  = LOW;              
            break;   
        case 12:
            RD7_DATA_LINE_A  = LOW;
            RD6_DATA_LINE_B  = LOW;
            RD5_DATA_LINE_C  = LOW;
            RD4_DATA_LINE_D  = LOW;
            RC0_DATA_LINE_E  = LOW;
            RC1_DATA_LINE_F  = LOW;
            RC2_DATA_LINE_G  = HIGH;
            RA6_DATA_LINE_H  = LOW;              
            break;            
        default:
            break;
            
            
    }
}
void TMR1_DefaultInterruptHandler(void){
    // add your TMR1 interrupt custom code
    // or set custom function using TMR1_SetInterruptHandler()
    ///////////////Time clock functionality ///////////////////////////

    countMs++;
    
    if(countMs > 999)
    {
        countMs = 0;
        countSec++;
        secFlag = 1;
        if(countSec>=59)
        {
            countSec = 0;
            countMin++;
            minFlag = 1;
            if(countMin >= 59)
            {
                countMin = 0;
                countHr++;
                if(countHr >=24)
                {
                    countHr = 0;
                    countDay++;
                }
            }
        }       
    }
    counter++;
    //if(counter >= 2)
    {
       // counter = 0;
            
        //count500uS++;
        //////////// seven segment routine
        switch(counter)
        {
            case SEGMENT1:

                dataToDisplay(seg1Value); 

                RC5_LED_SWITCHING = LOW;  

                RB4_Segment_Control_1 = HIGH;
                RB5_Segment_Control_2 = LOW;
                RB6_Segment_Control_3 = LOW;
                RB7_Segment_Control_4 = LOW;    
                RA5_Segment_Control_5 = LOW;
                RA7_Segment_Control_6 = LOW;    
                RA4_Segment_Control_7 = LOW;
                RA3_Segment_Control_8 = LOW;
                RD3_Segment_Control_9  = LOW;
                RD2_Segment_Control_10 = LOW;
                RD1_Segment_Control_11 = LOW;
                RD0_Segment_Control_12 = LOW;



                break;
            case SEGMENT2:
                dataToDisplay(seg2Value);
                RC5_LED_SWITCHING = LOW;    
                RB4_Segment_Control_1 = LOW;
                RB5_Segment_Control_2 = HIGH;
                RB6_Segment_Control_3 = LOW;
                RB7_Segment_Control_4 = LOW;    
                RA5_Segment_Control_5 = LOW;
                RA7_Segment_Control_6 = LOW;    
                RA4_Segment_Control_7 = LOW;
                RA3_Segment_Control_8 = LOW;
                RD3_Segment_Control_9  = LOW;
                RD2_Segment_Control_10 = LOW;
                RD1_Segment_Control_11 = LOW;
                RD0_Segment_Control_12 = LOW;


                //count500uS = LOW;
                break;

            case SEGMENT3:
                dataToDisplay(seg3Value);
                RC5_LED_SWITCHING = LOW;    
                RB4_Segment_Control_1 = LOW;
                RB5_Segment_Control_2 = LOW;
                RB6_Segment_Control_3 = HIGH;
                RB7_Segment_Control_4 = LOW;    
                RA5_Segment_Control_5 = LOW;
                RA7_Segment_Control_6 = LOW;    
                RA4_Segment_Control_7 = LOW;
                RA3_Segment_Control_8 = LOW;
                RD3_Segment_Control_9  = LOW;
                RD2_Segment_Control_10 = LOW;
                RD1_Segment_Control_11 = LOW;
                RD0_Segment_Control_12 = LOW;



                break;
            case SEGMENT4:
                dataToDisplay(seg4Value);
                RC5_LED_SWITCHING = LOW;    
                RB4_Segment_Control_1 = LOW;
                RB5_Segment_Control_2 = LOW;
                RB6_Segment_Control_3 = LOW;
                RB7_Segment_Control_4 = HIGH;    
                RA5_Segment_Control_5 = LOW;
                RA7_Segment_Control_6 = LOW;    
                RA4_Segment_Control_7 = LOW;
                RA3_Segment_Control_8 = LOW;
                RD3_Segment_Control_9  = LOW;
                RD2_Segment_Control_10 = LOW;
                RD1_Segment_Control_11 = LOW;
                RD0_Segment_Control_12 = LOW;


                break;            
             case SEGMENT5:
                dataToDisplay(seg5Value);  
                RC5_LED_SWITCHING = LOW;    
                RB4_Segment_Control_1 = LOW;
                RB5_Segment_Control_2 = LOW;
                RB6_Segment_Control_3 = LOW;
                RB7_Segment_Control_4 = LOW;    
                RA5_Segment_Control_5 = HIGH;
                RA7_Segment_Control_6 = LOW;    
                RA4_Segment_Control_7 = LOW;
                RA3_Segment_Control_8 = LOW;
                RD3_Segment_Control_9  = LOW;
                RD2_Segment_Control_10 = LOW;
                RD1_Segment_Control_11 = LOW;
                RD0_Segment_Control_12 = LOW;


                break;

            case SEGMENT6:
                dataToDisplay(seg6Value);
                RC5_LED_SWITCHING = LOW;    
                RB4_Segment_Control_1 = LOW;
                RB5_Segment_Control_2 = LOW;
                RB6_Segment_Control_3 = LOW;
                RB7_Segment_Control_4 = LOW;    
                RA5_Segment_Control_5 = LOW;
                RA7_Segment_Control_6 = HIGH;    
                RA4_Segment_Control_7 = LOW;
                RA3_Segment_Control_8 = LOW;
                RD3_Segment_Control_9  = LOW;
                RD2_Segment_Control_10 = LOW;
                RD1_Segment_Control_11 = LOW;
                RD0_Segment_Control_12 = LOW;


                break;

            case SEGMENT7:
                dataToDisplay(seg7Value);
                RC5_LED_SWITCHING = LOW;    
                RB4_Segment_Control_1 = LOW;
                RB5_Segment_Control_2 = LOW;
                RB6_Segment_Control_3 = LOW;
                RB7_Segment_Control_4 = LOW;    
                RA5_Segment_Control_5 = LOW;
                RA7_Segment_Control_6 = LOW;    
                RA4_Segment_Control_7 = HIGH;
                RA3_Segment_Control_8 = LOW;
                RD3_Segment_Control_9  = LOW;
                RD2_Segment_Control_10 = LOW;
                RD1_Segment_Control_11 = LOW;
                RD0_Segment_Control_12 = LOW;


                break;
            case SEGMENT8:
                dataToDisplay(seg8Value);
                RC5_LED_SWITCHING = LOW;    
                RB4_Segment_Control_1 = LOW;
                RB5_Segment_Control_2 = LOW;
                RB6_Segment_Control_3 = LOW;
                RB7_Segment_Control_4 = LOW;    
                RA5_Segment_Control_5 = LOW;
                RA7_Segment_Control_6 = LOW;    
                RA4_Segment_Control_7 = LOW;
                RA3_Segment_Control_8 = HIGH;
                RD3_Segment_Control_9  = LOW;
                RD2_Segment_Control_10 = LOW;
                RD1_Segment_Control_11 = LOW;
                RD0_Segment_Control_12 = LOW;


                break;
            case SEGMENT9:

                dataToDisplay(seg9Value); 
                RC5_LED_SWITCHING = LOW;    
                RB4_Segment_Control_1 = LOW;
                RB5_Segment_Control_2 = LOW;
                RB6_Segment_Control_3 = LOW;
                RB7_Segment_Control_4 = LOW;    
                RA5_Segment_Control_5 = LOW;
                RA7_Segment_Control_6 = LOW;    
                RA4_Segment_Control_7 = LOW;
                RA3_Segment_Control_8 = LOW;
                RD3_Segment_Control_9  = HIGH;
                RD2_Segment_Control_10 = LOW;
                RD1_Segment_Control_11 = LOW;
                RD0_Segment_Control_12 = LOW;


                break;
            case SEGMENT10:

                dataToDisplay(seg10Value);
                RC5_LED_SWITCHING = LOW;    
                RB4_Segment_Control_1 = LOW;
                RB5_Segment_Control_2 = LOW;
                RB6_Segment_Control_3 = LOW;
                RB7_Segment_Control_4 = LOW;    
                RA5_Segment_Control_5 = LOW;
                RA7_Segment_Control_6 = LOW;    
                RA4_Segment_Control_7 = LOW;
                RA3_Segment_Control_8 = LOW;
                RD3_Segment_Control_9  = LOW;
                RD2_Segment_Control_10 = HIGH;
                RD1_Segment_Control_11 = LOW;
                RD0_Segment_Control_12 = LOW;

                break;
            case SEGMENT11:

                dataToDisplay(seg11Value);
                RC5_LED_SWITCHING = LOW;    
                RB4_Segment_Control_1 = LOW;
                RB5_Segment_Control_2 = LOW;
                RB6_Segment_Control_3 = LOW;
                RB7_Segment_Control_4 = LOW;    
                RA5_Segment_Control_5 = LOW;
                RA7_Segment_Control_6 = LOW;    
                RA4_Segment_Control_7 = LOW;
                RA3_Segment_Control_8 = LOW;
                RD3_Segment_Control_9  = LOW;
                RD2_Segment_Control_10 = LOW;
                RD1_Segment_Control_11 = HIGH;
                RD0_Segment_Control_12 = LOW;

                break;
            case SEGMENT12:

                dataToDisplay(seg12Value);
                RC5_LED_SWITCHING = LOW;    
                RB4_Segment_Control_1 = LOW;
                RB5_Segment_Control_2 = LOW;
                RB6_Segment_Control_3 = LOW;
                RB7_Segment_Control_4 = LOW;    
                RA5_Segment_Control_5 = LOW;
                RA7_Segment_Control_6 = LOW;    
                RA4_Segment_Control_7 = LOW;
                RA3_Segment_Control_8 = LOW;
                RD3_Segment_Control_9  = LOW;
                RD2_Segment_Control_10 = LOW;
                RD1_Segment_Control_11 = LOW;
                RD0_Segment_Control_12 = HIGH;

                break;            
             case LED_SEGMENT13:
                dataToDisplay(seg13Value);
                RB4_Segment_Control_1 = LOW;
                RB5_Segment_Control_2 = LOW;
                RB6_Segment_Control_3 = LOW;
                RB7_Segment_Control_4 = LOW;    
                RA5_Segment_Control_5 = LOW;
                RA7_Segment_Control_6 = LOW;    
                RA4_Segment_Control_7 = LOW;
                RA3_Segment_Control_8 = LOW;
                RD3_Segment_Control_9  = LOW;
                RD2_Segment_Control_10 = LOW;
                RD1_Segment_Control_11 = LOW;
                RD0_Segment_Control_12 = LOW;

                RC5_LED_SWITCHING = HIGH;      

                counter = 0;
                break;

            default:
                counter = 0;
        }


    }
}

/**
  End of File
*/
