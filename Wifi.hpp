#pragma once
#include "DEBUG.h"
#include <WiFiManager.h>
#include <WiFi.h>

class Wifi{
  private:

  WiFiManager*          const   _wifiManager    {nullptr};
  WiFiManagerParameter* const   _botTokenParam  {nullptr};
  String*                       _botToken       {nullptr};

  public:

  Wifi() = delete;
  Wifi(String*, const bool);
  ~Wifi();

};