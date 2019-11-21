#pragma once

enum Mode {
	Init,
	Init2,
	Matrix,
	Time,
	TimeMatrix,
};

struct Config {
	struct {
		const char* version = "1.0.0";
	} firmware;

	struct {
		char hostname[32] = {0};
		char ssid[32] = {0};
		char password[64] = {0};
	} wifi;

	struct {
		char domain[32];
	} ntp;

	struct {
		Mode modeConnecting = Mode::Init;
		Mode modeSyncingTime = Mode::Matrix;
		Mode modeSynced = Mode::TimeMatrix;
		bool modeCustomEnabled = false;
		Mode modeCustom = Mode::Init2;
	} display;
};
