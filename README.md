# Vehicle Tracker

A weekend Arduino project that turns your vehicle into a GPS-trackable device. Send an SMS command to receive the vehicle's current location via a Google Maps link.

## Overview

This project uses an Arduino microcontroller with a GPS module and GSM module to create a simple vehicle tracking system. When you send an SMS with the command "WHERE IS MY CAR" to the device, it responds with the vehicle's current GPS coordinates as a clickable Google Maps link.

## Features

- 📍 Real-time GPS location tracking
- 📱 SMS-based command system
- 🗺️ Automatic Google Maps link generation
- ⏱️ GPS fix waiting mechanism (up to 60 seconds)
- 🔄 Continuous GPS monitoring in the background

## Hardware Requirements

- Arduino Uno (or compatible board)
- GPS Module (NEO-6M or similar)
- SIM900 GSM/GPRS Module
- SIM card with active SMS service
- Jumper wires
- Power supply (12V recommended for SIM900)

## Wiring

### GPS Module
- **GPS RX** → Arduino Pin 4
- **GPS TX** → Arduino Pin 3
- **GPS VCC** → 5V
- **GPS GND** → GND

### SIM900 Module
- **SIM900 RX** → Arduino Pin 8
- **SIM900 TX** → Arduino Pin 7
- **SIM900 VCC** → 12V (or appropriate voltage for your module)
- **SIM900 GND** → GND

## Software Requirements

- Arduino IDE
- Libraries:
  - `TinyGPSPlus` (by Mikal Hart)
  - `SoftwareSerial` (built-in)

### Installing Libraries

1. Open Arduino IDE
2. Go to **Sketch → Include Library → Manage Libraries**
3. Search for "TinyGPSPlus" and install it

## Setup Instructions

1. **Install the required libraries** (see above)

2. **Configure your phone number**:
   - Open `vehicle_tracker.ino`
   - Find the line: `const char PHONE[] = "+250xxxxxxx";`
   - Replace `"+250xxxxxxx"` with your phone number (include country code, e.g., `"+1234567890"`)

3. **Upload the code**:
   - Connect your Arduino to your computer
   - Select the correct board and port in Arduino IDE
   - Click Upload

4. **Power up the system**:
   - Ensure the SIM900 module has a valid SIM card inserted
   - Power on the Arduino and modules
   - Wait for the GPS to acquire a satellite fix (this may take a few minutes, especially on first boot)

## Usage

1. **Initial Setup**: After powering on, wait for the GPS to get a fix. The device will continuously monitor GPS data in the background.

2. **Request Location**: Send an SMS to the SIM card number in the device with the text:
   ```
   WHERE IS MY CAR
   ```
   (The command is case-insensitive)

3. **Receive Response**: You'll receive an SMS with:
   - A Google Maps link to the vehicle's location
   - Or a message saying "GPS not fixed yet" if no location is available

## How It Works

1. **GPS Monitoring**: The device continuously reads GPS data and updates the last known location when a valid fix is available.

2. **SMS Monitoring**: The device listens for incoming SMS messages on the SIM900 module.

3. **Command Processing**: When an SMS containing "WHERE IS MY CAR" is received:
   - The device attempts to get a GPS fix (waits up to 60 seconds)
   - If a fix is available, it formats the coordinates into a Google Maps URL
   - Sends the location link back via SMS

4. **Response**: The location is sent as a clickable Google Maps link that opens directly in your phone's maps app.

## Code Structure

- `setup()`: Initializes serial communications and configures the SIM900 module for SMS mode
- `loop()`: Continuously monitors GPS data and checks for incoming SMS messages
- `handleLocationRequest()`: Processes location requests, waits for GPS fix, and sends response
- `sendSMS()`: Helper function to send SMS messages via the SIM900 module

## Troubleshooting

- **No GPS fix**: Ensure the GPS module has a clear view of the sky. First-time fixes can take 5-15 minutes.
- **SMS not received**: Check that the SIM card has credit and SMS service is active.
- **SIM900 not responding**: Verify power supply (SIM900 needs adequate current, typically 2A peak).
- **Wrong location**: Ensure the GPS module has had time to acquire a proper satellite fix before requesting location.

## Future Enhancements

Potential improvements for future versions:
- Multiple command support (e.g., "STATUS", "BATTERY")
- Automatic periodic location updates
- Geofencing alerts
- Data logging to SD card
- Web dashboard integration
- Low-power modes for battery operation

## License

This is a weekend project - feel free to use and modify as needed!

## Author

Built as a weekend project for vehicle tracking and location monitoring.

