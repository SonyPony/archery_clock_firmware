#include <string.h>

#include "pico/stdlib.h"

#include "lwip/pbuf.h"
#include "lib/network/dhcpserver.h"
#include <lib/network/tcp_server.h>
#include <lib/logging/logging.h>
#include <lib/network/wifi.h>

constexpr const char* SSID_NAME = "picow_test";
constexpr const char* PASSWORD = "password";

static void recvHandler(TCPClientInfo *clientInfo, tcp_pcb_t *clientPcb, pbuf_t *buffer, err_t err)
{
    for (pbuf_t *q = buffer; q != NULL; q = q->next)
    {
        Logging::log(LoggingLevel::Debug, "Received: ");
        for (int i = 0; i < q->len; i++)
        {
            Logging::log("%c", reinterpret_cast<uint8_t *>(q->payload)[i]);
        }
        Logging::log(", %d bytes.\n", q->len);
    }
}

int main()
{
    stdio_init_all();

    if (!WiFi::startAP(SSID_NAME, PASSWORD))
    {
        Logging::log(LoggingLevel::Error, "Failed to initialize.\n");
        return 1;
    }

    ip4_addr_t mask;
    ip4_addr_t serverIpAddress;
    IP4_ADDR(ip_2_ip4(&serverIpAddress), 192, 168, 4, 1);
    IP4_ADDR(ip_2_ip4(&mask), 255, 255, 255, 0);

    // Start the dhcp server
    DHCPServer dhcp_server(&serverIpAddress, &mask);

    // start tcp server
    TCPServer tcpServer(80);
    tcpServer.recvHandler = &recvHandler;

    if (!tcpServer.start())
    {
        Logging::log(LoggingLevel::Error, "Failed to open server.\n");
        return 1;
    }

    while (true)
    {
        sleep_ms(1000);
    }
    
    tcpServer.stop();
    WiFi::stop();
    return 0;
}