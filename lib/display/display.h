#ifndef DISPLAY_H
#define DISPLAY_H


#ifndef DISPLAY_ADDRESS
#define DISPLAY_ADDRESS 0x27
#endif  // DISPLAY_ADDRESS

#define PIN_WIRE_SDA 21
#define PIN_WIRE_SCL 22

#include <Wire.h>
#include <timetable.h>
#include <configuration.h>

// Liquid Crystal includes
#include <LiquidCrystal_I2C.h>

class Display {
 public:
  inline void updateTimetable(Timetable* timetable) {
    this->timetable = timetable;
    this->time_table_ticker = 0;
  };

  virtual void init() = 0;
  virtual void clear() = 0;
  virtual void showTimetable() = 0;
  virtual void showLoadingScreen() = 0;
  virtual void showError() = 0;
  virtual void turnOff() = 0;
  virtual void showConfiguration(Configuration* configuration) = 0;

 protected:
  int time_table_ticker = 0;
  Timetable* timetable;
};

class LiquidCrystalDisplay : public Display {
 public:
  void init();
  void clear();
  void showTimetable();
  void showLoadingScreen();
  void showError();
  void turnOff();
  virtual void showConfiguration(Configuration* configuration);

 private:
  LiquidCrystal_I2C* lcd;
};
#endif  // DISPLAY_H
