#include "LSM303D.h"

//Constructors
LSM303D::LSM303D(){}
LSM303D::LSM303D(int PWR_MODE): PWR_MODE(PWR_MODE){}
LSM303D::LSM303D(int SENS_ADDR, int PWR_MODE) : SENS_ADDR(SENS_ADDR), PWR_MODE(PWR_MODE) {}

void LSM303D::begin(){
    uint8_t conn = readWhoAmI();
    if (conn != WHO_AM_I_EXP){
        Serial.println("Failed to connect to LSM303D (Accs)");
    } else {
        Serial.println("LSM303D connected successfully");
    }
    SET_SENSITIVITY();
    SET_ODR_ACC();
    SET_ODR_MAG();
}

LSM303D_ACC_DATA LSM303D::getACC(void){
    LSM303D_ACC_DATA data;

    Wire.beginTransmission(static_cast<int>(SENS_ADDR));
    Wire.write(0x28 | 0x80);  // Set auto-increment for multiple byte read
    Wire.endTransmission(false);
    Wire.requestFrom(static_cast<int>(SENS_ADDR), 6); // Requesting 6 bytes for all 3 axes (2 bytes each)

    int16_t GyroX = Wire.read() | Wire.read() << 8;
    int16_t GyroY = Wire.read() | Wire.read() << 8;
    int16_t GyroZ = Wire.read() | Wire.read() << 8;

    // Convert raw values to degrees per second using the scale factor
    data.AccX= static_cast<float>(GyroX) * SCALE_FACTOR;
    data.AccY = static_cast<float>(GyroY) * SCALE_FACTOR;
    data.AccZ = static_cast<float>(GyroZ) * SCALE_FACTOR;

    return data;
}

LSM303D_MAG_DATA LSM303D::getMAG(void){
    LSM303D_MAG_DATA data;

    Wire.beginTransmission(static_cast<int>(SENS_ADDR));
    Wire.write(0x08 | 0x80);
    Wire.endTransmission(false);
    Wire.requestFrom(static_cast<int>(SENS_ADDR), 6);

    int16_t MagX = Wire.read() | Wire.read() << 8;
    int16_t MagY = Wire.read() | Wire.read() << 8;
    int16_t MagZ = Wire.read() | Wire.read() << 8;

    data.MagX= static_cast<float>(MagX) * SCALE_FACTOR;
    data.MagY = static_cast<float>(MagY) * SCALE_FACTOR;
    data.MagZ = static_cast<float>(MagZ) * SCALE_FACTOR;

    return data;
}
uint8_t LSM303D::readWhoAmI() {
    Wire.beginTransmission(SENS_ADDR); // Start I2C communication with the device
    Wire.write(WHO_AM_I_ADDR);            // Specify the WHO_AM_I register address
    Wire.endTransmission(false);         // End the transmission but keep the connection active

    Wire.requestFrom(SENS_ADDR, 1);    // Request 1 byte from the device
    if (Wire.available()) {
        return Wire.read();              // Read and return the value from WHO_AM_I register
    }
    return 0;                            // Return 0 if no data is available
}

int LSM303D::registerWrite(int ADDR, uint8_t MESSAGE){
    Wire.beginTransmission(SENS_ADDR);
    Wire.write(ADDR);
    Wire.write(MESSAGE);
    int trans_flag = Wire.endTransmission();
    return trans_flag;
}

void LSM303D::SET_SENSITIVITY(){
    uint8_t ctrlReg2Value = 0x00;
    uint8_t ctrlReg3Value = 0x00;

    switch (config.ACC_SENSITIVITY) {
        case 2:
            ctrlReg2Value = 0x00;  // ±2g
            break;
        case 4:
            ctrlReg2Value = 0x10;  // ±4g
            break;
        case 6:
            ctrlReg2Value = 0x20;  // ±6g
            break;
        case 8:
            ctrlReg2Value = 0x30;  // ±8g
            break;
        case 16:
            ctrlReg2Value = 0x40;  // ±16g
            break;
        default:
            Serial.println("Invalid accelerometer sensitivity setting");
            return;
    }


    switch (config.MAG_SENSITIVITY) {
        case 2:
            ctrlReg3Value = 0x00;  // ±2 gauss
            break;
        case 4:
            ctrlReg3Value = 0x20;  // ±4 gauss
            break;
        case 8:
            ctrlReg3Value = 0x40;  // ±8 gauss
            break;
        case 12:
            ctrlReg3Value = 0x60;  // ±12 gauss
            break;
        default:
            Serial.println("Invalid magnetometer sensitivity setting");
            return;
    }

    
    registerWrite(0x21, ctrlReg2Value);
    registerWrite(0x25, ctrlReg3Value);
}

void LSM303D::SET_ODR_ACC(){
    uint8_t ctrlRegValue = 0x00;
    switch (config.OUTPUT_DATA_RATE_ACC) {
        case 100:
            ctrlRegValue = 0x6F; // BDU enabled
            break;
        case 200:
            ctrlRegValue = 0x7F; // BDU enabled
            break;
        case 400:
            ctrlRegValue = 0x8F; // BDU enabled
            break;
        case 800:
            ctrlRegValue = 0x9F; // BDU enabled
            break;
        case 1600:
            ctrlRegValue = 0xAF; // BDU enabled
            break;
        default:
            Serial.println("Invalid accelerometer output data rate setting");
            return;
    }
    registerWrite(0x20, ctrlRegValue);
}

void LSM303D::SET_ODR_MAG() {
    uint8_t ctrlRegValue = 0x00;

    switch (config.OUTPUT_DATA_RATE_MAG) {
        case 0:
            ctrlRegValue = 0x03;  // Power off mode, equivalent to setting ODR to "Do not use" in your table
            break;
        case 3:
            ctrlRegValue = 0x00;  // 3.125 Hz (MODR2, MODR1, MODR0 = 000)
            break;
        case 6:
            ctrlRegValue = 0x01;  // 6.25 Hz (MODR2, MODR1, MODR0 = 001)
            break;
        case 12:
            ctrlRegValue = 0x02;  // 12.5 Hz (MODR2, MODR1, MODR0 = 010)
            break;
        case 25:
            ctrlRegValue = 0x03;  // 25 Hz (MODR2, MODR1, MODR0 = 011)
            break;
        case 50:
            ctrlRegValue = 0x04;  // 50 Hz (MODR2, MODR1, MODR0 = 100)
            break;
        case 100:
            ctrlRegValue = 0x05;  // 100 Hz (MODR2, MODR1, MODR0 = 101)
            break;
        default:
            Serial.println("Invalid magnetometer output data rate setting");
            return;
    }
    
    registerWrite(0x24, ctrlRegValue);  // Write to CTRL5 register (0x24)
}

