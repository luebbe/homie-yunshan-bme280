#define FW_NAME "yunshan-relay-contact-bme280"
#define FW_VERSION "1.0.8"

#include <Homie.hpp>
#include "ota.hpp"
#include "welcome.hpp"
#include "RelayNode.hpp"
#include "PulseNode.hpp"
#include "BME280Node.hpp"

// forward declaration
void ICACHE_RAM_ATTR onOptoCouplerPulse();

// Yunshan relay specific internal pins
#define PIN_RELAY 4
#define PIN_OPTOCOUPLER 5
RelayNode relayNode("relay", PIN_RELAY);
PulseNode pulseNode("pulse", PIN_OPTOCOUPLER);

// Sensor nodes soldered directly to the ESP pins on the Yunshan board
#define PIN_SDA 14 // Gelber Draht
#define PIN_SCL 12 // Weißer Draht

#define I2C_BME280_ADDRESS_1 0x76
#define I2C_BME280_ADDRESS_2 0x77
BME280Node bme280OutdoorNode("outdoor", I2C_BME280_ADDRESS_1);
BME280Node bme280IndoorNode("indoor", I2C_BME280_ADDRESS_2);

// Setup OTA logging via Homie logger
OtaLogger ota;
Welcome welcome(FW_NAME, FW_VERSION);

void ICACHE_RAM_ATTR onOptoCouplerPulse()
{
  pulseNode.pulseDetected();
}

void setupHandler()
{
  // This is called after the MQTT_CONNECTED event
  ota.setup();
  // Attach interupt for the pulse node *after* MQTT has connected.
  // If we attach before Homie.setup(), we get stuck in an endless 
  // reboot loop when interrupts are already coming in
  attachInterrupt(PIN_OPTOCOUPLER, onOptoCouplerPulse, FALLING);
}

void loopHandler()
{
  ota.loop();
}

void setup()
{
  Homie_setFirmware(FW_NAME, FW_VERSION);

  Serial.begin(SERIAL_SPEED);
  Serial << endl
         << endl;

  welcome.show();
  ota.setup();

  // One of the two bme280 nodes has to initialize the default temperature offset
  // Otherwise Homie will go into configuration mode
  bme280OutdoorNode.beforeHomieSetup();

  // Initializes I2C for BME280 sensor
  Homie.getLogger() << "• Wire begin SDA=" << PIN_SDA << " SCL=" << PIN_SCL << endl;
  Wire.begin(PIN_SDA, PIN_SCL);

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
