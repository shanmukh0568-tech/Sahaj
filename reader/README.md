# SAHAJ — Field Reader

The brain on each farm. It reads the field, decides when to water, and
switches the pump — all on the farm itself, so it keeps working even if the
village hub or the internet is down.

## Two modes, one firmware
Set the mode per field at the top of `reader.ino`:

- `#define CROP_RICE` on  -> AWD mode. An ultrasonic sensor on a perforated
  field tube watches the paddy water level. Safe AWD: re-flood when the water
  drops ~15 cm below the soil, stop ~5 cm above it (saves ~25% water).
- `#define CROP_RICE` off -> soil-moisture mode (cotton, wheat, maize).
  Pump runs below 30% moisture, stops at 60%.

## Pins
| Part | Role | ESP32 pin |
| --- | --- | --- |
| Soil moisture (analog) | soil mode | GPIO34 |
| HC-SR04 TRIG / ECHO | AWD mode | GPIO25 / GPIO33 |
| Relay (pump) | both | GPIO26 |
| Grid-power switch | both | GPIO27 |
| LED (pump status) | both | GPIO2 |

## Safety (both modes)
- Pump never runs without grid power — it queues for the next window.
- Hard maximum run time, then a cooldown, so a fault can't water all day.
- The farmer's manual switch always works.

## Run it in Wokwi
Open `diagram.json` and `reader.ino` in a Wokwi ESP32 project, run, open Serial
Monitor at 115200. Drag the potentiometer (soil) or the HC-SR04 distance (AWD).

## Calibrate on real hardware
- Soil: `RAW_DRY` (sensor in dry air), `RAW_WET` (in water).
- AWD: `SENSOR_TO_SOIL_CM` = how far the sensor sits above the soil.

## Not built yet
The reader-to-hub radio link (ESP-NOW) and its encryption are not implemented.
Do not call the data "encrypted" or "secure" until the shared key is added on
real boards and a mentor has reviewed it.
