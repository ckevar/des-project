#include <stdio.h>
#include "esp8266/buffer.h"

void esp8_buf_purge(uint16_t count) {
	while(count) {
		esp8_buf_purge_unit();
		count--;
	}
}
