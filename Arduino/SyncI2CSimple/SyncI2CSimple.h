#ifndef SYNC_I2C_SIMPLE_H
#define SYNC_I2C_SIMPLE_H

#include <Arduino.h>
#include <Wire.h>

class SyncI2CSimple {
public:
    SyncI2CSimple(uint8_t address, bool isMaster);
    void begin();

    void send(bool& variable);
    void send(int& variable);
    void update();

private:
    uint8_t _address;
    bool _isMaster;
    bool* _flagPtr;
    int* _counterPtr;
    uint8_t _lastSentType;  // Added this line

    void sendData(uint8_t type, uint8_t* data, uint8_t length);
    void requestDataFromSlave();

    static void onReceiveService(int byteCount);
    static void onRequestService();
    static SyncI2CSimple* _instance;
};

#endif // SYNC_I2C_SIMPLE_H