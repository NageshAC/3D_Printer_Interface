#pragma once
#define DEBUG
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>

// Pre-defined messages
#define BUSY_MSG "The Printer is busy"
#define NOT_IMPLIMENTED "This function is not yet implimented"

class Telebot {
    private:
      unsigned             _numMsgs{0};
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