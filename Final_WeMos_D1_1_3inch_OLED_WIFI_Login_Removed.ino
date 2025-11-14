#include <ESP8266WiFi.h>
#include <ModbusIP_ESP8266.h>

#include <SPI.h>              //Switch for Other display
#include <Wire.h>             //Switch for Other display
#include <Adafruit_GFX.h>     //Switch for Other display
#include <Adafruit_SH110X.h>  //Switch for Other display

#define INTERVAL     10000

#define i2c_Address 0x3c     //Switch for Other display
#define SCREEN_WIDTH 128     //Switch for Other display
#define SCREEN_HEIGHT 64     //Switch for Other display
#define OLED_RESET -1        //Switch for Other display
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);   //Switch for Other display

IPAddress remote(192,168,0,96); // Classic IP *****CHANGE THIS TO YOURS*******************************************************************************************

WiFiClient client;
ModbusIP mb;

// specific registers to use
uint16_t reg4372, reg4370, reg4118, reg4117;
uint32_t LastModbusRequest = 0;

void setup() {
  Serial.begin(19200);

  // connect to wifi
  WiFi.begin("USERNAME", "PASSWORD"); // INCLUDE USER AND PASSWORD HERE*******************************************************************************************
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  if(!display.begin(i2c_Address, true)) {           //Switch for Other display
    Serial.println(F("SH110X allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();                                   //Switch for Other display

  // Initialize modbus
  mb.client();
}

void loop() {
  mb.task();  

  if (millis() - LastModbusRequest >= INTERVAL) {
    LastModbusRequest = millis();

    if (mb.isConnected(remote)) {
      Serial.println("Reading specific registers...");

      // --- Read Register 4372 SOC ---
      uint8_t tr1 = mb.readHreg(remote, 4372, &reg4372, 1, nullptr, 1);
      while (mb.isTransaction(tr1)) {
        mb.task();
        delay(10);
      }

      // --- Read Register 4370 WhzBng Amps ---
      uint8_t tr2 = mb.readHreg(remote, 4370, &reg4370, 1, nullptr, 1);
      while (mb.isTransaction(tr2)) {
        mb.task();
        delay(10);
      }

      // --- Read Register 4118 PV Watts in Classic used as remote ---
      uint8_t tr3 = mb.readHreg(remote, 4118, &reg4118, 1, nullptr, 1);
      while (mb.isTransaction(tr3)) {
        mb.task();
        delay(10);
      }

      // --- Abandoned In Place for Now - only shows up in serial output ---
      uint8_t tr4 = mb.readHreg(remote, 4117, &reg4117, 1, nullptr, 1);
      while (mb.isTransaction(tr4)) {
        mb.task();
        delay(10);
      }

      // --- Interpret and scale register 4370 Wbjr Amps Followed by variable definitions---
      int16_t signed4370 = (int16_t)reg4370;   // interpret as signed 16-bit
      float amps = signed4370 / 10.0;          // apply scaling ÷10
      float soc = (float)reg4372;       // battery % SOC 
      float PVW = (float)reg4118;       // PV Watts 

      // --- Print values in serial output for debug ---
      Serial.println("Register Values:");
      Serial.printf("4372 SOC: %.1f %%\n", soc);
      Serial.printf("4370 Whizbang Current: %.1f Amps (raw: %u)\n", amps, reg4370);
      Serial.printf("4118: PV %.0f W\n", PVW);
      Serial.printf("4117: %u\n", reg4117);
      Serial.println("---------------------------");

      // Display on OLED 1.3"
      display.clearDisplay();
      display.setTextColor(SH110X_WHITE);

      // Top Left SOC
      display.setCursor(0,0);
      display.setTextSize(3);
      display.printf("SOC %.0f%%", soc);
      display.display();
      delay(500);     // Middle Left Wbjr Amps
      display.setCursor(0,28);
      display.setTextSize(2);
      display.printf("Wbjr %.1fA", amps);
      display.display();
      // Bottom Left PV Watts
      display.setCursor(0,50);
      display.printf("PV %.0f W\n", PVW);
      display.display();
      delay(1000);

    } else { // Will show up in serial output as you're connecting to the Classic
      Serial.println("Connecting to Modbus server...");
      Serial.println("Turn off other software accessing this classic.");
      Serial.println(".");
      mb.connect(remote);
    }
  }
}