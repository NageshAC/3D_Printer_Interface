#include "Wifi.hpp"

Wifi::Wifi(String* str, const bool rst )
: _wifiManager {new WiFiManager()}, _botTokenParam {new WiFiManagerParameter("botToken", "Bot Token", BOT_TOKEN, 50)}, _botToken {str}
{
  if (rst)  _wifiManager -> resetSettings();

  _wifiManager -> setDarkMode(true);

  // wifiManager -> setConfigPortalTimeout(120);
  // wifiManager -> setConnectTimeout(30);

  _wifiManager -> addParameter(_botTokenParam);
  _wifiManager -> autoConnect(AP_NAME, AP_PASSWORD);

  // check if WiFi is connected
  #ifdef DEBUG
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Connected to WiFi!");
      // Serial.println(BOT_TOKEN);
    } else {
      Serial.println("Not connected to WiFi!");
    }
  #endif

  // if autoconnected then _botTokenParam is empty. Then fetch data from mem
  // else the data from _botTokenParam -> getValue()
  String value{_botTokenParam -> getValue()};
  Serial.print("Token:  ");
  Serial.println(value);
  // if (value == ""){
  //   value = BOT_TOKEN;
  // }

  *_botToken = value;

}


Wifi::~Wifi(){
  delete _wifiManager, _botTokenParam;
}