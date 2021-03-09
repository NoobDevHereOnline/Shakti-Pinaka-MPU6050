/*MPU6050 SoftI2C lib try 2*/

#include "gpio_i2c.h"
#include<stdint.h>

#define MPU6050_Address     0x68
#define delay               20

//#define mpu_start_output  0x3B
#define mpu_pow_mgmt        0x6B
#define mpu_reset           0x80

//#define mpu_acc_sen       0x10
#define mpu_gyro_config     0x1B
#define mpu_accl_config     0x1C
//#define mpu_gyro_sen      0x08

#define AccX_Out_1          0x3B
#define AccX_Out_2          0x3C
#define AccY_Out_1          0x3D
#define AccY_Out_2          0x3E
#define AccZ_Out_1          0X3F
#define AccZ_Out_2          0x40

#define GyroX_Out_1         0x43
#define GyroX_Out_2         0x44
#define GyroY_Out_1         0x45
#define GyroY_Out_2         0x46
#define GyroZ_Out_1         0x47
#define GyroZ_Out_2         0X48

int AccX1, AccY1, AccZ1, AccX2, AccY2, AccZ2;
int AccX, AccY, AccZ;
int gforceX, gforceY, gforceZ; 

int GyroX1, GyroY1, GyroZ1, GyroX2, GyroY2, GyroZ2;
int GyroX, GyroY, GyroZ;
int RotX, RotY, RotZ;


/*Code To Try - 1*/

int StartMPU(){

    I2cStart(delay);
//    I2cSendSlaveAddress(MPU6050_Address, 0, 200);

    I2c_Write_byte(MPU6050_Address, mpu_pow_mgmt, mpu_reset, delay);
    ReadAckForWrite(delay);
    I2c_Write_byte(MPU6050_Address, mpu_pow_mgmt, 00, delay); // Come out of sleep 
    ReadAckForWrite(delay);
    I2c_Write_byte(MPU6050_Address, mpu_gyro_config, 00, delay); // Gyro sensitivity
    ReadAckForWrite(delay);
    I2c_Write_byte(MPU6050_Address, mpu_accl_config, 00, delay); // Accelerometer sensitivity
    ReadAckForWrite(delay);

    printf("\t Namaskaram... \n");
}

int MPU_Accelerometer(){

    AccX1=I2c_Read_byte(MPU6050_Address, AccX_Out_1, delay);
    AccX2=I2c_Read_byte(MPU6050_Address, AccX_Out_2, delay);
    AccY1=I2c_Read_byte(MPU6050_Address, AccY_Out_1, delay);
    AccY2=I2c_Read_byte(MPU6050_Address, AccY_Out_2, delay);
    AccZ1=I2c_Read_byte(MPU6050_Address, AccZ_Out_1, delay);
    AccZ2=I2c_Read_byte(MPU6050_Address, AccZ_Out_2, delay);

    AccX = (AccX1 <<8 | AccX2);
    AccY = (AccY1 <<8 | AccY2);
    AccZ = (AccZ1 <<8 | AccZ2);

    //printf("\t| AX = %d , AY = %d , AZ = %d | ",AccX,AccY,AccZ);
    
    processAccData();

    printf("\t| GX = %d , GY = %d , GZ = %d | ",gforceX,gforceY,gforceZ);
}


void processAccData(){
    gforceX = (AccX/16384.0);
    gforceY = (AccY/16384.0);
    gforceZ = (AccZ/16384.0);
}


int MPU_Gyro(){
    GyroX1=I2c_Read_byte(MPU6050_Address, GyroX_Out_1, delay);
    GyroX2=I2c_Read_byte(MPU6050_Address, GyroX_Out_2, delay);
    GyroY1=I2c_Read_byte(MPU6050_Address, GyroY_Out_1, delay);
    GyroY2=I2c_Read_byte(MPU6050_Address, GyroY_Out_2, delay);
    GyroZ1=I2c_Read_byte(MPU6050_Address, GyroZ_Out_1, delay);
    GyroZ2=I2c_Read_byte(MPU6050_Address, GyroZ_Out_2, delay);

    GyroX = (GyroX1 << 8 | GyroX2);
    GyroY = (GyroY1 << 8 | GyroY2);
    GyroZ = (GyroZ1 << 8 | GyroZ2);

    //printf("\t| GX = %d , GY = %d , GZ = %d |\n",GyroX, GyroY, GyroZ);

    processGyroData();

    printf("\t| RX = %d , RY = %d , RZ = %d |\n",RotX, RotY, RotZ);
}


void processGyroData(){
    RotX = (GyroX/131.0);
    RotY = (GyroY/131.0);
    RotZ = (GyroZ/131.0);
}


void main(){

    I2cInit();

    StartMPU();

    while(1){
        MPU_Accelerometer();
        MPU_Gyro();
        delay_loop(1000,1000);
    }
}
    
