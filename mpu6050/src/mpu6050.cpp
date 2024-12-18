#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdio.h>

#include "i2cManager.h"

// I2C defines
//#define I2C_PORT i2c0 //port namely for I2C HW Block 0
#define MPU6050_ADDR 0x68 //address of the MPU6050 sensor, by def in mpu , p.45
I2CManager i2c;
// MPU6050 register addresses
#define REG_PWR_MGMT_1 0x6B //Power management register used to control sleep modes and clock settings
#define REG_ACCEL_XOUT_H 0x3B //High byte of the accelerometer�s X-axis output data
#define REG_GYRO_CONFIG 0x1B //Register to configure gyroscope sensitivity and range
#define REG_ACCEL_CONFIG 0x1C //Register to configure accelerometer sensitivity and range
//controls the output data rate by dividing the internal sampling rate of the sensor 1 kHz by (1 + number stored)
#define REG_SMPLRT_DIV 0x19 //Sample rate divider register to set the sensor's output data rate
#define WHO_AM_I_REG 0x75 //A register used to verify the identity of the device (returns 0x68 for MPU6050 / stores address)

// Sensitivity scale factors for different ranges
#define ACCEL_SCALE_FACTOR_2G 16384.0  // for �2g
#define ACCEL_SCALE_FACTOR_4G 8192.0   // for �4g
#define ACCEL_SCALE_FACTOR_8G 4096.0   // for �8g
#define ACCEL_SCALE_FACTOR_16G 2048.0  // for �16g

#define GYRO_SCALE_FACTOR_250DPS 131.0    // for �250 degrees per second
#define GYRO_SCALE_FACTOR_500DPS 65.5     // for �500 degrees per second
#define GYRO_SCALE_FACTOR_1000DPS 32.8    // for �1000 degrees per second
#define GY    src/main.cppRO_SCALE_FACTOR_2000DPS 16.4    // for �2000 degrees per second

// Select the desired scale factor
#define ACCEL_SCALE_FACTOR ACCEL_SCALE_FACTOR_4G  // Change this to the desired accelerometer range
#define GYRO_SCALE_FACTOR GYRO_SCALE_FACTOR_250DPS // Change this to the desired gyroscope range

// Corresponding configuration values
//acc: 000__000
// 2g: 00
// 4g: 01
// 8g: 10
// 16g: 11
#define ACCEL_CONFIG_VALUE 0x08  // for �4g programming manual p15 
//gtro: 000__000
//250: 00
//500: 01
//1000: 10
//2000: 11
#define GYRO_CONFIG_VALUE 0x00  // for �250 degrees per second p14     
#define SAMPLE_RATE_DIV 1  // Sample rate = 1kHz / (1 + 1) = 500Hz
i2c_inst_t * I2C_PORT = i2c.I2C_PORT;



void mpu6050_reset() {
    uint8_t reset[] = { REG_PWR_MGMT_1, 0x80 }; //program manual p.40
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, reset, 2, false);
    sleep_ms(200);
    uint8_t wake[] = { REG_PWR_MGMT_1, 0x00 }; //p.41 CYCLE bit better to set 1 (add clock) + TEMP_DIS if dont need temperature set to 1
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, wake, 2, false);
    sleep_ms(200);
}

void mpu6050_configure() {
    // Set accelerometer range
    uint8_t accel_config[] = { REG_ACCEL_CONFIG, ACCEL_CONFIG_VALUE };
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, accel_config, 2, false);

    // Set gyroscope range
    uint8_t gyro_config[] = { REG_GYRO_CONFIG, GYRO_CONFIG_VALUE };
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, gyro_config, 2, false);

    // Set sample rate
    uint8_t sample_rate[] = { REG_SMPLRT_DIV, SAMPLE_RATE_DIV };
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, sample_rate, 2, false);
}

void mpu6050_read_raw(int16_t accel[3], int16_t gyro[3], int16_t* temp) {
    uint8_t buffer[14]; //buffer array for storing result
    uint8_t reg = REG_ACCEL_XOUT_H; //start from address of x hb
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, &reg, 1, true); //tell mpu where to start reading
    i2c_read_blocking(I2C_PORT, MPU6050_ADDR, buffer, 14, false); //read 14bytes starting from x hb address

    accel[0] = (buffer[0] << 8) | buffer[1]; //8bit bus --> hb in buffer[0], lb in buffer[1] --> merge them into 16bit val
    accel[1] = (buffer[2] << 8) | buffer[3]; 
    accel[2] = (buffer[4] << 8) | buffer[5];
    *temp = (buffer[6] << 8) | buffer[7];    //raw temperature reading
    gyro[0] = (buffer[8] << 8) | buffer[9];
    gyro[1] = (buffer[10] << 8) | buffer[11];
    gyro[2] = (buffer[12] << 8) | buffer[13];
}

int main() {
    // Initialize chosen serial port
    stdio_init_all();

    // Initialize I2C
    //i2c_init(I2C_PORT, 400 * 1000); //bus i2c0 with 400kHz freq
    //gpio_set_function(16, GPIO_FUNC_I2C); //pin 4 (SDA)
    //gpio_set_function(17, GPIO_FUNC_I2C); //pin 5 (SCL)
    //gpio_pull_up(4); //ennable internal pull-up res pin 4
    //gpio_pull_up(5); //pull-up pin 5

    // Reset and configure MPU6050
    mpu6050_reset();
    mpu6050_configure();

    //here bascally check if device we want to talk is really mpu or not
    uint8_t who_am_i = 0;
    uint8_t reg = WHO_AM_I_REG; //prog man p.45, contains address of mpu
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, MPU6050_ADDR, &who_am_i, 1, false); //master reads address of mpu, def is 0x68
    printf("MPU6050 WHO_AM_I: 0x%02X\n", who_am_i);

    if (who_am_i != 0x68) {
        printf("MPU6050 not found!\n");
        while (1);
    }

    int16_t accel[3], gyro[3], temp;

    while (1) {
        mpu6050_read_raw(accel, gyro, &temp);

        // Convert raw accelerometer values to g
        float accel_g[3];
        accel_g[0] = accel[0] / ACCEL_SCALE_FACTOR;
        accel_g[1] = accel[1] / ACCEL_SCALE_FACTOR;
        accel_g[2] = accel[2] / ACCEL_SCALE_FACTOR;

        // Convert raw gyroscope values to degrees per second
        float gyro_dps[3];
        gyro_dps[0] = gyro[0] / GYRO_SCALE_FACTOR;
        gyro_dps[1] = gyro[1] / GYRO_SCALE_FACTOR;
        gyro_dps[2] = gyro[2] / GYRO_SCALE_FACTOR;

        // Print converted values
        printf("aX = %.2f g | aY = %.2f g | aZ = %.2f g | gX = %.2f dps | gY = %.2f dps | gZ = %.2f dps | temp = %.2f�C\n",
            accel_g[0], accel_g[1], accel_g[2], gyro_dps[0], gyro_dps[1], gyro_dps[2], temp / 340.00 + 36.53);

        sleep_ms(500); //read every 0.5s, mb more frequently?? bc sample rate is 500Hz --> too much for 0.5s read
    }
}