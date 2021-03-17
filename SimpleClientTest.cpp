/***************************************************
*  File SimpleClientTest.cpp
*  Copyright (C) 2021-3-16 ModernCpp.ir . All Rights Reserved.  *
***************************************************/

#include <iostream>

#include <trantor/net/EventLoop.h>
#include <trantor/net/TcpClient.h>

int main()
{
    std::cout << "Hello Client!" << std::endl;

    auto event_loop = std::make_shared<trantor::EventLoop>();

    if (!event_loop->isInLoopThread())
    {
        event_loop->moveToCurrentThread();
    }
    trantor::InetAddress server_addr(8500);

    auto tcp_client_ptr = std::make_shared<trantor::TcpClient>(event_loop.get(), server_addr,
                                                               "httpClient");

    tcp_client_ptr->setConnectionCallback([](const trantor::TcpConnectionPtr& connPtr) {
        if (connPtr->connected())
        {
            std::string data{"O_o"};
            trantor::MsgBuffer buffer;
            buffer.append(data.c_str(), data.size());

            if (connPtr->connected())
            {
                connPtr->send(std::move(buffer));
            }
        }
        else
        {
            std::cerr << "connection closed " << std::endl;
        }
    });
    tcp_client_ptr->setConnectionErrorCallback(
        []() { std::cerr << "connection error " << std::endl; });
    tcp_client_ptr->setMessageCallback(
        [](const trantor::TcpConnectionPtr& connPtr, trantor::MsgBuffer* msg) {
            std::cerr << "data received  " << msg->read(4096) << std::endl;
            connPtr->shutdown();
        });

    tcp_client_ptr->setWriteCompleteCallback([](const trantor::TcpConnectionPtr& connection_ptr) {
        std::clog << "byte sent " << connection_ptr->bytesSent() << std::endl;
    });

    tcp_client_ptr->connect();

    event_loop->loop();
    return 0;
}
