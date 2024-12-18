#include "mpu6050.h"


#include "mpu6050.h"

MPU6050::MPU6050(){}


void MPU6050::mpu6050_reset() {
    Serial.println("Resetting MPU6050...\n");
    delay(200);
    register_write(REG_PWR_MGMT_1, PWR_SETTING);
    delay(200);
}

void MPU6050::mpu6050_configure() {
    Serial.println("Configuring MPU6050...\n");
    // Set low pass filter frequancies
    register_write(REG_LOW_PASS_FILTER, LOW_PASS_FILTER_VALUE);
    // Set gyroscope range
    register_write(REG_GYRO_CONFIG, GYRO_CONFIG_VALUE);
    // Set accelerometer range
    register_write(REG_ACC_CONFIG, ACCEL_CONFIG_VALUE);
}

void MPU6050::mpu6050_calibrate(){
    MPU6050Data data;
    for (int RateCalibrationNumber=0; RateCalibrationNumber<4000; RateCalibrationNumber ++) {
        mpu6050_read_raw(&data);
        GYRO_OFFSET_X+=data.gyro_dps[0];
        GYRO_OFFSET_Y+=data.gyro_dps[1];
        GYRO_OFFSET_Z+=data.gyro_dps[2];
        delay(1);
    }
    GYRO_OFFSET_X/=4000;
    GYRO_OFFSET_Y/=4000;
    GYRO_OFFSET_Z/=4000;
    Serial.print("OffsetX = ");
    Serial.print(GYRO_OFFSET_X);
    Serial.print("OffsetY = ");
    Serial.print(GYRO_OFFSET_Y);
    Serial.print("OffsetZ = ");
    Serial.println(GYRO_OFFSET_Z);
}


void MPU6050::mpu6050_read_raw(MPU6050Data *data) {
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(REG_ACCEL_XOUT_H);
    Wire.endTransmission();
    Wire.requestFrom(MPU6050_ADDR, 14);
    
    int16_t accX = Wire.read() << 8 | Wire.read();
    int16_t accY = Wire.read() << 8 | Wire.read();
    int16_t accZ = Wire.read() << 8 | Wire.read();

    int16_t temp = Wire.read() << 8 | Wire.read();

    int16_t gyroX = Wire.read() << 8 | Wire.read();
    int16_t gyroY = Wire.read() << 8 | Wire.read();
    int16_t gyroZ = Wire.read() << 8 | Wire.read();

    data->gyro_dps[0] = (float)gyroX / GYRO_SCALE_FACTOR;
    data->gyro_dps[1] = (float)gyroY / GYRO_SCALE_FACTOR;
    data->gyro_dps[2] = (float)gyroZ / GYRO_SCALE_FACTOR;

    data->accel_g[0] = (float)accX / ACCEL_SCALE_FACTOR;
    data->accel_g[1] = (float)accY / ACCEL_SCALE_FACTOR;
    data->accel_g[2] = (float)accZ / ACCEL_SCALE_FACTOR;

    data->temp = (float)temp / 340.00f + 36.53f;
    
    // Convert raw gyroscope values to degrees per second
}

int MPU6050::mpu6050_self_check(){
    Serial.println("Performing self-check...\n");
    Wire.beginTransmission(MPU6050_ADDR); // Start I2C communication with the device
    Wire.write(WHO_AM_I_REG);            // Specify the WHO_AM_I register address
    Wire.endTransmission(false);         // End the transmission but keep the connection active
    Wire.requestFrom(MPU6050_ADDR, 1);    // Request 1 byte from the device
    if (Wire.available()) {
        if (Wire.read() == 0x68) {
            return 1;  // Success, MPU6050 is ready
        } 
        else {
            return 0;  // Failure, incorrect value
        }
    }
    return 0;  // Failure, no data available
}

void MPU6050::register_write(int8_t ADDR, int8_t MESSAGE){
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(ADDR);
    Wire.write(MESSAGE);
    //Wire.endTransmission();

    int8_t status = Wire.endTransmission();  // Capture the transmission status

    if (status == 1) {
    Serial.println("Data too long to fit in transmit buffer.");
    } else if (status == 2) {
        Serial.println("Received NACK on transmit of address.");
    } else if (status == 3) {
        Serial.println("Received NACK on transmit of data.");
    } else if (status == 4) {
        Serial.println("Other error occurred.");
    }
}

