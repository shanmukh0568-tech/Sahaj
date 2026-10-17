import { readFileSync } from "node:fs";

const { WHATSAPP_TOKEN, PHONE_NUMBER_ID, RECIPIENT } = process.env;

// 1) upload the audio file, get a media id
const form = new FormData();
form.append("messaging_product", "whatsapp");
form.append("type", "audio/mp4");
form.append(
  "file",
  new Blob([readFileSync("sahaj-te.m4a")], { type: "audio/mp4" }),
  "sahaj-te.m4a"
);

const up = await fetch(
  `https://graph.facebook.com/v21.0/${PHONE_NUMBER_ID}/media`,
  { method: "POST", headers: { Authorization: `Bearer ${WHATSAPP_TOKEN}` }, body: form }
);
const upJson = await up.json();
console.log("upload:", up.status, upJson);
if (!up.ok) process.exit(1);

// 2) send that media id as a voice note
const send = await fetch(
  `https://graph.facebook.com/v21.0/${PHONE_NUMBER_ID}/messages`,
  {
    method: "POST",
    headers: {
      Authorization: `Bearer ${WHATSAPP_TOKEN}`,
      "Content-Type": "application/json",
    },
    body: JSON.stringify({
      messaging_product: "whatsapp",
      to: RECIPIENT,
      type: "audio",
      audio: { id: upJson.id },
    }),
  }
);
console.log("send:", send.status, await send.json());
