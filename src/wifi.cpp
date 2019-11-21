#include "wifi.h"


void Wifi::setupAccessPoint(const IPAddress& apIP, const IPAddress& netMsk, const char* ssid) {
	WiFi.softAPConfig(apIP, apIP, netMsk);
	WiFi.softAP(ssid);
	mode = Mode::ACCESSPOINT;
}

void Wifi::connect(const char* ssid, const char* password) {
	if(ssid && strlen(ssid) > 0) {
		WiFi.mode(WIFI_STA);
		WiFi.begin(ssid, (password && strlen(password) > 0) ? password : nullptr);
		mode = Mode::CLIENT;
	}
}

bool Wifi::isConnected() {
	return WiFi.status() == WL_CONNECTED;
}

bool Wifi::waitForConnection(int timeout) {
	int retry_count = 0;
	while (!isConnected() && retry_count < timeout*2) {
		delay(500);
		++retry_count;
	}
	return isConnected();
}

void Wifi::reconnect() {
	WiFi.reconnect();
}
