#ifndef ESP8266_STATUS_H
#define ESP8266_STATUS_H

#include "mnet/mnet_state.h"

/* Response hash */
/* BoIx stands for x-th Bit of Interest
 */
#define ESP8_RESP_HASH(BoI0, BoI2) 	(BoI0 ^ (BoI2 << 1))
/* Result of typical/used response */
#define ESP8_UNKNOWN 				0
#define ESP8_WRAP					42	// Wrap command: >
#define ESP8_OK						85 	// 
#define ESP8_IPD 					139 // 
#define ESP8_ready					176 // 
#define ESP8_CIPSTA 				185 // 
#define ESP8_STATUS 				209 // 
#define ESP8_FAIL 					212 //  
#define ESP8_CLOSED 				221 // 
#define ESP8_ERROR 					225	// 
#define ESP8_GOT_IP 				239 // 

MnetState esp8266_status(MnetState onSuccess[static 1]);

#endif
