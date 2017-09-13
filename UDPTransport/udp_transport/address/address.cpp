#include <stdafx.h>
#include "address.h"

namespace udp_transport
{
	Address::Address(const uint8 a, const uint8 b, const uint8 c, const uint8 d, const uint16 port)
	{
		m_address = (a << 24)
			| (b << 16)
			| (c << 8)
			| d;

		m_port = port;
	}

	Address::Address(const uint address, const uint16 port)
	{
		m_address = address;
		m_port = port;
	}

	uint Address::getAddress() const
	{
		return m_address;
	}

	uint8 Address::getAddressA() const
	{
		return (m_address >> 24) & 255;
	}

	uint8 Address::getAddressB() const
	{
		return (m_address >> 16) & 255;
	}

	uint8 Address::getAddressC() const
	{
		return (m_address >> 8) & 255;
	}

	uint8 Address::getAddressD() const
	{
		return m_address & 255;
	}

	uint16 Address::getPort() const
	{
		return m_port;
	}
}