/***************************************************
*  File SimpleServerTest.cpp
*  Copyright (C) 2021-3-16 ModernCpp.ir . All Rights Reserved.  *
***************************************************/

#include <iostream>

#include <trantor/net/EventLoop.h>
#include <trantor/net/TcpServer.h>

int main()
{
    std::cout << "Hello Server!" << std::endl;

    auto event_loop = std::make_shared<trantor::EventLoop>();

    if (!event_loop->isInLoopThread())
    {
        event_loop->moveToCurrentThread();
    }
    trantor::InetAddress server_addr(8500);

    auto server = std::make_shared<trantor::TcpServer>(event_loop.get(), server_addr, "httpClient");

    server->setConnectionCallback([](const trantor::TcpConnectionPtr& connPtr) {
        std::clog << __LINE__ << __FUNCTION__ << std::endl;
        if (connPtr->connected())
        {
            std::clog << __LINE__ << __FUNCTION__ << std::endl;
            LOG_DEBUG << "new connection from " << connPtr->peerAddr().toIpPort();
            auto size = connPtr->bytesReceived();
            auto buff = connPtr->getRecvBuffer();

            std::clog << "received data : \n" << buff->read(size);

            connPtr->send(*buff);
        }
        else if (connPtr->disconnected())
        {
            std::clog << __LINE__ << __FUNCTION__ << std::endl;
            LOG_DEBUG << "conn disconnected!" << connPtr->peerAddr().toIpPort();
        }
    });
    server->setRecvMessageCallback(
        [](const trantor::TcpConnectionPtr& connPtr, trantor::MsgBuffer* buff) {
            std::clog << __LINE__ << __FUNCTION__ << std::endl;

            auto size = connPtr->bytesReceived();

            std::string data = buff->read(size);

            std::clog << "received data : \n" << data;

            connPtr->send(std::move(data));
        });

    server->start();

    event_loop->loop();
    return 0;
}
