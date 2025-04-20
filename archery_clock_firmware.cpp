#include <string.h>

#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"

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
#include <lib/peripheral/beeper.h>

// defintition of beeps
constexpr uint32_t BEEP_SIGNAL_FREQ = 7940;         // in Hz
constexpr uint32_t BEEP_DURATION = 33;              // in ms
constexpr uint32_t BEEP_PAUSE_DURATION = 100;       // in ms

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
constexpr uint32_t BEEPER_PIN = 2;

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
    // TODO abstract into app
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

    int currentStep = 0;
    ModeManager modeManager;
    MessageParser msgParser(&messageBuffer);
    ShiftRegister shiftRegister(SR_CLK_PIN, SR_DATA_PIN, SR_CS_PIN);
    DisplayController displayController(&shiftRegister);
    Beeper beeper(BEEPER_PIN, BEEP_SIGNAL_FREQ, BEEP_DURATION, BEEP_PAUSE_DURATION);

    tcpServer.newClientCallback = [&tcpServer, &modeManager](TCPClientInfo* newClient) -> void {
        ModeChangeCommandInfo modeChangeInfo(modeManager.currentModeData());
        tcpServer.send(newClient, modeChangeInfo);

        const BaseModeData* currentMode = modeManager.currentMode();
        if(currentMode == nullptr)
            return;
        
        RoundChangeCommandInfo roundChangeInfo(currentMode->roundInfo());
        PauseChangeCommandInfo pauseChangeInfo(currentMode->paused());
        
        tcpServer.send(newClient, roundChangeInfo);
        tcpServer.send(newClient, pauseChangeInfo);
    };

    modeManager.roundChangeCallback = [&tcpServer](RoundInfo roundInfo) -> void
    {
        RoundChangeCommandInfo roundChangeInfo(roundInfo);
        tcpServer.send(roundChangeInfo);
    };

    modeManager.modeChangeCallback = [&tcpServer, &modeManager](InitializationCommand initData) -> void
    {
        ModeChangeCommandInfo modeChangeInfo(initData);
        tcpServer.send(modeChangeInfo);

        RoundChangeCommandInfo roundChangeInfo(modeManager.currentMode()->roundInfo());
        tcpServer.send(roundChangeInfo);
        
        PauseChangeCommandInfo pauseChangeInfo(modeManager.currentMode()->paused());
        tcpServer.send(pauseChangeInfo);
    };

    modeManager.pausedChangeCallback = [&tcpServer](bool paused) -> void
    {
        PauseChangeCommandInfo pauseChangeInfo(paused);
        tcpServer.send(pauseChangeInfo);
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
        for (int i = 0; i < 20; i++)    // 20 * 50 ->1s ticking
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