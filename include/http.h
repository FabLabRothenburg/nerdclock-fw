#pragma once

#include "config.h"
#include <ESP8266WebServer.h>

struct HttpServer {
private:
	ESP8266WebServer server;
	Config& config;
public:
	HttpServer(Config& config);
	void init();
	void update();
};
