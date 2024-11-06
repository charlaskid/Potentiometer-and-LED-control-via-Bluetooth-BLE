#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Pin definitions
const int potPin = 35;  // Potentiometer connected to pin D35 (ADC)
const int ledPin = 32;  // LED connected to pin 32

// Constants for ADC conversion and threshold
const int maxADCValue = 4095;
const float referenceVoltage = 3.3;  // Reference voltage (3.3V)
const float voltageThreshold = 2.1;  // Threshold to turn on the LED

// UUIDs for BLE service and characteristics
#define SERVICE_UUID         "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_VOLTAGE "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_LED     "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

BLECharacteristic *pCharacteristicVoltage;
BLECharacteristic *pCharacteristicLED;

bool deviceConnected = false;
float currentVoltage = 0.0;
bool ledState = false;  // true = LED on, false = LED off

// Callback class for handling BLE connection events
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

void setup() {
  // Start Serial for debugging
  Serial.begin(115200);

  // Initialize pin modes
  pinMode(potPin, INPUT);  // Potentiometer pin as input
  pinMode(ledPin, OUTPUT); // LED pin as output
  digitalWrite(ledPin, LOW);  // Start with LED off

  // Initialize BLE device
  BLEDevice::init("ESP32_MAX_PRO_ULTRA");

  // Create BLE server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create BLE service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create BLE characteristics
  pCharacteristicVoltage = pService->createCharacteristic(
                            CHARACTERISTIC_UUID_VOLTAGE,
                            BLECharacteristic::PROPERTY_READ |
                            BLECharacteristic::PROPERTY_NOTIFY
                          );
  pCharacteristicVoltage->addDescriptor(new BLE2902());

  pCharacteristicLED = pService->createCharacteristic(
                         CHARACTERISTIC_UUID_LED,
                         BLECharacteristic::PROPERTY_READ |
                         BLECharacteristic::PROPERTY_NOTIFY
                       );
  pCharacteristicLED->addDescriptor(new BLE2902());

  // Start the service and begin advertising
  pService->start();
  pServer->getAdvertising()->start();
  Serial.println("BLE advertising started...");
}

void loop() {
  // Read the potentiometer value (0-4095)
  int adcValue = analogRead(potPin);

  // Convert the ADC value to voltage
  currentVoltage = (adcValue * referenceVoltage) / maxADCValue;

  // Check if voltage exceeds the threshold and update LED state
  if (currentVoltage > voltageThreshold) {
    digitalWrite(ledPin, HIGH);  // Turn on the LED
    ledState = true;
  } else {
    digitalWrite(ledPin, LOW);   // Turn off the LED
    ledState = false;
  }

  // Print current values to the Serial Monitor
  Serial.print("Voltage: ");
  Serial.print(currentVoltage);
  Serial.print(" V, LED is ");
  Serial.println(ledState ? "ON" : "OFF");

  // Send BLE notifications if the device is connected
  if (deviceConnected) {
    // Notify voltage
    char voltageStr[8];
    dtostrf(currentVoltage, 4, 2, voltageStr);  // Convert float to string
    pCharacteristicVoltage->setValue(voltageStr);
    pCharacteristicVoltage->notify();

    // Notify LED state
    char ledStatusStr[4];
    sprintf(ledStatusStr, "%d", ledState);  // Convert bool to string ("1" for ON, "0" for OFF)
    pCharacteristicLED->setValue(ledStatusStr);
    pCharacteristicLED->notify();

    delay(1000);  // Send data every second
  }

  delay(500);  // Avoid flooding the Serial Monitor
}
