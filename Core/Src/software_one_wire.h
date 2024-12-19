#pragma once
#include "gpio.h"
#include "base_one_wire.h"
extern "C" {
#include "dwt_micro_delay.h"
}

#define SW_ONE_WIRE_WAIT_TIMEOUT 90


class SoftwareOneWire: public BaseOneWire {
public:
	SoftwareOneWire(void);
	SoftwareOneWire(GPIO_TypeDef *prt, uint16_t wire_pin);
	void init(GPIO_TypeDef *prt, uint16_t wire_pin);
	void init(void);
	OneWireStatus reset(void);
	uint8_t process_bit(void) override;
	uint8_t process_byte(void) override;
	void transmit_bit(const uint8_t bit) override;
	void transmit_byte(const uint8_t byte) override;

protected:
	GPIO_TypeDef *p_port;
	uint16_t pin;
};
