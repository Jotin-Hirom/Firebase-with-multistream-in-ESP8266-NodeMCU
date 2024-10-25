#include "FirebaseESP8266.h"  // Enables communication with Firebase
#include <ESP8266WiFi.h>      // Connects ESP8266 to Wi-Fi network
// #include <ESP8266Firebase.h>
#include <ArduinoJson.h>  // Parses JSON data from Firebase

#define trigger_pin D2  // Trigger pin for distance sensor
#define echo_pin D3     // Echo pin for distance sensor
#define BUZZER D5       // Buzzer control pin
#define DHTPIN D1       // Pin connected to DHT sensor
#define DHTTYPE DHT11   // DHT sensor type (DHT11)
#define LDR A0          // Analog input pin for light-dependent resistor (LDR)

#define FIREBASE_HOST             //HIDDEN      // Firebase Database URL
#define FIREBASE_AUTH             //HIDDEN      // Firebase Database Authentication Token
#define WIFI_SSID "Lab 11"        // Wi-Fi SSID
#define WIFI_PASSWORD "21212121"  // Wi-Fi Password

//Define FirebaseESP8266 data object
FirebaseData firebaseData;      // Firebase object for data management
const size_t MAX_DEVICES = 20;  // Maximum devices for management
const char* devicesKeys[MAX_DEVICES];
const char* devicesValues[MAX_DEVICES];
unsigned long sendDataPrevMillis = 0;

String devices = "/Mac Japan/Devices";  // Firebase database path for devices
String sensors = "/Mac Japan/Sensors";  // Firebase database path for sensors

void printResult(FirebaseData& data);  // Process and print the streamed data
void setup() {
  pinMode(4, OUTPUT);    // Set GPIO 4 as output for a relay or LED control
  Serial.begin(115200);  // Initialize Serial Monitor for debugging

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");  // Show connection progress
    delay(300);
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());  // Print device IP address

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);  // Connect to Firebase
  Firebase.reconnectWiFi(true);                  // Automatically reconnect Wi-Fi

  // Set Firebase data buffer sizes for receiving large data
  //Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
  firebaseData.setBSSLBufferSize(1024, 1024);

  //Set the size of HTTP response buffers in the case where we want to work with large data.
  firebaseData.setResponseSize(1024);

  // Start Firebase stream for real-time database updates

  if (!Firebase.beginStream(firebaseData, devices)) {
    Serial.println("------------------------------------");
    Serial.println("Can't begin stream connection...");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
}


void loop() {
  if (!Firebase.readStream(firebaseData)) {
    Serial.println("------------------------------------");
    Serial.println("Can't read stream data...");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
  if (firebaseData.streamTimeout()) {
    Serial.println("Stream timeout, resume streaming...");
    Serial.println();
  }

  if (firebaseData.streamAvailable()) {
    Serial.println("------------------------------------");
    Serial.println("Stream Data available... Printing");
    Serial.println("STREAM PATH: " + firebaseData.streamPath());
    Serial.println("EVENT PATH: " + firebaseData.dataPath());
    Serial.println("DATA TYPE: " + firebaseData.dataType());
    Serial.println("EVENT TYPE: " + firebaseData.eventType());
    Serial.print("VALUE: ");
    printResult(firebaseData);  // Process and print the streamed data
  }
}

void printResult(FirebaseData& data) {
  if (data.dataType() == "int")
    Serial.println(data.intData());
  else if (data.dataType() == "float")
    Serial.println(data.floatData(), 5);
  else if (data.dataType() == "double")
    printf("%.9lf\n", data.doubleData());
  else if (data.dataType() == "boolean")
    Serial.println(data.boolData() == 1 ? "true" : "false");
  else if (data.dataType() == "string")
    Serial.println(data.stringData());

  else if (data.dataType() == "json") {
    Serial.println();
    FirebaseJson& json = data.jsonObject();

    // Iterate through the JSON data and extract key-value pairs
    StaticJsonDocument<200> doc;  // Memory pool for JSON parsing

    DeserializationError error = deserializeJson(doc, json.toString());
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }

    Serial.println(value);
    const char* Device_key_switch;
    size_t numKeys = doc.size();
    Serial.println(numKeys);
    // bool deviceSwitch[numKeys];
    bool* deviceSwitch = new bool[numKeys];
    size_t numDevices = 0;

    // Parse JSON and store each device's switch state
    for (JsonPair device : doc.as<JsonObject>()) {
      devicesKeys[numDevices] = device.key().c_str();
      bool switchD = device.value()["switch"].as<bool>();
      deviceSwitch[numDevices] = switchD;
      numDevices++;
      // Check if the maximum number of devices has been reached
      if (numDevices >= numKeys) {
        break;
      }
    }

    // Control devices based on switch state
    // You can iterate over the devices array
    for (size_t i = 0; i < numDevices; i++) {
      const char* deviceName = devicesKeys[i];
      const char* deviceValue = devicesValues[i];
      bool switchs = deviceSwitch[i];
      Serial.print("Device: ");
      Serial.println(deviceName);
      Serial.print("Value: ");
      Serial.println(deviceValue);
      Serial.print("Switch: ");
      Serial.println(switchs ? "On" : "Off");
      switch (i) {
        case 0:
          if (switchs) {
            Serial.print(deviceName);
            Serial.println(" : turn on");
            digitalWrite(RELAY, HIGH);
          } else {
            Serial.print(deviceName);
            Serial.println(" : turn off");
            digitalWrite(RELAY, LOW);
          }
          break;
        case 1:
          if (switchs) {
            Serial.print(deviceName);
            Serial.println(" : turn on");
            digitalWrite(RELAY, HIGH);

          } else {
            Serial.print(deviceName);
            Serial.println(" : turn off");
            digitalWrite(RELAY, LOW);
          }
          break;
        case 2:
          if (switchs) {
            Serial.print(deviceName);
            Serial.println(" : turn on");
            // digitalWrite(3, HIGH);
          } else {
            Serial.print(deviceName);
            Serial.println(" : turn off");
            // digitalWrite(3, HIGH);
          }
          break;
          // Add more cases for additional devices
        default:
          break;
      }
    }
    delete[] deviceSwitch;  // Clean up dynamic memory //delete[] deviceSwitch ensures no memory leaks.
  }
}
