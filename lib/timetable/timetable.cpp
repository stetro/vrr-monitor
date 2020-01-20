
#include "timetable.h"

bool Timetable::queryTimetable(const char *station_id) {
  bool success = false;
  Serial.print("[HTTP] start query ...\n");

  // start htps query
  HTTPClient http;
  String url = TIMETABLE_URL;
  url.replace("TIMETABLE_STATION_ID", station_id);
  http.begin(url, TIMETABLE_CERTIFICATE);
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.GET();

  if (httpCode > 0) {
    Serial.printf("[HTTP] query result code: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK) {
      success = this->handleTimetableResponse(&http);
    } else {
      Serial.printf("[HTTP] query failed, code %d error: %s\n", httpCode,
                    http.errorToString(httpCode).c_str());
    }
  } else {
    Serial.printf("[HTTP] query failed, error: %s\n",
                  http.errorToString(httpCode).c_str());
  }
  http.end();
  return success;
}

bool Timetable::handleTimetableResponse(HTTPClient *http) {
  bool success = false;
  const char *payload = http->getString().c_str();
  Serial.println("[JSON] parsing response ...");
  JsonObject &root = json_buffer.parseObject(payload);
  if (root.success()) {
    Serial.println("[JSON] success, parsed correctly");

    int track0 = 0;
    int track1 = 0;

    for (int i = 0; i < root["departures"].size(); i++) {
      if (root["departures"][i]["lineNumber"].as<int>() != 6) {
        continue;
      }
      if (root["departures"][i]["platform"].as<int>() == 3 &&
          track0 < TIMETABLE_LINES) {
        long long departureTimestamp =
            root["departures"][i]["departure"]["scheduledTime"].as<long long>();
        setTime(departureTimestamp / 1000 + 3600);
        int delay = root["departures"][i]["departure"]["delay"].as<int>();
        if (delay > 0) {
          sprintf(timetable[track0][0], "%02d:%02d+%d", hour(), minute(), delay);
        } else {
          sprintf(timetable[track0][0], "%02d:%02d", hour(), minute());
        }
        Serial.printf("0 %s\n",
                      root["departures"][i]["destination"].as<char *>());
        track0++;
      } else if (root["departures"][i]["platform"].as<int>() == 4 &&
                 track1 < TIMETABLE_LINES) {
        long long departureTimestamp =
            root["departures"][i]["departure"]["scheduledTime"].as<long long>();
        setTime(departureTimestamp / 1000 + 3600);
        int delay = root["departures"][i]["departure"]["delay"].as<int>();
        if (delay > 0) {
          sprintf(timetable[track0][1], "%02d:%02d+%d", hour(), minute(), delay);

        } else {
          sprintf(timetable[track0][1], "%02d:%02d", hour(), minute());
        }
        Serial.printf("0 %s\n",
                      root["departures"][i]["destination"].as<char *>());
        track1++;
      }
      success = true;
    }
  } else {
    Serial.println("[JSON] could not parse response");
  }
  json_buffer.clear();
  return success;
}