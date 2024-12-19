#pragma once
#include <stdint.h>


constexpr uint8_t ONEWIRE_BITS_NUM      = 8;

enum class OneWireStatus {
    ONEWIRE_OK      = 0x00,
    ONEWIRE_ERROR   = 0x01,
};

class BaseOneWire {
public:
	virtual OneWireStatus reset(void) = 0;
	virtual uint8_t process_bit(void) = 0;
	virtual uint8_t process_byte(void) = 0;
	virtual void transmit_bit(const uint8_t bit) = 0;
	virtual void transmit_byte(const uint8_t byte) = 0;
};
