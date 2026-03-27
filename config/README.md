# Configuration

## Setup

1. Copy the template to the gateway firmware folder:

```bash
cp config/config.h.example \
   firmware/gateway/Diesel_Motor_Lora_Gateway_NoOLED/config.h
```

2. Edit `config.h` with your Blynk credentials, WiFi network, and OTA password.

3. `config.h` is listed in `.gitignore` — it will never be committed.

## Files

| File | Description |
|------|-------------|
| `config.h.example` | Template with all configurable parameters and documentation |
| `Virtual_Pins_Blynk.xlsx` | Blynk virtual pin mapping spreadsheet (V1–V24) |

## Blynk Setup

Get your credentials from the Blynk Cloud console:
- Template ID: Device Info tab → Template ID
- Auth Token: Device Info tab → Auth Token

See `docs/blynk-setup.md` for the complete virtual pin mapping.
