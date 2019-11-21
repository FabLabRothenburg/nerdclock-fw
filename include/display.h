#pragma once

#include <Adafruit_NeoPixel.h>

#include "color.h"

struct Display {
	static const int WIDTH = 11;
	static const int HEIGHT = 10;
	static const int PIN = 5;
	Adafruit_NeoPixel strip = { WIDTH * HEIGHT + 4, PIN, NEO_GRB + NEO_KHZ800 };

	void init() {
		strip.begin();
		strip.show();
		clear();
	}

	void clear() {
		strip.clear();
	}

	void setPixel(int x, int y, Color color) {
		if ((y % 2) == 1)
			x = WIDTH - 1 - x;
		strip.setPixelColor(y * WIDTH + x, color.r(), color.g(), color.b());
	}

	void setPixel(int o, Color color) {
		strip.setPixelColor(o, color.r(), color.g(), color.b());
	}

	void commit() {
		strip.show();
	}

	int numPixels() {
		return strip.numPixels();
	}

	void render(Color dots[11][10]) {
		for(int y = 0; y < 10; ++y) {
			for(int x = 0; x < 11; ++x) {
				this->setPixel(x, y, dots[x][y]);
			}
		}
	}
};
