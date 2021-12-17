#ifndef WHOAMIVERIFIER_BNO08X_H_
#define WHOAMIVERIFIER_BNO08X_H_

#include "Arduino.h"
#include "I2Cdev.h"

namespace BNO08X
{
    bool IsMatch(uint8_t address);
    bool reset(uint8_t addr);
    bool writePacket(uint8_t addr, uint8_t channel, uint8_t seq, uint8_t length, uint8_t *data);
    bool readHeader(uint8_t addr, uint16_t &length, uint8_t &channel, uint8_t &seq);
    bool readPacket(uint8_t addr, uint8_t length, uint8_t *data);
    bool waitForReceive();

    bool IsMatch(uint8_t address)
    {
        if (address != 0x4A || address != 0x4B)
        {
            return false;
        }
        reset(address);
        byte PIR[2]{0xF9, 0};
        if (writePacket(address, 2, 0, 2, PIR))
        {
            uint16_t length;
            uint8_t channel;
            uint8_t seq;
            while (readHeader(address, length, channel, seq) && length > 0)
            {
                byte data[length];
                if (readPacket(address, length, data) && data[0] == 0xF8 && length == 20)
                {
                    uint8_t SW = data[6];
                    uint8_t sw = data[7];
                    int32_t pid = data[8] | data[9] << 8 | data[10] << 16 | data[11] << 24;
                    int32_t build = data[12] | data[13] << 8 | data[14] << 16 | data[15] << 24;
                    int16_t version = data[16] | data[17] << 8;
                    Serial.print("[BNO08X] BNO data: SW:");
                    Serial.print(SW);
                    Serial.print(".");
                    Serial.print(sw);
                    Serial.print(" Part: ");
                    Serial.print(pid);
                    Serial.print(" Build: ");
                    Serial.print(build);
                    Serial.print(" Patch: ");
                    Serial.print(version);
                    return true;
                }
            }
        }
        return false;
    }

    bool reset(uint8_t addr)
    {
        while (Wire.read() >= 0)
        {
        }
        byte reset[] = {1};
        if (!writePacket(addr, 1, 0, 1, reset))
        {
            return false;
        }
        delay(50);
        uint16_t length;
        uint8_t channel;
        uint8_t seq;
        while (readHeader(addr, length, channel, seq))
        {
            byte data[length];
            readPacket(addr, length, data);
        }
        return true;
    }

    bool writePacket(uint8_t addr, uint8_t channel, uint8_t seq, uint8_t length, uint8_t *data)
    {
        uint8_t packet = length + 4;
        uint8_t status = 0;
        Wire.beginTransmission(addr);
        Wire.write(packet);
        Wire.write((uint8_t)0);
        Wire.write(channel);
        Wire.write(seq);
        for (uint8_t i = 0; i < length; i++)
        {
            Wire.write((uint8_t)data[i]);
        }
        status = Wire.endTransmission();
        return status == 0;
    }

    bool readHeader(uint8_t addr, uint16_t &length, uint8_t &channel, uint8_t &seq)
    {
        byte header[4]{0, 0, 0, 0};
        auto ret = readPacket(addr, 4, header);
        if (ret)
        {
            length = header[0];
            length |= header[1] << 8;
            length &= ~(1 << 15);
            channel = header[2];
            seq = header[3];
        }
        return ret;
    }

    bool readPacket(uint8_t addr, uint8_t length, uint8_t *data)
    {
        Wire.requestFrom(addr, length);
        if (!waitForReceive())
        {
            return false;
        }
        for (uint8_t x = 0; x < length; x++)
        {
            int i = Wire.read();
            if (i >= 0)
            {
                data[x] = (uint8_t)i;
            }
            else
            {
                return false;
            }
        }
        return true;
    }

    bool waitForReceive()
    {
        for (uint8_t i = 0; i < 100; i++)
        {
            if (Wire.available() > 0)
                return true;
            delay(1);
        }
        return false;
    }
}

#endif // WHOAMIVERIFIER_BNO08X_H_