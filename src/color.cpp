#include "color.h"

#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <tuple>

namespace {
	std::tuple<float, float, float> rgb2hsl(unsigned char _r, unsigned char _g, unsigned char _b) {
		float r = float(_r) / 255.0;
		float g = float(_g) / 255.0;
		float b = float(_b) / 255.0;
		auto max = std::max(r, std::max(g, b));
		auto min = std::min(r, std::max(g, b));
		float h = 0;
		float s = 0;
		float l = (max + min) / 2;
		if(max != min) {
			auto d = max - min;
			s = l > 0.5 ? d / (2.0 - max - min) : d / (max + min);
			if(max == r) { h = (g - b) / d + (g < b ? 6.0 : 0); }
			else if(max == g) { h = (b - r) / d + 2.0; }
			else if(max == b) { h = (r - g) / d + 4.0; }
			h /= 6.0;
		}
		return std::make_tuple(h, s, l);
	}

	float hue2rgb(float p, float q, float t) {
		if(t < 0.0) t += 1.0;
		if(t > 1.0) t -= 1.0;
		if(t < 1.0/6.0) return p + (q - p) * 6.0 * t;
		if(t < 1.0/2.0) return q;
		if(t < 2.0/3.0) return p + (q - p) * (2.0/3.0 - t) * 6.0;
		return p;
	}

	std::tuple<int, int, int> hsl2rgb(float h, float s, float l) {
		float r, g, b;
		if(s == 0.0) {
			r = g = b = l;
		}
		else {
			float q = l < 0.5 ? l * (1.0 + s) : l + s - l * s;
			float p = 2.0 * l - q;
			r = hue2rgb(p, q, h + 1.0/3.0);
			g = hue2rgb(p, q, h);
			b = hue2rgb(p, q, h - 1.0/3.0);
		}
		return std::make_tuple(r * 255.0, g * 255.0, b * 255.0);
	}

	std::tuple<float, float, float> rgbToHsv(unsigned char _r, unsigned char _g, unsigned char _b) {
		float r = float(_r) / 255.0;
		float g = float(_g) / 255.0;
		float b = float(_b) / 255.0;
		auto max = std::max(r, std::max(g, b));
		auto min = std::min(r, std::max(g, b));
		float h = 0;
		float v = max;
		float d = max - min;
		float s = max == 0.0 ? 0.0 : d / max;
		if(max != min) {
			if(max == r) { h = (g - b) / d + (g < b ? 6.0 : 0.0); }
			else if(max == g) { h = (b - r) / d + 2.0; }
			else if(max == b) { h = (r - g) / d + 4.0; }
			h /= 6.0;
		}
		return std::make_tuple(h, s, v);
	}

	std::tuple<int, int, int> hsvToRgb(float h, float s, float v) {
		float r, g, b;
		int i = std::floor(h * 6.0);
		float f = h * 6.0 - i;
		float p = v * (1.0 - s);
		float q = v * (1.0 - f * s);
		float t = v * (1.0 - (1.0 - f) * s);
		switch(i % 6) {
			case 0: r = v, g = t, b = p; break;
			case 1: r = q, g = v, b = p; break;
			case 2: r = p, g = v, b = t; break;
			case 3: r = p, g = q, b = v; break;
			case 4: r = t, g = p, b = v; break;
			case 5: r = v, g = p, b = q; break;
		}
		return std::make_tuple(r * 255.0, g * 255.0, b * 255.0);
	}
}

Color Color::fromRGB(unsigned char r, unsigned char g, unsigned char b) {
	Color c;
	c._r = r;
	c._g = g;
	c._b = b;
	return c;
}

Color Color::fromHSL(float h, float s, float l) {
	int r, g, b;
	std::tie(r, g, b) = hsl2rgb(h, s, l);
	return Color::fromRGB(r, g, b);
}

bool Color::operator==(const Color& rhs) const {
	return (_r == rhs._r) && (_g == rhs._g) && (_b == rhs._b);
}

bool Color::operator!=(const Color& rhs) const {
	return (*this == rhs) == false;
}

unsigned char Color::r() const {
	return _r;
}

unsigned char Color::g() const {
	return _g;
}

unsigned char Color::b() const {
	return _b;
}

void Color::r(unsigned char r) {
	_r = r;
}

void Color::g(unsigned char g) {
	_g = g;
}

void Color::b(unsigned char b) {
	_b = b;
}

float Color::h() const {
	float h, s, l;
	std::tie(h, s, l) = rgb2hsl(_r, _g, _b);
	return h;
}

float Color::s() const {
	float h, s, l;
	std::tie(h, s, l) = rgb2hsl(_r, _g, _b);
	return s;
}

float Color::l() const {
	float h, s, l;
	std::tie(h, s, l) = rgb2hsl(_r, _g, _b);
	return l;
}

void Color::h(float h) {
	*this = Color::fromHSL(h, s(), l());
}

void Color::s(float s) {
	*this = Color::fromHSL(h(), s, l());
}

void Color::l(float l) {
	*this = Color::fromHSL(h(), s(), l);
}

