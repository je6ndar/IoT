#pragma once
#include <Wire.h>
#include <Arduino.h>

struct LSM303D_ACC_DATA {
    float AccX;
    float AccY;
    float AccZ;

};

struct LSM303D_MAG_DATA {
    float MagX;
    float MagY;
    float MagZ;
};


class LSM303D {
    public:
        //Constructors
        LSM303D();
        LSM303D(int PWR_MODE);
        LSM303D(int SENS_ADDR, int PWR_MODE);

        void begin();
        //TODO
        //void setLowPassFilter(float);
        //void setHighPassFilter(float);

        LSM303D_ACC_DATA getACC(void);
        LSM303D_MAG_DATA getMAG(void);
        uint8_t readWhoAmI();
        int registerWrite(int ADDR, uint8_t MESSAGE);

        struct settings {
            int ACC_SENSITIVITY = 2; //2, 4, 6, 8, 16
            int MAG_SENSITIVITY = 2; //2, 4, 8, 12    
            int OUTPUT_DATA_RATE_ACC = 100; //100, 200, 400, 800, 1600Hz
            int OUTPUT_DATA_RATE_MAG = 0; //0, 12.5(125), 25, 50, 100Hz  
        } config;

    private:
        int SENS_ADDR = 0x1D;
        int WHO_AM_I_ADDR = 0x0F;
        int WHO_AM_I_EXP = 0x49;
        int PWR_MODE = 1;
        float SCALE_FACTOR_ACC = 1;
        float SCALE_FACTOR_MAG = 1;

        void SET_SENSITIVITY();
        void SET_ODR_MAG();
        void SET_ODR_ACC();

};