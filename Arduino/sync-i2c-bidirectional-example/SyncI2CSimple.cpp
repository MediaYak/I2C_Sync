#include "SyncI2CSimple.h"

SyncI2CSimple* SyncI2CSimple::_instance = nullptr;

SyncI2CSimple::SyncI2CSimple(uint8_t address, bool isMaster)
    : _address(address), _isMaster(isMaster), _flagPtr(nullptr), _counterPtr(nullptr), _lastSentType(255) {
    _instance = this;
}

void SyncI2CSimple::begin() {
    if (_isMaster) {
        Wire.begin();
        Serial.println("Master initialized");
    } else {
        Wire.begin(_address);
        Wire.onReceive(onReceiveService);
        Wire.onRequest(onRequestService);
        Serial.println("Slave initialized");
    }
}

void SyncI2CSimple::send(bool& variable) {
    _flagPtr = &variable;
    _lastSentType = 0;
    if (_isMaster) {
        uint8_t data = variable ? 1 : 0;
        sendData(0, &data, 1);
    }
    Serial.print("Prepared to send bool: ");
    Serial.println(variable ? "true" : "false");
}

void SyncI2CSimple::send(int& variable) {
    _counterPtr = &variable;
    _lastSentType = 1;
    if (_isMaster) {
        uint8_t data[2];
        data[0] = (variable >> 8) & 0xFF;
        data[1] = variable & 0xFF;
        sendData(1, data, 2);
    }
    Serial.print("Prepared to send int: ");
    Serial.println(variable);
}

void SyncI2CSimple::sendData(uint8_t type, uint8_t* data, uint8_t length) {
    if (_isMaster) {
        Wire.beginTransmission(_address);
        Wire.write(type);
        Wire.write(data, length);
        Wire.endTransmission();
        Serial.println("Master sent data");
    }
}

void SyncI2CSimple::update() {
    if (_isMaster) {
        requestDataFromSlave();
    }
}

void SyncI2CSimple::requestDataFromSlave() {
    Wire.requestFrom(_address, (uint8_t)3);
    Serial.println("Master requested data from slave");
    if (Wire.available() >= 3) {
        uint8_t type = Wire.read();
        if (type == 0) {
            bool receivedFlag = Wire.read() != 0;
            if (_flagPtr) *_flagPtr = receivedFlag;
            Serial.print("Master received bool: ");
            Serial.println(receivedFlag ? "true" : "false");
        } else if (type == 1) {
            int receivedCounter = (Wire.read() << 8) | Wire.read();
            if (_counterPtr) *_counterPtr = receivedCounter;
            Serial.print("Master received int: ");
            Serial.println(receivedCounter);
        }
    } else {
        Serial.println("Master: Not enough data received from slave");
    }
}

void SyncI2CSimple::onReceiveService(int byteCount) {
    if (_instance && !_instance->_isMaster) {
        Serial.print("Slave received ");
        Serial.print(byteCount);
        Serial.println(" bytes");
        uint8_t type = Wire.read();
        if (type == 0 && Wire.available() >= 1 && _instance->_flagPtr) {
            *(_instance->_flagPtr) = Wire.read() != 0;
            Serial.print("Slave received bool: ");
            Serial.println(*(_instance->_flagPtr) ? "true" : "false");
        } else if (type == 1 && Wire.available() >= 2 && _instance->_counterPtr) {
            *(_instance->_counterPtr) = (Wire.read() << 8) | Wire.read();
            Serial.print("Slave received int: ");
            Serial.println(*(_instance->_counterPtr));
        }
    }
}

void SyncI2CSimple::onRequestService() {
    if (_instance && !_instance->_isMaster) {
        if (_instance->_lastSentType == 0 && _instance->_flagPtr) {
            Wire.write(0);
            Wire.write(*(_instance->_flagPtr) ? 1 : 0);
            Serial.print("Slave sent bool: ");
            Serial.println(*(_instance->_flagPtr) ? "true" : "false");
        } else if (_instance->_lastSentType == 1 && _instance->_counterPtr) {
            Wire.write(1);
            Wire.write((*(_instance->_counterPtr) >> 8) & 0xFF);
            Wire.write(*(_instance->_counterPtr) & 0xFF);
            Serial.print("Slave sent int: ");
            Serial.println(*(_instance->_counterPtr));
        } else {
            Wire.write(255);  // Invalid type
            Wire.write(0);
            Wire.write(0);
            Serial.println("Slave: No data prepared to send");
        }
    }
}