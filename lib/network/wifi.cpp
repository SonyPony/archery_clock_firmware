#include "wifi.h"
#include <lib/logging/logging.h>
#include "pico/cyw43_arch.h"


bool WiFi::startAP(const char* ssid, const char* password)
{
    if (cyw43_arch_init())
    {
        return false;
    }

    cyw43_arch_enable_ap_mode(ssid, password, CYW43_AUTH_WPA2_AES_PSK);
    return true;
}

void WiFi::stop() 
{
    cyw43_arch_deinit();
}