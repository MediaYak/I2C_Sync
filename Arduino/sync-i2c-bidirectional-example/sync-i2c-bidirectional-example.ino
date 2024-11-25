#include "SyncI2CSimple.h"

#define SLAVE_ADDRESS 8
#define IS_MASTER true  // Set to false for the slave device

SyncI2CSimple syncI2C(SLAVE_ADDRESS, IS_MASTER);

bool flag = false;
int counter = 0;

void setup() {
  Serial.begin(115200);  // Higher baud rate for faster debug output
  delay(2000);  // Wait for Serial Monitor to be opened
  
  Serial.println("\n\nStarting SyncI2CSimple Example");
  Serial.println(IS_MASTER ? "Master Device" : "Slave Device");

  syncI2C.begin();

  Serial.println("Commands:");
  Serial.println("f: Toggle flag");
  Serial.println("c: Increment counter");
  Serial.println("b: Send flag (Bool)");
  Serial.println("i: Send counter (Int)");
  Serial.println("p: Print all variables");
}

void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();
    switch (cmd) {
      case 'f':
        flag = !flag;
        Serial.print("Local flag toggled to: ");
        Serial.println(flag ? "true" : "false");
        syncI2C.send(flag);  // Prepare the flag to be sent
        break;
      case 'c':
        counter++;
        Serial.print("Local counter incremented to: ");
        Serial.println(counter);
        syncI2C.send(counter);  // Prepare the counter to be sent
        break;
      case 'b':
        syncI2C.send(flag);
        Serial.println("Flag prepared to send");
        break;
      case 'i':
        syncI2C.send(counter);
        Serial.println("Counter prepared to send");
        break;
      case 'p':
        Serial.println("Current variable values:");
        Serial.print("Flag: ");
        Serial.println(flag ? "true" : "false");
        Serial.print("Counter: ");
        Serial.println(counter);
        break;
    }
  }

  syncI2C.update(); // Call this regularly to handle communication
  delay(10);  // Small delay to prevent Serial buffer overflow
}