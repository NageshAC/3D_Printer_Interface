#pragma once
#define DEBUG
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>
#include <SD_MMC.h>

#define WAITING_TIME_RESPONSE 10000 // 10s waiting time for responce from BOT

// Pre-defined messages
#define SD_ERR            "SD Card Error.       :("
#define BUSY_MSG          "The Printer is busy.   :P"
#define NOT_IMPLIMENTED   "This function is not yet implimented.    :("
#define DOWNLOADING_FILE  "Downloading the file. Please wait!"
#define DONE_DOWNLOADING  "Done Downloading."
#define DOWNLOAD_FAILED   "Download Failed."
#define LARGE_FILE        "Unfortunately, It is not possible to download file larger than 20MB."

class Telebot {
    private:
      bool                  _isBusy{false};
      const String          _token {"Don\'t know"};
      WiFiClientSecure      _client;
      UniversalTelegramBot  _bot;
      String                _lockChatID{""};
      bool                  _sdInit{false};

    public:
      Telebot() = delete;
      Telebot(const String token);
      const bool initSD();
      void       handleMsgs();
      const bool handlePrint(const short msgIdx, const bool);
      const bool handleStatus(const short msgIdx);
      const bool handleVideo(const short);
      const bool downloadFile(const telegramMessage&);

      const bool   sendMsg(const String&, const String);
      const bool   sendBusyMsg(const String);
      const String get_token() const;
      const bool   is_connected();

      void sPrint_telegramMessage(const telegramMessage&) const ;

};