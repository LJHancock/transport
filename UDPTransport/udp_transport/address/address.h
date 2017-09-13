#pragma once

#include "../../utils/types.h"

namespace udp_transport
{
	class Address
	{
	public:
		Address(const uint8 a, const uint8 b, const uint8 c, const uint8 d, const uint16 port);
		Address(const uint address, const uint16 port);

		uint getAddress() const;
		uint8 getAddressA() const;
		uint8 getAddressB() const;
		uint8 getAddressC() const;
		uint8 getAddressD() const;
		uint16 getPort() const;

	private:
		uint m_address = 0;
		uint16 m_port = 0;		
	}; // class Address
} // namespace udp_transport