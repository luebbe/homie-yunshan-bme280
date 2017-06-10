#define FW_NAME "yunshan-relay-contact-dht"
#define FW_VERSION "1.0.1"

#include <Homie.h>
#include "homie-node-collection.h"

// I prefer to see the first ESP Boot messages as well, hence 74880 instead of 115200
#define SERIAL_SPEED 74880

// Yunshan relay specific pins
RelayNode relayNode("relay", 4);
ContactNode contactNode("contact", 5);

// Sensor nodes soldered directly to the ESP pins on the Yunshan board
DHT22Node dht22IndoorNode("indoor", 13);
DHT22Node dht22OutdoorNode("outdoor", 12);

void setupHandler() {
  // Advertise units for sensor nodes
  dht22IndoorNode.setupHandler();
  dht22OutdoorNode.setupHandler();
};


void setup() {
  Serial.begin(SERIAL_SPEED);
  Serial << endl << endl;

  welcome();
  otaSetup();

  Homie_setFirmware(FW_NAME, FW_VERSION);

  Homie.disableResetTrigger();
  Homie.disableLedFeedback();

  Homie.setSetupFunction(setupHandler);
  Homie.setup();
}

void loop() {
   Homie.loop();
   otaLoop();
}
