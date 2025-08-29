# ESP LCD Eyes Project

This project is designed for the ESP32 microcontroller and utilizes the LovyanGFX library to control a display. The main functionality includes displaying text and shapes, as well as animating a circle with changing colors.

## Project Structure

```
esp_lcd_eyes
├── src
│   └── main.cpp        # Main code for the ESP32 project
├── .gitignore          # Specifies files and directories to ignore by Git
└── README.md           # Documentation for the project
```

## Requirements

- ESP32 microcontroller
- LovyanGFX library

## Setup Instructions

1. Clone the repository to your local machine.
2. Open the project in your preferred IDE.
3. Ensure that the LovyanGFX library is installed.
4. Connect the display to the ESP32 according to the pin configuration specified in `main.cpp`.
5. Upload the code to the ESP32.

## Usage

Once the code is uploaded, the display will initialize, and you will see the message "Hello GC9A01!" along with a blue filled circle and a red outline. The circle will animate with changing colors.

## License

This project is licensed under the MIT License.