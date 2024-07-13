#include "tcp_server.h"
#include <lib/logging/logging.h>


static err_t serverSent(void *arg, struct tcp_pcb *pcb, u16_t len)
{
    TCPClientInfo *clientInfo = reinterpret_cast<TCPClientInfo *>(arg);
    Logging::log(LoggingLevel::Debug, "TCP server sent %u bytes.\n", len);

    auto sentHandler = clientInfo->server->sentHandler;
    if (sentHandler != nullptr)
    {
        sentHandler(clientInfo, pcb, len);
    }

    return ERR_OK;
}

err_t serverRecv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
    TCPClientInfo *clientInfo = reinterpret_cast<TCPClientInfo *>(arg);
    if (p == nullptr)
    {
        Logging::log(LoggingLevel::Error, "Invalid buffer.\n");
        return clientInfo->server->disconnectClient(clientInfo, pcb);
    }
    assert(clientInfo != nullptr && clientInfo->pcb == pcb);
    if (p->tot_len > 0)
    {
        auto recvHandler = clientInfo->server->recvHandler;
        if (recvHandler != nullptr)
        {
            recvHandler(clientInfo, pcb, p, err);
        }
        tcp_recved(pcb, p->tot_len);
    }
    pbuf_free(p);
    return ERR_OK;
}

static err_t serverPoll(void *arg, struct tcp_pcb *pcb)
{
    TCPClientInfo *clientInfo = reinterpret_cast<TCPClientInfo *>(arg);

    if (clientInfo->server->pollHandler != nullptr)
    {
        return clientInfo->server->pollHandler(clientInfo, pcb);
    }

    return ERR_OK;
}

static void serverErr(void *arg, err_t err)
{
    TCPClientInfo *clientInfo = reinterpret_cast<TCPClientInfo *>(arg);
    if (err != ERR_ABRT)
    {
        Logging::log(LoggingLevel::Error, "TCP error %d.\n", err);
        clientInfo->server->disconnectClient(clientInfo, clientInfo->pcb);
    }
}

TCPServer::TCPServer(uint16_t port)
{
    this->m_port = port;
    this->m_serverInfo = new TCPEndpointInfo;
    this->sentHandler = nullptr;
    this->recvHandler = nullptr;
    this->pollHandler = nullptr;
}

TCPServer::~TCPServer()
{
    if (this->m_serverInfo == nullptr)
        return;

    if (this->m_serverInfo->pcb)
    {
        tcp_arg(this->m_serverInfo->pcb, nullptr);
        tcp_close(this->m_serverInfo->pcb);
        m_serverInfo->pcb = nullptr;
    }
    delete this->m_serverInfo;
}

void TCPServer::stop()
{
    tcp_pcb_t *serverPcb = this->m_serverInfo->pcb;
    this->m_clients.clear();

    if (serverPcb != nullptr)
    {
        tcp_arg(serverPcb, nullptr);
        tcp_close(serverPcb);
        this->m_serverInfo->pcb = nullptr;
    }
}

err_t TCPServer::disconnectClient(TCPClientInfo *clientInfo, tcp_pcb_t *clientPcb)
{
    if (clientPcb == nullptr)
        return ERR_OK;
    assert(clientInfo != nullptr && clientInfo->pcb == clientPcb);

    tcp_arg(clientPcb, nullptr);
    tcp_poll(clientPcb, nullptr, 0);
    tcp_sent(clientPcb, nullptr);
    tcp_recv(clientPcb, nullptr);
    tcp_err(clientPcb, nullptr);
    err_t err = tcp_close(clientPcb);

    if (err != ERR_OK)
    {
        Logging::log(LoggingLevel::Error, "Close failed %d, calling abort\n", err);
        tcp_abort(clientPcb);
        err = ERR_ABRT;
    }
    if (clientInfo)
    {
        free(clientInfo);
    }
    return ERR_OK;
}

TCPEndpointInfo *TCPServer::serverInfo() const
{
    return this->m_serverInfo;
}

void TCPServer::addClient(tcp_pcb_t *clientPcb)
{
    this->m_clients.push_back(clientPcb);
}

void TCPServer::removeClient(tcp_pcb_t *clientPcb)
{
    for (int i = 0; i < this->m_clients.size(); i++)
    {
        if (this->m_clients[i] == clientPcb)
        {
            this->m_clients.erase(this->m_clients.begin() + i);
            break;
        }
    }
}

uint32_t TCPServer::pollTime() const
{
    return 5; // in seconds
}

static err_t acceptClient(void *arg, struct tcp_pcb *clientPcb, err_t err)
{
    TCPServer *tcpServer = reinterpret_cast<TCPServer *>(arg);
    if (err != ERR_OK || clientPcb == nullptr)
    {
        Logging::log(LoggingLevel::Error, "Failed connecting client.\n");
        return ERR_VAL;
    }

    Logging::log(LoggingLevel::Debug, "Client connected.\n");

    // add pcb to server
    TCPClientInfo *clientInfo = new TCPClientInfo;
    clientInfo->pcb = clientPcb;
    clientInfo->server = tcpServer;

    tcpServer->addClient(clientPcb);

    // setup connection to client
    tcp_arg(clientPcb, clientInfo);
    tcp_sent(clientPcb, serverSent);
    tcp_recv(clientPcb, serverRecv);
    tcp_poll(clientPcb, serverPoll, tcpServer->pollTime() * 2);
    tcp_err(clientPcb, serverErr);

    return ERR_OK;
}

bool TCPServer::start()
{
    TCPEndpointInfo *serverInfo = this->m_serverInfo;
    if (serverInfo == nullptr)
    {
        Logging::log(LoggingLevel::Error, "Invalid server info.\n");
        return false;
    }

    struct tcp_pcb *pcb = tcp_new_ip_type(IPADDR_TYPE_ANY);
    if (pcb == nullptr)
    {
        Logging::log(LoggingLevel::Error, "Failed to create pcb.\n");
        return false;
    }

    err_t err = tcp_bind(pcb, IP_ANY_TYPE, this->m_port);
    if (err)
    {
        Logging::log(LoggingLevel::Error, "Failed to bind to %d port.\n", this->m_port);
        return false;
    }

    this->m_serverInfo->pcb = tcp_listen_with_backlog(pcb, 1);
    if (this->m_serverInfo->pcb == nullptr)
    {
        Logging::log(LoggingLevel::Error, "Failed to start listening.\n");
        if (pcb != nullptr)
        {
            tcp_close(pcb);
        }
        return false;
    }

    tcp_arg(this->m_serverInfo->pcb, this);
    tcp_accept(this->m_serverInfo->pcb, acceptClient);
    return true;
}