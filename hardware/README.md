# Hardware

## Directory Structure

```
hardware/
├── pcb/
│   ├── PCB_Motor.pdf       — Schematic PDF (EasyEDA export)
│   └── gerbers/            — Manufacturing files (add before ordering)
├── bom/
│   ├── BOM.md              — Formatted bill of materials with part numbers
│   └── Coste.xlsx          — Original cost spreadsheet (sensor calibration + BOM)
└── enclosure/              — 3D-printable enclosure (planned)
```

## PCB

The custom PCB was designed in EasyEDA. Key design decisions:

- **2-layer board** — cost-efficient for JLCPCB, adequate for 5 V/3.3 V logic
- **Screw terminals (Faston 6.3 mm)** — field-replaceable wiring without soldering
- **RJ45 connectors** — for sensor runs up to ~10 m from the enclosure
- **Flyback diodes on all relay coils** — protects Arduino from inductive spikes
- **Ground plane on bottom layer** — reduces ADC noise from engine alternator

### Ordering PCBs

Upload `gerbers/` to [JLCPCB](https://jlcpcb.com) or [PCBWay](https://pcbway.com).
Minimum order is typically 5 boards (~€5 including shipping from China).

## Power Architecture

```
Engine battery (12 V)
        │
        ▼
  LM2596 Buck (12V → 5V, 3A)
        │
        ├── Arduino Nano (5V)
        ├── Relay coils (5V)
        │
        ▼
  LM1117T-3.3 LDO (5V → 3.3V, 800mA)
        │
        └── RFM95W LoRa module (3.3V)
```

The 1N5819 Schottky diode provides reverse-polarity protection at the 12 V input.
