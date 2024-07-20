#define SHIFT_DATA 2
#define SHIFT_CLOCK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D7 12
#define WRITE_ENABLE 13

// 7 segment cathode display
//byte data[16] = {0x7e, 0x30, 0x6d, 0x79, 0x33, 0x5b, 0x5f, 0x70, 0x7f, 0x7b, 0x77, 0x1f, 0x4e, 0x3d, 0x4f, 0x47};
byte digits[10] = {0x7e, 0x30, 0x6d, 0x79, 0x33, 0x5b, 0x5f, 0x70, 0x7f, 0x7b};

// set store register latch when shifting EEPROM address
void setLatch() {
  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}

// pulse write enable when writing to EEPROM
void writeEnable() {
  digitalWrite(WRITE_ENABLE, LOW);
  delayMicroseconds(1);
  digitalWrite(WRITE_ENABLE, HIGH);
  delay(1);
}

// insert EEPROM address to shift register
void setAddress(int address, bool outputEnable) {
  shiftOut(SHIFT_DATA, SHIFT_CLOCK, MSBFIRST, (address >> 8) | (outputEnable ? 0x00 : 0x80));
  shiftOut(SHIFT_DATA, SHIFT_CLOCK, MSBFIRST, address);
  setLatch();
}

byte readEEPROM(int address) {
  setAddress(address, true);
  byte data = 0;
  for(int pin = EEPROM_D7; pin >= EEPROM_D0; pin--) {
    pinMode(pin, INPUT);
    data = (data << 1) + digitalRead(pin);
  }
  return data;
}

void writeEEPROM(int address, byte data) {
  setAddress(address, false);
  for(int pin = EEPROM_D0; pin <= EEPROM_D7; pin++) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, data & 1);
    data >>= 1;
  }
  writeEnable();
}

// display EEPROM contents in hex (16 bytes per row)
void dump() {
  for(int base = 0; base <= 255; base += 16) {
    byte data[16];
    for(int offset = 0; offset <= 15; offset++)
      data[offset] = readEEPROM(base + offset);
    
    char buffer[100];
    sprintf(buffer, "%03x:   %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x",
      base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
      data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);
    
    Serial.println(buffer);
  }
}

void setup() {
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLOCK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  digitalWrite(WRITE_ENABLE, HIGH);
  pinMode(WRITE_ENABLE, OUTPUT);
  Serial.begin(57600);

  // erase existing contents
  Serial.println("Erasing EEPROM...");
  for(int address = 0; address <= 2047; address++)
    writeEEPROM(address, 0x00);

  // output register display instructions
  Serial.println("Writing output register display instructions...");
  int base = 1;
  for(int p = 0; p <= 3; p++) {
    if(p != 0) base *= 10;
    for(int val = 0; val <= 255; val++) {
      if(p < 3) writeEEPROM(val + 256 * p, digits[(val / base) % 10]);
      else writeEEPROM(val + 256 * p, 0x00);
    }
  }
  base = 1;
  for(int p = 4; p <= 7; p++) {
    if(p != 4) base *= 10;
    for(int val = -128; val <= 127; val++) {
      if(p < 7) writeEEPROM((byte)val + 256 * p, digits[(abs(val) / base) % 10]);
      else writeEEPROM((byte)val + 256 * p, (val < 0 ? 0x01 : 0x00));
    }
  }

  dump();
}

void loop() {}
