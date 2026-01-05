#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>

/* ---------- GPS ---------- */
SoftwareSerial gpsSerial(4, 3); // RX, TX
TinyGPSPlus gps;

/* ---------- SIM900 ---------- */
SoftwareSerial sim900(8, 7); // RX, TX

const char PHONE[] = "+250xxxxxxx";

/* Last known GPS */
double lastLat = 0.0;
double lastLon = 0.0;
bool hasFix = false;

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
  sim900.begin(9600);

  Serial.println("System starting...");
  delay(3000);

  sim900.listen();
  sim900.println("AT");
  delay(1000);
  sim900.println("AT+CMGF=1");
  delay(1000);
  sim900.println("AT+CSCS=\"GSM\"");
  delay(1000);
  sim900.println("AT+CNMI=2,2,0,0,0");
  delay(1000);

  Serial.println("GSM ready. Waiting for SMS commands...");
}

void loop() {
  /* ----- READ GPS CONTINUOUSLY ----- */
  gpsSerial.listen();
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  if (gps.location.isUpdated()) {
    lastLat = gps.location.lat();
    lastLon = gps.location.lng();
    hasFix = true;
  }

  /* ----- READ SMS ----- */
  sim900.listen();
  if (sim900.available()) {
    String sms = sim900.readString();
    sms.toUpperCase();

    Serial.println("SMS received:");
    Serial.println(sms);

    if (sms.indexOf("WHERE IS MY CAR") != -1 || sms.indexOf("WHERE IS MY CAR") != -1) {
      handleLocationRequest();
    }
  }
}

void handleLocationRequest() {
  // Wait up to 60 seconds for GPS fix
  unsigned long start = millis();
  while (!hasFix && millis() - start < 60000) {
    gpsSerial.listen();
    while (gpsSerial.available()) {
      gps.encode(gpsSerial.read());
    }
    if (gps.location.isUpdated()) {
      lastLat = gps.location.lat();
      lastLon = gps.location.lng();
      hasFix = true;
      break;
    }
  }

  if (!hasFix) {
    sendSMS("GPS not fixed yet. Try again in a few minutes.");
    return;
  }

  String msg = "Vehicle Location:\n";
  msg += "https://maps.google.com/?q=";
  msg += String(lastLat, 6);
  msg += ",";
  msg += String(lastLon, 6);

  sendSMS(msg);
}

void sendSMS(String text) {
  sim900.listen();

  sim900.print("AT+CMGS=\"");
  sim900.print(PHONE);
  sim900.println("\"");
  delay(1000);

  sim900.print(text);
  delay(500);

  sim900.write(26);
  delay(5000);

  Serial.println("SMS sent:");
  Serial.println(text);
}
