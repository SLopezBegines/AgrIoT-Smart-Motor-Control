#!/usr/bin/env python3
"""
AgrIoT Serial Monitor
Displays timestamped output from gateway or node with RSSI/SNR highlighting.

Usage:
    python3 serial-monitor.py [PORT] [BAUDRATE]

Examples:
    python3 serial-monitor.py /dev/ttyUSB0
    python3 serial-monitor.py COM3 115200
"""

import serial
import sys
from datetime import datetime


def monitor(port: str = "/dev/ttyUSB0", baudrate: int = 115200) -> None:
    try:
        ser = serial.Serial(port, baudrate, timeout=1)
        print(f"Monitoring {port} at {baudrate} baud. Ctrl+C to stop.")
        print("=" * 70)

        while True:
            if ser.in_waiting:
                raw = ser.readline()
                line = raw.decode("utf-8", errors="replace").strip()
                if not line:
                    continue

                ts = datetime.now().strftime("%H:%M:%S.%f")[:-3]

                # Highlight key events
                prefix = "   "
                if "RSSI:" in line:
                    rssi_val = _extract_int(line, "RSSI:")
                    prefix = "📶 " if rssi_val and rssi_val > -100 else "⚠️  "
                elif "Sending packet" in line:
                    prefix = "▶  "
                elif "Received from" in line:
                    prefix = "◀  "
                elif "failed" in line.lower() or "error" in line.lower():
                    prefix = "❌ "
                elif "Relay" in line and "enabled" in line:
                    prefix = "🔌 "
                elif "Relay" in line and "disabled" in line:
                    prefix = "⭕ "

                print(f"[{ts}] {prefix}{line}")

    except KeyboardInterrupt:
        print("\nMonitoring stopped.")
    except serial.SerialException as e:
        print(f"Serial error: {e}")
        print("Available ports: check `ls /dev/ttyUSB*` or `ls /dev/ttyACM*`")
        sys.exit(1)
    finally:
        try:
            ser.close()
        except Exception:
            pass


def _extract_int(line: str, key: str) -> int | None:
    try:
        idx = line.index(key) + len(key)
        return int(line[idx:].split()[0])
    except (ValueError, IndexError):
        return None


if __name__ == "__main__":
    port = sys.argv[1] if len(sys.argv) > 1 else "/dev/ttyUSB0"
    baud = int(sys.argv[2]) if len(sys.argv) > 2 else 115200
    monitor(port, baud)
