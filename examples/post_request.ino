#include <R4Quest.h>
#include <WiFi.h>

#define IP "192.168.1.100"

char ssid[] = "YOUR_NETWORK_SSID";
char pass[] = "YOUR_NETWORK_PASS";

static HTTPClient client;
int status = WL_IDLE_STATUS; 

void setup() {
    Serial.begin(9600);
    if (WiFi.status() == WL_NO_SHIELD) {
        Serial.println("Communication with WiFi module failed!");
        while (true)
        ;
    }

    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
        
        status = WiFi.begin(ssid, pass);
        delay(10000);
    }
}

void loop() {
    client.set_header("Content-Type", "application/json");
    client.request_post(IP, 80, "{\"name\": \"HTTPLib\"}");
}