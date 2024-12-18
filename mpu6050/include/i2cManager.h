#pragma once
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdio.h>

class I2CManager {
    public:
        I2CManager();
        I2CManager(int SDA_PIN, int SCL_PIN, int FREQUENCY);
        static constexpr i2c_inst_t* I2C_PORT = i2c0;  // I2C hardware block 0

    private:
        int SDA_PIN = 16;
        int SCL_PIN = 17;
        int FREQUENCY = 400; //Hz
        void i2c_start(); 
};




