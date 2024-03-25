#ifndef _MNET_STATE_H_
#define _MNET_STATE_H_
/* mnet state */
typedef enum {
	MNET_READY,
	MNET_CONF,
	MNET_CONN,
	MNET_SERVER,
	SSOCKET_POLLIN,
	MNET_BUSY,
	MNET_ERROR,
} MnetState;

#endif
