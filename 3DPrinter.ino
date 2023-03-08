#include "DEBUG.h"

// initialising Telebot
#include "Telebot.hpp"
Telebot* telebot{nullptr};

// initialize WiFiManager
#include "Wifi.hpp"

void setup(){
  // initialize Serial port
  Serial.begin(115200);

  
  String botToken{};
  Wifi wifi {&botToken, false};

  telebot = new Telebot(botToken);

}

void loop(){
  if(telebot -> is_connected())
    telebot -> handleMsgs();
}
