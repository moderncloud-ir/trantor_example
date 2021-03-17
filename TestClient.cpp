/***************************************************
*  File TestClient.cpp
*  Copyright (C) 2021-3-16 ModernCpp.ir . All Rights Reserved.  *
***************************************************/

#include "TestClient.h"

TestClient::TestClient(trantor::EventLoop* loop, const trantor::InetAddress& serverAddr,
                       const std::string& nameArg)
    : trantor::TcpClient(loop, serverAddr, nameArg)
{}
