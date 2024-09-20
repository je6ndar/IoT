#include "L3GD20.h"
#include "LSM303D.h"
#include <Wire.h>
#include <Arduino.h>



void printGYRO(L3GD20_GYRO_DATA, double offsetX, double offsetY, double offsetZ);
void printMAG(LSM303D_MAG_DATA, double offsetX, double offsetY, double offsetZ);
void printACC(LSM303D_ACC_DATA, double offsetX, double offsetY, double offsetZ);


L3GD20 gyro;           
LSM303D acc;

L3GD20_GYRO_DATA GYRO_DATA;
LSM303D_ACC_DATA ACC_DATA;
LSM303D_MAG_DATA MAG_DATA;


void setup() {
  Serial.begin(115200);
  
  gyro.config.SENSITIVITY = 500;
  Serial.println("Setting up gyroscope....");

  delay(2000);
  
  acc.config.ACC_SENSITIVITY=8;
  acc.config.MAG_SENSITIVITY=4;
  acc.config.OUTPUT_DATA_RATE_ACC=800;
  acc.config.OUTPUT_DATA_RATE_MAG=125;
  Serial.println("Setting up accelerometer....");

  delay(2000);

  Wire.begin();        // Optional: Pass specific SDA, SCL pins if not using default
  Wire.setClock(400000);
  acc.begin();
  gyro.begin();        // Initialize the gyro sensor
}

void loop() {
  
  GYRO_DATA = gyro.getMeasurements();  
  ACC_DATA = acc.getACC();
  MAG_DATA = acc.getMAG();

  //printGYRO(GYRO_DATA, 1, 1, 1);
  printGYRO(GYRO_DATA, 1.2631, -0.9757, -0.1784);
  printACC(ACC_DATA,0,0,0);
  printMAG(MAG_DATA,0,0,0);
  
  delay(1000);
}

void printGYRO(L3GD20_GYRO_DATA GYRO_DATA, double offsetX, double offsetY, double offsetZ){
  // if (typeid(GYRO_DATA)!=typeid(L3GD20_GYRO_DATA)){
  //   Serial.print("WRONG GYRO DATA TYPE");
  //   return;
  // }

  float RateX = GYRO_DATA.x + offsetX; //1.2631
  float RateY = GYRO_DATA.y + offsetY; //-0.9757
  float RateZ = GYRO_DATA.z + offsetZ; //-0.1784
  
  Serial.print("RateX: ");
  Serial.print(RateX);
  Serial.print(" | RateY: ");
  Serial.print(RateY);
  Serial.print(" | RateZ: ");
  Serial.println(RateZ);
  Serial.println();
}

void printMAG(LSM303D_MAG_DATA MAG_DATA, double offsetX = 0, double offsetY = 0, double offsetZ = 0){
  // if (typeid(MAG_DATA)!=typeid(LSM303D_MAG_DATA)){
  //   Serial.print("WRONG MAG DATA TYPE");
  //   return;
  // }
  float MagX = MAG_DATA.MagX + offsetX;
  float MagY = MAG_DATA.MagY + offsetY;
  float MagZ = MAG_DATA.MagZ + offsetZ;

  
  Serial.print("MagX: ");
  Serial.print(MagX);
  Serial.print(" | MagY: ");
  Serial.print(MagY);
  Serial.print(" | MagZ: ");
  Serial.println(MagZ);
  Serial.println();
}

void printACC(LSM303D_ACC_DATA ACC_DATA, double offsetX = 0, double offsetY = 0, double offsetZ = 0){
  // if (typeid(ACC_DATA)!=typeid(LSM303D_ACC_DATA)){
  //   Serial.print("WRONG ACC DATA TYPE");
  //   return;
  // }
  
  float AccX = ACC_DATA.AccX + offsetX;
  float AccY = ACC_DATA.AccY + offsetY;
  float AccZ = ACC_DATA.AccZ + offsetZ;
  
  Serial.print("AccX: ");
  Serial.print(AccX);
  Serial.print(" | AccY: ");
  Serial.print(AccY);
  Serial.print(" | AccZ: ");
  Serial.println(AccZ);
  Serial.println();
}