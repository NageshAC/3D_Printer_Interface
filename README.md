<h1 align="center">**3D Printer Interface**</h1>


<h4> Connecting ESP32 Devices to WiFi </h4>

<p align='justify'><strong>Local Access Point</strong> (or Local AP for short) is an ideal method to use with the ESP32. It allows you to use a phone, tablet, or WiFi-capable computer to set up your ESP32 WiFi credentials, as well as any additional configuration parameters you might require.</p>

```c++
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
