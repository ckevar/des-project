#ifndef _ESP8266_CMD_H_
#define _ESP8266_CMD_H_

#define ESP8_RESTART 			"RST\r\n"
#define ESP8_RESTART_CSIZE 		5				// strlen(ESP8_RESTART)
												//
#define ESP8_RESTORE			"RESTORE\r\n"
#define ESP8_RESTORE_CSIZE 		9

#define ESP8_NOECHO				"0\r\n"			// ATE0\r\n
#define ESP8_NOECHO_CSIZE		3				// strlen(ESP8_NOECHO_CSIZE)

/* Operating Mode: SoftAP, Station or both */
#define ESP8_MODE				"CWMODE_DEF=\0\r\n"
#define ESP8_MODE_MODE			11				// index after '='
#define ESP8_STAONLY			'1'
#define ESP8_APONLY				'2'
#define ESP8_STAAP				'3'
#define ESP8_MODE_CSIZE			14				// strlen(ESP8_MODE)

/* Set AP credentials */
#define ESP8_SetAP				"CWSAP_DEF="
#define	IWCONFIG				"CWJAP_DEF="
#define ESP8_AP_SSID 			10				// index after '='
#define ESP8_AP_SSID_FMT		"%s\r\n"		// "CWxAP_DEF=%s\r\n"
#define IWCONFIG_FMT 			ESP8_AP_SSID_FMT
#define ESP8_xAP_CSIZE			10				// strlen(ES8_xAP)
#define ESP8_SetAP_CSIZE		ESP8_xAP_CSIZE
#define IWCONFIG_CSIZE			ESP8_xAP_CSIZE

/* Amount of connections */
#define SOCKET_COUNT			"CIPMUX=\0\r\n"
#define SOCKET_SINGLE 			'0'
#define SOCKET_MULTIPLE			'1'
#define SOCKET_COUNT_ALLOW		7
#define SOCKET_COUNT_CSIZE 		10

/* ifconfig -a */
#define IFCONFIG_A 				"CIPSTA_CUR?\r\n"
#define IFCONFIG_A_CSIZE 		13				// strlen(IFCONFIG_A)

/* Network status */
#define NETSTAT					"CIPSTATUS\r\n"
#define NETSTAT_CSIZE 			11				// strlen(NETSTAT)

/* Close connections */
#define CLOSE_SOCK_ALL 			"CIPCLOSE\r\n"
#define CLOSE_SOCK_ALL_CSIZE 	10

/* Server Socket Stream */
#define SERVSOCK_STREAM			"CIPSERVER=\0\r\n"
#define SERVSOCK_FD				10
#define SERVSOCK_PORT			(SERVSOCK_FD + 1)
#define SERVSOCK_PORT_FMT		",%d\r\n"
#define SERVSOCK_OPEN			'1'
#define SERVSOCK_CLOSE			'0'
#define SERVSOCK_STREAM_CSIZE	13

/* CIPSTATUS Response: STATUS <stat> */
#define NETSTAT_AP_CONNECTED	'2' // The dev is connected to an AP
#define NETSTAT_SOCKET_CREATED	'3'	// A TCP or UDP socket has being created
#define NETSTAT_SOCKET_DISCONN 	'4'	// A TCP socket has being disconnected
#define NETSTAT_NO_AP 			'5'	// The dev is no connected to an AP

#define DHCPS_RANGE 			"CWDHCP_DEF?\r\n"
#define DHCPS_RANGE_CSIZE 		13

#endif
