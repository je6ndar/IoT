#include "LSM303D.h"

//Constructors
LSM303D::LSM303D(){}
LSM303D::LSM303D(int PWR_MODE): PWR_MODE(PWR_MODE){}
LSM303D::LSM303D(int SENS_ADDR, int PWR_MODE) : SENS_ADDR(SENS_ADDR), PWR_MODE(PWR_MODE) {}

void LSM303D::begin(){
    uint8_t conn = readWhoAmI();
    if (conn != WHO_AM_I_EXP){
        Serial.println("Failed to connect to LSM303D (Acc)");
    } else {
        Serial.println("LSM303D connected successfully");
    }
    SET_SENSETIVITY();
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

    Wire.requestFrom(static_cast<int>(SENS_ADDR), 1);    // Request 1 byte from the device
    if (Wire.available()) {
        return Wire.read();              // Read and return the value from WHO_AM_I register
    }
    return 0;                            // Return 0 if no data is available
}

int LSM303D::registerWrite(uint16_t ADDR, uint8_t MESSAGE){
    Wire.beginTransmission(SENS_ADDR);
    Wire.write(ADDR);
    Wire.write(MESSAGE);
    int trans_flag = Wire.endTransmission();
    return trans_flag;
}

void LSM303D::SET_SENSETIVITY(){
    uint8_t ctrlReg2Value = 0x00;
    uint8_t ctrlReg3Value = 0x00;

    switch (config.ACC_SENSITIVITY) {
         case 4:
            ctrlReg2Value = 0x4;
            break;
        case 6:
            ctrlReg2Value = 0x8;
            break;
        case 8:
            ctrlReg2Value = 0xC;     
            break;
        case 16:
            ctrlReg2Value = 0x10;     
            break;
        default:
            Serial.println("Invalid accelerometer sensitivity setting");
            return;
    }

        switch (config.MAG_SENSITIVITY) {
         case 2:
            break;
        case 4:
            ctrlReg3Value = 0x10;
            break;
        case 8:
            ctrlReg3Value = 0x20;     
            break;
        case 12:
            ctrlReg3Value = 0x30;     
            break;
        default:
            Serial.println("Invalid accelerometer sensitivity setting");
            return;
    }
    
    registerWrite(0x21, ctrlReg2Value);
    registerWrite(0x25, ctrlReg3Value);
}

void LSM303D::SET_ODR_ACC(){
        uint8_t ctrlRegValue = 0x00;
        switch (config.OUTPUT_DATA_RATE_ACC) {
         case 100:
            ctrlRegValue = 0x67;
            break;
        case 200:
            ctrlRegValue = 0x77;
            break;
        case 400:
            ctrlRegValue = 0x87;     
            break;
        case 800:
            ctrlRegValue = 0x97;     
            break;
        case 1600:
            ctrlRegValue = 0xA7;     
            break;
        default:
            Serial.println("Invalid accelerometer output data rate setting");
            return;
    }
    registerWrite(0x20, ctrlRegValue);
}

void LSM303D::SET_ODR_MAG(){
    uint8_t ctrlRegValue = 0x00;
    switch (config.OUTPUT_DATA_RATE_MAG) {
        case 0:
        ctrlRegValue = 0x18;
        break;
    case 25:
        ctrlRegValue = 0xC;
        break;
    case 50:
        ctrlRegValue = 0x8;     
        break;
    case 125:
        ctrlRegValue = 0x4;     
        break;

    default:
        Serial.println("Invalid megnetometer output data rate setting");
        return;
    }
    registerWrite(0x24, ctrlRegValue);
}
