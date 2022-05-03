/***************************************************
*  File SimpleServerTest.cpp
*  Copyright (C) 2021-3-16 ModernCpp.ir . All Rights Reserved.  *
***************************************************/

#include <trantor/net/EventLoop.h>
#include <trantor/net/TcpServer.h>

#include <chrono>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

size_t counter;

size_t connection_count = 30000;

int main()
{
	auto start = std::chrono::high_resolution_clock::now();
	auto last_time = std::chrono::high_resolution_clock::now();

	std::cout << "Hello Server!" << std::endl;

	auto event_loop = std::make_shared<trantor::EventLoop>();

	if (!event_loop->isInLoopThread())
	{
		event_loop->moveToCurrentThread();
	}

	trantor::InetAddress server_addr("0.0.0.0", 8500);

	auto server = std::make_shared<trantor::TcpServer>(event_loop.get(), server_addr, "httpClient");

	server->setIoLoopNum(8);

	server->setConnectionCallback([](const trantor::TcpConnectionPtr &connPtr) {
		//        std::clog << __LINE__ << __FUNCTION__ << std::endl;
		if (connPtr->connected())
		{
			//            std::clog << __LINE__ << __FUNCTION__ << std::endl;
			//            LOG_DEBUG << "new connection from " << connPtr->peerAddr().toIpPort();
			//			auto size = connPtr->bytesReceived();
			//			auto buff = connPtr->getRecvBuffer();

			//            std::clog << "received data : \n" << buff->read(size);

			//			connPtr->send(*buff);
		}
		else if (connPtr->disconnected())
		{
			//            std::clog << __LINE__ << __FUNCTION__ << std::endl;
			std::clog << counter << " message count ping/pong" << std::endl;
			LOG_DEBUG << "conn disconnected!" << connPtr->peerAddr().toIpPort();
		}
	});
	server->setRecvMessageCallback(
		[&start, &last_time](const trantor::TcpConnectionPtr &connPtr, trantor::MsgBuffer *buff) {
			//            std::clog << __LINE__ << __FUNCTION__ << std::endl;

			counter = buff->readInt64();

			if (counter % connection_count == 0)
			{

				// record end time
				auto now = std::chrono::high_resolution_clock::now();

				std::cout << "one iterate time: "
						  << std::chrono::duration<double>(now - last_time).count() << " s"
						  << std::endl;
				std::cout << "overall time: " << std::chrono::duration<double>(now - start).count()
						  << " s" << std::endl;

				last_time = now;

				std::clog << counter << std::endl;
			}

			trantor::MsgBuffer buffer;
			buffer.appendInt64(counter);

			//			connPtr->send(std::move(buffer));
		});

	server->start();

	event_loop->loop();
	return 0;
}
