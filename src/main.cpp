#include <Arduino.h>
#include "LiquidCrystal_I2C.h" // https://github.com/johnrickman/LiquidCrystal_I2C.git
#include "WiFi.h"
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "icons.h"


int lcdColumns = 16;
int lcdRows = 2;
int tempF = 0;

const unsigned long refreshRate = 1 * 60 * 1000UL; // 1 minute in milliseconds
unsigned long previousMillis = 0;

String weather = "";

const char* ssid = "Harmony-Student-Crash";
const char* password = "Spike-They-Putty-67!";
const char* api_url = "https://api.weather.gov/stations/KBMG/observations/latest";

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  




bool containsIgnoreCase(String mainString, String subString) {
  
  // Create lowercase copies for comparison
    String lowerMain = mainString;
    lowerMain.toLowerCase();
    String lowerSub = subString;
    lowerSub.toLowerCase();

  // Check if the lowercase substring is found in the lowercase main string
    if (lowerMain.indexOf(lowerSub) != -1) {
      return true;
    } else {
      return false;
    }
}



void getTemperature() {
  HTTPClient client = HTTPClient();
  if (client.begin(api_url)) {
    int httpCode = client.GET();
      if (httpCode > 0) {
        String payload = client.getString();
      
      // Serial.println(payload); // Print the raw JSON response for debugging
      
        JsonDocument doc;
        deserializeJson(doc, payload);

          float tempC = doc["properties"]["temperature"]["value"];
          Serial.println("Temperature in Celsius: " + String(tempC));
          float tempF_float = tempC * 9.0 / 5.0 + 32.0;

          weather = doc["properties"]["textDescription"].as<String>();
          Serial.println("Current Weather: " + weather);
        
        tempF = (int)round(tempF_float);
        Serial.print(tempF);
        Serial.println("F");
      } else {
        Serial.printf("API Request failed, error: %s\n", client.errorToString(httpCode).c_str());
      }
    }
      client.end();
}

void printCenter(String text, int row) {
  
  int len = text.length();
  int pos = (16 - len) / 2;
  lcd.setCursor(pos, row);
  lcd.print(text);

}

void setup(){
  Serial.begin(115200);

    lcd.init(); // initialize LCD
    lcd.backlight(); // turn on LCD backlight

  // create custom characters

    lcd.createChar(0, sunChar);
    lcd.createChar(1, cloudChar);
    lcd.createChar(2, snowChar);
    lcd.createChar(3, rainChar);
    lcd.createChar(4, windChar);
    lcd.createChar(5, boltChar);

  // connect to Wi-Fi network

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  getTemperature();
}

void loop(){

  lcd.setCursor(6, 0);
  lcd.print(tempF);
  lcd.print((char)223);
  lcd.print("F");

  lcd.setCursor(10, 0);
 
    // if (tempF >= 65) {
    //   lcd.write(0);
    // } else if (tempF >= 50) {
    //   lcd.write(1);
    // } else if (tempF >= 32) {
    //   lcd.write(2);
    // }

    if (containsIgnoreCase(weather, "sun") || containsIgnoreCase(weather, "clear")) {
      lcd.write(0);
    } 
    
    if (containsIgnoreCase(weather, "cloud")) {
      lcd.write(1);
    } 
    
    if (containsIgnoreCase(weather, "snow")) {
      lcd.write(2);
    } 
    
    if (containsIgnoreCase(weather, "rain") || containsIgnoreCase(weather, "drizzle")) {
      lcd.write(3);
    } 
    
    if (containsIgnoreCase(weather, "wind") || containsIgnoreCase(weather, "breezy")) {
      lcd.write(4);
    } 
    
    if (containsIgnoreCase(weather, "thunder") || containsIgnoreCase(weather, "storm")) {
      lcd.write(5);
    }

  printCenter(weather, 1);

  delay(20000);
  lcd.clear();

  if (millis() - previousMillis >= refreshRate) {
    previousMillis = millis(); // Save the time the function was run
    getTemperature();
  }

}