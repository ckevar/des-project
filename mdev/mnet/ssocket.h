#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include <stdint.h>

#define listen_socks() 				ioctrl_mnet(LISTEN, NULL)
#define read_socks(li, buf, size)	read_mnet(li, buf, size)

uint8_t bind_socks(uint16_t port);

uint8_t close_socks(uint8_t link);

#endif
