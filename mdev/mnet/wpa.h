#ifndef WPA_H
#define WPA_H

#define WPA_WPA 		12
#define WPA_AUTH		((WPA_WPA + 2) * 2 + 1)
#define WPA_AUTH_AP		(WPA_AUTH + 4)

#define mk_network(ssid, pwd) \
	const char network[WPA_AUTH] = #ssid","#pwd

#define mk_ap_network(ssid, pwd, channel) \
	const char ap_network[WPA_AUTH_AP] = "\""#ssid"\",\""#pwd"\","#channel

#endif
