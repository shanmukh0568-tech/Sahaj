import { writeFileSync } from "node:fs";

const KEY = process.env.SARVAM_KEY;
const TEXT = "మీ పొలం ఎండిపోయింది. ఫీల్డ్ 1 కి నీరు పెట్టండి.";

const res = await fetch("https://api.sarvam.ai/text-to-speech", {
  method: "POST",
  headers: { "api-subscription-key": KEY, "Content-Type": "application/json" },
  body: JSON.stringify({
    text: TEXT,
    target_language_code: "te-IN",
    speaker: "kavya",
    model: "bulbul:v3"

  })
});

const data = await res.json();
console.log("status:", res.status);
if (!res.ok) { console.log(data); process.exit(1); }

writeFileSync("sahaj-te.wav", Buffer.from(data.audios[0], "base64"));
console.log("saved sahaj-te.wav ✓");
