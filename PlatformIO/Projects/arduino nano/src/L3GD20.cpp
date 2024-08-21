#include "L3GD20.h"

//Constructors
L3GD20::L3GD20(){}
L3GD20::L3GD20(int PWR_MODE): PWR_MODE(PWR_MODE){}
L3GD20::L3GD20(int SENS_ADDR, int PWR_MODE) : SENS_ADDR(SENS_ADDR), PWR_MODE(PWR_MODE) {}

void L3GD20::begin(){
    SET_SENSITIVITY();
    uint8_t conn = readWhoAmI();
    if (conn != WHO_AM_I_EXP){
        Serial.println("Failed to connect to L3GD20 (Gyro)");
    } else {
        Serial.println("L3GD20 connected successfully");
    }
}

L3GD20_GYRO_DATA L3GD20::getMeasurements(void){
    L3GD20_GYRO_DATA data;

    Wire.beginTransmission(SENS_ADDR);
    Wire.write(0x28 | 0x80);  // Set auto-increment for multiple byte read
    Wire.endTransmission(false);
    Wire.requestFrom(SENS_ADDR, 6); // Requesting 6 bytes for all 3 axes (2 bytes each)

    int16_t GyroX = Wire.read() | Wire.read() << 8;
    int16_t GyroY = Wire.read() | Wire.read() << 8;
    int16_t GyroZ = Wire.read() | Wire.read() << 8;

    // Convert raw values to degrees per second using the scale factor
    data.x = static_cast<float>(GyroX) * SCALE_FACTOR;
    data.y = static_cast<float>(GyroY) * SCALE_FACTOR;
    data.z = static_cast<float>(GyroZ) * SCALE_FACTOR;

    return data;
}


uint8_t L3GD20::readWhoAmI() {
    Wire.beginTransmission(SENS_ADDR); // Start I2C communication with the device
    Wire.write(WHO_AM_I_ADDR);            // Specify the WHO_AM_I register address
    Wire.endTransmission(false);         // End the transmission but keep the connection active

    Wire.requestFrom(static_cast<int>(SENS_ADDR), 1);    // Request 1 byte from the device
    if (Wire.available()) {
        return Wire.read();              // Read and return the value from WHO_AM_I register
    }
    return 0;                            // Return 0 if no data is available
}

int L3GD20::registerWrite(int ADDR, uint8_t MESSAGE){
    Wire.beginTransmission(SENS_ADDR);
    Wire.write(ADDR);
    Wire.write(MESSAGE);
    int trans_flag = Wire.endTransmission();
    return trans_flag;
}

void L3GD20::SET_SENSITIVITY(){
    uint8_t ctrlReg4Value = 0x00;

    switch (config.SENSITIVITY) {
        case 250:
            //ctrlReg4Value &= ~(0x30);  // Clear bits 4 and 5 (to 00)
            ctrlReg4Value |= 0x00;     // Set bits to 00 for 250 dps
            SCALE_FACTOR = 8.75 / 1000.0;
            break;
        case 500:
            //ctrlReg4Value &= ~(0x30);  // Clear bits 4 and 5
            ctrlReg4Value |= 0x10;     // Set bit 4 to 1 (01) for 500 dps
            SCALE_FACTOR = 17.5 / 1000.0;
            break;
        case 2000:
            //ctrlReg4Value &= ~(0x30);  // Clear bits 4 and 5
            ctrlReg4Value |= 0x20;     // Set bit 5 to 1 (10) for 2000 dps
            SCALE_FACTOR = 70.0 / 1000.0;
            break;
        default:
            Serial.println("Invalid sensitivity setting");
            return;
    }

    // Write to the CTRL_REG4 register
    registerWrite(0x23, ctrlReg4Value);
}



