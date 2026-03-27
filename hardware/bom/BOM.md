# Bill of Materials

> Costs extracted from `Coste.xlsx`. Prices are approximate and vary by supplier.

## Electronics

| Qty | Component | Designator | Manufacturer Part | Supplier | Part No. |
|-----|-----------|------------|-------------------|----------|----------|
| 1 | Arduino Nano (ATmega328P) | U1 | — | Amazon | — |
| 1 | RFM95W LoRa Module 868 MHz | U3 | RFM98-433S2 | LCSC | C171661 |
| 1 | 4-channel relay module | U2 | — | Amazon | — |
| 1 | LM2596 buck converter (12V→5V) | REG1 | — | Amazon | — |
| 1 | LM1117T-3.3 LDO regulator | U4 | LM1117T-3.3/NOPB | LCSC | C201726 |
| 2 | 10 µF electrolytic capacitor 16V | C1, C2 | 10uF 16V 3×5 | LCSC | C43798 |
| 1 | 1N5819 Schottky diode | D3 | 1N5819 | LCSC | C110032 |
| 1 | 1N4007 rectifier diode | D1 | 1N4007-E3/73 | LCSC | C466973 |
| 1 | Fuse holder | FH1 | 64600001003 | LCSC | C206887 |
| 2 | Resistor 22 kΩ | R1, R5 | — | Local | — |
| 2 | Resistor 4.7 kΩ | R2, R3 | — | Local | — |
| 2 | RJ45 connector (PCB mount) | RJ1, RJ2 | RJ45 | EasyEDA | E36373 |
| 16 | Faston terminal 6.3 mm | ST2–ST17 | 62409-1 | Element14 | 892609 |
| 23 | Pin header male 2.54 mm | J1–J24 | — | LCSC | C81276 |
| 45 | Pin header female 2.54 mm | — | — | Amazon | — |
| 1 | U.FL coaxial connector | J22 | — | — | — |

## Sensors

| Qty | Component | Connection | Approx. Cost (€) |
|-----|-----------|------------|------------------|
| 1 | Pressure transducer (0–5 V analog) | A5 | 10–15 |
| 1 | Hall effect sensor (A3144 or equiv.) | D3 (INT1) | 1–2 |
| 1 | Fuel level sensor (digital float) | D4 | 13.99 |

## Mechanical & Enclosure

| Qty | Component | Spec | Approx. Cost (€) |
|-----|-----------|------|------------------|
| 1 | Custom PCB | 2-layer, JLCPCB | 2.60 (batch of 5) |
| 1 | Waterproof enclosure (IP65) | 200×120×75 mm | 5 |
| 1 | LoRa antenna 868 MHz | SMA, 3–5 dBi | 3–5 |
| 1 | Linear actuator (throttle/brake) | 12 V DC | 15 |
| 1 | Electrovalve (fertiliser) | 12 V | 12 |
| — | Wire 18 AWG (power), 22 AWG (signal) | — | 2–3 |
| — | Cable glands PG7 / PG9 | — | 2–3 |

## Gateway Node (ESP32)

| Qty | Component | Approx. Cost (€) |
|-----|-----------|------------------|
| 1 | ESP32 TTGO LoRa v2 (built-in SX1278, 868 MHz) | 15–20 |
| 1 | Weatherproof enclosure | 5 |
| 1 | 5 V USB power supply or DC adapter | 3–5 |

---

**Estimated total hardware cost (single installation): €80–120**

Excludes labour, installation, and Blynk app energy credits.
See `Coste.xlsx` for the original detailed cost breakdown with supplier quotes.
