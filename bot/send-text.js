const { WHATSAPP_TOKEN, PHONE_NUMBER_ID, RECIPIENT } = process.env;

const res = await fetch(
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
      type: "text",
      text: { body: "SAHAJ test: your well field is dry. Water field 1." },
    }),
  }
);
console.log(res.status, await res.json());
