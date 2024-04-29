#include "SHTC3.h"
#include <pigpio.h>
#include <unistd.h> // For sleep functions
#include <stdexcept>


SHTC3::SHTC3(int i2c_bus, uint8_t i2c_addr) : i2c_addr(i2c_addr), low_power(true), sleeping(true) {
    if (gpioInitialise() < 0)
    {
        throw std::runtime_error("GPIO initialization failed.");
    }
    i2c_handle = i2cOpen(i2c_bus, i2c_addr, 0); // Open I2C device
    reset();
}

SHTC3::~SHTC3() {
    i2cClose(i2c_handle); // Close I2C handle on destruction
    gpioTerminate();
}

void SHTC3::reset() {
    writeCommand(0x805D); // Soft reset command
    usleep(1000); // Wait for reset to complete
    setLowPower(true);
    sleep();
}

std::pair<float, float> SHTC3::getMeasurements() {
    wakeup();
    writeCommand(low_power ? 0x609C : 0x7866); // Select measurement command based on power mode
    usleep(low_power ? 1000 : 13000); // Delay depends on measurement type

    std::array<uint8_t, 6> buffer;
    i2cReadDevice(i2c_handle, reinterpret_cast<char*>(buffer.data()), buffer.size()); // Read 6 bytes from sensor
    uint16_t raw_temp = (buffer[0] << 8) | buffer[1];
    uint16_t raw_humidity = (buffer[3] << 8) | buffer[4];

    if (crc8({buffer[0], buffer[1]}) != buffer[2] || crc8({buffer[3], buffer[4]}) != buffer[5]) {
        throw std::runtime_error("CRC check failed");
    }

    float temperature = -45 + 175 * static_cast<float>(raw_temp) / 65535.0f;
    float humidity = 100 * static_cast<float>(raw_humidity) / 65535.0f;
    
    sleep();
    return {temperature, humidity};
}

uint8_t SHTC3::crc8(const std::array<uint8_t, 2>& data) {
    uint8_t crc = 0xFF;
    for (auto byte : data) {
        crc ^= byte;
        for (int i = 0; i < 8; i++) {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x31;
            else
                crc <<= 1;
        }
    }
    return crc & 0xFF;
}

void SHTC3::writeCommand(uint16_t command) {
    std::array<uint8_t, 2> buffer = {static_cast<uint8_t>(command >> 8), static_cast<uint8_t>(command & 0xFF)};
    i2cWriteDevice(i2c_handle, reinterpret_cast<char*>(buffer.data()), buffer.size());
}

void SHTC3::sleep() {
    if (!sleeping) {
        writeCommand(0xB098); // Sleep command
        sleeping = true;
    }
}

void SHTC3::wakeup() {
    if (sleeping) {
        writeCommand(0x3517); // Wakeup command
        usleep(1000); // Wait for sensor to wake up
        sleeping = false;
    }
}

void SHTC3::setLowPower(bool mode) {
    low_power = mode;
}
