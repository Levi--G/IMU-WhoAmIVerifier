#ifndef WHOAMIVERIFIER_IMU_H_
#define WHOAMIVERIFIER_IMU_H_

#include "Arduino.h"

struct Magnetometer
{
    bool HasMag;
    String Name;
    uint8_t Address;
    uint8_t WhoAmI;
    uint8_t WhoAmIRegister;
    bool UseBypass;
    uint8_t BypassRegister;
    uint8_t BypassMask;
    uint8_t BypassValue;
    uint8_t UnBypassRegister;
    uint8_t UnBypassMask;
    uint8_t UnBypassValue;
    Magnetometer() { HasMag = false; }
    Magnetometer(String Name_, uint8_t Address_, uint8_t WhoAmI_, uint8_t WhoAmIRegister_)
    {
        Name = Name_;
        HasMag = true;
        Address = Address_;
        WhoAmI = WhoAmI_;
        WhoAmIRegister = WhoAmIRegister_;
    }
    Magnetometer(String Name_, uint8_t Address_, uint8_t WhoAmI_, uint8_t WhoAmIRegister_, uint8_t BypassRegister_, uint8_t BypassMask_, uint8_t BypassValue_, uint8_t UnBypassRegister_, uint8_t UnBypassMask_, uint8_t UnBypassValue_)
    {
        Name = Name_;
        HasMag = true;
        Address = Address_;
        WhoAmI = WhoAmI_;
        WhoAmIRegister = WhoAmIRegister_;
        UseBypass = true;
        BypassRegister = BypassRegister_;
        BypassMask = BypassMask_;
        BypassValue = BypassValue_;
        UnBypassRegister = UnBypassRegister_;
        UnBypassMask = UnBypassMask_;
        UnBypassValue = UnBypassValue_;
    }
};

struct IMU
{
    String Name;
    uint8_t Address;
    uint8_t AltAddress;
    uint8_t WhoAmI;
    uint8_t WhoAmIRegister;
    Magnetometer Mag;

    IMU() {}

    IMU(String Name_, uint8_t Address_, uint8_t AltAddress_, uint8_t WhoAmI_, uint8_t WhoAmIRegister_)
    {
        Name = Name_;
        Address = Address_;
        AltAddress = AltAddress_;
        WhoAmI = WhoAmI_;
        WhoAmIRegister = WhoAmIRegister_;
    }

    IMU(String Name_, uint8_t Address_, uint8_t AltAddress_, uint8_t WhoAmI_, uint8_t WhoAmIRegister_, Magnetometer Mag_)
    {
        Name = Name_;
        Address = Address_;
        AltAddress = AltAddress_;
        WhoAmI = WhoAmI_;
        WhoAmIRegister = WhoAmIRegister_;
        Mag = Mag_;
    }
};

#endif // WHOAMIVERIFIER_IMU_H_