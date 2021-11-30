/*
Based on various samples and sources from the SlimeVR community!
Added:
-Magnetometer WHOAMI checking
-MPU9255, ICM20948 and ICM20689 support
-Code cleanup
*/

#include "Wire.h"
#include <i2cscan.h>
#include <I2Cdev.h>

//ESP32
//#define PIN_IMU_SDA 21
//#define PIN_IMU_SCL 22
//D1
#define PIN_IMU_SDA D2
#define PIN_IMU_SCL D1
#define ESP8266
//ESP-01
//#define PIN_IMU_SDA 2
//#define PIN_IMU_SCL 0
//#define ESP8266

#define SERIAL_BAUDRATE 115200
#define I2C_SPEED 400000

#define MPU_ADDRESS_AD0_LOW 0x68  // address pin low (GND)
#define MPU_ADDRESS_AD0_HIGH 0x69 // address pin high (VCC)
#define MPU_MAG_ADDRESS 0x0C

#define ICM20948_GENUINE_WHOAMI 234
#define ICM20689_GENUINE_WHOAMI 152
#define MPU9255_GENUINE_WHOAMI 115
#define MPU9250_GENUINE_WHOAMI 113
#define MPU6500_GENUINE_WHOAMI 112
#define MPU6050_GENUINE_WHOAMI 104

#define AK8963_GENUINE_WHOAMI 72
#define AK09916_GENUINE_WHOAMI 9

#define MPU_RA_INT_PIN_CFG 0x37
#define ICM20948_RA_INT_PIN_CFG 0x0F

#define MPU_RA_WHO_AM_I 0x75
#define ICM20948_RA_WHO_AM_I 0x00
#define AK8963_RA_WHO_AM_I 0
#define AK09916_RA_WHO_AM_I 1

//#define MPU_RA_PWR_MGMT_1 0x6B

uint8_t buffer[1] = {0};

void SetBypass(uint8_t addr, uint8_t regaddr, uint8_t bit, uint8_t data)
{
  buffer[0] = 0;
  I2Cdev::writeBit(addr, regaddr, bit, data);  // Enable I2C bypass to check magnetometer (if present)
  I2Cdev::readBit(addr, regaddr, bit, buffer); // Read I2C bypass status
  Serial.printf("[INFO] I2C bypass mode is %s\n", buffer[0] ? "enabled." : "disabled.");
}

void CheckMag(uint8_t addr, uint8_t regaddr, uint8_t genuine, String name)
{
  if (!I2CSCAN::isI2CExist(addr))
  {
    Serial.printf("[ERR] Magnetometer not found on addr 0x%02x\n", addr);
    return;
  }
  Serial.printf("[INFO] Magnetometer was found on addr 0x%02x\n", addr);
  buffer[0] = 0;
  I2Cdev::readByte(addr, regaddr, buffer);
  if (buffer[0] == genuine)
  {
    Serial.print("[OK] Connected Mag is ");
    Serial.print(name);
    Serial.print(" with device ID ");
    Serial.println(buffer[0]);
  }
  else
  {
    Serial.print("[NOK] Connected Mag is unknown with device ID ");
    Serial.println(buffer[0]);
  }
}

void checkMPU(uint8_t addr)
{
  if (!I2CSCAN::isI2CExist(addr))
  {
    Serial.printf("[ERR] Can't find I2C device on addr 0x%02x\n", addr);
  }
  else
  {
    Serial.printf("[INFO] Found I2C device on addr 0x%02x\n", addr);

    //I2Cdev::writeBit(addr, MPU_RA_PWR_MGMT_1, 6, false); // Set sleep disabled

    buffer[0] = 0;
    I2Cdev::readByte(addr, MPU_RA_WHO_AM_I, buffer); // Read WHOAMI register
    auto WhoAmI1 = buffer[0];
    switch (WhoAmI1)
    {
    case ICM20689_GENUINE_WHOAMI:
    {
      Serial.printf("[OK] Connected IMU is ICM-20689 with device ID %d\n", ICM20689_GENUINE_WHOAMI);
      break;
    }
    case MPU9255_GENUINE_WHOAMI:
    {
      Serial.printf("[OK] Connected IMU is MPU-9255 with device ID %d\n", MPU9250_GENUINE_WHOAMI);
      SetBypass(addr, MPU_RA_INT_PIN_CFG, 1, true);
      CheckMag(MPU_MAG_ADDRESS, AK8963_RA_WHO_AM_I, AK8963_GENUINE_WHOAMI, "AK8963");
      SetBypass(addr, MPU_RA_INT_PIN_CFG, 1, false);
      break;
    }
    case MPU9250_GENUINE_WHOAMI:
    {
      Serial.printf("[OK] Connected IMU is MPU-9250 with device ID %d\n", MPU9250_GENUINE_WHOAMI);
      SetBypass(addr, MPU_RA_INT_PIN_CFG, 1, true);
      CheckMag(MPU_MAG_ADDRESS, AK8963_RA_WHO_AM_I, AK8963_GENUINE_WHOAMI, "AK8963");
      SetBypass(addr, MPU_RA_INT_PIN_CFG, 1, false);
      break;
    }
    case MPU6500_GENUINE_WHOAMI:
    {
      Serial.printf("[OK] Connected IMU is MPU-6500 with device ID %d\n", MPU6500_GENUINE_WHOAMI);
      break;
    }
    case MPU6050_GENUINE_WHOAMI:
    {
      Serial.printf("[OK] Connected IMU is MPU-6050 with device ID %d\n", MPU6050_GENUINE_WHOAMI);
      break;
    }
    default:
    {
      Serial.println("[INFO] WHOAMI not found, checking secondary location!");
      if (I2Cdev::readByte(addr, ICM20948_RA_WHO_AM_I, buffer))
      {
        auto WhoAmI2 = buffer[0];
        switch (WhoAmI2)
        {
        case ICM20948_GENUINE_WHOAMI:
        {
          Serial.printf("[OK] Connected IMU is ICM-20948 with device ID %d\n", ICM20948_GENUINE_WHOAMI);
          SetBypass(addr, ICM20948_RA_INT_PIN_CFG, 1, true);
          CheckMag(MPU_MAG_ADDRESS, AK09916_RA_WHO_AM_I, AK09916_GENUINE_WHOAMI, "AK09916");
          SetBypass(addr, ICM20948_RA_INT_PIN_CFG, 1, false);
          break;
        }
        default:
        {
          break;
        }
        }
        Serial.printf("[NOK] Connected IMU is unknown with device ID %d or %d\n", WhoAmI1, WhoAmI2);
      }
      break;
    }
    }
  }
}

void setup()
{
  Serial.begin(SERIAL_BAUDRATE);
  I2CSCAN::clearBus(PIN_IMU_SDA, PIN_IMU_SCL);
  Wire.begin(PIN_IMU_SDA, PIN_IMU_SCL);
#ifdef ESP8266
  Wire.setClockStretchLimit(150000L); // Default stretch limit 150mS
#endif
  Wire.setClock(I2C_SPEED);
}

void loop()
{
  delay(5000);
  Serial.println();
  checkMPU(MPU_ADDRESS_AD0_LOW);
  Serial.println("-----------------------");
  checkMPU(MPU_ADDRESS_AD0_HIGH);
  Serial.println("-----------------------");
}
