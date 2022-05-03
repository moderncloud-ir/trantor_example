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
	trantor::InetAddress server_addr(8500, true);
	std::atomic_uint64_t counter = 0;

	for (size_t i = 0; i < 3; ++i)
	{

		std::thread([&server_addr, &counter] {
			auto event_loop = std::make_shared<trantor::EventLoop>();

			if (!event_loop->isInLoopThread())
			{
				event_loop->moveToCurrentThread();
			}

			const size_t connection_count = 10000;

			std::vector<std::shared_ptr<trantor::TcpClient>> connections(connection_count);

			for (size_t i = 0; i < connection_count; ++i)
			{

				connections[i] = std::make_shared<trantor::TcpClient>(event_loop.get(), server_addr,
																	  "httpClient");

				const auto &tcp_client_ptr = connections[i];

				tcp_client_ptr->setConnectionCallback(
					[&event_loop, &counter](const trantor::TcpConnectionPtr &connPtr) {
						if (connPtr->connected())
						{

							event_loop->runEvery(3, [&, connPtr]() {
								static trantor::MsgBuffer buffer;
								buffer.retrieveAll();
								++counter;
								buffer.appendInt64(counter);
								connPtr->send(std::move(buffer));
							});
						}
						else
						{
							std::cerr << "connection closed " << std::endl;
						}
					});
				tcp_client_ptr->setConnectionErrorCallback(
					[]() { std::cerr << "connection error " << std::endl; });
				tcp_client_ptr->setMessageCallback(
					[](const trantor::TcpConnectionPtr &connPtr, trantor::MsgBuffer *msg) {
						//			++cc;
						//			if (cc > 100)
						//			{
						//				connPtr->shutdown();
						//			}
						auto counter = msg->readInt64();
						//			++counter;
						std::cerr << "data received  " << counter << std::endl;
						//			trantor::MsgBuffer buffer;
						//			buffer.appendInt64(counter);

						//			connPtr->send(std::move(buffer));

						//            connPtr->shutdown();
					});

				tcp_client_ptr->setWriteCompleteCallback(
					[](const trantor::TcpConnectionPtr &connection_ptr) {
						std::clog << "byte sent " << connection_ptr->bytesSent() << std::endl;
					});

				tcp_client_ptr->connect();
			}
			event_loop->loop();
		}).detach();
	}

	std::cin.get();
	return 0;
}
