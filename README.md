
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