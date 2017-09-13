#include "stdafx.h"
#include <iostream>
#include "udp_transport/sockets/socket.h"
#include "udp_transport/address/address.h"
#include "utils/trace.h"

int main()
{
	static const uint16 sc_port = 30000;
	const auto initialisedSockets = udp_transport::sockets::initialiseSockets();
	if (initialisedSockets)
	{
		auto socket = udp_transport::sockets::Socket();

		if (!socket.isOpen())
		{
			if (!socket.open(sc_port))
			{
				return 1;
			}
		}

		const char data[] = "Hello world!";
		socket.send(udp_transport::Address(127, 0, 0, 1, sc_port), data, sizeof(data));

		char intData[100] = {};
		for (char i = 0; i < 100; ++i)
		{
			intData[i] = i;
		}
		socket.send(udp_transport::Address(127, 0, 0, 1, sc_port), intData, sizeof(intData));

		uint8 buffer[udp_transport::sockets::sc_receiveBufferSizeBytes];
		while (true)
		{
			auto sender = udp_transport::Address{ 0,0 };
			memset(buffer, 0, sizeof(buffer));
			while (true)
			{
				const auto bytesReceived = socket.receive(sender, buffer, sizeof(buffer));
				if (bytesReceived <= 0)
				{
					break;
				}
				else
				{
					UDP_TRACE("Received data %s", buffer);
				}
			}
		}

	}
	else
	{
		return 1;
	}
    return 0;
}

