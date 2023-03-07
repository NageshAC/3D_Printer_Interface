#include "esp32-hal.h"
#include "Telebot.hpp"


// Constructor
Telebot::Telebot(const String token)
: _token{token}, _bot{UniversalTelegramBot(_token, _client)}
{
  // Add root certificate for api.telegram.org
  _client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

  // Initialize SD card
  initSD();

}

const bool Telebot::initSD(){
  if(SD_MMC.begin()){
    #ifdef DEBUG
    Serial.println("Card Mount Successful");
    #endif
    return true;
  }
  else {
    #ifdef DEBUG
    Serial.println("Card Mount Failed");
    #endif
  }
  return false;
}

// Handle new messages sent by user
void Telebot::handleMsgs(){

  short n = _bot.getUpdates(_bot.last_message_received + 1);

  if (!n) return;

  for (short msgIdx = 0; msgIdx < n; msgIdx++){

    String ChatID = _bot.messages[msgIdx].chat_id;

    // if busy in printing or downloading file then print busy message and do nothing
    if(_isBusy){
     sendBusyMsg(ChatID);
     return;
    }

    String Msg      = _bot.messages[msgIdx].text;
    bool is_caption = false;
    
    #ifdef DEBUG
      Serial.println(Msg);
    #endif

    if (Msg == "" && _bot.messages[msgIdx].hasDocument){
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
const bool Telebot::handlePrint(const short msgIdx, const bool is_caption){
  if (is_caption){
    return downloadFile(_bot.messages[msgIdx]);
  }
  else {
    #ifdef DEBUG
      Serial.println("File not attached.");
    #endif
    sendMsg(_bot.messages[msgIdx].chat_id, USAGE_INFO_PRINT);
    return false;
  }
}

// Download file and save to SD card
inline const bool Telebot::downloadFile(const telegramMessage& tMsg){
  // Download a file from given URL
  // Save it under the folder name "from_name"
  // and name it as "file_name"

  // if SD card not initialize
  if (!initSD()){
    sendMsg(tMsg.chat_id, SD_ERROR);   
    return false;   
  }

  // Saving it in folder "<chat_id>" as "<file_name>"
  String folderPath {"/" + tMsg.chat_id};
  String filePath {"/" + tMsg.file_name};
  
  // if folder doesn't exits -> create
  if (!SD_MMC.exists(folderPath)){
    #ifdef DEBUG
      Serial.print("Creating folder: ");
      Serial.println(folderPath);
    #endif
    if(!SD_MMC.mkdir(folderPath)){
      #ifdef DEBUG
        Serial.println(FOLDER_ERROR);
      #endif
      sendMsg(tMsg.chat_id, FOLDER_ERROR);   
      return false;
    }
  }

  sendMsg(tMsg.chat_id, DOWNLOADING_FILE);

  // Getting file content from telegram server
  String urlPath {tMsg.file_path};
  urlPath = urlPath.substring(25); // length of "https://api.telegram.org/"

  // Checking connection to BOT
  if (!_client.connected()) {
    #ifdef DEBUG  
        Serial.println(F("[BOT]Connecting to server"));
    #endif
    if (!_client.connect(TELEGRAM_HOST, TELEGRAM_SSL_PORT)) {
      #ifdef DEBUG  
        Serial.println(F("[BOT]Conection error"));
      #endif
    }
  }

  // Send "GET" request
  if (_client.connected()) {
    _client.flush();

    #ifdef DEBUG  
        Serial.println("sending: " + urlPath);
    #endif  

    _client.print(F("GET /"));
    _client.print(urlPath);
    _client.println(F(" HTTP/1.1"));
    _client.println(F("Host:" TELEGRAM_HOST));
    _client.println(F("Accept: application/json"));
    _client.println(F("Cache-Control: no-cache"));
    _client.println();

    // Recieving Responce
    File file = SD_MMC.open(folderPath + filePath, FILE_WRITE);
    if (file){
      unsigned long now = millis();

      uint8_t* data {new uint8_t[BUFF_SIZE]};
      uint8_t len;
      while (millis() - now < WAITING_TIME_RESPONSE) {
        while (_client.available()) {
          now = millis();
          len = _client.read(data, sizeof(data));
          file.write(data, len);
        }

        // if (responseReceived)
        //   break;
      }
      delete[] data;

      #ifdef DEBUG
          Serial.print("\nTransfer Time:  ");
          Serial.println(millis() - now );
      #endif

      file.close();
    } else {
      sendMsg(tMsg.chat_id, DOWNLOAD_FAILED);
      return false;
    }

  }

  
  sendMsg(tMsg.chat_id, DONE_DOWNLOADING);
  // sPrint_telegramMessage(tMsg);
  return true;
}

// Handle /status and /update commands
const bool Telebot::handleStatus(const short msgIdx){
  sendMsg(_bot.messages[msgIdx].chat_id, NOT_IMPLIMENTED);
  return true;
}

// Handle /video request 
const bool Telebot::handleVideo(const short msgIdx){
  sendMsg(_bot.messages[msgIdx].chat_id, NOT_IMPLIMENTED);
  return true;
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

void Telebot::sPrint_telegramMessage(const telegramMessage& tMsg) const {
  Serial.print("  text_____________________________  ");
  Serial.println(tMsg.text);
  Serial.print("  chat_id__________________________  ");
  Serial.println(tMsg.chat_id);
  Serial.print("  chat_title_______________________  ");
  Serial.println(tMsg.chat_title);
  Serial.print("  from_id__________________________  ");
  Serial.println(tMsg.from_id);
  Serial.print("  from_name________________________  ");
  Serial.println(tMsg.from_name);
  Serial.print("  date_____________________________  ");
  Serial.println(tMsg.date);
  Serial.print("  type_____________________________  ");
  Serial.println(tMsg.type);
  Serial.print("  file_caption_____________________  ");
  Serial.println(tMsg.file_caption);
  Serial.print("  file_path________________________  ");
  Serial.println(tMsg.file_path);
  Serial.print("  file_name________________________  ");
  Serial.println(tMsg.file_name);
  Serial.print("  hasDocument______________________  ");
  Serial.println(tMsg.hasDocument);
  Serial.print("  file_size________________________  ");
  Serial.println(tMsg.file_size);
  Serial.print("  longitude________________________  ");
  Serial.println(tMsg.longitude);
  Serial.print("  latitude_________________________  ");
  Serial.println(tMsg.latitude);
  Serial.print("  update_id________________________  ");
  Serial.println(tMsg.update_id);
  Serial.print("  message_id_______________________  ");
  Serial.println(tMsg.message_id)  ;
  Serial.print("  reply_to_message_id______________  ");
  Serial.println(tMsg.reply_to_message_id);
  Serial.print("  reply_to_text____________________  ");
  Serial.println(tMsg.reply_to_text);
  Serial.print("  query_id_________________________  ");
  Serial.println(tMsg.query_id);
}
