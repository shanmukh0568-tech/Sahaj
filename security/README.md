# SAHAJ — Security demo (fake reader rejected)

Shows the hub accepts only readers holding the shared key, and rejects one
that doesn't.

- `real_reader.ino` encrypts a reading with AES-128 (shared key) and
  broadcasts it over ESP-NOW.
- `hub_secure.ino` decrypts with the same key and checks a marker. Match ->
  ACCEPTED. No match -> REJECTED.
- `fake_reader.ino` dumps correctly-sized but un-keyed junk. It passes the
  size check but fails the key check, so the hub rejects it.

## Run it (three ESP32 boards)
1. Flash `hub_secure.ino` to board 1, open Serial Monitor at 115200.
2. Flash `real_reader.ino` to board 2 -> hub prints ACCEPTED.
3. Flash `fake_reader.ino` to board 3 -> hub prints REJECTED.
(ESP-NOW does not run in Wokwi -- use real boards. Keep all three on the
default WiFi channel.)

## Honest scope
Demo-grade: AES-128-ECB, one shared key, no anti-replay. It proves a reader
without the key is blocked, but it is not production-hardened. Next:
per-device keys set at QR pairing, a nonce/counter against replay, and a
reviewed cipher mode (CBC/GCM). A mentor should review before we call it
"secure."
