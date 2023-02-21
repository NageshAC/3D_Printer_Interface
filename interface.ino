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
