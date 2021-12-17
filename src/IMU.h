#ifndef WHOAMIVERIFIER_IMU_H_
#define WHOAMIVERIFIER_IMU_H_

#include "Arduino.h"

struct Magnetometer
{
    bool HasMag = false;
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
    Magnetometer() {}
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
    bool UseCustom = false;
    bool (*IsMatch)(uint8_t address);
    String SlimeVRSupport;
    String SlimeVRDefine;

    IMU() {}

    IMU(String Name_, uint8_t Address_, uint8_t AltAddress_, uint8_t WhoAmI_, uint8_t WhoAmIRegister_)
    {
        Name = Name_;
        Address = Address_;
        AltAddress = AltAddress_;
        WhoAmI = WhoAmI_;
        WhoAmIRegister = WhoAmIRegister_;
    }

    IMU(String Name_, uint8_t Address_, uint8_t AltAddress_, uint8_t WhoAmI_, uint8_t WhoAmIRegister_, String SlimeVRSupport_, String SlimeVRDefine_)
    {
        Name = Name_;
        Address = Address_;
        AltAddress = AltAddress_;
        WhoAmI = WhoAmI_;
        WhoAmIRegister = WhoAmIRegister_;
        SlimeVRSupport = SlimeVRSupport_;
        SlimeVRDefine = SlimeVRDefine_;
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

    IMU(String Name_, uint8_t Address_, uint8_t AltAddress_, uint8_t WhoAmI_, uint8_t WhoAmIRegister_, String SlimeVRSupport_, String SlimeVRDefine_, Magnetometer Mag_)
    {
        Name = Name_;
        Address = Address_;
        AltAddress = AltAddress_;
        WhoAmI = WhoAmI_;
        WhoAmIRegister = WhoAmIRegister_;
        SlimeVRSupport = SlimeVRSupport_;
        SlimeVRDefine = SlimeVRDefine_;
        Mag = Mag_;
    }

    IMU(String Name_, uint8_t Address_, uint8_t AltAddress_, bool (*IsMatch_)(uint8_t address))
    {
        Name = Name_;
        Address = Address_;
        AltAddress = AltAddress_;
        UseCustom = true;
        IsMatch = IsMatch_;
    }

    IMU(String Name_, uint8_t Address_, uint8_t AltAddress_, String SlimeVRSupport_, String SlimeVRDefine_, bool (*IsMatch_)(uint8_t address))
    {
        Name = Name_;
        Address = Address_;
        AltAddress = AltAddress_;
        SlimeVRSupport = SlimeVRSupport_;
        SlimeVRDefine = SlimeVRDefine_;
        UseCustom = true;
        IsMatch = IsMatch_;
    }
};

#endif // WHOAMIVERIFIER_IMU_H_