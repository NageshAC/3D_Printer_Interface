#include "DEBUG.h"
#define BOT_TOKEN "5853416457:AAEgn54OQRnHY4rTJ7SbzVlz48e601SVExk"

  
#include <WiFi.h>

// initialize WiFiManager
#include <WiFiManager.h>
WiFiManager* const wifiManager {new WiFiManager()};
#define AP_NAME "3D Printer Interface"
#define AP_PASSWORD "cgi@munich"

// initialising Telebot
#include "Telebot.hpp"
Telebot* telebot{nullptr};

void setup(){
  // initialize Serial port
  Serial.begin(115200);

  // Setup wifi manager
  // wifiManager -> resetSettings();
  // wifiManager -> setConfigPortalTimeout(120);
  // wifiManager -> setConnectTimeout(30);
  wifiManager -> setDarkMode(true);
  // WiFiManagerParameter* botTokenParam {new WiFiManagerParameter("botToken", "Bot Token", BOT_TOKEN, 50)};
  // wifiManager -> addParameter(botTokenParam);
  wifiManager -> autoConnect(AP_NAME, AP_PASSWORD);

  // Creating Telebot in heap
  // telebot = new Telebot(botTokenParam -> getValue());
  telebot = new Telebot(BOT_TOKEN);

  #ifdef DEBUG
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Connected to WiFi!");
      Serial.println(BOT_TOKEN);
    } else {
      Serial.println("Not connected to WiFi!");
    }
  #endif

  // Deleting unwanted Params
  delete wifiManager;
  // delete botTokenParam;
}

void loop(){
  if(telebot -> is_connected())
    telebot -> handleMsgs();
}
