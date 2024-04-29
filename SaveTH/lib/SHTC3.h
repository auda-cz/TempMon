#ifndef SHTC3_H
#define SHTC3_H

#include <cstdint>
#include <array>


class SHTC3 {
public:
    SHTC3(int i2c_bus, uint8_t i2c_addr = 0x70);
    ~SHTC3();
    void reset();
    std::pair<float, float> getMeasurements();
    void setLowPower(bool mode);
    void sleep();
    void wakeup();

private:
    int i2c_handle;
    uint8_t i2c_addr;
    bool low_power;
    bool sleeping;

    uint16_t readCommand(uint16_t command);
    void writeCommand(uint16_t command);
    uint8_t crc8(const std::array<uint8_t, 2>& data);
    void init();

};

#endif // SHTC3_H