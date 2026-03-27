# Contributing to AgrIoT Smart Motor Control

## Reporting Bugs

Open a GitHub Issue and include:
- Hardware version (PCB revision, MCU boards used)
- Firmware version / commit hash
- Steps to reproduce
- Serial monitor output (115200 baud)
- RSSI / SNR values if the issue is RF-related

## Suggesting Features

Open an issue with the `[FEATURE]` tag. Describe the use case and expected
behaviour. If it involves RF changes, note the regulatory constraints for your
region (868 MHz EU / 915 MHz US).

## Code Contributions

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/my-feature`
3. Follow the code style below
4. Test on real hardware — serial monitor output required
5. Commit with clear messages: `git commit -m 'Add: brief description'`
6. Open a Pull Request against `main`

## Code Style (Arduino / C++)

- Indent: 2 spaces (no tabs)
- Braces: K&R style (opening brace on same line)
- Constants: `UPPER_CASE`
- Variables: `camelCase`
- Functions: `camelCase()`
- Comments: explain *why*, not *what*

```cpp
const int RELAY_START = 8;

// Activate starter relay for exactly 500 ms to mimic physical key turn.
// Longer pulses risk draining the battery if the engine fails to catch.
void Start() {
  digitalWrite(RELAY_START, LOW);
  timer.setTimeout(500, turnRelay1Off);
}
```

## Safety Note

This firmware controls high-current relays connected to a real diesel engine.
Test all relay sequences on the bench before field deployment. Incorrect
start/stop timing can damage the engine or create electrical hazards.
