#pragma once

struct Word {
	int x;
	int y;
	int length;
};

static const Word words[] = {
	Word{0, 0, 2},  // ES
	Word{3, 0, 3},  // IST
	Word{7, 0, 4},  // FUENF
	Word{0, 1, 4},  // ZEHN
	Word{4, 1, 7},  // ZWANZIG
	Word{4, 2, 7},  // VIERTEL
	Word{0, 4, 4},  // HALB
	Word{0, 2, 11}, // DREIVIERTEL
	Word{6, 3, 3},  // VOR
	Word{2, 3, 4},  // NACH
	Word{2, 5, 3},  // EIN
	Word{8, 9, 3},  // UHR
	Word{5, 4, 5},  // ZERO/ZWOELF
	Word{2, 5, 4},  // ONE
	Word{0, 5, 4},  // TWO
	Word{1, 6, 4},  // THREE
	Word{7, 7, 4},  // FOUR
	Word{7, 6, 4},  // FIVE
	Word{1, 9, 5},  // SIX
	Word{5, 5, 6},  // SEVEN
	Word{1, 8, 4},  // EIGHT
	Word{3, 7, 4},  // NINE
	Word{5, 8, 4},  // TEN
	Word{0, 7, 3},  // ELEVEN
};

enum WORD {
	ES,
	IST,
	FUENF,
	ZEHN,
	ZWANZIG,
	VIERTEL,
	HALB,
	DREIVIERTEL,
	VOR,
	NACH,
	EIN,
	UHR,
	ZERO,
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	TEN,
	ELEVEN,
	_COUNT
};
