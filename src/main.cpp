#include <Arduino.h>
#include "LiquidCrystal_I2C.h" // https://github.com/johnrickman/LiquidCrystal_I2C.git
#include "WiFi.h"
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>


int lcdColumns = 16;
int lcdRows = 2;
int tempF = 0;

const unsigned long fiveMinutes = 1 * 60 * 1000UL; 
unsigned long previousMillis = 0;

String weather = "";

const char* ssid = "Harmony-Student-Crash";
const char* password = "Spike-They-Putty-67!";
String api_url = "https://api.weather.gov/stations/KBMG/observations/latest";

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  


byte sunChar[] = {
  0b00100,
  0b10101,
  0b01110,
  0b11111,
  0b01110,
  0b10101,
  0b00100,
  0b00000
};

byte moonChar[] = {
  0b01110,
  0b00111,
  0b00011,
  0b00011,
  0b00011,
  0b00011,
  0b00111,
  0b11110
};

byte rainChar[] = {
  0b00000,
  0b00100,
  0b00110,
  0b01110,
  0b11111,
  0b11111,
  0b01110,
  0b00000
};

byte snowChar[] = {
  0b00000,
  0b10101,
  0b01110,
  0b01010,
  0b01110,
  0b10101,
  0b00000,
  0b00000
};

byte cloudChar[] = {
  0b00000,
  0b01100,
  0b11110,
  0b11111,
  0b00000,
  0b00110,
  0b01111,
  0b00000
};

byte windChar[] = {
  0b00000,
  0b00111,
  0b00001,
  0b11110,
  0b11100,
  0b10010,
  0b00110,
  0b00000
};

byte boltChar[] = {
  0b00001,
  0b00010,
  0b00110,
  0b01100,
  0b00110,
  0b01100,
  0b01000,
  0b10000
};


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
  if (client.begin(api_url.c_str())) {
    int httpCode = client.GET();
      if (httpCode > 0) {
        String payload = client.getString();
      
      // Serial.println(payload); // Print the raw JSON response for debugging
      
        JsonDocument doc;
        deserializeJson(doc, payload);

          float tempC = doc["properties"]["temperature"]["value"];
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

}

void loop(){

  lcd.setCursor(6, 0);
  lcd.print(tempF, 1);
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
    } else if (containsIgnoreCase(weather, "cloud")) {
      lcd.write(1);
    } else if (containsIgnoreCase(weather, "snow")) {
      lcd.createChar(2, snowChar);
    } else if (containsIgnoreCase(weather, "rain") || containsIgnoreCase(weather, "drizzle")) {
      lcd.write(3);
    } else if (containsIgnoreCase(weather, "wind") || containsIgnoreCase(weather, "breezy")) {
      lcd.write(4);
    } else if (containsIgnoreCase(weather, "thunder") || containsIgnoreCase(weather, "storm")) {
      lcd.write(5);
    }

  printCenter(weather, 1);

  delay(20000);
  lcd.clear();

  if (millis() - previousMillis >= fiveMinutes) {
    previousMillis = millis(); // Save the time the function was run
    getTemperature();
  }

}