# SAHAJ Area Reader

Firmware for the area reader: reads soil moisture and controls the pump. Tested in the Wokwi ESP32 simulator.

## What it does
- Waters only when the soil is dry; stops at a target, with a gap so the pump doesn't flicker.
- Runs only while grid power is available.
- Never runs longer than a safety cap; after a safety stop, waits a cooldown and flags the farmer.

## Pins
| Part | Role | ESP32 pin |
|------|------|-----------|
| Capacitive soil sensor (potentiometer in sim) | soil moisture | GPIO34 |
| Relay module | pump | GPIO26 |
| Switch (grid-power sense) | power available | GPIO27 |
| LED | pump indicator | GPIO2 |

On a real farm the relay switches the motor's starter, fitted by a licensed electrician.

## Calibration
The board reads the sensor as a raw number (0–4095); calibration maps raw → %.
1. Temporarily add in loop(): `Serial.print("raw = "); Serial.println(analogRead(34));`
2. Read in dry air → RAW_DRY. Read in water → RAW_WET.
3. Put both at the top of the sketch, remove the line.

Sim pot gives 4095 (dry) / 0 (wet). A real sensor differs (~3000 dry, ~1300 wet) and varies per unit, so re-calibrate each one.

## Settings (top of reader.ino)
| Constant | Meaning | Demo value |
|----------|---------|------------|
| RAW_DRY / RAW_WET | sensor calibration | 4095 / 0 |
| MOIST_START | % that triggers watering | 30 |
| MOIST_STOP | % that stops watering | 60 |
| MAX_RUN_MS | safety cap on run time | 60 s (demo) |
| COOLDOWN_MS | wait after a safety stop | 30 s (demo) |

Real per-crop thresholds come from the field pilot with KVK guidance.
