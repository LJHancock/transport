#pragma once

#include "../../utils/types.h"
#include "../../utils/platform.h"

#if PLATFORM == PLATFORM_WINDOWS
#include <WinSock2.h>
#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#endif

namespace udp_transport
{
	class Address;

	namespace sockets
	{
		static const unsigned int sc_receiveBufferSizeBytes = 300;

		class Socket
		{
		public:
			~Socket();

			bool isOpen() const;

			bool open(uint16 port);
			void close();
			bool send(const Address& addr, const void* data, const int size);
			int  receive(Address& senderOut, void* dataOut, const int size);

		private:
#if PLATFORM == PLATFORM_WINDOWS
			SOCKET m_handle = 0;
#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
			int m_handle;
#endif			
		}; // class Socket

		bool initialiseSockets();
		void shutdownSockets();
	} // namespace sockets
} // namespace udp_transport