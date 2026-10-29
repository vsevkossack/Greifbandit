#!/usr/bin/env python3

import argparse
import sys

import cv2
import numpy as np
import serial


def read_usb_jpeg_frames(port, baud=115200, timeout=1.0):
    ser = serial.Serial(port, baud, timeout=timeout)
    buffer = bytearray()

    try:
        while True:
            data = ser.read(4096)
            if not data:
                continue

            buffer.extend(data)
            start = buffer.find(b"\xff\xd8")
            end = buffer.find(b"\xff\xd9", start if start != -1 else 0)

            if start != -1 and end != -1 and end > start:
                jpg = bytes(buffer[start : end + 2])
                del buffer[: end + 2]
                arr = np.frombuffer(jpg, dtype=np.uint8)
                frame = cv2.imdecode(arr, cv2.IMREAD_COLOR)
                if frame is not None:
                    yield frame
    except KeyboardInterrupt:
        raise
    finally:
        ser.close()
        cv2.destroyAllWindows()


def auto_detect_port():
    candidates = []

    for base in ("/dev/ttyACM", "/dev/ttyUSB"):
        glob_pattern = base + "*"
        try:
            import glob
            candidates.extend(sorted(glob.glob(glob_pattern)))
        except Exception:
            pass

    for p in sorted(candidates):
        if p.startswith("/dev/ttyACM") or p.startswith("/dev/ttyUSB"):
            return p

    return None


def main():
    parser = argparse.ArgumentParser(description="Greifbandit ESP32 USB camera viewer")
    parser.add_argument("--port", default=None, help="Serial port for the ESP32 camera")
    parser.add_argument("--baud", type=int, default=115200, help="Serial baud rate")
    parser.add_argument("--window", default="Greifbandit - Auge Live", help="Window title")
    args = parser.parse_args()

    port = args.port or auto_detect_port()
    if port is None:
        print("No ESP32 serial port found. Check whether the board is connected and visible in /dev/ttyACM* or /dev/ttyUSB*.")
        print("If the board just reflashed, unplug/reconnect it and wait a few seconds for Linux to enumerate it.")
        sys.exit(1)

    print(f"Opening serial port: {port} @ {args.baud} baud")

    try:
        for frame in read_usb_jpeg_frames(port, baud=args.baud):
            cv2.imshow(args.window, frame)
            key = cv2.waitKey(1) & 0xFF
            if key == ord("q"):
                break
    except Exception as exc:
        print(f"USB monitor error: {exc}")
        print("The board may still be resetting or not enumerated yet. Try unplugging and reconnecting the ESP32.")
        sys.exit(1)
    finally:
        cv2.destroyAllWindows()


if __name__ == "__main__":
    main()
