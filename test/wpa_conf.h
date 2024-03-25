#ifndef WPA_SUPPLICANT_CONF_H
#define WPA_SUPPLICANT_CONF_H

#include "mnet/wpa.h"

mk_network("<SSID_TO_JOIN>", "<PASSWORD>");

// This is fully supported yet
mk_ap_network("<SSID_TO_CREATE>", "<PASSWORD>" , "<CHANNEL>");

#endif
