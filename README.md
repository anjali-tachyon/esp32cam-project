# esp32cam-project
ESP32-CAM Project with RTC, Deep Sleep, Camera, VL53L0X Sensor, SD Card, and HTTP Server.

Project Description:
This project leverages the ESP32-CAM module to create a versatile and low-power system with several key features:

Real-Time Clock (RTC) for accurate timekeeping.
Deep Sleep mode to conserve power.
Camera module for capturing images.
VL53L0X sensor for distance measurement.
SD card for data storage.
HTTP server for remote access and control.

Features:

Real-Time Clock (RTC): Keeps track of time even during deep sleep, allowing for scheduled tasks and timestamps.
Deep Sleep: Reduces power consumption by putting the ESP32-CAM into a low-power state when not in use.
Camera Module: Captures and saves images to the SD card or serves them via the HTTP server.
VL53L0X Sensor: Measures the distance to objects and can trigger events based on proximity.
SD Card Storage: Saves images and data logs for later retrieval.
HTTP Server: Allows remote access to images, sensor data, and controls via a web interface.

Hardware Requirements
ESP32-CAM module
RTC module (e.g., DS3231)
VL53L0X distance sensor
MicroSD card
Jumper wires and breadboard
Power supply (e.g., 5V USB power)

Software Requirements: 
Arduino IDE with ESP32 board support
Required libraries:
ESP32 
Adafruit_VL53L0X
RTClib
ESP32WebServer
SD

Getting Started:
Hardware Setup
ESP32-CAM Connections:
Connect the camera module to the ESP32-CAM.
Connect the VL53L0X sensor to the I2C pins (SDA, SCL) of the ESP32.
Connect the RTC module to the I2C pins.
Insert the SD card into the SD card slot.
Power Supply:
Ensure the ESP32-CAM is powered via a suitable 5V source.

Software Setup
1.Install Arduino IDE:
Download and install the Arduino IDE from the official website.
2.Add ESP32 Board Support:
Open Arduino IDE and go to File > Preferences.
In the "Additional Boards Manager URLs" field, add the following URL:
arduino
Copy code:
https://dl.espressif.com/dl/package_esp32_index.json
3. Go to Tools > Board > Boards Manager and install the ESP32 board package.
Install Required Libraries:

4. Go to Sketch > Include Library > Manage Libraries.
Search for and install the following libraries:
ESP32Servo
Adafruit_VL53L0X
RTClib
ESP32WebServer
SD

Upload Code:
Clone or download the project repository from GitHub.
Open the project in Arduino IDE.
Configure the necessary settings in the code (e.g., WiFi credentials, pins).
Upload the code to the ESP32-CAM.
Usage
Power On:
Power on the ESP32-CAM module.
Access HTTP Server:

Connect to the same WiFi network as the ESP32-CAM.
Open a web browser and navigate to the ESP32-CAM's IP address to access the web interface.
Capture Images:

Use the web interface to capture and view images.
Images are also saved to the SD card.
Distance Measurement:

The VL53L0X sensor continuously measures the distance to objects.
Data is available on the web interface and can be logged to the SD card.
Deep Sleep Mode:

The system automatically enters deep sleep mode when inactive to conserve power.
The RTC module keeps track of time and can wake the system for scheduled tasks.
