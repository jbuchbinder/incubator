#include <LiquidCrystalIO.h>
#include <IoAbstractionWire.h>
#include <Wire.h>
#include <WiFi.h>
#include <Preferences.h>
#include <DHT_Async.h>

#include "config.h"
#include "helpers.h"
#include "sensors.h"
#include "sleep.h"
#include "esp_log.h"

// Initial setup for LCD screen (2004A I2C model)
// For ESP32-WROOM:
//    SCL -> D23
//    SDA -> D22
//    VCC -> V1N (5V)
//    GND -> GND (Ground)
//
LiquidCrystalI2C_RS_EN(lcd, LCD_I2C_ADDRESS, false)

// Keep non-volatile information in preferences API
Preferences preferences;

// Define DHT sensor
DHT_Async dht_sensor(DHT22_SENSOR_PIN_NUMBER, DHT_TYPE_22);

#define MODE_MAIN   0
#define MODE_ALT    1
#define MODE_DEBUG  2

char  version[]        = VERSION;
float tempSet          = 93.0F;
float humiditySet      = 90.0F;
float tempNow          = 0.5F; // initial value -- so far off that we know that it's not set
float humidityNow      = 0.5F; // initial value -- so far off that we know that it's not set
bool  heaterOn         = false;
bool  fanOn            = false;
bool  humidifierOn     = false;
bool  pumpOn           = false;
int   currentMode      = MODE_MAIN;
bool  changingMode     = false;

bool  buttonState[5] = { false, false, false, false, false };
int   buttonPins[5]  = { GPIO_BUTTON0, GPIO_BUTTON1, GPIO_BUTTON2, GPIO_BUTTON3, GPIO_BUTTON4 };

// All functions called later on
static void triggerButtonEvent(int buttonId);
static void displayAlt();
static void displayDebug();
static void displayMain();

void setup() {
    delay(1000);
    Serial.begin(115200);

    // Make sure the serial console is ready before proceeding
    //while(Serial.available()==0){}

    ESP_LOGI("setup", "Initializing Incubator v%s", version);

    preferences.begin("incubator", false);

    tempSet     = preferences.getDouble("temperature", tempSet    );
    humiditySet = preferences.getDouble("humidity",    humiditySet);

    // Necessary for I2C displays, needs to be called first.
    Wire.begin();

    // Set up switching for heater and humidifier pins
    ESP_LOGI("setup", "Setting up relay pins for heater and humidifier, set to LOW by default");
    pinMode     (GPIO_HEATER,     OUTPUT);
    pinMode     (GPIO_HEATER_FAN, OUTPUT);
    pinMode     (GPIO_HUMIDIFIER, OUTPUT);
    digitalWrite(GPIO_HEATER,     HIGH  );
    digitalWrite(GPIO_HEATER_FAN, HIGH  );
    digitalWrite(GPIO_HUMIDIFIER, HIGH  );
    digitalWrite(GPIO_PUMP,       HIGH  );

    ESP_LOGI("setup", "Setting up pins for buttons");
    pinMode(GPIO_BUTTON0, INPUT_PULLUP);
    pinMode(GPIO_BUTTON1, INPUT_PULLUP);
    pinMode(GPIO_BUTTON2, INPUT_PULLUP);
    pinMode(GPIO_BUTTON3, INPUT_PULLUP);
    pinMode(GPIO_BUTTON4, INPUT_PULLUP);

    ESP_LOGI("setup", "Configure LCD");

    // set up the LCD's number of columns and rows, must be called.
    lcd.begin(20, 4);

    // Enable backlight
    lcd.configureBacklightPin(3);
    lcd.backlight();

    // Define up and down arrows
    defChar (lcd, UP_ARROW,    up_arrow   );
    defChar (lcd, DOWN_ARROW,  down_arrow );
    defChar (lcd, HEATER_CHAR, heater_char);
    defChar (lcd, HUMID_CHAR,  humid_char );

    // Initial display
    displayMain();

    taskManager.scheduleFixedRate(LCD_REFRESH_RATE, [] {
      if (changingMode) {
        lcd.clear();
        changingMode = false;
      }
      switch (currentMode) {
        case MODE_MAIN:
          displayMain();
          break;
        case MODE_ALT:
          displayAlt();
          break;
        case MODE_DEBUG:
          displayDebug();
          break;
        }
    });

    taskManager.scheduleFixedRate(150, [] {
      for (int i=0; i<5; i++) {
        bool dr = (digitalRead(buttonPins[i]) == LOW);
        if (dr != buttonState[i]) {
          if (dr) {
            ESP_LOGI("button_read", "Triggered event for button %d", i);
            triggerButtonEvent(i);            
          }
          buttonState[i] = dr;
        }
      }
    });

    taskManager.scheduleFixedRate(2000, [] {
      float temperature;
      float humidity;

      if (measure_environment(&dht_sensor, &temperature, &humidity)) {
        // Pass this along to the display variables
        humidityNow = humidity;
#ifdef USE_FAHRENHEIT
        tempNow = temperature * 1.8 + 32; // convert to fahrenheit
#else
        tempNow = temperature;
#endif // USE_FAHRENHEIT
        ESP_LOGD("measure_environment", "TEMP: %f HUMIDITY: %f", tempNow, humidityNow);
      }
    });

    taskManager.scheduleFixedRate(CONTROL_REFRESH_RATE, [] {
      if (tempNow < tempSet && tempNow > 0.6F) {
        if (!heaterOn) {
          ESP_LOGI("heaterControl", "Heater: ON");
          heaterOn = true;
          digitalWrite(GPIO_HEATER,     LOW);
          digitalWrite(GPIO_HEATER_FAN, LOW);
        }
      } else {
        if (heaterOn) {
          ESP_LOGI("heaterControl", "Heater: OFF");
          heaterOn = false;
          digitalWrite(GPIO_HEATER,     HIGH);
          digitalWrite(GPIO_HEATER_FAN, HIGH);
        }
      }
      if (humidityNow < humiditySet && humidityNow > 0.6F) {
        if (!humidifierOn) {
          ESP_LOGI("heaterControl", "Humidifier: ON");
          humidifierOn = true;
          digitalWrite(     GPIO_PUMP,         LOW);
          //delayMicroseconds(PUMP_OFFSET_MS * 1000);
          digitalWrite(     GPIO_HUMIDIFIER,   LOW);
        }
      } else {
        if (humidifierOn) {
          ESP_LOGI("heaterControl", "Humidifier: OFF");
          humidifierOn = false;
          digitalWrite(     GPIO_HUMIDIFIER,  HIGH);
          //delayMicroseconds(PUMP_OFFSET_MS * 1000);
          digitalWrite(     GPIO_PUMP,        HIGH);
        }
      }
    });
}

static void triggerButtonEvent(int buttonId) {
  ESP_LOGI("triggerButtonEvent", "TRIGGER EVENT %d", buttonId);
  switch (buttonId) {
    case 0:
      ESP_LOGI("triggerButtonEvent", "BUTTON: SELECT");
      switch (currentMode) {
        case MODE_MAIN:
          currentMode = MODE_ALT;
          break;
        case MODE_ALT:
          currentMode = MODE_DEBUG;
          break;
        case MODE_DEBUG:
          currentMode = MODE_MAIN;
          break;
        }
      changingMode = true;
      break;
    case 1:
      ESP_LOGI("triggerButtonEvent", "BUTTON: DEC");
      switch (currentMode) {
        case MODE_MAIN:
          tempSet -= TEMP_INCREMENT;
          preferences.putDouble("temperature", tempSet);
          break;
        case MODE_ALT:
          break;
        case MODE_DEBUG:
          break;
      }
      break;
    case 2:
      ESP_LOGI("triggerButtonEvent", "BUTTON: BEFORE");
      switch (currentMode) {
        case MODE_MAIN:
          humiditySet -= HUMIDITY_INCREMENT;
          preferences.putDouble("humidity", humiditySet);
          break;
        case MODE_ALT:
          break;
        case MODE_DEBUG:
          break;
      }
      break;
    case 3:
      ESP_LOGI("triggerButtonEvent", "BUTTON: INC");
      switch (currentMode) {
        case MODE_MAIN:
          tempSet += TEMP_INCREMENT;
          preferences.putDouble("temperature", tempSet);
          break;
        case MODE_ALT:
          break;
        case MODE_DEBUG:
          break;
      }
      break;
    case 4:
      ESP_LOGI("triggerButtonEvent", "BUTTON: NEXT");
      switch (currentMode) {
        case MODE_MAIN:
          humiditySet += HUMIDITY_INCREMENT;
          preferences.putDouble("humidity", humiditySet);
          break;
        case MODE_ALT:
          break;
        case MODE_DEBUG:
          break;
      }
      break;
  }
}

static void displayMain() {
  lcd.setCursor(0, 0);
  
  // Set up initial display
  lcd.print("Incubator v");
  lcd.print(VERSION);

  // Show temperature and humidity display
  lcd.setCursor(0, 1);
  lcd.print(HEATER_CHAR);
  lcd.print(" S ");
  lcd.print(tempSet, 1);
  lcd.print(" @ ");
  if (tempNow == 0.5F) {
    lcd.print("---");
  } else {
    lcd.print(tempNow, 1);
  }
  lcd.print(DEGREE_CHAR);
#ifdef USE_FAHRENHEIT
  lcd.print("F");
#else
  lcd.print("C");
#endif // USE_FAHRENHEIT

  lcd.setCursor(0, 2);
  lcd.print(HUMID_CHAR);
  lcd.print(" S ");
  lcd.print(humiditySet, 1);
  lcd.print(" @ ");
  if (humidityNow == 0.5F) {
    lcd.print("---");
  } else {
    lcd.print(humidityNow, 1);
  }
  lcd.print("%");

  // Heater running status display
  lcd.setCursor(19, 1);
  if (heaterOn) {
    lcd.print(UP_ARROW);        
  } else {
    lcd.print(DOWN_ARROW);
  }

  // Humidifier running status display
  lcd.setCursor(19, 2);
  if (humidifierOn) {
    lcd.print(UP_ARROW);        
  } else {
    lcd.print(DOWN_ARROW);
  }

  // Run time display

  lcd.setCursor(0, 3);
  lcd.print("Run Time: ");
  float secondsFraction = millis()  /    1000.0F;
  int minutes = int(secondsFraction / 60) %   60;
  int seconds = int(secondsFraction)      %   60;
  int hours   = int(secondsFraction)      / 3600;
  if (hours < 100) {
    lcd.print(" ");
  }
  if (hours < 10) {
    lcd.print(" ");
  }
  lcd.print(hours);
  lcd.print("h");
  if (minutes < 10) {
    lcd.print("0");
  }
  lcd.print(minutes);
  lcd.print("m");
  if (seconds < 10) {
    lcd.print("0");
  }
  lcd.print(seconds);
  lcd.print("s");
}

static void displayAlt() {

}


static void displayDebug() {
  for (int i=0; i<5; i++) {
    lcd.setCursor(i+1, 0);
    if (buttonState[i]) {
      lcd.print(String(i));        
    } else {
      lcd.print(" ");
    }
  }
}

void loop() {
    taskManager.runLoop();
}
