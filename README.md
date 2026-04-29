# Arduino WiFi Weather Display

- Displays current temperature (°F) and weather description on a 16x2 I2C LCD using an ESP32  
- Fetches data from weather.gov API  

## Requirements  
  - ESP32 (or WiFi-capable board)  
  - 16x2 I2C LCD  
  - Libraries: LiquidCrystal_I2C, WiFi, HTTPClient, ArduinoJson  

## Setup  
  - Set WiFi credentials in code  
  - Upload to board  

## Behavior  
  - Connects to WiFi  
  - Requests latest weather data  
  - Converts Celsius to Fahrenheit  
  - Displays temperature and weather on LCD  
  - Updates periodically  

### Notes  
  - Requires `icons.h` for custom characters  

