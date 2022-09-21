#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

WiFiServer Server(555); // Port : 555
WiFiClient Client;

const char* ssid     = "your-ssid";
const char* password = "your-password";

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ;
    WiFi.mode(WIFI_STA); // client mode
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    ArduinoOTA.setHostname("ESP32-WebSerial-Test");
    ArduinoOTA.onStart([]()
                       { Serial.println("OTA Update"); });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                          { Serial.printf("Progress: %u%%\r\n", (progress / (total / 100))); });
    ArduinoOTA.onEnd([]()
                     { Serial.println("Update completed"); });
    ArduinoOTA.begin();

    if (WiFi.isConnected())
    {
        Server.begin(); // start server
        Server.setNoDelay(true);
    }
    else
    {
        Server.end();
    }
}

int i = 0;
void loop()
{
    if (WiFi.isConnected()) // if wifi connected
    {
        ArduinoOTA.handle();
        // Web serial
        if (Server.hasClient())
        { // check if there are any new clients
            Client = Server.available();
            if (!Client)
            {
            }
            else
            {
                Client.print("Hello, I am client\r\nMy ip is : ");
                Client.println(Client.remoteIP());
            }
        }
        if (Client && Client.connected())
        {
            Client.println("Hello I am from Wifi Serial.\t" + String(i));
            i++;
            delay(500);
        }
        else if (Client)
        {
            Client.stop(); // Stop web server
        }
    }
}