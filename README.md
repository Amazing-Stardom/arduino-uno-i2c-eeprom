
# Interface an external EEPROM (e.g., AT24C256) using I2C to store and retrieve configuration data

Design an embedded system for non-volatile storage using the AT24C256 EEPROM via I2C. Key challenges include managing I2C communications (start/stop conditions, device addressing, acknowledgements), handling memory organization (ensuring data is read/written within 64-byte page boundaries), accommodating write cycle delays, and meeting microcontroller resource constraints. The project employs a modular coding approach to ensure reliable EEPROM operations.

In addition to the basic EEPROM interfacing (as shown in the first code example), the project is extended to a practical application that demonstrates recording and playback functionality. In this extended demo, an analog sensor input (from a potentiometer) is used to generate servo positions that are recorded into the external EEPROM. After a recording phase, the stored data is read back to control a servo—effectively playing back the recorded positions. This additional example showcases how the same I2Cbased EEPROM techniques can be applied to dynamic data logging and actuator control, highlighting both the flexibility and real-world applicability of the design.

READ This Article for more information [Link](https://dev.to/ganesh-kumar/eeprom-data-storage-using-i2c-with-arduino-uno-and-at24c256-module-4pke)

- **lib/**: Contains project-specific libraries.
- **include/**: Contains header files for shared declarations.
- **src/**: Contains the main source code for the project.
- **test/**: Contains unit tests for the project.
- **platformio.ini**: Configuration file for PlatformIO.

## Getting Started

### Prerequisites

- [PlatformIO](https://platformio.org/) installed in your IDE (e.g., VSCode).
- An Arduino board (e.g., Arduino Uno).
- Required libraries (automatically managed by PlatformIO).

### Installation

1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd <repository-directory>
   ```

2. Open the project in your IDE with PlatformIO support.

3. Connect your Arduino board to your computer.

### Usage

1. Upload the code to your Arduino board using PlatformIO.
2. Open the Serial Monitor (set to 9600 baud rate).
3. Select the mode:
   - Press `R` to record servo positions.
   - Press `P` to playback recorded positions.

### Functions

- **recordData()**: Records the servo positions based on potentiometer readings and stores them in EEPROM.
- **playbackData()**: Reads the stored positions from EEPROM and moves the servo accordingly.
- **getModeInput()**: Prompts the user to select the mode of operation.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- [Arduino](https://www.arduino.cc/) for the platform.
- [PlatformIO](https://platformio.org/) for the development environment.