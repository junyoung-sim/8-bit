#define SHIFT_DATA 2
#define SHIFT_CLOCK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D7 12
#define WRITE_ENABLE 13

#define HLT 0b1000000000000000 // Clock (HALT)
#define MI  0b0100000000000000 // Memory Address (IN)
#define RI  0b0010000000000000 // RAM (IN)
#define RO  0b0001000000000000 // RAM (OUT)
#define IO  0b0000100000000000 // Instruction Register (OUT)
#define II  0b0000010000000000 // Instruction Register (IN)
#define AI  0b0000001000000000 // Register A (IN)
#define AO  0b0000000100000000 // Register A (OUT)
#define SU  0b0000000010000000 // Subtract
#define EO  0b0000000001000000 // Sum (OUT)
#define BI  0b0000000000100000 // Register B (IN)
#define OUT 0b0000000000010000 // Output
#define CE  0b0000000000001000 // Program Counter (ENABLE)
#define CJ  0b0000000000000100 // Program Counter (JUMP)
#define CO  0b0000000000000010 // Program Counter (OUT)

uint16_t data[] = {
  CO|MI, RO|II|CE, 0,      0,     0,        0,0,0, // NOP (0000)
  CO|MI, RO|II|CE, IO|MI,  RO|AI, 0,        0,0,0, // LDA (0001)
  CO|MI, RO|II|CE, IO|MI,  RO|BI, EO|AI,    0,0,0, // ADD (0010)
  CO|MI, RO|II|CE, IO|MI,  RO|BI, EO|AI|SU, 0,0,0, // SUB (0011)
  CO|MI, RO|II|CE, IO|MI,  AO|RI, 0,        0,0,0, // STA (0100)
  CO|MI, RO|II|CE, IO|AI,  0,     0,        0,0,0, // LDI (0101)
  CO|MI, RO|II|CE, IO|CJ,  0,     0,        0,0,0, // JMP (0110)
  CO|MI, RO|II|CE, 0,      0,     0,        0,0,0, // NOP (0111)
  CO|MI, RO|II|CE, 0,      0,     0,        0,0,0, // NOP (1000)
  CO|MI, RO|II|CE, 0,      0,     0,        0,0,0, // NOP (1001)
  CO|MI, RO|II|CE, 0,      0,     0,        0,0,0, // NOP (1010)
  CO|MI, RO|II|CE, 0,      0,     0,        0,0,0, // NOP (1011)
  CO|MI, RO|II|CE, 0,      0,     0,        0,0,0, // NOP (1100)
  CO|MI, RO|II|CE, 0,      0,     0,        0,0,0, // NOP (1101)
  CO|MI, RO|II|CE, AO|OUT, 0,     0,        0,0,0, // OUT (1110)
  CO|MI, RO|II|CE, HLT,    0,     0,        0,0,0, // HLT (1111)
};

void setLatch() {
  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}

void writeEnable() {
  digitalWrite(WRITE_ENABLE, LOW);
  delayMicroseconds(1);
  digitalWrite(WRITE_ENABLE, HIGH);
  delay(1);
}

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

  Serial.println("Erasing EEPROM...");
  for(int address = 0; address <= 2047; address++)
    writeEEPROM(address, 0x00);

  Serial.println("Programming EEPROM...");
  for(int address = 0; address < sizeof(data) / sizeof(data[0]); address++) {
    writeEEPROM(address, data[address] >> 8); // left
    writeEEPROM(address + 128, data[address]); // right
  }

  dump();
}

void loop() {}
