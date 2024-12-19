#include "software_one_wire.h"


SoftwareOneWire::SoftwareOneWire(void) : p_port{nullptr}, pin{0} {}

SoftwareOneWire::SoftwareOneWire(GPIO_TypeDef *prt, uint16_t wire_pin) : p_port{prt}, pin{wire_pin} {}

void SoftwareOneWire::init(GPIO_TypeDef *prt, uint16_t wire_pin) {
	p_port = prt;
	pin = wire_pin;
	DWT_init();
}

void SoftwareOneWire::init(void) {
	DWT_init();
}

OneWireStatus SoftwareOneWire::reset(void) {
	uint32_t time_stamp_ms;
	HAL_GPIO_WritePin(p_port, pin, static_cast<GPIO_PinState>(0));
	delay_micros(600);
	HAL_GPIO_WritePin(p_port, pin, static_cast<GPIO_PinState>(1));
	delay_micros(2);

	for (uint8_t i = 80; i > 0; i--) {
		if (!HAL_GPIO_ReadPin(p_port, pin)) {
			time_stamp_ms = HAL_GetTick();
			while (!HAL_GPIO_ReadPin(p_port, pin)) {
                // check timeout
                if (HAL_GetTick() - time_stamp_ms > SW_ONE_WIRE_WAIT_TIMEOUT) {
                    return OneWireStatus::ONEWIRE_ERROR; // return, because there are two cycles
                }
			}
			return OneWireStatus::ONEWIRE_OK;
		}
		delay_micros(1);
	}

	return OneWireStatus::ONEWIRE_ERROR;
}

void SoftwareOneWire::transmit_bit(const uint8_t bit) {
	HAL_GPIO_WritePin(p_port, pin, static_cast<GPIO_PinState>(0));

	switch (bit) {
		case 0:
			delay_micros(90);
			HAL_GPIO_WritePin(p_port, pin, static_cast<GPIO_PinState>(1));
			delay_micros(5);
			break;
		case 1:
			delay_micros(5);
			HAL_GPIO_WritePin(p_port, pin, static_cast<GPIO_PinState>(1));
			delay_micros(90);
			break;
		default:
			break;
	}
}

void SoftwareOneWire::transmit_byte(const uint8_t byte) {
	uint8_t bt = byte;
	for (uint8_t i = 0; i < 8; i++) {
		transmit_bit(bt & 0x01);
		bt >>= 1;
	}
}

uint8_t SoftwareOneWire::process_bit(void) {
	HAL_GPIO_WritePin(p_port, pin, static_cast<GPIO_PinState>(0));
	delay_micros(2);
	HAL_GPIO_WritePin(p_port, pin, static_cast<GPIO_PinState>(1));
	delay_micros(5);
	uint8_t lvl = static_cast<uint8_t>(HAL_GPIO_ReadPin(p_port, pin));
	delay_micros(60);
	return lvl;
}

uint8_t SoftwareOneWire::process_byte(void) {
	uint8_t byte = 0;

	for (uint8_t i = 0; i < 8; i++) {
		byte >>= 1;
		if (process_bit()) byte |= 0x80;
	}

	return byte;
}
