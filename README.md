# Firebase-with-multistream-in-ESP8266-NodeMCU
An IoT-based ESP8266 program that connects to Firebase Realtime Database to monitor and control devices remotely. It reads sensor data and sends it to Firebase while also checking for updates to the Firebase database that can control connected devices. 

Library used:
FirebaseESP8266.h by Mobizt: This library enables communication between the ESP8266 module and Firebase Realtime Database. It allows reading from and writing to the database, setting up streams for real-time updates, and handling JSON data. 
Link: https://github.com/mobizt/Firebase-ESP8266

ESP8266WiFi.h by : This library connects the ESP8266 module to a Wi-Fi network, enabling internet connectivity required to communicate with Firebase.
Link: https://github.com/esp8266/Arduino

ArduinoJson.h by : This library provides JSON parsing and generation for Arduino-based projects. It’s used here to handle JSON data received from Firebase, allowing the ESP8266 to read and control multiple devices based on Firebase’s JSON structure. 
Link: https://github.com/bblanchon/ArduinoJson

Here is the some explantion to the code:
1. The FirebaseData object manages communication with Firebase. MAX_DEVICES is a limit for the devices controlled in this project, and devicesKeys and devicesValues hold device information.
2. Wi-Fi Connection: Connects to the specified Wi-Fi network and prints the device's IP address on success.
3. Firebase Initialization: Sets up Firebase with the database host and authentication key, enabling automatic Wi-Fi reconnection and configuring data buffer sizes for handling larger responses.
4. Start Firebase Stream: Begins streaming data from the specified devices path in Firebase to receive real-time updates.
5. Firebase Stream Check: Continuously checks the Firebase stream. If there is an error or timeout, it prints the reason.
6. Handle Stream Data: If data is available in the stream, it calls printResult() to process the data.
7. Data Type Check: Based on dataType, the function reads and prints the value.
8. JSON Parsing: If the data is in JSON format, the JSON data is parsed to extract device switch states. A dynamic boolean array deviceSwitch is used to store each device's on/off state.
9. Control Devices: Based on each device's state (switch), it uses GPIO(General Purposes Input Output) pins to turn devices on or off.

Workflow
->. Connect to Wi-Fi and Firebase: The ESP8266 connects to Wi-Fi and then initializes Firebase.
->Listen to Firebase Stream: The ESP8266 listens for any data changes in Firebase under the /Mac Japan/Devices path.
->Parse and Control Devices: When Firebase data changes, the ESP8266 reads the data, parses JSON to determine each device’s switch state, and turns devices on or off accordingly.
