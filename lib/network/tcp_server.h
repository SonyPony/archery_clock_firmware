#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <lwip/tcp.h>
#include <functional>
#include <vector>


typedef struct tcp_pcb tcp_pcb_t;
typedef struct pbuf pbuf_t;
class TCPServer;

struct TCPEndpointInfo
{   
    tcp_pcb_t* pcb;  // protocol control block

    TCPEndpointInfo(): pcb(nullptr) {};
};

struct TCPClientInfo: public TCPEndpointInfo
{
    TCPServer* server;

    TCPClientInfo(): server(nullptr) {};
};

class TCPServer 
{
    private:
        TCPEndpointInfo* m_serverInfo;
        uint16_t m_port;
        std::vector<tcp_pcb_t*> m_clients;

    public:
        TCPServer(uint16_t port);
        ~TCPServer();

        void addClient(tcp_pcb_t* clientPcb);
        void removeClient(tcp_pcb_t* clientPcb);
        bool start();
        void stop();
        uint32_t pollTime() const;
        err_t disconnectClient(TCPClientInfo* clientInfo ,tcp_pcb_t* clientPcb);

        TCPEndpointInfo* serverInfo() const;

        // TODO send message
        std::function<void(TCPClientInfo*, tcp_pcb_t*, u16_t)> sentHandler;
        std::function<void(TCPClientInfo*, tcp_pcb_t*, pbuf_t*, err_t)> recvHandler;
        std::function<err_t(TCPClientInfo*, tcp_pcb_t*)> pollHandler;
};

#endif