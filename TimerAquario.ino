#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <time.h>

const char* ssid = "Capiroto";
const char* password = "hal90000";

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
  int sunrise = 9;
  int sunset = 20;
  HTTPClient http;  //Declare an object of class HTTPClient
  http.begin("http://sunapi.herokuapp.com/day");  //Specify request destination
  int httpCode = http.GET();//Send the request
  Serial.println("code: " + httpCode);
  if (httpCode > 0) { //Check the returning code
    const size_t bufferSize = JSON_OBJECT_SIZE(3) + 60;
    DynamicJsonBuffer jsonBuffer(bufferSize);
    JsonObject& root = jsonBuffer.parseObject(http.getString());
    const char* sunrise_c = root["sunrise"]; // "1"
    const char* sunset_c = root["sunset"]; // "23"
    sunrise = atol( sunrise_c );
    sunset = atol (sunset_c);
    Serial.print("sunrise: ");
    Serial.println(sunrise);
    Serial.print("sunset: ");
    Serial.println(sunset);
  }
  http.end();   //Close connection

  time_t now = time(nullptr);
  struct tm * timeinfo;
  time(&now);
  timeinfo = localtime(&now);
  if (sunrise == 0) {
    sunrise = 9;
  }
  if (sunset == 0) {
    sunset = 20;
  }
  int a = (int) timeinfo->tm_hour;
  if (a >= sunrise && a < sunset) {
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }
  Serial.println(a);
  delay(10000);
}
