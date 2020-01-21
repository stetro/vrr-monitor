#include "display.h"

void LiquidCrystalDisplay::clear() { lcd->clear(); }

void LiquidCrystalDisplay::init() {
  Serial.printf("[Display] Connecting to Liquid Display 0x%02x\n",
                DISPLAY_ADDRESS);
  lcd = new LiquidCrystal_I2C(DISPLAY_ADDRESS, 20, 4);
  lcd->init();
  lcd->noCursor();
  lcd->display();
  lcd->backlight();
  lcd->clear();
  lcd->setCursor(1, 1);
}

void LiquidCrystalDisplay::showLoadingScreen() {
  Serial.println("[Display] Show loading Screen");
  lcd->setCursor(0, 1);
  lcd->print("Loading ...");
}

void LiquidCrystalDisplay::turnOff() {
  Serial.println("[Display] Turn Off");
  lcd->noBacklight();
  lcd->noDisplay();
}

void LiquidCrystalDisplay::showError() {
  Serial.println("[Display] Show Error");
  lcd->setCursor(0, 1);
  lcd->print("Query Error!");
}

void LiquidCrystalDisplay::showTimetable() {
  Serial.println("[Display] Show Timetable");
  lcd->print(timetable->title);
  for (size_t i = 0; i < TIMETABLE_LINES; i++) {
    lcd->setCursor(0, i + 1);
    lcd->printf("%c%-8s| %c%-8s", 0b0111110, timetable->timetable[i][0],
                0b0111110, timetable->timetable[i][1]);
  }
}

void LiquidCrystalDisplay::showConfiguration(Configuration* configuration) {
  Serial.println("[Display] Configuration");
  lcd->setCursor(0, 0);
  lcd->print("WiFi AccessPoint:");
  lcd->setCursor(0, 1);
  lcd->printf("SSID: %s", configuration->get_ssid());
  lcd->setCursor(0, 2);
  lcd->print("IP Address:");
  lcd->setCursor(0, 3);
  lcd->printf("%s", configuration->get_ip_address());
}
