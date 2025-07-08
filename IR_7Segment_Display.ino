#include <HardwareSerial.h>

// UART setup
HardwareSerial IRSerial(2);
#define IR_RXD 16
#define IR_TXD 17

// Segment pins
const int segPins[7] = {23, 22, 21, 19, 18, 5, 4}; // A B C D E F G

// Hex-to-segment map for 0–F (common cathode)
const byte digitMap[16] = {
  //GFEDCBA
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111, // 9
  0b01110111, // A
  0b01111100, // b
  0b00111001, // C
  0b01011110, // d
  0b01111001, // E
  0b01110001  // F
};

void setup() {
  Serial.begin(115200);
  IRSerial.begin(9600, SERIAL_8N1, IR_RXD, IR_TXD);

  // Setup segment pins
  for (int i = 0; i < 7; i++) {
    pinMode(segPins[i], OUTPUT);
    digitalWrite(segPins[i], LOW);
  }

  Serial.println("Waiting for IR command...");
}

void loop() {
  if (IRSerial.available() >= 3) {
    byte userCode1 = IRSerial.read();
    byte userCode2 = IRSerial.read();
    byte command = IRSerial.read();

    Serial.printf("Received IR code: %02X %02X %02X\n", userCode1, userCode2, command);

    // Display command value on 7-segment (only 0x0–0xF supported)
    if (command < 16) {
      displayDigit(command);
    } else {
      Serial.println("Unsupported command > 0x0F");
    }

    delay(1000); // display time
  }
}

void displayDigit(byte val) {
  byte segments = digitMap[val];
  for (int i = 0; i < 7; i++) {
    digitalWrite(segPins[i], bitRead(segments, i));
  }
}
