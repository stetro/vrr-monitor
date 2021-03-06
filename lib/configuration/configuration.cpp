#include <configuration-site.h>
#include <configuration.h>

void Configuration::init() {
  WiFi.begin();
  WiFi.macAddress(ssid_buffer);
  sprintf(ssid, "%02X%02X%02X%02X%02X%02X", ssid_buffer[0], ssid_buffer[1],
          ssid_buffer[2], ssid_buffer[3], ssid_buffer[4], ssid_buffer[5]);
  WiFi.softAP(ssid, NULL);
  ip_address = WiFi.softAPIP().toString();

  server = new AsyncWebServer(80);

  server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("[Configuration] loading index.html");
    request->send(200, "text/html", INDEX_HTML);
  });

  server->on("/update", HTTP_POST, [](AsyncWebServerRequest *request) {
    Serial.println("[Configuration] updating configuration");

    EEPROM.begin(512);

    ConfigurationData data;

    String ssid = request->arg("ssid");
    Serial.println("[Configuration] ssid " + ssid);
    sprintf(data.ssid, "%s", ssid.c_str());

    String password = request->arg("password");
    Serial.println("[Configuration] password ***");
    sprintf(data.password, "%s", password.c_str());

    sprintf(data.eeprom_check, "OK");

    EEPROM.put(0, data);
    EEPROM.commit();

    request->send(200, "text/html",
                  "Configuration updated. Restart your device.");
  });

  server->begin();

  Serial.printf("[Configuration] Running Soft AP at %s\n",
                WiFi.softAPIP().toString().c_str());
}

ConfigurationData *Configuration::get_configuration() {
  Serial.println("[Configuration] loading configuration");
  EEPROM.begin(512);
  EEPROM.get(0, configuration_data);

  Serial.printf("[Configuration] EEPROM %s \n",
                configuration_data.eeprom_check);
  Serial.printf("[Configuration] ssid %s \n", configuration_data.ssid);
  Serial.printf("[Configuration] password ***\n");
  return &configuration_data;
}

void Configuration::clear() {
  Serial.println("[Configuration] clear configuration");
  EEPROM.begin(512);
  ConfigurationData data;
  sprintf(data.eeprom_check, "**");
  sprintf(data.ssid, "***");
  sprintf(data.password, "***");
  EEPROM.put(0, data);
  EEPROM.commit();
}