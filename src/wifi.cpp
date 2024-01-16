#include <WiFi.h>
#include "config.h"
#include "wifi.h"

WiFiServer wifiServer(WIFI_HTTP_PORT);

void wifi_connect()
{
    WiFi.mode(WIFI_STA); // Optional
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.println("\nConnecting");

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(100);
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
}

void get_network_info()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.print("[*] Network information for ");
        Serial.println(WIFI_SSID);

        Serial.println("[+] BSSID : " + WiFi.BSSIDstr());
        Serial.print("[+] Gateway IP : ");
        Serial.println(WiFi.gatewayIP());
        Serial.print("[+] Subnet Mask : ");
        Serial.println(WiFi.subnetMask());
        Serial.println((String) "[+] RSSI : " + WiFi.RSSI() + " dB");
        Serial.print("[+] ESP32 IP : ");
        Serial.println(WiFi.localIP());
    }
}