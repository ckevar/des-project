#ifndef ESP8266_BUFFER_H
#define ESP8266_BUFFER_H 

#define UTX_CMD_BUF_SIZE 			64
#define URX_BUF_SIZE 				512

struct Buffer8b {
	uint8_t 		data[URX_BUF_SIZE];
	uint16_t 		i;
	uint16_t 		len;
};

struct mesp8266Buf {
	struct Buffer8b  rx;
	char			 c_buf[UTX_CMD_BUF_SIZE];
};


#define AT_							3
#define esp8_cmdset(at_cmd)			(AT_ + at_cmd##_CSIZE); \
									memcpy(espb.c_buf + AT_, at_cmd, at_cmd##_CSIZE)

#define esp8_charargset(offset,arg)	espb.c_buf[AT_ + offset] = arg

#define esp8_strargset(offset, cmd_size, fmt, arg) \
		snprintf(espb.c_buf + AT_ + offset, UTX_CMD_BUF_SIZE - cmd_size - AT_, fmt, arg);

#define esp8_buf_purge_unit() 		espb.rx.data[espb.rx.i] = 0; \
									espb.rx.i = (espb.rx.i + 1) % URX_BUF_SIZE

void esp8_buf_purge(uint16_t count);

extern struct mesp8266Buf espb;


#endif
