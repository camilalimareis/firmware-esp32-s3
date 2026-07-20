#include "wifi.h"

namespace WIFI {
    void setup() {
        WiFi.mode(WIFI_AP_STA);

        WiFiManager manager;
        manager.setConfigPortalBlocking(true);
        manager.setTimeout(120);
        manager.setDebugOutput(true);

        Serial.println("======================================");
        Serial.println("[WiFi] Initializing Connection...");

        if (!manager.autoConnect("Telemetry-Setup")) {
            Serial.println("[WiFi] Connection failed, restarting...");
            delay(3000);
            ESP.restart();
        }

        Serial.printf("[WiFi] Connected | IP: %s\n", WiFi.localIP().toString().c_str());
        Serial.println("UART Arduino in Serial1 (GPIO9 RX, GPIO10 TX)");
        Serial.println("======================================");
    }

    bool loop() {
        if (WiFi.status() != WL_CONNECTED) {
            static uint32_t lastTry = 0;

            if (millis() - lastTry > 5000) {
                Serial.println("[WiFi] Reconnecting..."); 
                WiFi.reconnect();
                lastTry = millis();
            }
            return false;
        }
        return true;
    }
}