#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#define CONFIGURATION_DATA_BUFFER_SIZE 30

#include <EEPROM.h>

#include <WiFi.h>
#include <AsyncTCP.h>

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

struct ConfigurationData{
  char eeprom_check[3];
  char ssid[CONFIGURATION_DATA_BUFFER_SIZE];
  char password[CONFIGURATION_DATA_BUFFER_SIZE];
};

class Configuration {
 private:
  unsigned char ssid_buffer[6];
  char ssid[13];
  String ip_address;
  AsyncWebServer* server;
  ConfigurationData configuration_data;

 public:
  void init();
  inline char* get_ssid() { return ssid; }
  inline const char* get_ip_address() { return ip_address.c_str(); }
  ConfigurationData* get_configuration();
  void clear();
};


#endif  // CONFIGURATION_H
