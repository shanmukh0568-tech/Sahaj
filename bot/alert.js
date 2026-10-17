const { WHATSAPP_TOKEN, PHONE_NUMBER_ID, RECIPIENT } = process.env;

async function sendDryAlert(to, field, moisturePct) {
  const body =
    `SAHAJ: ${field} is dry (${moisturePct}%). ` +
    `Watering starts when power comes.`;
  const res = await fetch(
    `https://graph.facebook.com/v21.0/${PHONE_NUMBER_ID}/messages`,
    { method: "POST",
      headers: { Authorization: `Bearer ${WHATSAPP_TOKEN}`,
                 "Content-Type": "application/json" },
      body: JSON.stringify({ messaging_product: "whatsapp", to,
        type: "text", text: { body } }) });
  return res.json();
}

// pretend reading — later the ESP32 sends the real number
console.log(await sendDryAlert(RECIPIENT, "Well field", 18));
