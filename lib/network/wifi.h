#ifndef WIFI_H
#define WIFI_H

namespace WiFi {
    bool startAP(const char* ssid, const char* password);
    void stop();
}

#endif