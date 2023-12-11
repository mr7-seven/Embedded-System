#include <ModbusRtu.h>
#include <SoftwareSerial.h>
SoftwareSerial Slave_1(10, 11);

#define slaveID 12
#define pwmLED  9

uint16_t distMM = 0;
int pwmValue = 0;
unsigned long lastPrint = 0;

// data array for modbus network sharing
uint16_t au16data[4] = {
  slaveID, 225, 8888, 9999
};

Modbus slave(slaveID, Slave_1, 12); // pin 12 and RS-485

void setup() {
  Serial.begin(9600);
  Slave_1.begin(19200);
  slave.start();
  delay(10);
}

void loop() {
  slave.poll( au16data, 4 );
  if (millis() - lastPrint > 200) {
    Serial.print(au16data[0]); Serial.print(":");
    Serial.print(au16data[1]); Serial.print(":");
    Serial.print(au16data[2]); Serial.println();
    lastPrint = millis();
  }
  readSensor(); //for ultrasonic sensor
  actuator();
}

void readSensor() {
  distMM = random(1, 100);
  au16data[1] = distMM; //data to be sent to master device
}
void actuator() {
  pwmValue = au16data[2]*0.25;
  analogWrite(pwmLED, pwmValue);
}
