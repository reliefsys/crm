/**
  EUSART Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    eusart.c

  @Summary
    This is the generated driver implementation file for the EUSART driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This source file provides APIs for EUSART.
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
/**
  Section: Included Files
*/
#include "eusart.h"

/**
  Section: Macro Declarations
*/
#define EUSART_TX_BUFFER_SIZE 8
#define EUSART_RX_BUFFER_SIZE 1

/**
  Section: Global Variables
*/

volatile uint8_t eusartTxHead = 0;
volatile uint8_t eusartTxTail = 0;
volatile uint8_t eusartTxBuffer[EUSART_TX_BUFFER_SIZE];
volatile uint8_t eusartTxBufferRemaining;

volatile uint8_t eusartRxHead = 0;
volatile uint8_t eusartRxTail = 0;
volatile uint8_t eusartRxBuffer[EUSART_RX_BUFFER_SIZE];
volatile uint8_t eusartRxCount;

/**
  Section: EUSART APIs
*/

void EUSART_Initialize(void)
{
    // disable interrupts before changing states
    PIE3bits.RCIE = 0;
    PIE3bits.TXIE = 0;

    // Set the EUSART module to the options selected in the user interface.

    // ABDOVF no_overflow; SCKP Non-Inverted; BRG16 16bit_generator; WUE disabled; ABDEN disabled; 
    BAUD1CON = 0x08;

    // SPEN enabled; RX9 8-bit; CREN disabled; ADDEN disabled; SREN disabled; 
    RC1STA = 0x80;

    // TX9 8-bit; TX9D 0; SENDB sync_break_complete; TXEN enabled; SYNC asynchronous; BRGH hi_speed; CSRC slave; 
    TX1STA = 0x24;

    // Baud Rate = 9600; SP1BRGL 207; 
    SP1BRGL = 0xCF;

    // Baud Rate = 9600; SP1BRGH 0; 
    SP1BRGH = 0x00;


    // initializing the driver state
    eusartTxHead = 0;
    eusartTxTail = 0;
    eusartTxBufferRemaining = sizeof(eusartTxBuffer);

    eusartRxHead = 0;
    eusartRxTail = 0;
    eusartRxCount = 0;

    // enable receive interrupt
    PIE3bits.RCIE = 1;
}

uint8_t EUSART_Read(void)
{
    uint8_t readValue  = 0;
    RC1STAbits.CREN = 1;	
    
    while(0 == eusartRxCount)
    {
    }

    readValue = eusartRxBuffer[eusartRxTail++];
    if(sizeof(eusartRxBuffer) <= eusartRxTail)
    {
        eusartRxTail = 0;
    }
    PIE3bits.RCIE = 0;
    eusartRxCount--;
    PIE3bits.RCIE = 1;

    return readValue;
}

void EUSART_Write(uint8_t txData)
{
    while(0 == eusartTxBufferRemaining)
    {
    }

    if(0 == PIE3bits.TXIE)
    {
        TX1REG = txData;
    }
    else
    {
        PIE3bits.TXIE = 0;
        eusartTxBuffer[eusartTxHead++] = txData;
        if(sizeof(eusartTxBuffer) <= eusartTxHead)
        {
            eusartTxHead = 0;
        }
        eusartTxBufferRemaining--;
    }
    PIE3bits.TXIE = 1;
}

char getch(void)
{
    uint8_t retVal = 0;
    
    if(eusartRxCount > 0)
    {
        retVal = eusartRxBuffer[eusartRxHead];
        eusartRxBuffer[eusartRxHead] = 0x00;
        eusartRxCount = 0; // only reading one byte at a time
    }            
    return retVal;
    //return EUSART_Read();
}

void putch(char txData)
{
    TX1REG = txData;
    //EUSART_Write(txData);
}

void EUSART_Transmit_ISR(void)
{
    
#if 0

    // add your EUSART interrupt custom code
    if(sizeof(eusartTxBuffer) > eusartTxBufferRemaining)
    {
        TX1REG = eusartTxBuffer[eusartTxTail++];
        if(sizeof(eusartTxBuffer) <= eusartTxTail)
        {
            eusartTxTail = 0;
        }
        eusartTxBufferRemaining++;
    }
    else
    {
        PIE3bits.TXIE = 0;
    }
    
#endif
    PIE3bits.TXIE = 0;
}

void EUSART_Receive_ISR(void)
{
#if 0
    if(1 == RC1STAbits.OERR)
    {
        // EUSART error - restart
        RC1STAbits.CREN = 0;
        RC1STAbits.CREN = 1;
    }

    // buffer overruns are ignored
    eusartRxBuffer[eusartRxHead++] = RC1REG;
    if(sizeof(eusartRxBuffer) <= eusartRxHead)
    {
        eusartRxHead = 0;
    }
    eusartRxCount++;
#endif 

   PIE3bits.RCIE = 0;
   //eusartRxCount = RC1REG;
   
   eusartRxBuffer[eusartRxHead++] = RC1REG;
   
   eusartRxCount++;
   
   if(eusartRxHead >= EUSART_RX_BUFFER_SIZE)
        eusartRxHead = 0;
   
   PIR3bits.RCIF = 0;
   PIE3bits.RCIE = 1;
}
/**
  End of File
*/
