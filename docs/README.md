# Code Explanation

## Files

* <a href="#main-file">Main File</a>
* <a href="#declare_location-function">declare_location()</a>
* <a href="#data_post-function">data_post()</a>
* <a href="#wifi_led-function">wifi_led()</a>
* <a href="#lcd_stats-function">lcd_stats()</a>
* <a href="#lcd_welcome-function">lcd_welcome()</a>
* <a href="#moisture-function">moisture()</a>
* <a href="#get_distance-function">get_distance()</a>

## Main File

### _Include Project Libraries_

```cpp
#include <Stepper.h> // Stepper Library
#include <LiquidCrystal_I2C.h> // LiquidCrystal for I2C Library
#include <WiFi.h> // WiFi Library
#include <HTTPClient.h> // HTTPClient Library
#include <Wire.h> // Wire Library
#include "RTClib.h" // RTClib Library
```

### _Define RTC_

```cpp
RTC_DS3231 rtc; // Define RTC
DateTime now; // ESP32 Time
```

### _Define WiFi Settings_

```cpp
// WiFi
const char* ssid = ""; // SSID for the WiFi
const char* password = ""; // Password for the WiFi
const char* serverName = "ecoseed**.com"; // Define the Server for the data
```

### _Define Component Pins_

```cpp
// Ldr Sensor
const int ldrPin = 4; // LDR Sensor Pin

// Moisture Sensor
int sensor_pin = 34; // Moisture Sensor Pin

// Force Sensor
const int forcePin = 32; // Force Sensor Pin

// Ultrasonic Sensor
const int trigPin = 33; // Trigger Pin
const int echoPin = 25; // Echo Pin
#define SOUND_SPEED 0.034 // Sound Speed

// Leds
const int led = 26; // LED Pin

// Relay
const int relayPin = 2; // Relay Pin
```

### _Create LCD_

```cpp
// Lcd I2c
const int lcdColumns = 16; // Define the Number of Columns for the LCD
const int lcdRows = 2; // Define the Number of Rows for the LCD
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); // Create the LCD
```

### _Create Stepper_

```cpp
// Steppers
#define S1IN1 13 // IN1 for the 1st Stepper
#define S1IN2 12 // IN2 for the 1st Stepper
#define S1IN3 14 // IN3 for the 1st Stepper
#define S1IN4 27 // IN4 for the 1st Stepper

#define S2IN1 18 // IN1 for the 2st Stepper
#define S2IN2 5 // IN2 for the 2st Stepper
#define S2IN3 17 // IN3 for the 2st Stepper
#define S2IN4 16 // IN4 for the 2st Stepper
const int stepsPerRevolution = 2048; // Steps per Revolution
Stepper ldrStepper(stepsPerRevolution, S1IN1, S1IN3, S1IN2, S1IN4); // Create 1st Stepper
Stepper plantStepper(stepsPerRevolution, S2IN1, S2IN3, S2IN2, S2IN4); // Create 2st Stepper
```

### _Open Setup Function_

```cpp
void setup() {
```

#### Begin Serial

```cpp
  Serial.begin(115200);
```

#### Start LCD

```cpp
  lcd.init();
```

#### Enable Backlight of LCD

```cpp
  lcd.backlight();
```

#### Show Welcome Text on LCD

```cpp
  lcd_welcome();
```

#### Check RTC Module

```cpp
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
  }
```

#### Set Pin Modes

```cpp
  // LED Mode
  pinMode(led, OUTPUT);
  // Trigger Mode
  pinMode(trigPin, OUTPUT);
  // Echo Mode
  pinMode(echoPin, INPUT);
```

#### Set Steppers' Speeds

```cpp
  // 1st Stepper Speed
  ldrStepper.setSpeed(10);
  // 2st Stepper Speed
  plantStepper.setSpeed(10);
```

#### Begin Wi-Fi and Check for its Status

```cpp
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
```

#### Print the IP of the ESP32 to the Console

```cpp
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
```

#### Adjust System Time and Close Setup Function

```cpp
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}
```

### _Open Loop Function_

```cpp
void loop() {
```

#### Define ESP32 time through RTC

```cpp
  now = rtc.now();
```

#### Call Functions

```cpp
  declare_location();
  data_post();
  wifi_led();
  lcd_stats();
```

#### Wait 1 Second and Close Loop Function

```cpp
  delay(1000);
}
```

## declare_location Function

### _Define Variables_

```cpp
int stops = 10;
int ldrmax = 0;
int steppos = 0;
int current = 0;
```

### _Create Function_

```cpp
void declare_location() {
```

### _Rotate 1st Stepper Clockwise_

```cpp
  for (int i = 0; i < (stops / 2 + 1); i++) {
    int l = analogRead(ldrPin); // Get LDR Value
    if (l > ldrmax) { // Check if the value is greater that the variable
      ldrmax = l; // Save value
      steppos = (stepsPerRevolution / stops) * i; // Save position
    }
    ldrStepper.step(stepsPerRevolution / stops); // Turn Stepper One Step
    delay(500);
  }
  ldrStepper.step(-(stepsPerRevolution / stops) * 5); // Return Stepper to its original position
```

### _Rotate 1st Stepper Counterclockwise_

```cpp
  for (int i = 0; i < (stops / 2 + 1); i++) {
    int l = analogRead(ldrPin); // Get LDR Value
    if (l > ldrmax) { // Check if the value is greater that the variable
      ldrmax = l; // Save value
      steppos = (-stepsPerRevolution / stops) * i; // Save position
    }
    ldrStepper.step(-stepsPerRevolution / stops); // Turn Stepper One Step
    delay(500);
  }
  ldrStepper.step((stepsPerRevolution / stops) * 5); // Return Stepper to its original position
```

### _Turn 2nd Stepper For the First Time_

```cpp
  if (current == 0) {
    plantStepper.step(steppos * 1.8);
    current = steppos * 1.8;
  } 
```

### _Turn 2nd Stepper_

```cpp
  else {
    plantStepper.step(-current);
    current = steppos * 1.8;
    plantStepper.step(steppos * 1.8);
  }
```

### _Close Function_

```cpp
}
```

## data_post Function

### _Define Variables_

```cpp
int logged = 0;
```

### _Create Function_

```cpp
void data_post() {
```

### _Check if it's the time to post data_

```cpp
  if (now.hour() % 2 == 0 and logged == 0) {
```

### _Check for Wi-Fi_

```cpp
    if (WiFi.status() == WL_CONNECTED) {
```

### _Define Clients_

```cpp
      WiFiClient client;
      HTTPClient http;
```

### _Begin HTTP Server_

```cpp
      http.begin(client, serverName);
```

### _Define Strings_

```cpp
      String weight = "weight=" + String(analogRead(forcePin));
      String humidity = "humidity=" + moisture();
      String height = "height=" + get_distance();
```

### _Define Request Data_

```cpp
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      String httpRequestData = humidity + "&" + height + "&" + weight;
      int httpResponseCode = http.POST(httpRequestData);
```

### _Print Responce Code to Console_

```cpp
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      logged = 1;
      http.end();
    }
```

### _Wi-Fi Disconnected_

```cpp
    else {
      Serial.println("WiFi Disconnected");
    }
```

### _Check for Odd Hour_

```cpp
  } else if (now.hour() % 2 == 1) {
    logged = 0;
  }
```

### _Close Function_

```cpp
}
```

## wifi_led Function

### _Create Function and blink the led_

```cpp
void wifi_led() {
  digitalWrite(led, HIGH);
  delay(500);
  digitalWrite(led, LOW);
  delay(500);
}
```

## lcd_stats Function

### _Create Function and Clear LCD Screen_

```cpp
void lcd_stats(){
  lcd.clear();
```

### _Set Cursor and write the Weight_

```cpp
  lcd.setCursor(0, 0);
  lcd.print("W: " + String(analogRead(forcePin)));
```

### _Set Cursor and Write the Humidity_

```cpp
  lcd.setCursor(8,0);
  lcd.print("H: " + moisture() + "%");
```

### _Set Cursor and Write the Height_

```cpp
  lcd.setCursor(0,1);
  lcd.print("H: " + get_distance() + "cm");
```

### _Close Function_

```cpp
}
```

## lcd_welcome Function

### _Create Function and Clear LCD Screen_

```cpp
void lcd_welcome() {
  lcd.clear();
```

### _Set Cursor and Write Text_

```cpp
  lcd.setCursor(4, 0);
  lcd.print("Eco Seed");
  lcd.setCursor(3, 1);
  lcd.print("Initialize");
  delay(1000);
```

### _Close Function_

```cpp
}
```

## moisture Function

### _Create Function_

```cpp
String moisture() {
```

### _Get sensor value and Convert it to percentage_

```cpp
  int value = analogRead(sensor_pin);
  value = map(value, 4095, 1500, 0, 100);
  return String(value);
```

### _Close Function_

```cpp
}
```

## get_distance Function

### _Create Function_

```cpp
String get_distance() {
```

### _Write Ultrasonic Sensor Pins_

```cpp
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

```

### _Measure Sensor Values_

```cpp
  long duration = pulseIn(echoPin, HIGH);
  float distanceCm = duration * SOUND_SPEED / 2;
  return String(distanceCm);
}
```
