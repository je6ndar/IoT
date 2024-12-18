#pragma once
#include <Wire.h>
#include <Arduino.h>

struct MPU6050Data {
    float accel_g[3];  // Accelerometer data in g's
    float gyro_dps[3]; // Gyroscope data in degrees per second
    float temp;      // Raw temperature data
};


class MPU6050 {
public:
    // Constructor
    MPU6050();
    // Public methods
    int mpu6050_self_check();
    void mpu6050_reset();
    void mpu6050_configure();
    void mpu6050_calibrate();
    void mpu6050_read_raw(MPU6050Data *data);

    float GYRO_OFFSET_X = 0; 
    float GYRO_OFFSET_Y = 0; 
    float GYRO_OFFSET_Z = 0;

private:
    
    void register_write(int8_t ADDR, int8_t MESSAGE);

    static constexpr int8_t PWR_SETTING = 0x00;

    // I2C defines
    static constexpr int8_t MPU6050_ADDR = 0x68; // MPU6050 sensor address
    // MPU6050 register addresses
    static constexpr int8_t REG_GYRO_CONFIG = 0x1B;
    static constexpr int8_t REG_ACC_CONFIG = 0x1C;
    static constexpr int8_t REG_PWR_MGMT_1 = 0x6B; //PWR managment register
    static constexpr int8_t REG_ACCEL_XOUT_H = 0x3B; //data out starting adress
    static constexpr int8_t REG_LOW_PASS_FILTER = 0x1A;
    static constexpr int8_t WHO_AM_I_REG = 0x75;

    static constexpr int8_t LOW_PASS_FILTER_VALUE = 0x03; //acc 44Hz gyro 42Hz

    // Sensitivity scale factors
    static constexpr float ACCEL_SCALE_FACTOR_2G = 16384.0f;
    static constexpr float ACCEL_SCALE_FACTOR_4G = 8192.0f;
    static constexpr float ACCEL_SCALE_FACTOR_8G = 4096.0f;
    static constexpr float ACCEL_SCALE_FACTOR_16G = 2048.0f;

    static constexpr float GYRO_SCALE_FACTOR_250DPS = 131.0f;
    static constexpr float GYRO_SCALE_FACTOR_500DPS = 65.5f;
    static constexpr float GYRO_SCALE_FACTOR_1000DPS = 32.8f;
    static constexpr float GYRO_SCALE_FACTOR_2000DPS = 16.4f;

    static constexpr float ACCEL_SCALE_FACTOR = ACCEL_SCALE_FACTOR_8G;  // Change this to the desired accelerometer range
    static constexpr float GYRO_SCALE_FACTOR = GYRO_SCALE_FACTOR_500DPS; // Change this to the desired gyroscope range

    // Corresponding configuration values
    //acc: 000__000
    // 2g: 00
    // 4g: 01
    // 8g: 10
    // 16g: 11
    static constexpr int8_t ACCEL_CONFIG_VALUE = 0x10; // ±8g
    //gtro: 000__000
    //250: 00
    //500: 01
    //1000: 10
    //2000: 11
    static constexpr int8_t GYRO_CONFIG_VALUE = 0x08;  // ±500 degrees/sec
    static constexpr int8_t SAMPLE_RATE_DIV = 1;  // Sample rate = 500Hz

};