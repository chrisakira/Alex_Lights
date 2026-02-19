/*
 * Expander pin index map (matches I2C_IO_Expander.ino):
 *   Index  0 → D2    Index  6 → D8    Index 12 → D44
 *   Index  1 → D3    Index  7 → D9    Index 13 → D45
 *   Index  2 → D4    Index  8 → D10   Index 14 → D46
 *   Index  3 → D5    Index  9 → D11
 *   Index  4 → D6    Index 10 → D12
 *   Index  5 → D7    Index 11 → D13
 */
#include <Arduino.h>
#include <Wire.h>

// ── Configuration ──────────────────────────────────────────────────────────────

#define I2C_SLAVE_ADDR   0x08   // Must match I2C_IO_Expander slave address

#define SDA_PIN          4      // GPIO4 – NodeMCU label D2
#define SCL_PIN          5      // GPIO5 – NodeMCU label D1

// ── Pin Index Constants (as defined in the expander sketch) ────────────────────

#define IDX_D2           0
#define IDX_D13          11
#define IDX_D44          12
#define IDX_D45          13
#define IDX_D46          14

// ── Timing ─────────────────────────────────────────────────────────────────────
//
// Target: each pin ramps 0→255 in exactly 1 000 ms, using step size 20.
//   • Steps per pin: 0, 20, 40, …, 240, 255 → 14 steps
//   • Budget per step: 1 000 ms ÷ 14 ≈ 71 ms
//   • I2C write at 400 kHz ≈ 0.15 ms (negligible at this scale)
//
// Total time for D2→D13: 12 pins × 1 s = 12 s per phase.
// Adjust STEP_DELAY_MS if your measured ramp time drifts from 1 s.

#define RAMP_PINS        12     // D2 through D13
#define STEP_SIZE        20     // PWM increment per ramp step
#define STEP_DELAY_MS    71     // Delay between steps (ms) → ~1 s per pin

// ── Helpers ────────────────────────────────────────────────────────────────────

// Send a single [pinIndex, value] command to the expander.
void setPin(uint8_t pinIndex, uint8_t value) {
  Wire.beginTransmission(I2C_SLAVE_ADDR);
  Wire.write(pinIndex);
  Wire.write(value);
  Wire.endTransmission();
}

// Reset all D2–D13 channels to 0.
void clearRampPins() {
  for (uint8_t i = IDX_D2; i <= IDX_D13; i++) {
    setPin(i, 0);
  }
}

// Ramp a single channel from 0 to 255 in steps of STEP_SIZE.
// Values: 0, 20, 40, …, 240, 255 (14 steps) → total ≈ 1 s per pin.
void rampPin(uint8_t pinIndex) {
  for (uint16_t v = 0; v < 255; v += STEP_SIZE) {
    setPin(pinIndex, (uint8_t)v);
    delay(STEP_DELAY_MS);
  }
  // Always end at full brightness
  setPin(pinIndex, 255);
  delay(STEP_DELAY_MS);
}

// Ramp D2 → D13 sequentially (all 12 channels, total ≈ 1 s).
void runRampSequence() {
  for (uint8_t i = IDX_D2; i <= IDX_D13; i++) {
    rampPin(i);
  }
}

// ── Setup ──────────────────────────────────────────────────────────────────────

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println(F("\nESP8266 Light Sequencer – starting"));

  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(400000);  // 400 kHz Fast Mode

  // Ensure all expander outputs start at 0
  clearRampPins();
  setPin(IDX_D44, 0);
  setPin(IDX_D45, 0);
  setPin(IDX_D46, 0);
  delay(100);

  Serial.println(F("I2C initialised. Starting sequence."));
}

// ── Main Loop ──────────────────────────────────────────────────────────────────

void loop() {

  // ── Phase 1: D44 active ──────────────────────────────────────────────────────
  Serial.println(F("Phase 1 – D44"));
  setPin(IDX_D44, 255);

  clearRampPins();          // Start all ramp pins from 0
  runRampSequence();        // Ramp D2→D13, ≈ 1 s

  setPin(IDX_D44, 0);
  clearRampPins();          // Reset ramp pins before next phase

  // ── Phase 2: D45 active ──────────────────────────────────────────────────────
  Serial.println(F("Phase 2 – D45"));
  setPin(IDX_D45, 255);

  clearRampPins();
  runRampSequence();        // Ramp D2→D13, ≈ 1 s

  setPin(IDX_D45, 0);
  clearRampPins();

  // ── Phase 3: D46 active ──────────────────────────────────────────────────────
  Serial.println(F("Phase 3 – D46"));
  setPin(IDX_D46, 255);

  clearRampPins();
  runRampSequence();        // Ramp D2→D13, ≈ 1 s

  setPin(IDX_D46, 0);
  clearRampPins();

  // Loop repeats immediately → Phase 1 again
}
