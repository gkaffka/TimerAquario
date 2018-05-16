#include <ESP8266WiFi.h>
#include <time.h>

const char* ssid = "ssid";
const char* password = "psw";

int timezone = 3;
int dst = 0;

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  pinMode(13, OUTPUT);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  configTime(-3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("\nWaiting for time");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(10000);
  }
  Serial.println("");
}

void loop() {
  time_t now = time(nullptr);
  struct tm * timeinfo;
  time(&now);
  timeinfo = localtime(&now);

  int a = (int) timeinfo->tm_hour;
  if (a >= 8 && a < 20) {
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }
  Serial.println(a);
  delay(10000);
}
