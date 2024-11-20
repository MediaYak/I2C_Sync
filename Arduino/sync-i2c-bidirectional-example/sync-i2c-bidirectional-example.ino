#include <SyncI2CSimple.h>

#define SLAVE_ADDRESS 7
#define IS_MASTER true  // Set to false for the slave device

SyncI2CSimple syncI2C(SLAVE_ADDRESS, IS_MASTER);

bool flag = false;
int counter = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  Serial.println(IS_MASTER ? "Master Device" : "Slave Device");

  syncI2C.begin();
  syncI2C.send(flag);
  syncI2C.send(counter);

  Serial.println("Commands: f (toggle flag), c (increment counter), p (print values)");
}

void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();
    switch (cmd) {
      case 'f':
        flag = !flag;
        syncI2C.send(flag);
        break;
      case 'c':
        counter++;
        syncI2C.send(counter);
        break;
      case 'p':
        Serial.print("Flag: ");
        Serial.print(flag ? "true" : "false");
        Serial.print(", Counter: ");
        Serial.println(counter);
        break;
    }
  }

  syncI2C.update();
  delay(10);
}
