/* 
 * File:   config.h
 * Author: abc
 *
 * Created on July 4, 2018, 11:00 AM
 */

#ifndef CONFIG_H
#define	CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

#define   HIGH            1
#define   LOW             0
#define   TRUE              1
#define   FALSE             0
    
#define ESSID_NAME "relief"
#define ESSID_PASS "password"    
    
#define RC5_LED_SWITCHING     RC5
//#define RD0_SEG_SWITCHING     RD0
//#define RD1_SEG_SWITCHING     RD1
//#define RD2_SEG_SWITCHING     RD2
//#define RD3_SEG_SWITCHING     RD3
    
#define RB4_Segment_Control_1 RB4   
#define RB5_Segment_Control_2 RB5
#define RB6_Segment_Control_3 RB6
#define RB7_Segment_Control_4 RB7
#define RA5_Segment_Control_5 RA5
#define RA7_Segment_Control_6 RA7    
#define RA4_Segment_Control_7 RA4
#define RA3_Segment_Control_8 RA3
#define RD3_Segment_Control_9   RD3
#define RD2_Segment_Control_10  RD2
#define RD1_Segment_Control_11  RD1
#define RD0_Segment_Control_12  RD0

#define RD7_DATA_LINE_A         RD7
#define RD6_DATA_LINE_B         RD6
#define RD5_DATA_LINE_C         RD5
#define RD4_DATA_LINE_D         RD4    
#define RC0_DATA_LINE_E         RC0
#define RC1_DATA_LINE_F         RC1
#define RC2_DATA_LINE_G         RC2 
#define RA6_DATA_LINE_H         RA6         

#define BUZZER                  RE2
#define RELAY                   RE1

#define KEY1                    RB0
#define KEY2                    RB1
#define KEY3                    RB2
#define KEY4                    RB3
    
#define LED_D1                  RD7
#define LED_D2                  RD6
#define LED_D3                  RC2
    
typedef enum SEGMENT_CONTROL {
    SEGMENT1 = 1,
    SEGMENT2,            
    SEGMENT3,            
    SEGMENT4,            
    SEGMENT5,            
    SEGMENT6,            
    SEGMENT7,            
    SEGMENT8,            
    SEGMENT9,            
    SEGMENT10,            
    SEGMENT11,            
    SEGMENT12,
    LED_SEGMENT13            
                
};    

    
/***********************************/
#define TIMER_OFFSET            2
#define TIMER_1MS               2000
#define TIMER_1S                120000 

////////////////////////////////////////////

bit minFlag;
bit secFlag;

/////////////////////////////////////////////
extern uint16_t   counter;    
extern uint16_t   count500uS;
extern uint16_t   countMs;
extern uint8_t   countSec;
extern uint8_t   countMin;
extern uint8_t   countHr;
extern uint16_t   countDay;


extern uint8_t     seg1Value;
extern uint8_t     seg2Value;
extern uint8_t     seg3Value;
extern uint8_t     seg4Value;
extern uint8_t     seg5Value;
extern uint8_t     seg6Value;
extern uint8_t     seg7Value;
extern uint8_t     seg8Value;
extern uint8_t     seg9Value;
extern uint8_t     seg10Value;
extern uint8_t     seg11Value;
extern uint8_t     seg12Value;
extern uint8_t     seg13Value; // 3 LED indicator 
#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_H */

