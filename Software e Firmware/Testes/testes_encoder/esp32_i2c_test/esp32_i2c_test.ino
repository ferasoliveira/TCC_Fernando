#include <Wire.h>

#define AS5600_ADDR 0x36

uint8_t read8(uint8_t reg) {
  Wire.beginTransmission(AS5600_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(AS5600_ADDR, 1);
  return Wire.available() ? Wire.read() : 0;
}

uint16_t read16(uint8_t regHigh) {
  Wire.beginTransmission(AS5600_ADDR);
  Wire.write(regHigh);
  Wire.endTransmission(false);
  Wire.requestFrom(AS5600_ADDR, 2);
  uint16_t hi = Wire.available() ? Wire.read() : 0;
  uint16_t lo = Wire.available() ? Wire.read() : 0;
  return (hi << 8) | lo;
}

void setup() {
  Serial.begin(115200);
  Wire.begin(18, 17); // troque pelos seus pinos SDA/SCL
  delay(500);

  Wire.beginTransmission(AS5600_ADDR);
  byte err = Wire.endTransmission();
  Serial.print("I2C erro = ");
  Serial.println(err); // 0 = achou
}

void loop() {
  uint8_t status = read8(0x0B);
  uint16_t angle = read16(0x0E) & 0x0FFF;     // ANGLE
  uint16_t raw   = read16(0x0C) & 0x0FFF;     // RAW ANGLE

  Serial.print("STATUS=0b");
  Serial.print(status, BIN);
  Serial.print("  MD=");
  Serial.print((status >> 5) & 1);
  Serial.print(" ML=");
  Serial.print((status >> 4) & 1);
  Serial.print(" MH=");
  Serial.print((status >> 3) & 1);

  Serial.print("  RAW=");
  Serial.print(raw);
  Serial.print("  ANGLE=");
  Serial.println(angle);

  delay(200);
}