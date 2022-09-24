#include "mcp23xxx.h"
#include "bus_register.h"
#include "bus_register_bits.h"
#include <stdio.h>

MCP23XXX::~MCP23XXX()
{
    if(i2c_dev)
    {
        delete i2c_dev;
    }
}

bool MCP23XXX::begin(i2c_inst_t *i2cInst, uint8_t addr)
{
    i2c_dev = new I2CDevice(addr, i2cInst);
    return i2c_dev->begin();
}

void MCP23XXX::gpio_set_dir(uint8_t gpio, bool out)
{
    BusRegister IODIR(i2c_dev, getRegister(MCP23XXX_IODIR, MCP_PORT(gpio)));
    BusRegisterBits dir_bit(&IODIR, 1, gpio % 8);

    dir_bit.write(out == true ? 0 : 1);
}

void MCP23XXX::gpio_pull_up(uint8_t gpio)
{
    BusRegister GPPU(i2c_dev, getRegister(MCP23XXX_GPPU, MCP_PORT(gpio)));
    BusRegisterBits pullup_bit(&GPPU, 1, gpio % 8);

    pullup_bit.write(1);
}

void MCP23XXX::gpio_pull_down(uint8_t gpio)
{
    BusRegister GPPU(i2c_dev, getRegister(MCP23XXX_GPPU, MCP_PORT(gpio)));
    BusRegisterBits pullup_bit(&GPPU, 1, gpio % 8);

    pullup_bit.write(0);
}

bool MCP23XXX::gpio_get(uint8_t gpio)
{
    BusRegister GPIO(i2c_dev, getRegister(MCP23XXX_GPIO, MCP_PORT(gpio)));
    BusRegisterBits pin_bit(&GPIO, 1, gpio % 8);

    return ((pin_bit.read() == 0) ? false : true);
}

void MCP23XXX::gpio_put(uint8_t gpio, bool value)
{
    BusRegister GPIO(i2c_dev, getRegister(MCP23XXX_GPIO, MCP_PORT(gpio)));
    BusRegisterBits pin_bit(&GPIO, 1, gpio % 8);

    pin_bit.write((value == false) ? 0 : 1);
}


uint8_t MCP23XXX::gpio_get_all(uint8_t port)
{
  BusRegister GPIO(i2c_dev, getRegister(MCP23XXX_GPIO, port));
  return GPIO.read() & 0xFF;
}


void MCP23XXX::gpio_put_all(uint8_t value, uint8_t port)
{
  BusRegister GPIO(i2c_dev, getRegister(MCP23XXX_GPIO, port));
  GPIO.write(value);
}

uint16_t MCP23XXX::getRegister(uint8_t baseAddress, uint8_t port)
{
    // MCP23x08
    uint16_t reg = baseAddress;
    // MCP23x17 BANK=0
    if (pinCount > 8)
    {
        reg *= 2;
        // Port B
        if (port)
        {
            reg++;
        }
    }

    return reg;
}