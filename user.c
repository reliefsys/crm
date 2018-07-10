
/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>    /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#endif
#include "config.h"

#include "user.h"



void initESP(void) {
    //debug_line("Restarting ESP...");
    esp8266_restart();
    esp8266_echoCmds(true);
    esp8266_mode(ESP8266_STATION);
    //debug_line("Connecting to AP...");
    unsigned char connect = esp8266_connect((unsigned char *) ESSID_NAME, (unsigned char *) ESSID_PASS);
    if (connect == ESP8266_FAIL) {
      //  debug_line("Failed to connect");
            seg1Value = 3;
            seg2Value = 3;
            seg3Value = 3;
            seg4Value = 3;
            seg13Value = 3;
    } else {
      //  debug_line("Getting IP...");
        unsigned char ip[4];
        esp8266_ip(ip);
        unsigned char ip_string[15];
        sprintf(ip_string, "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
            seg1Value = ip[0];
            seg2Value = ip[1];
            seg3Value = ip[2];
            seg4Value = ip[3];
       // debug_line(ip_string);
    }
}

