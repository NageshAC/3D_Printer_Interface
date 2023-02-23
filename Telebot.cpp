#include "Telebot.hpp"


// Constructor
Telebot::Telebot(const String token)
: _token{token}, _bot{UniversalTelegramBot(_token, _client)}
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
  if (_bot.messages[msgIdx].hasDocument && is_caption){
    Serial.print("text        ");
    Serial.println(_bot.messages[msgIdx].text);
    Serial.print("chat_id     ");
    Serial.println(_bot.messages[msgIdx].chat_id);
    Serial.print("chat_title      ");
    Serial.println(_bot.messages[msgIdx].chat_title);
    Serial.print("from_id     ");
    Serial.println(_bot.messages[msgIdx].from_id);
    Serial.print("from_name     ");
    Serial.println(_bot.messages[msgIdx].from_name);
    Serial.print("date      ");
    Serial.println(_bot.messages[msgIdx].date);
    Serial.print("type      ");
    Serial.println(_bot.messages[msgIdx].type);
    Serial.print("file_caption      ");
    Serial.println(_bot.messages[msgIdx].file_caption);
    Serial.print("file_path     ");
    Serial.println(_bot.messages[msgIdx].file_path);
    Serial.print("file_name     ");
    Serial.println(_bot.messages[msgIdx].file_name);
    Serial.print("hasDocument     ");
    Serial.println(_bot.messages[msgIdx].hasDocument);
    Serial.print("file_size     ");
    Serial.println(_bot.messages[msgIdx].file_size);
    Serial.print("longitude     ");
    Serial.println(_bot.messages[msgIdx].longitude);
    Serial.print("latitude      ");
    Serial.println(_bot.messages[msgIdx].latitude);
    Serial.print("update_id     ");
    Serial.println(_bot.messages[msgIdx].update_id);
    Serial.print("message_id      ");
    Serial.println(_bot.messages[msgIdx].message_id)  ;

    Serial.print("reply_to_message_id     ");
    Serial.println(_bot.messages[msgIdx].reply_to_message_id);
    Serial.print("reply_to_text     ");
    Serial.println(_bot.messages[msgIdx].reply_to_text);
    Serial.print("query_id      ");
    Serial.println(_bot.messages[msgIdx].query_id);
  }
  else {
    Serial.println("File not attached.");
  }
  _bot.sendMessage(_bot.messages[msgIdx].chat_id, NOT_IMPLIMENTED);
  return;
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
