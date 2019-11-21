#include "time.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
static WiFiUDP s_udp;

static const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
static byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

// send an NTP request to the time server at the given address
static void sendNTPpacket() {
	// set all bytes in the buffer to 0
	memset(packetBuffer, 0, NTP_PACKET_SIZE);
	// Initialize values needed to form NTP request
	// (see URL above for details on the packets)
	packetBuffer[0] = 0b11100011;   // LI, Version, Mode
	packetBuffer[1] = 0;     // Stratum, or type of clock
	packetBuffer[2] = 6;     // Polling Interval
	packetBuffer[3] = 0xEC;  // Peer Clock Precision
	// 8 bytes of zero for Root Delay & Root Dispersion
	packetBuffer[12]  = 49;
	packetBuffer[13]  = 0x4E;
	packetBuffer[14]  = 49;
	packetBuffer[15]  = 52;
	// all NTP fields have been given values, now
	// you can send a packet requesting a timestamp:
	s_udp.beginPacket("0.de.pool.ntp.org", 123); //NTP requests are to port 123
	s_udp.write(packetBuffer, NTP_PACKET_SIZE);
	s_udp.endPacket();
}

static time_t getNtpTime() {
	while (s_udp.parsePacket() > 0) ; // discard any previously received packets
	//Serial.println("Transmit NTP Request");
	sendNTPpacket();
	uint32_t beginWait = millis();
	while (millis() - beginWait < 1500) {
		int size = s_udp.parsePacket();
		if(size >= NTP_PACKET_SIZE) {
			//Serial.println("Receive NTP Response");
			s_udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
			unsigned long secsSince1900;
			// convert four bytes starting at location 40 to a long integer
			secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
			secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
			secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
			secsSince1900 |= (unsigned long)packetBuffer[43];

			time_t result = secsSince1900 - 2208988800UL;
			//Serial.println("NTP Timestamp: " + String(result));
			return result;
		}
	}
	//Serial.println("No NTP Response :-(");
	return 0; // return 0 if unable to get the time
}

void syncTime() {
	s_udp.begin(8888);
	setSyncProvider(getNtpTime);
}

time_t getLocalTime() {
	TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};
	TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};
	Timezone localTimezone(CEST, CET);
	return localTimezone.toLocal(now());
}
