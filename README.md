<h1 align="center">3D Printer Interface</h1>


<h4> Connecting ESP32 Devices to WiFi </h4>

<p align='justify'><strong>Local Access Point</strong> (or Local AP for short) is an ideal method to use with the ESP32. It allows you to use a phone, tablet, or WiFi-capable computer to set up your ESP32 WiFi credentials, as well as any additional configuration parameters you might require.</p>

```cpp
// interface.ino

#include <WiFiManager.h>

// initialize WiFiManager
WiFiManager wifiManager;
#define AP_NAME "3D Printer Interface"
#define AP_PASSWORD "cgi@munich"

void setup(){
    // initialize Serial port
    Serial.begin(115200);

    // Setup wifi manager
    wifiManager.setConfigPortalTimeout(120);
    wifiManager.setConnectTimeout(30);
    wifiManager.autoConnect(AP_NAME, AP_PASSWORD);

}

void loop(){}
```
<p align='justify'> To get bot tocken from the access point the above code is modified to use WifiManagerParameters</p>

```cpp
// interface.ino

#include <WiFiManager.h>

// initialize WiFiManager
WiFiManager wifiManager;
#define AP_NAME "3D Printer Interface"
#define AP_PASSWORD "cgi@munich"

void setup(){
    // initialize Serial port
    Serial.begin(115200);

    // Setup wifi manager
    wifiManager.setConfigPortalTimeout(120);
    wifiManager.setConnectTimeout(30);
    wifiManager.autoConnect(AP_NAME, AP_PASSWORD);

}

void loop(){}
```


<h4> Creating a skeleton of Telegram Bot class <i>'Telebot'</i> </h4>

```cpp
% Telebot.hpp

#pragma once
#define DEBUG
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>

// Pre-defined messages
#define BUSY_MSG "The Printer is busy"
#define NOT_IMPLIMENTED "This function is not yet implimented"

class Telebot {
    private:
      const String         _token {"Don\'t know"};
      WiFiClientSecure     _client;
      UniversalTelegramBot _bot;
      String               _lockChatID{""};

    public:
      Telebot() = delete;
      Telebot(const String token);
      
      void handleMsgs();
      void handlePrint(const short msgIdx, const bool);
      void handleStatus(const short msgIdx);
      void handleVideo(const short);
      
      const bool sendMsg(const String&, const String);
      const bool sendBusyMsg(const String);
      const String get_token() const;
      const bool is_connected();

};
```
```cpp
%Telebot.cpp

#include "Telebot.hpp"

// Constructor
Telebot::Telebot(const String token)
: _token{token}, 
_bot{UniversalTelegramBot(_token, _client)}
{
  // Add root certificate for api.telegram.org
  _client.setCACert(TELEGRAM_CERTIFICATE_ROOT); 
}

// Handle new messages sent by user
void Telebot::handleMsgs(){

  short n = _bot.getUpdates(_bot.last_message_received + 1);

  for (short msgIdx = 0; msgIdx < n; msgIdx++){
    String ChatID = _bot.messages[msgIdx].chat_id;
    String Msg    = _bot.messages[msgIdx].text;
    bool is_caption = false;
    
    #ifdef DEBUG
      Serial.println(Msg);
    #endif

    if (Msg == ""){
      #ifdef DEBUG
        Serial.println(_bot.messages[msgIdx].file_caption);
      #endif

      Msg = _bot.messages[msgIdx].file_caption;
      is_caption = true;
    }

    if (_lockChatID == "")
      _lockChatID = ChatID;
    
    if (_lockChatID != ChatID){
      sendBusyMsg(ChatID);
      return;
    }

    // if starts with '/' only then process to respond
    // if /print then ask for document, if document not provided with /print command
    // if /status then give status update
    if (Msg == "/print") {
      handlePrint(msgIdx, is_caption);
    } else if (Msg == "/update" or Msg == "/status") {
      handleStatus(msgIdx);
    }

  }
}

// Handles /print command
void Telebot::handlePrint(const short msgIdx, const bool is_caption){
  sendMsg(_bot.messages[msgIdx].chat_id, NOT_IMPLIMENTED);
}

// Handle /status and /update commands
void Telebot::handleStatus(const short msgIdx){
  sendMsg(_bot.messages[msgIdx].chat_id, NOT_IMPLIMENTED);
}

// Handle /video request 
void Telebot::handleVideo(const short msgIdx){
  sendMsg(_bot.messages[msgIdx].chat_id, NOT_IMPLIMENTED);
}

// Helper function to send simple message
const bool Telebot::sendMsg(const String& chatId, const String Msg){
  return _bot.sendMessage(chatId, Msg);
}

// Helper function to send simple "Busy" message
const bool Telebot::sendBusyMsg(const String chatId){
  return sendMsg(chatId, BUSY_MSG);
}

const String Telebot::get_token() const{
  return _token;
}

// Helper function to check if the connection to 
// bot is established
const bool Telebot::is_connected(){
  return _bot.getMe();
}

```
