/***************************************************
*  File TestClient.h
*  Copyright (C) 2021-3-16 ModernCpp.ir . All Rights Reserved.  *
***************************************************/

#ifndef TESTCLIENT_H
#define TESTCLIENT_H

#include <trantor/net/TcpClient.h>


class TestClient : trantor::TcpClient

{

public:
    explicit TestClient(trantor::EventLoop* loop, const trantor::InetAddress& serverAddr,
                        const std::string& nameArg);
};

#endif // TESTCLIENT_H
