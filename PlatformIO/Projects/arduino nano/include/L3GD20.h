#pragma once
#include <Wire.h>
#include <Arduino.h>

struct L3GD20_GYRO_DATA {
    float x;
    float y;
    float z;
};

class L3GD20 {
    public:
        //Constructors
        L3GD20();
        L3GD20(int PWR_MODE);
        L3GD20(int SENS_ADDR, int PWR_MODE);

        void begin();
        //TODO
        //void setLowPassFilter(float);
        //void setHighPassFilter(float);

        L3GD20_GYRO_DATA getMeasurements(void);
        uint8_t readWhoAmI();
        int registerWrite(int ADDR, uint8_t MESSAGE);

        struct settings {
            int SENSITIVITY = 250;      
        } config;

    private:
        int SENS_ADDR = 0x6B;
        int WHO_AM_I_ADDR = 0x0F;
        int WHO_AM_I_EXP = 0xD4;
        int PWR_MODE = 1;
        float SCALE_FACTOR;

        void SET_SENSITIVITY();
};