## **Basic Functionality** 

```cpp
#include <Arduino.h>
#include <Wire.h>

#define EEPROM_I2C_ADDRESS 0x50   // I2C address of EEPROM
#define EEPROM_PAGE_SIZE   64     // EEPROM page size in bytes
#define FLAG_ADDR          0x0000 // EEPROM address reserved for our flag
#define FLAG_VALUE         0xAA   // Value used to mark that data was written
#define BLOCK_ADDR         0x0010 // EEPROM memory start address for our data
#define LENGTH             48     // Number of bytes to write/read

// Print a buffer in hex format (16 bytes per line)
void disp_buf(uint8_t *buf, int len) {
  for (int i = 0; i < len; i++) {
    if (buf[i] < 0x10) Serial.print("0");
    Serial.print(buf[i], HEX);
    Serial.print(" ");
    if ((i + 1) % 16 == 0) {
      Serial.println();
    }
  }
  Serial.println();
}

// Write data to EEPROM with page writes
void writeEEPROM(uint16_t eeAddress, uint8_t *data, uint16_t length) {
  uint16_t bytesWritten = 0;
  while (bytesWritten < length) {
    // Begin transmission and send the 16-bit address (high byte first)
    Wire.beginTransmission(EEPROM_I2C_ADDRESS);
    Wire.write((uint8_t)(eeAddress >> 8));   // High address byte
    Wire.write((uint8_t)(eeAddress & 0xFF));   // Low address byte

    // Write data until page boundary or end of data
    uint8_t pageBytes = 0;
    while (pageBytes < EEPROM_PAGE_SIZE && bytesWritten < length) {
      Wire.write(data[bytesWritten]);
      pageBytes++;
      bytesWritten++;
      eeAddress++;
    }
    Wire.endTransmission();
    
    // Wait for EEPROM to complete the write cycle.
    delay(10);
  }
}

// Read data from EEPROM
void readEEPROM(uint16_t eeAddress, uint8_t *buffer, uint16_t length) {
  uint16_t bytesRead = 0;
  while (bytesRead < length) {
    // Set EEPROM internal address pointer
    Wire.beginTransmission(EEPROM_I2C_ADDRESS);
    Wire.write((uint8_t)(eeAddress >> 8));   // High address byte
    Wire.write((uint8_t)(eeAddress & 0xFF));   // Low address byte
    Wire.endTransmission();

    // Request up to 32 bytes at a time (due to Wire library buffer limit)
    uint8_t bytesToRead = (length - bytesRead) > 32 ? 32 : (length - bytesRead);
    Wire.requestFrom((uint8_t)EEPROM_I2C_ADDRESS, (uint8_t)bytesToRead);

    for (uint8_t i = 0; i < bytesToRead && Wire.available(); i++) {
      buffer[bytesRead++] = Wire.read();
      eeAddress++;
    }
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for the serial monitor to open
  Wire.begin();
  Serial.println("EEPROM test started");

  // Read the flag byte from address 0
  uint8_t flag;
  readEEPROM(FLAG_ADDR, &flag, 1);

  // If the flag is not set, then write our data and mark the flag
  if (flag != FLAG_VALUE) {
    // Prepare the data to write (if the string is shorter than LENGTH, the remaining bytes will be '\0')
    uint8_t writeBuffer[LENGTH] = "HELLO WORLD Charevtore storing ";
    writeEEPROM(BLOCK_ADDR, writeBuffer, LENGTH);
    Serial.println("Data written to EEPROM.");

    // Write the flag value (0xAA) at address 0 to indicate the data has been written
    uint8_t flagValue = FLAG_VALUE;
    writeEEPROM(FLAG_ADDR, &flagValue, 1);
  } else {
    Serial.println("EEPROM already written; skipping write.");
  }
  
  // Read back the data for verification
  uint8_t readBuffer[LENGTH];
  readEEPROM(BLOCK_ADDR, readBuffer, LENGTH);
  Serial.println("Data read from EEPROM:");
  disp_buf(readBuffer, LENGTH);
}

void loop() {
  // Nothing to do in loop
}


```

# **Extended Application: Recording and Playback Demo**

```cpp
#include <Wire.h>
#include <Servo.h>
#include <Arduino.h>

#define EEPROM_I2C_ADDRESS 0x50

int analogPin = 0;
int val = 0;
int readVal = 0;
int maxaddress = 1500;
Servo myservo;

// Function to write to EEPROM
void writeEEPROM(int address, byte val, int i2c_address) {
  Wire.beginTransmission(i2c_address);
  Wire.write((int)(address >> 8));   // MSB
  Wire.write((int)(address & 0xFF));   // LSB
  Wire.write(val);
  Wire.endTransmission();
  delay(5);
}

// Function to read from EEPROM
byte readEEPROM(int address, int i2c_address) {
  byte rcvData = 0xFF;
  Wire.beginTransmission(i2c_address);
  Wire.write((int)(address >> 8));   // MSB
  Wire.write((int)(address & 0xFF));   // LSB
  Wire.endTransmission();
  Wire.requestFrom(i2c_address, 1);
  rcvData = Wire.read();
  return rcvData;
}

// Function to record data (write mode)
void recordData() {
  Serial.println("Start Recording...");
  for (int address = 0; address <= maxaddress; address++) {
    // Read potentiometer value and map to servo angle
    val = map(analogRead(analogPin), 0, 1023, 0, 180);
    myservo.write(val);
    delay(15);
    
    // Write the value to EEPROM
    writeEEPROM(address, val, EEPROM_I2C_ADDRESS);

    // Print address and value to Serial Monitor
    Serial.print("ADDR = ");
    Serial.print(address);
    Serial.print("\t");
    Serial.println(val);
  }
  Serial.println("Recording Finished!");
  delay(1000); // Short pause before prompting again
}

// Function to playback data (read mode)
void playbackData() {
  Serial.println("Begin Playback...");
  for (int address = 0; address <= maxaddress; address++) {
    readVal = readEEPROM(address, EEPROM_I2C_ADDRESS);
    myservo.write(readVal);
    delay(15);
    
    // Print address and value to Serial Monitor
    Serial.print("ADDR = ");
    Serial.print(address);
    Serial.print("\t");
    Serial.println(readVal);
  }
  Serial.println("Playback Finished!");
  delay(1000); // Short pause before prompting again
}

// Function to prompt user for mode selection and return the selected mode
char getModeInput() {
  Serial.println("\nSelect Mode:");
  Serial.println("(R) for Record/Write mode");
  Serial.println("(P) for Playback/Read mode");
  
  // Wait indefinitely until input is available
  while (!Serial.available()) {
    delay(10); // Prevents a busy loop
  }
  
  // Read the user's input
  char mode = Serial.read();
  
  // Clear any extra characters from the Serial buffer
  while (Serial.available()) {
    Serial.read();
  }
  
  // Convert lowercase input to uppercase
  if (mode == 'r') {
    mode = 'R';
  }
  if (mode == 'p') {
    mode = 'P';
  }
  
  return mode;
}

void setup() {
  Wire.begin();
  Serial.begin(9600);
  myservo.attach(9);
  
  // Wait for the Serial Monitor to open (important for some boards)
  while (!Serial) { ; }
  
  Serial.println("Arduino EEPROM and Servo Controller");
}

void loop() {
  // Get the user's mode selection and perform the corresponding operation
  char mode = getModeInput();
  
  if (mode == 'R') {
    recordData();
  } else if (mode == 'P') {
    playbackData();
  } else {
    Serial.println("Invalid input. Please enter 'R' or 'P'.");
  }
  
  // After completing the selected operation, loop() will prompt again.
}
```