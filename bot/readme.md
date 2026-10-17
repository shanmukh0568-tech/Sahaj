# SAHAJ WhatsApp + Voice Bot

The messaging side of SAHAJ: sends irrigation alerts to farmers on WhatsApp and speaks
them in Telugu. Runs with plain Node, no libraries to install. Part of the SAHAJ project
for Plaksha YCL 2026.

## What each file does
- send-text.js  — sends a plain WhatsApp text (first proof it works).
- alert.js      — defines sendDryAlert(to, field, moisture%), the function the sensor hub
  calls when the soil is dry.
- tts.js        — turns a Telugu sentence into audio (sahaj-te.wav) using Sarvam AI's Bulbul model.
- send-voice.js — uploads the audio and sends it as a WhatsApp voice note.

## Setup
Needs Node 20.6+ and a .env file (never committed) with:

        WHATSAPP_TOKEN=   # temporary token from Meta API Setup (expires ~24h)
        PHONE_NUMBER_ID=  # from Meta API Setup
        RECIPIENT=        # test number, country code, no +
        SARVAM_KEY=       # from sarvam.ai

## Run

        node --env-file=.env send-text.js
        node --env-file=.env alert.js
        node --env-file=.env tts.js
        afconvert sahaj-te.wav sahaj-te.m4a -f m4af -d aac
        node --env-file=.env send-voice.js

## Notes
- The WhatsApp test token expires about every 24h — refresh it in .env when you get a 190 error.
- Voice uses Sarvam bulbul:v3 (an Indian-language model). Bhashini is the government option for scale.
- Never commit .env — it holds secret keys.
