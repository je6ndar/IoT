#include "i2cManager.h"

I2CManager::I2CManager(){
    i2c_start();
}

I2CManager::I2CManager(int SDA, int SCL, int freq){
    SDA_PIN = SDA;
    SCL_PIN = SCL;
    FREQUENCY = freq;
    i2c_start();
}

void I2CManager::i2c_start(){
    // Initialize I2C
    i2c_init(I2C_PORT, FREQUENCY * 1000); //bus i2c0 with 400kHz freq
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C); //pin 4 (SDA)
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C); //pin 5 (SCL)
    //gpio_pull_up(4); //ennable internal pull-up res pin 4
    //gpio_pull_up(5); //pull-up pin 5
}