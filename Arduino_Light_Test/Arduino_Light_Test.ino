#include <Wire.h>

// ── Configuration ──────────────────────────────────────────────────────────────

#define I2C_SLAVE_ADDRESS  0x08   // Change to avoid conflicts on your I2C bus

// ── Pin Map ────────────────────────────────────────────────────────────────────

const uint8_t PWM_PINS[] = {
  2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,  // D2–D13
  44, 45, 46                                 // D44–D46
};

const uint8_t PIN_COUNT = sizeof(PWM_PINS) / sizeof(PWM_PINS[0]);  // 15

// ── Receive Buffer (shared between ISR and loop) ────────────────────────────────

volatile uint8_t rxPinIndex = 0;
volatile uint8_t rxValue    = 0;
volatile bool    newCommand = false;

// ── Setup ──────────────────────────────────────────────────────────────────────

void setup() {
  // Initialise all controlled pins as outputs, starting at 0
  for (uint8_t i = 0; i < PIN_COUNT; i++) {
    pinMode(PWM_PINS[i], OUTPUT);
    analogWrite(PWM_PINS[i], 0);
  }

  // Start I2C as Slave
  Wire.begin(I2C_SLAVE_ADDRESS);
  Wire.onReceive(onReceiveI2C);

  // Optional: debug output over USB Serial
  Serial.begin(115200);
  Serial.print(F("I2C IO Expander ready. Address: 0x"));
  Serial.println(I2C_SLAVE_ADDRESS, HEX);
  Serial.print(F("Controlling "));
  Serial.print(PIN_COUNT);
  Serial.println(F(" PWM pins."));
}

// ── Main Loop ──────────────────────────────────────────────────────────────────

void loop() {
  if (newCommand) {
    // Snapshot volatile variables safely
    uint8_t pinIdx = rxPinIndex;
    uint8_t value  = rxValue;
    newCommand = false;

    if (pinIdx < PIN_COUNT) {
      uint8_t clamped = (value > 255) ? 255 : value;
      analogWrite(PWM_PINS[pinIdx], clamped);

      Serial.print(F("D"));
      Serial.print(PWM_PINS[pinIdx]);
      Serial.print(F(" -> "));
      Serial.println(clamped);
    } else {
      Serial.print(F("ERR: Invalid pin index "));
      Serial.println(pinIdx);
    }
  }
}

// ── I2C Receive ISR ────────────────────────────────────────────────────────────

void onReceiveI2C(int numBytes) {
  if (numBytes >= 2) {
    rxPinIndex = Wire.read();   // Byte 0: pin index (0–14)
    rxValue    = Wire.read();   // Byte 1: PWM value (0–255)

    // Drain any unexpected extra bytes
    while (Wire.available()) {
      Wire.read();
    }

    newCommand = true;
  } else {
    // Drain malformed packet
    while (Wire.available()) {
      Wire.read();
    }
  }
}
