# SAHAJ

**Sensor-driven Agricultural Help & Advisory in your Jubaan.** SAHAJ gives small farmers
irrigation advice on WhatsApp, in their own language, from sensors in their own field, and
switches the pump on only when the soil needs water. Built for Plaksha University's Young
Creators League 2026.
- Rice fields use **AWD water-level sensing** (ultrasonic on a field tube), not soil moisture.
- Voice advice is generated with **Sarvam** (Indian-language voice AI).
By Shanmukh, Saanvi and Durgashree.

## What's in this repo
- **reader/** — firmware for the area reader: reads soil moisture and controls the pump
  (dry → on, target → off, power-aware, safety cap + cooldown). Tested in the Wokwi ESP32 simulator.
- **hub/** — firmware for the village hub: reads weather (BME280), receives reader data over
  radio, and is where the WhatsApp alert is triggered.
- **bot/** — the WhatsApp + voice side: sends alerts to farmers and speaks them in Telugu
  (Sarvam AI). Plain Node, no libraries.
- **security/** - The security for the encryption of the readers to the hub, building privacy for the farmer(not hardened yet)

## How the parts fit together
Sensors in the field → the reader decides when to water and runs the pump → readings go to the
hub → the hub sends the farmer a WhatsApp message, in their language, by voice.

## Status
Prototype for Round 1. The reader logic and the WhatsApp + voice pipeline both work. Still
ahead: the radio link between the two real boards, per-device encryption, and a field pilot.

## Roadmap
- ESP-NOW radio link from reader to hub (real boards).
- Per-device encryption and rejection of fake readers.
- Water-level sensing for rice (Alternate Wetting and Drying).
- Field pilot with KVK guidance; Bhashini for voice at national scale.

## License
MIT.
