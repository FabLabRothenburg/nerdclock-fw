#include <Arduino.h>

#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>

#include <algorithm>

#include "color.h"
#include "config.h"
#include "time.h"
#include "display.h"
#include "wordclock.h"
#include "http.h"
#include "wifi.h"

int frame = 0;
Color dots_matrix[11][10];
Color dots_words[11][10];
Display display;

void genDotsMatrix() {
	// shift rows down
	for(int y = 9; y > 0; --y) {
		for(int x = 0; x < 11; ++x) {
			dots_matrix[x][y] = dots_matrix[x][y-1];
		}
	}
	// gen new top row
	for(int x = 0; x < 11; ++x) {
		const auto& ov = dots_matrix[x][1];
		auto& nv = dots_matrix[x][0];
		bool chance = random(0, 4) == 0;
		if(ov.l() < 0.1) {
			nv = Color::fromHSL(0.33, 1, chance ? 1 : 0);
		}
		else {
			nv = Color::fromHSL(0.33, 1, ov.l() / 1.5);
		}
	}
}

void setWord(WORD iw, Color color) {
	auto& word = words[iw];
	for(int i = 0; i < word.length; ++i) {
		dots_words[word.x + i][word.y] = color;
	}
}

void genDotsWords(time_t t) {
	memset(dots_words, 0, sizeof(dots_words));
	int h = hour(t) % 12;
	int m = minute(t);
	int minuteCluster = m / 5;


	setWord(WORD::ES, Color::fromRGB(255, 0, 0));
	setWord(WORD::IST, Color::fromRGB(255, 0, 0));

	switch (minuteCluster) {
		case 0:
			setWord(WORD::UHR, Color::fromRGB(255, 0, 0));
			if(h == 1) {
				setWord(WORD::EIN, Color::fromRGB(255, 0, 0));
				return;
			}
			break;
		case 1:
			setWord(WORD::FUENF, Color::fromRGB(255, 0, 0));
			setWord(WORD::NACH, Color::fromRGB(255, 0, 0));
			break;
		case 2:
			setWord(WORD::ZEHN, Color::fromRGB(255, 0, 0));
			setWord(WORD::NACH, Color::fromRGB(255, 0, 0));
			break;
		case 3:
			setWord(WORD::VIERTEL, Color::fromRGB(255, 0, 0));
			break;
		case 4:
			setWord(WORD::ZWANZIG, Color::fromRGB(255, 0, 0));
			setWord(WORD::NACH, Color::fromRGB(255, 0, 0));
			break;
		case 5:
			setWord(WORD::FUENF, Color::fromRGB(255, 0, 0));
			setWord(WORD::VOR, Color::fromRGB(255, 0, 0));
			setWord(WORD::HALB, Color::fromRGB(255, 0, 0));
			break;
		case 6:
			setWord(WORD::HALB, Color::fromRGB(255, 0, 0));
			break;
		case 7:
			setWord(WORD::FUENF, Color::fromRGB(255, 0, 0));
			setWord(WORD::NACH, Color::fromRGB(255, 0, 0));
			setWord(WORD::HALB, Color::fromRGB(255, 0, 0));
			break;
		case 8:
			setWord(WORD::ZWANZIG, Color::fromRGB(255, 0, 0));
			setWord(WORD::VOR, Color::fromRGB(255, 0, 0));
			break;
		case 9:
			setWord(WORD::DREIVIERTEL, Color::fromRGB(255, 0, 0));
			break;
		case 10:
			setWord(WORD::ZEHN, Color::fromRGB(255, 0, 0));
			setWord(WORD::VOR, Color::fromRGB(255, 0, 0));
			break;
		case 11:
			setWord(WORD::FUENF, Color::fromRGB(255, 0, 0));
			setWord(WORD::VOR, Color::fromRGB(255, 0, 0));
			break;
	}

	if(minuteCluster == 3 || minuteCluster >= 5)
		h++;

	switch(h % 12) {
		case 0: setWord(WORD::ZERO, Color::fromRGB(255, 0, 0)); break;
		case 1: setWord(WORD::ONE, Color::fromRGB(255, 0, 0)); break;
		case 2: setWord(WORD::TWO, Color::fromRGB(255, 0, 0)); break;
		case 3: setWord(WORD::THREE, Color::fromRGB(255, 0, 0)); break;
		case 4: setWord(WORD::FOUR, Color::fromRGB(255, 0, 0)); break;
		case 5: setWord(WORD::FIVE, Color::fromRGB(255, 0, 0)); break;
		case 6: setWord(WORD::SIX, Color::fromRGB(255, 0, 0)); break;
		case 7: setWord(WORD::SEVEN, Color::fromRGB(255, 0, 0)); break;
		case 8: setWord(WORD::EIGHT, Color::fromRGB(255, 0, 0)); break;
		case 9: setWord(WORD::NINE, Color::fromRGB(255, 0, 0)); break;
		case 10: setWord(WORD::TEN, Color::fromRGB(255, 0, 0)); break;
		case 11: setWord(WORD::ELEVEN, Color::fromRGB(255, 0, 0)); break;
	}
}

static Config config;
Wifi wifi;
HttpServer httpServer(config);
bool _wifiInitialized = false;

void setup() {
	sprintf(config.wifi.hostname, "nerdclock-%06x", ESP.getChipId());
	Serial.begin(115200);
	display.init();
	wifi.connect("_________", "________");
	ArduinoOTA.setHostname(config.wifi.hostname);
	ArduinoOTA.begin();
	MDNS.addService("http", "tcp", 80);
	httpServer.init();
}

void loop() {
	// If we are not connected after some time start Access Point instead
	if(_wifiInitialized == false && millis() > (10 * 1000)) {
		wifi.setupAccessPoint(IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0), config.wifi.hostname);
		_wifiInitialized = true;
	}
	else if(_wifiInitialized == false && wifi.isConnected()) {
		syncTime();
		_wifiInitialized = true;
	}

	if(!wifi.isConnected()) {
		// wifi.reconnect();
	}

	MDNS.update();
	ArduinoOTA.handle();
	httpServer.update();

	auto curMode =
		config.display.modeCustomEnabled ? config.display.modeCustom
		: !wifi.isConnected() ? config.display.modeConnecting
		: timeStatus() == timeNotSet ? config.display.modeSyncingTime
		: config.display.modeSynced;

	switch(curMode) {
		case Mode::Init:
			display.clear();
			display.setPixel(frame % display.numPixels(), Color::fromRGB(255, 0, 0));
			display.commit();
			delay(100);
			break;
		case Mode::Init2:
			setWord((WORD)(frame % WORD::_COUNT), Color::fromRGB(255, 0, 0));
			display.clear();
			std::fill( &dots_words[0][0], &dots_words[0][0] + 10*11, Color());
			display.render(dots_words);
			display.commit();
			delay(500);
			break;
		case Mode::Matrix:
			genDotsMatrix();
			display.clear();
			display.render(dots_matrix);
			display.commit();
			delay(100);
			break;
		case Mode::Time:{
			auto t = getLocalTime();
			genDotsWords(t);
			display.clear();
			display.render(dots_words);
			display.commit();
			delay(100);
			break;
		}
		case Mode::TimeMatrix: {
			genDotsMatrix();
			auto t = getLocalTime();
			genDotsWords(t);
			Color dots_merged[11][10];
			memcpy(dots_merged, dots_matrix, sizeof(dots_matrix));

			for(int y = 0; y < 10; ++y) {
				for(int x = 0; x < 11; ++x) {
					auto& px = dots_merged[x][y];
					if(dots_words[x][y].l() > 0) {
						px.h(0.0);
					}
				}
			}

			display.clear();
			display.render(dots_merged);
			display.commit();
			delay(100);
			break;
		}
	}
	++frame;
}
