#define FW_NAME "yunshan-relay-contact-dht"
#define FW_VERSION "1.0.5"

#include <Homie.h>
#include "ota.hpp"
#include "welcome.hpp"
#include "PulseNode.hpp"
#include "homie-node-collection.hpp"

// forward declaration
void onOptoCouplerPulse();

// Yunshan relay specific internal pins
#define PIN_RELAY 4
#define PIN_OPTOCOUPLER 5
RelayNode relayNode("relay", PIN_RELAY);
PulseNode pulseNode("pulse", PIN_OPTOCOUPLER);

// Sensor nodes soldered directly to the ESP pins on the Yunshan board
#define PIN_SDA 12
#define PIN_SCL 14
#define PIN_DHT22 13

DHT22Node dht22IndoorNode("indoor", PIN_DHT22);

#define I2C_BME280_ADDRESS 0x76
BME280Node bme280OutdoorNode("outdoor", I2C_BME280_ADDRESS);

// Setup OTA logging via Homie logger
OtaLogger ota;
Welcome welcome(FW_NAME, FW_VERSION);

void onOptoCouplerPulse()
{
  pulseNode.pulseDetected();
}

void setupHandler()
{
  // This is called after the MQTT_CONNECTED event
  ota.setup();
  // Advertise units for sensor nodes
  dht22IndoorNode.setupHandler();
  bme280OutdoorNode.setupHandler();
};

void loopHandler()
{
  ota.loop();
}

void setup()
{
  Serial.begin(SERIAL_SPEED);
  Serial << endl
         << endl;

  welcome.show();
  ota.setup();

  // Initializes I2C for BME280 sensor
  Homie.getLogger() << "• Wire begin SDA=" << PIN_SDA << " SCL=" << PIN_SCL << endl;
  Wire.begin(PIN_SDA, PIN_SCL);

  attachInterrupt(PIN_OPTOCOUPLER, onOptoCouplerPulse, FALLING);

  Homie_setFirmware(FW_NAME, FW_VERSION);

  Homie.disableResetTrigger();
  Homie.disableLedFeedback();

  Homie.setSetupFunction(setupHandler);
  Homie.setLoopFunction(loopHandler);

  Homie.setup();
}

void loop()
{
  Homie.loop();
  ota.loop();
}
