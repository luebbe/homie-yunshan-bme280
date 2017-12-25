#define FW_NAME "yunshan-relay-contact-dht"
#define FW_VERSION "1.0.4"

#include <Homie.h>
#include "ota.hpp"
#include "welcome.hpp"
#include "homie-node-collection.hpp"

// Yunshan relay specific pins
RelayNode relayNode("relay", 4);
ContactNode contactNode("contact", 5);

// Sensor nodes soldered directly to the ESP pins on the Yunshan board
DHT22Node dht22IndoorNode("indoor", 13);
DHT22Node dht22OutdoorNode("outdoor", 12);

// Setup OTA logging via Homie logger
// Setup OTA logging via Homie logger
OtaLogger ota;
Welcome welcome(FW_NAME, FW_VERSION);

void setupHandler() {
  // This is called after the MQTT_CONNECTED event
  ota.setup();
  // Advertise units for sensor nodes
  dht22IndoorNode.setupHandler();
  dht22OutdoorNode.setupHandler();
};

void loopHandler() {
  ota.loop();
}

void setup() {
  Serial.begin(SERIAL_SPEED);
  Serial << endl << endl;

  welcome.show();
  ota.setup();

  Homie_setFirmware(FW_NAME, FW_VERSION);

  Homie.disableResetTrigger();
  Homie.disableLedFeedback();

  Homie.setSetupFunction(setupHandler);
  Homie.setLoopFunction(loopHandler);

  Homie.setup();
}

void loop() {
   Homie.loop();
   ota.loop();
}
