# SAHAJ — Area Reader + Pump Controller

**SAHAJ** (Sensor-driven Agricultural Help & Advisory in your Jubaan) gives small farmers
irrigation advice on WhatsApp in their own language, from sensors in their own field, and
switches the pump on only when the soil needs water.

This repo holds the firmware for the **area reader** — the part that reads soil moisture and
controls the pump. Built for Plaksha University's Young Creators League 2026.

By Shanmukh, Saanvi and Durgashree.

## Status
Prototype. The reader logic is complete and tested in the Wokwi ESP32 simulator. The radio
link, encryption and the WhatsApp bridge are next (see Roadmap).

## What it does
- Reads soil moisture and waters only when the soil is dry.
- Stops at a target moisture, with a gap between start and stop so the pump doesn't flicker.
- Runs only while grid power is available (farm pumps get power at limited hours).
- Never runs longer than a safety cap.
- After a safety cutoff, waits out a cooldown and flags the farmer to check the pump,
  instead of blindly restarting.

## Hardware / pins
| Part | Role | ESP32 pin |
|------|------|-----------|
| Capacitive soil sensor (potentiometer in simulation) | soil moisture | GPIO34 |
| Relay module | pump | GPIO26 |
| Switch (grid-power sense) | power available | GPIO27 |
| LED | pump indicator | GPIO2 |

On a real farm the relay switches the motor's **starter**, fitted by a licensed electrician,
not the motor directly.

## Run it in the simulator
1. Open wokwi.com and start a new ESP32 (Arduino / C++) project.
2. Paste `reader.ino` into the sketch tab and `diagram.json` into the diagram tab.
3. Press Play and open the Serial Monitor (115200, set in code).
4. Drag the potentiometer and flip the switch to test the behaviours below.

## Behaviours to test
- Dry + power  -> pump ON
- Target moisture -> pump OFF
- Power lost -> pump OFF (waits)
- Max run time -> SAFETY stop + farmer alert + cooldown

## Calibration
The board reads the sensor as a raw number (0–4095). Calibration tells it which raw number
means "dry" and which means "wet".

1. Temporarily add this line in loop(), just after reading moisture:

        Serial.print("raw = "); Serial.println(analogRead(34));

2. Read the sensor in dry air  -> that number is RAW_DRY.
3. Read it sitting in water    -> that number is RAW_WET.
4. Put both numbers at the top of the sketch, then remove the line from step 1.

In the simulator the potentiometer gives 4095 (dry) and 0 (wet). A real capacitive sensor
reads differently (around 3000 dry, 1300 wet) and varies per unit, so re-calibrate each one.

## Settings (top of reader.ino)
| Constant | Meaning | Demo value |
|----------|---------|------------|
| RAW_DRY / RAW_WET | sensor calibration | 4095 / 0 |
| MOIST_START | % that triggers watering | 30 |
| MOIST_STOP | % that stops watering | 60 |
| MAX_RUN_MS | safety cap on run time | 60 s (demo) |
| COOLDOWN_MS | wait after a safety stop | 30 s (demo) |

Real per-crop thresholds and run times are set in the field pilot with KVK (government
agriculture centre) guidance. The values above are placeholders for the demo.

## Roadmap
- ESP-NOW radio link from reader to the village hub.
- Per-device encryption and rejection of fake readers.
- Hub triggers a WhatsApp voice note (Telugu / Hindi) via Bhashini.
- Water-level sensing for rice (Alternate Wetting and Drying).

## License
MIT.
