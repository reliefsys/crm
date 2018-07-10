/**
  @Generated PIC10 / PIC12 / PIC16 / PIC18 MCUs Source File

  @Company:
    Microchip Technology Inc.

  @File Name:
    mcc.c

  @Summary:
    This is the mcc.c file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.65.2
        Device            :  PIC16F18877
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.45 or later
        MPLAB             :  MPLAB X 4.15
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
#include "mcc_generated_files/mcc.h"        
#include "config.h"
#include "esp8266.h"
#include "user.h"        
/////////////////////////////////////////////////////////////////////////////        
        
        
        
        
        
        
        
        
/////////////////////////////////////////////////////////////////////////////
uint16_t   counter;    
uint16_t   count500uS;
uint16_t   countMs;
uint8_t   countSec;
uint8_t   countMin;
uint8_t   countHr;
uint16_t   countDay;


uint8_t     seg1Value;
uint8_t     seg2Value;
uint8_t     seg3Value;
uint8_t     seg4Value;
uint8_t     seg5Value;
uint8_t     seg6Value;
uint8_t     seg7Value;
uint8_t     seg8Value;
uint8_t     seg9Value;
uint8_t     seg10Value;
uint8_t     seg11Value;
uint8_t     seg12Value;
uint8_t     seg13Value; // 3 LED indicator 
unsigned short humidity;
short temperature;
unsigned char _buff[4];
/********************************************************************/
//********************************************************************************************************//
void InitializeI2C (void)
{


//    SSP1STAT = 0x80;
//    // SSPEN enabled; CKP Idle:Low, Active:High; SSPM FOSC/4_SSPxADD_I2C; 
//    SSP1CON1 = 0x28;
//    SSP1CON2 = 0x00;
//    // SBCDE disabled; BOEN disabled; SCIE disabled; PCIE disabled; DHEN disabled; SDAHT 300ns; AHEN disabled; 
//    SSP1CON3 = 0x08;
//
//    // Baud Rate Generator Value: SSPADD 9;   
//    SSP1ADD = 0x4F;
//	//Clear master Synchronous Serial Port Interrupt Flag bit
//	SSP1IF = 0;
    
    // SMP High Speed; CKE disabled; 
    SSP1STAT = 0x00;
    // SSPEN enabled; CKP Idle:Low, Active:High; SSPM FOSC/4_SSPxADD_I2C; 
    SSP1CON1 = 0x28;
    // SBCDE disabled; BOEN disabled; SCIE disabled; PCIE disabled; DHEN disabled; SDAHT 300ns; AHEN disabled; 
    SSP1CON3 = 0x08;
    // SSPADD 3; 
    SSP1ADD = 0x03;
    
    // clear the interrupt flags
    PIR3bits.SSP1IF = 0;
    PIR3bits.BCL1IF = 0;
	
    // enable the interrupts
    PIE3bits.SSP1IE = 0;
    PIE3bits.BCL1IE = 0;    
}
//.................................................................................................//
void WaitSSP (void)
{
	while(!(SSP1IF));
//    __nop();
//    __nop();
//    __delay_us(10);
	SSP1IF = 0;
}
//**************************************************************************************************//
void SendStart (void)
{
    SSP1CON2bits.SEN=1;
    WaitSSP();
}
 //*****************************************************************************//
void SendRestart(void)
{
    SSP1CON2bits.RSEN=1;
    WaitSSP();
}
//*************************************************************************************************//
void SendStop(void)
{
    SSP1CON2bits.PEN=1;
    WaitSSP();
}
//*****************************************************************************************************//
void SendByte(char Byte)
{
    SSP1BUF=Byte;
    WaitSSP();
	while (SSP1CON2bits.ACKSTAT);
}

//*****************************************************************************************************//
unsigned char ReceiveByte(void)
{
    SSP1CON2bits.RCEN=1;
    WaitSSP();
    return(SSP1BUF);
}
//*****************************************************************************************************//
void SendNack(void)
{
    SSP1CON2bits.ACKDT=1;
    SSP1CON2bits.ACKEN=1;
    WaitSSP();
}

//*****************************************************************************************************//
void SendAck(void)
{
    SSP1CON2bits.ACKDT=0;
    SSP1CON2bits.ACKEN=1;
    WaitSSP();
}
//*******************************************************************************************************//
void I2CGetBytes (unsigned char address, unsigned char bytes, unsigned char *buff)
{
	char i = bytes;
	SendStart();	
	SendByte (address);
	for (i = 0; i < bytes - 1; i++)
	{
		buff[i] = ReceiveByte();
		SendAck();
	}
	buff[bytes - 1] = ReceiveByte();
	SendNack();
	SendStop();
}
//*****************************************************************************************************//
char GetHihReading (unsigned short *pHumd, short *pTemp)
{
	char attempts = 0;
	unsigned long temp, temperature, humidity;
	unsigned char status = 0;
	//Delay to ensure sensor is ready
	__delay_ms(100);	

	SendStart();
	//Send write command
	SendByte (0x27<< 1);
	SendStop();
	do
	{
        __delay_ms(50);
		//Get data with read command
		I2CGetBytes (((0x27<< 1) | 0x01), 4, &_buff);
		++attempts;
	}
	while ((_buff[0] & 0xC0) && (attempts < 5));
	status = (_buff[0] >> 6) & 0x03;
	temp = 0;
	temp = _buff [0] & 0x3F;
	
	//Conversations step by step
	humidity = temp << 8;
	humidity = humidity | _buff [1];
	humidity = humidity*100;
	humidity = humidity / 16383;
	temp = 0;
	temp = _buff[2];
	
	temperature = temp << 8;
	temperature = (temperature | _buff[3]);
	temperature = temperature >> 2;
	temperature = temperature * 165;
	temperature = temperature / 16383;
     //LED_high =~LED_high;
	*pTemp = (short)temperature;
	*pTemp -= 40;
	*pHumd = (unsigned short)humidity;
	return status;
}
//*********************************************************************************************************//

/********************************************************************/
void main(void)
{

    SYSTEM_Initialize();
    
    /**
    TRISx registers
    */
    TRISE = 0x00;
    TRISA = 0x07;
    TRISB = 0x0F;
    TRISC = 0x98;
    TRISD = 0x00;

    /**
    ANSELx registers
    */
    ANSELD = 0x00;
    ANSELC = 0x00; //67 -> 0110 1110
    ANSELB = 0x00;
    ANSELE = 0x00;
    ANSELA = 0x07;    
    
    
    seg1Value = 0;
    seg2Value = 0;
    seg3Value = 0;
    seg4Value = 0;
    seg5Value = 0;
    seg6Value = 0;
    seg7Value = 0;
    seg8Value = 0;
    seg9Value = 0;
	seg10Value = 0;
	seg11Value = 0;
    seg12Value = 0;
    seg13Value = 0;
    
    InitializeI2C ();  
    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
   INTERRUPT_PeripheralInterruptEnable();
    
   TMR1_StartTimer();
   
    while(1)
    {
        if(secFlag)
        {
            secFlag = 0;
            
            seg9Value = countSec % 10;
            seg10Value = countSec / 10;
            seg11Value = countMin % 10;
            seg12Value = countMin / 10;
            
            //GetHihReading (&humidity, &temperature);
            
            seg1Value = 0;
            seg2Value= 0;
            seg3Value=temperature%10;
            seg4Value=temperature/10;

            seg7Value = 0;
            seg8Value= 0;            
            seg6Value=humidity%10;
            seg5Value=humidity/10;            
            //seg13Value = countMin / 10;            
        }
    
    }
}