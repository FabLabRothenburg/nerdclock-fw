#pragma once

struct Color {
private:
	unsigned char _r = 0;
	unsigned char _g = 0;
	unsigned char _b = 0;
public:
	static Color fromRGB(unsigned char r, unsigned char g, unsigned char b);
	static Color fromHSL(float h, float s, float l);
	bool operator==(const Color& rhs) const;
	bool operator!=(const Color& rhs) const;

	unsigned char r() const;
	unsigned char g() const;
	unsigned char b() const;
	void r(unsigned char r);
	void g(unsigned char g);
	void b(unsigned char b);

	float h() const;
	float s() const;
	float l() const;
	void h(float h);
	void s(float s);
	void l(float l);
};
