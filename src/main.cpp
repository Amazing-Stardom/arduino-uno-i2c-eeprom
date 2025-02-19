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
