#pragma once

#include <ESP8266WiFi.h>

struct Wifi {
	enum Mode {
		NONE,
		ACCESSPOINT,
		CLIENT,
	};
	Mode mode = Mode::NONE;
	void setupAccessPoint(const IPAddress& apIP, const IPAddress& netMsk, const char* ssid);
	void connect(const char* ssid, const char* password);
	bool isConnected();
	bool waitForConnection(int timeout);
	void reconnect();
};
