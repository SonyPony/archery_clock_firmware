#include <string.h>

#include "pico/stdlib.h"

#include "lwip/pbuf.h"
#include "lib/network/dhcpserver.h"
#include <lib/network/tcp_server.h>
#include <lib/logging/logging.h>
#include <lib/network/wifi.h>
#include <lib/message/parser.h>
#include <lib/core/buffer.h>
#include <string.h>
#include <lib/data/commands.h>
#include <lib/message/message_info.h>

#include <lib/mode/mode_manager.h>
#include <lib/peripheral/shift_register.h>

// definitions fow wifi
constexpr const char *SSID_NAME = "picow_test";
constexpr const char *PASSWORD = "password";

// definitions for message buffer
constexpr uint32_t MESSAGE_BUFFER_SIZE = 128;
volatile char messageBufferData[MESSAGE_BUFFER_SIZE];
volatile uint32_t messageBufferDataEndIdx = 0;
Buffer messageBuffer(messageBufferData, &messageBufferDataEndIdx, MESSAGE_BUFFER_SIZE);

// IO definitions
constexpr uint32_t SR_CLK_PIN = 28;
constexpr uint32_t SR_DATA_PIN = 27;
constexpr uint32_t SR_CS_PIN = 26;

static void recvHandler(TCPClientInfo *clientInfo, tcp_pcb_t *clientPcb, pbuf_t *buffer, err_t err)
{
    for (pbuf_t *q = buffer; q != NULL; q = q->next)
    {
        Logging::log(LoggingLevel::Debug, "Received: ");
        for (int i = 0; i < q->len; i++)
        {
            const uint8_t byte = reinterpret_cast<uint8_t *>(q->payload)[i];
            Logging::log("%c", byte);
            messageBuffer.addByte(byte);
        }
        Logging::log(", %d bytes.\n", q->len);
    }
}

int main()
{
    stdio_init_all();

    // setup network
    if (!WiFi::startAP(SSID_NAME, PASSWORD))
    {
        Logging::log(LoggingLevel::Error, "Failed to initialize.\n");
        return 1;
    }

    ip4_addr_t mask;
    ip4_addr_t serverIpAddress;
    IP4_ADDR(ip_2_ip4(&serverIpAddress), 192, 168, 4, 1);
    IP4_ADDR(ip_2_ip4(&mask), 255, 255, 255, 0);

    // start tcp server
    DHCPServer dhcpServer(&serverIpAddress, &mask);
    TCPServer tcpServer(80);
    tcpServer.recvHandler = &recvHandler;

    // setup clock managers
    int currentStep = 0;
    ModeManager modeManager;
    MessageParser msgParser(&messageBuffer);
    ShiftRegister shiftRegister(SR_CLK_PIN, SR_DATA_PIN, SR_CS_PIN);
    DisplayController displayController(&shiftRegister);

    tcpServer.newClientCallback = [&tcpServer, &modeManager](TCPClientInfo* newClient) -> void {
        const TurnType modeType = modeManager.currentModeType();
        ModeChangeCommandInfo command(modeType);
        tcpServer.send(command.toBytes(), command.bytesCount());
        Logging::log(LoggingLevel::Debug, "Mode: %s\n", command.toBytes());
    };

    modeManager.roundChangeCallback = [&tcpServer](RoundInfo roundInfo) -> void
    {
        RoundChangeCommandInfo command(roundInfo);
        tcpServer.send(command.toBytes(), command.bytesCount());
        Logging::log(LoggingLevel::Debug, "Round: %s\n", command.toBytes());
    };

    modeManager.modeChangeCallback = [&tcpServer](TurnType mode) -> void
    {
        ModeChangeCommandInfo command(mode);
        tcpServer.send(command.toBytes(), command.bytesCount());
        Logging::log(LoggingLevel::Debug, "Mode: %s\n", command.toBytes());
    };

    // init display
    displayController.displayState()->clear();
    displayController.display();

    if (!tcpServer.start())
    {
        Logging::log(LoggingLevel::Error, "Failed to open server.\n");
        return 1;
    }

    while (true)
    {
        for (int i = 0; i < 20; i++)
        {
            const BaseCommand *command = msgParser.parseMessage();
            if (command != nullptr)
            {
                const MessageType msgType = ((BaseCommand *)command)->type;
                Logging::log("Message: %s\n", messageTypeToString(msgType));
                modeManager.processCommand(command);
            }
            sleep_ms(50);
        }

        if (modeManager.modeDataValid())
        {
            BaseModeData *modeData = modeManager.currentMode();
            modeData->display(displayController.displayState()); // set the display state
            // TODO display on display

            //- LOGGING -----------------------------------------

            Logging::log("%6d: ", currentStep);
            displayController.displayState()->log();
            Logging::log(",  ");
            modeData->log();
            //---------------------------------------------------

            modeData->handleSecTick();
        }
        currentStep++;
    }

    tcpServer.stop();
    WiFi::stop();
    return 0;
}