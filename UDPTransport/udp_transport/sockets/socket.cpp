#include <stdafx.h>

#include "socket.h"

#include "../../utils/trace.h"
#include "udp_transport/address/address.h"

namespace udp_transport
{
	namespace sockets
	{
		Socket::~Socket()
		{
			if (isOpen())
			{
				close();
			}
		}

		bool Socket::isOpen() const
		{
			return m_handle > 0;			
		}

		bool Socket::open(uint16 port)
		{
			const auto handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			if (handle <= 0)
			{
				UDP_TRACE("Failed to create socket");
				return false;
			}
			else
			{
				sockaddr_in address;
				address.sin_family = AF_INET;
				address.sin_addr.s_addr = INADDR_ANY;
				address.sin_port = htons(port); // Convert to network byte order.

				auto err = bind(handle, (const sockaddr*) &address, sizeof(sockaddr_in));
				if (err < 0)
				{
					UDP_TRACE("Failed to bind socket error %d", err);
					return false;
				}

#if PLATFORM == PLATFORM_WINDOWS
				DWORD nonBlocking = 1;
				err = ioctlsocket(handle, FIONBIO, &nonBlocking);
				if (err != 0)
				{
					UDP_TRACE("Failed to set non-blocking socket. %d", err);
					return false;
				}
#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
				int nonBlocking = 1;
				err = fcntl(handle, F_SETFL, O_NONBLOCK, nonBlocking);
				if (err == -1)
				{
					UDP_TRACE("Failed to set non-blocking socket. %d", err);
					return false;
				}
#endif
			}
			m_handle = handle;
			return true;
		}

		void Socket::close()
		{
#if PLATFORM == PLATFORM_WINDOWS
			closesocket(m_handle);
#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
			close(m_handle);
#endif
		}

		bool Socket::send(const Address& addr, const void* data, const int size)
		{
			sockaddr_in address;
			address.sin_family = AF_INET;
			address.sin_addr.s_addr = htonl(addr.getAddress());
			address.sin_port = htons(addr.getPort());

			int bytesSent = sendto(m_handle, (const char*)data, size, 0, (sockaddr*)&address, sizeof(sockaddr_in));

			if (bytesSent != size)
			{
				UDP_TRACE("failed to send packet of size %d, only sent %d", size, bytesSent);
				return false;
			}
			return true;
		}

		int Socket::receive(Address& sender, void* data, int size)
		{
#if PLATFORM == PLATFORM_WINDOWS
			using socklen_t = int;
#endif
			sockaddr_in from;
			socklen_t fromLength = sizeof(from);
			const int bytesReceived = recvfrom(m_handle, (char*)data, sc_receiveBufferSizeBytes, 0, (sockaddr*)&from, &fromLength);
			uint fromAddr = ntohl(from.sin_addr.s_addr);
			uint16 fromPort = ntohs(from.sin_port);
			sender = Address(fromAddr, fromPort);
			return bytesReceived;
		}

		bool initialiseSockets()
		{
#if PLATFORM == PLATFORM_WINDOWS
			WSADATA wsaData;
			auto err = WSAStartup(MAKEWORD(2, 2), &wsaData);
			UDP_TRACE("InitialiseSockets error %d", err);
			return WSAStartup(MAKEWORD(2, 2), &wsaData) == NO_ERROR;

#else
			return true;
#endif
		} 

		void shutdownSockets()
		{
#if PLATFORM == PLATFORM_WINDOWS
			WSACleanup();
#endif
		}
	} // namespace sockets
} // namespace udp_transport