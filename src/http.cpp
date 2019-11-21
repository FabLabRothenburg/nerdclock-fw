#include "http.h"

template<size_t N>
void readParam(ESP8266WebServer& server, char(&var)[N], const char* param) {
	if (server.hasArg(param)) {
		server.arg(param).toCharArray(var, N);
	}
}

void readParam(ESP8266WebServer& server, bool& var, const char* param) {
	var = false;
	if (server.hasArg(param)) {
		var = server.arg(param) == "1";
	}
}

void readParam(ESP8266WebServer& server, int& var, const char* param) {
	var = false;
	if (server.hasArg(param)) {
		var = server.arg(param).toInt();
	}
}

HttpServer::HttpServer(Config& config)
	: server(80)
	, config(config) {
}

void HttpServer::init() {
	server.on("/", [this]() {
		const char* htmlPage = R"HTMLPAGE(
			<!DOCTYPE html>
			<html lang="en">
				<head>
					<meta charset='utf-8' />
					<meta name='author' content='Felix Hufnagel' />
					<title>Nerdclock</title>
					<style>
					</style>
				</head>
				<body oncontextmenu="return false;">
					<script src="https://cdnjs.cloudflare.com/ajax/libs/vue/2.6.10/vue.js"></script>
					<div id="app">
						<h1>Nerdclock</h1>
						<div v-if="config">
							<form>
								<label>Firmware</label><input type="text" disabled v-model="firmware_version"></input>
								<label>Hostname:</label><input type="text" v-model="wifi_hostname"></input>
								<label>SSID:</label><input type="text" v-model="wifi_ssid"></input>
								<label>Password:</label><input type="password" v-model="wifi_password"></input>
								<label>NTP Domain:</label><input type="text" v-model="ntp_domain"></input>
								<label><Mode:</label>
								<select v-model="config.display_modeConnecting">
									<option value="0">Init</option>
									<option value="1">Init2</option>
									<option value="2">Matrix</option>
									<option value="3">Time</option>
									<option value="4">TimeMatrix</option>
								</select>
								<button @click.left="makePersistent">Store</button>
							<form>
						</div>
					</div>
					<script>
						function ajaxGet(url) {
							return new Promise(function(resolve, reject) {
								var xhr = window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject('Microsoft.XMLHTTP');
								xhr.open('GET', url);
								xhr.onreadystatechange = function() {
									if(xhr.readyState === XMLHttpRequest.DONE) {
										if(xhr.status === 200) {
											resolve(xhr.responseText);
										}
										else {
											reject(Error(xhr.statusText));
										}
									}
								};
								xhr.onerror = function() {
									reject(Error("Connection Lost"));
								};
								xhr.setRequestHeader('X-Requested-With', 'XMLHttpRequest');
								xhr.send();
							});
						}

						function ajaxPost(url, data) {
							return new Promise(function(resolve, reject) {
								var xhr = window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject("Microsoft.XMLHTTP");
								xhr.open('POST', url);
								xhr.onreadystatechange = function() {
									if(xhr.readyState === XMLHttpRequest.DONE) {
										if(xhr.status === 200) {
											resolve(xhr.responseText);
										}
										else {
											reject(Error(xhr.statusText));
										}
									}
								};
								xhr.onerror = function() {
									reject(Error("Connection Lost"));
								};
  								var fd  = new FormData();
								for(name in data) {
									fd.append(name, data[name]);
								}
								xhr.send(fd);
							});
						}

						let app = new Vue({
							el: '#app',
							data: {
								config: null,
							},
							watch: {
								config: {
									deep: true,
									handler() {
										this.send();
									}
								}
							},
							mounted() {
								// this.timer = setInterval(this.poll, 1000);
								this.poll();
							},
							beforeDestroy() {
								clearInterval(this.timer);
							},
							methods: {
								poll() {
									ajaxGet("/config").then((value) => {
										this.config = JSON.parse(value);
									});
								},
								send() {
									ajaxPost("/config", this.config).then((value) => {
									});
								},
								makePersistent() {
									ajaxPost("/makePersistent").then((value) => {
									});
								}
							},
						});
					</script>
				</body>
			</html>
		)HTMLPAGE";

		server.send(200, "text/html", htmlPage);
	});

	server.on("/config", HTTP_GET, [this]() {
		String json =  R"JSON({
			"firmware_version": "{{firmware_version}}",
			"wifi_hostname": "{{wifi_hostname}}",
			"wifi_ssid": "{{wifi_ssid}}",
			"wifi_password": "{{wifi_password}}",
			"ntp_domain": "{{ntp_domain}}",
			"display_modeConnecting": "{{display_modeConnecting}}"
		})JSON";
		json.replace("{{firmware_version}}", config.firmware.version);
		json.replace("{{wifi_hostname}}", config.wifi.hostname);
		json.replace("{{wifi_ssid}}", config.wifi.ssid);
		json.replace("{{wifi_password}}", config.wifi.password);
		json.replace("{{ntp_domain}}", config.ntp.domain);
		json.replace("{{display_modeConnecting}}", String((int)config.display.modeConnecting));
		server.send(200, "application/json", json);
	});

	server.on("/config", HTTP_POST, [this]() {
		readParam(server, config.wifi.hostname, "wifi._hostname");
		readParam(server, config.wifi.ssid, "wifi_ssid");
		readParam(server, config.wifi.password, "wifi_password");
		readParam(server, config.ntp.domain, "ntp_domain");
		readParam(server, (int&)config.display.modeConnecting, "display_modeConnecting");
		server.send(200, "text/plain", "OK");
	});

	server.on("/makePersistent", HTTP_POST, [this]() {
		server.send(200, "text/plain", "OK");
	});

	server.onNotFound([this](){
		String message = "File Not Found\n\n";
		message += "URI: ";
		message += server.uri();
		message += "\nMethod: ";
		message += (server.method() == HTTP_GET) ? "GET" : "POST";
		message += "\nArguments: ";
		message += server.args();
		message += "\n";

		for (uint8_t i = 0; i < server.args(); i ++) {
			message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
		}

		server.send(404, "text/plain", message);
	});

	server.begin();
}

void HttpServer::update() {
  server.handleClient();
}
