/*
Based on various samples and sources from the SlimeVR community!
*/

#include "Arduino.h"
#include "Wire.h"
#include <i2cscan.h>
#include <I2Cdev.h>
#include "IMU.h"
#include "BNO08x.h"
#include "config.h"

#define arrlen(x) (sizeof(x) / sizeof(x[0]))

IMU IMUs[]{
    //Invensense
    IMU("MPU6050", 0x68, 0x69, 0x68, 0x75, "SUPPORTED", "IMU_MPU6050"),
    IMU("MPU6500", 0x68, 0x69, 0x70, 0x75, "SUPPORTED", "IMU_MPU6500"),
    IMU("MPU9250", 0x68, 0x69, 0x71, 0x75, "SUPPORTED without Mag (WIP)", "IMU_MPU6050", Magnetometer("AK8963", 0x0C, 0x48, 0x00, 0x37, 0x02, 0xFF, 0x37, 0x02, 0x00)),
    IMU("MPU9255", 0x68, 0x69, 0x73, 0x75, "SUPPORTED without Mag (WIP)", "IMU_MPU6050", Magnetometer("AK8963", 0x0C, 0x48, 0x00, 0x37, 0x02, 0xFF, 0x37, 0x02, 0)),
    IMU("ICM-20600", 0x68, 0x69, 0x11, 0x75),
    IMU("ICM-20601", 0x68, 0x69, 0xAC, 0x75),
    IMU("ICM-20602", 0x68, 0x69, 0x12, 0x75),
    IMU("ICM-20608-G", 0x68, 0x69, 0xAF, 0x75),
    IMU("ICM-20609", 0x68, 0x69, 0xA6, 0x75),
    IMU("ICM-20648", 0x68, 0x69, 0xE0, 0x00),
    IMU("ICM-20649", 0x68, 0x69, 0xE1, 0x00),
    IMU("ICM-20689", 0x68, 0x69, 0x98, 0x75, "NOT SUPPORTED YET but is WIP ;)", ""),
    IMU("ICM-20690", 0x68, 0x69, 0x20, 0x75),
    IMU("ICM-20948", 0x68, 0x69, 0xEA, 0x00, "NOT SUPPORTED YET but is WIP ;)", "", Magnetometer("AK09916", 0x0C, 0x9, 0x01, 0x0F, 0x02, 0xFF, 0x0F, 0x02, 0x00)),
    IMU("ICM-40627", 0x68, 0x69, 0x4E, 0x75),
    IMU("ICM-42605", 0x68, 0x69, 0x42, 0x75),
    IMU("ICM-42670-P", 0x68, 0x69, 0x67, 0x75),
    IMU("ICM-42688-V", 0x68, 0x69, 0xDB, 0x75),
    IMU("ICM-42688-P", 0x68, 0x69, 0x47, 0x75),
    //Special?
    IMU("ICG-20660/L", 0x68, 0x69, 0x91, 0x75),
    IMU("IAM-20380", 0x68, 0x69, 0xB5, 0x75),
    IMU("IAM-20680", 0x68, 0x69, 0xA9, 0x75),
    IMU("IAM-20680HP", 0x68, 0x69, 0xF8, 0x75),
    IMU("IIM-42652", 0x68, 0x69, 0x6F, 0x75),
    //IMU("ICM-20603", 0x68, 0x69, 0x68, 0x75), PB only

    //Bosch
    IMU("BNO055", 0x29, 0x28, 0xA0, 0x00, "SUPPORTED", "IMU_BNO055"),
    IMU("BNO08x", 0x4A, 0x4B, "SUPPORTED", "IMU_BNO080 or IMU_BNO085 or IMU_BNO086 (choose the right one)", BNO08X::IsMatch),

    //Mags
    IMU("QMC5883L", 0x0D, 0x0D, 0xFF, 0x0D), //To be verified
    IMU("HMC5883L", 0x3D, 0x3C, 0x48, 0x0C), //To be verified, 0x0D=0x34, 0x0E=0x33 ASCII=H43

};

uint8_t Addresses[]{0x68, 0x69, 0x29, 0x28, 0x0D, 0x3C, 0x3D};
uint8_t Registers[]{0x75, 0x00, 0x0D, 0x0C};

uint8_t buffer[1] = {0};

bool SetBypass(uint8_t addr, uint8_t regaddr, uint8_t mask, uint8_t data)
{
  buffer[0] = 0;
  I2Cdev::readByte(addr, regaddr, buffer);
  auto exist = buffer[0];
  exist |= (mask & data);
  exist &= (~mask | data);
  I2Cdev::writeByte(addr, regaddr, exist);
  buffer[0] = 0;
  I2Cdev::readByte(addr, regaddr, buffer);
  exist = buffer[0];
  return (exist & mask) == (data & mask);
}

uint8_t ReadReg(uint8_t addr, uint8_t regaddr)
{
  buffer[0] = 0;
  I2Cdev::readByte(addr, regaddr, buffer);
  return buffer[0];
}

void PrintHex(uint8_t hex)
{
  if (hex < 0x0F)
  {
    Serial.print("0x0");
  }
  else
  {
    Serial.print("0x");
  }
  Serial.print(hex, 16);
}

void PrintHexln(uint8_t hex)
{
  PrintHex(hex);
  Serial.println();
}

void setup()
{
  Serial.begin(SERIAL_BAUDRATE);
  I2CSCAN::clearBus(PIN_IMU_SDA, PIN_IMU_SCL);
  Wire.begin(PIN_IMU_SDA, PIN_IMU_SCL);
  Wire.setClock(I2C_SPEED);
  //Get rid of garbo
  Serial.println();
  Serial.println();

  Serial.println("--IMU-WhoAmIVerifier---");
  Serial.println("Supported chips:");

  for (uint8_t i = 0; i < arrlen(IMUs); i++)
  {
    auto imu = IMUs[i];
    Serial.print(imu.Name);
    if (imu.Mag.HasMag)
    {
      Serial.print(" + ");
      Serial.print(imu.Mag.Name);
    }
    Serial.println();
  }
}

void loop()
{
  Serial.println("----------");
#if INFO_LOGGING
  Serial.println("Scanning...");
#endif
  bool foundI2C = false;
  for (uint8_t a = 0; a < arrlen(Addresses); a++)
  {
    auto addr = Addresses[a];
    if (I2CSCAN::isI2CExist(addr))
    {
      foundI2C = true;
      bool found = false;
      for (uint8_t r = 0; r < arrlen(Registers); r++)
      {
        auto reg = Registers[r];
        auto regval = ReadReg(addr, reg);
        for (uint8_t i = 0; i < arrlen(IMUs); i++)
        {
          auto imu = IMUs[i];
          if ((imu.Address == addr || imu.AltAddress == addr) && imu.WhoAmIRegister == reg && imu.WhoAmI == regval)
          {
            found = true;
            Serial.print("[IMU] Found IMU is ");
            Serial.print(imu.Name);
            Serial.print(" with device ID ");
            PrintHex(imu.WhoAmI);
            Serial.print(" on ");
            Serial.print(imu.Address == addr ? "(Primary)" : "(Secondary)");
            Serial.print(" address ");
            PrintHexln(addr);
            if (imu.Mag.HasMag)
            {
              if (imu.Mag.UseBypass)
              {
                if (SetBypass(addr, imu.Mag.BypassRegister, imu.Mag.BypassMask, imu.Mag.BypassValue))
                {
#if INFO_LOGGING
                  Serial.print("[");
                  Serial.print(imu.Name);
                  Serial.println("] I2C bypass mode is enabled");
#endif
                }
                else
                {
                  Serial.print(F("[ERR] Unable to enable bypass!"));
                }
              }
              if (I2CSCAN::isI2CExist(imu.Mag.Address))
              {
                auto magwho = ReadReg(imu.Mag.Address, imu.Mag.WhoAmIRegister);
                if (magwho == imu.Mag.WhoAmI)
                {
                  Serial.print("[MAG] Found Magnetometer is ");
                  Serial.print(imu.Mag.Name);
                  Serial.print(" with device ID ");
                  PrintHex(imu.Mag.WhoAmI);
                  Serial.print(" on address ");
                  PrintHexln(imu.Mag.Address);
                }
                else
                {
                  Serial.print(F("[ERR] Unknown magnetometer with device ID "));
                  PrintHex(magwho);
                  Serial.print(F(" was found on addr "));
                  PrintHexln(imu.Mag.Address);
                }
              }
              else
              {
                Serial.print(F("[ERR] Magnetometer not found on addr "));
                PrintHexln(imu.Mag.Address);
              }
              if (imu.Mag.UseBypass)
              {
                if (SetBypass(addr, imu.Mag.BypassRegister, imu.Mag.UnBypassMask, imu.Mag.UnBypassValue))
                {
#if INFO_LOGGING
                  Serial.print("[");
                  Serial.print(imu.Name);
                  Serial.println("] I2C bypass mode is disabled");
#endif
                }
                else
                {
                  Serial.print(F("[ERR] Unable to disable bypass!"));
                }
              }
            }
#if SLIMEVR_SUPPORT
            Serial.print("[SlimeVR] SlimeVR with this IMU is: ");
            if (!imu.SlimeVRSupport.isEmpty())
            {
              Serial.println(imu.SlimeVRSupport);
              if (!imu.SlimeVRDefine.isEmpty())
              {
                Serial.print("[SlimeVR] Use this in your defines.h: #define IMU ");
                Serial.println(imu.SlimeVRDefine);
              }
            }
            else{
                Serial.println("NOT SUPPORTED :c");
            }
#endif
          }
        }
      }
      if (!found)
      {
        Serial.print(F("[ERR] Found unknown I2C device on address "));
        PrintHex(addr);
        Serial.println(" with possible id's:");
        for (uint8_t r = 0; r < arrlen(Registers); r++)
        {
          auto reg = Registers[r];
          auto regval = ReadReg(addr, reg);
          Serial.print("Reg: ");
          PrintHex(reg);
          Serial.print(" Value:");
          PrintHexln(regval);
        }
        Serial.println(F("Please report this on https://github.com/Levi--G/IMU-WhoAmIVerifier/issues with information above. :)"));
      }
    }
  }
  if (!foundI2C)
  {
    Serial.println(F("Nothing found on the I2C bus, check connections!"));
  }
  delay(5000);
}
