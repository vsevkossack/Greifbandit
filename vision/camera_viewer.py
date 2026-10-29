import argparse
import sys
import time
import urllib.request
import numpy as np
import cv2


def read_mjpeg_frames(url, timeout=5.0):
    req = urllib.request.Request(url, headers={"User-Agent": "GreifbanditCam/1.0"})
    stream = urllib.request.urlopen(req, timeout=timeout)
    buffer = b""
    start_marker = b"\xff\xd8"
    end_marker = b"\xff\xd9"

    while True:
        chunk = stream.read(4096)
        if not chunk:
            break

        buffer += chunk
        a = buffer.find(start_marker)
        b = buffer.find(end_marker, a if a != -1 else 0)

        if a != -1 and b != -1:
            frame_bytes = buffer[a : b + 2]
            buffer = buffer[b + 2 :]
            arr = np.frombuffer(frame_bytes, dtype=np.uint8)
            frame = cv2.imdecode(arr, cv2.IMREAD_COLOR)
            if frame is not None:
                yield frame


def main():
    parser = argparse.ArgumentParser(description="Greifbandit ESP32 camera viewer")
    parser.add_argument("--url", default="http://192.168.4.1/stream", help="URL to the camera stream")
    parser.add_argument("--window", default="Greifbandit Camera", help="Window title")
    args = parser.parse_args()

    print("Connecting to:", args.url)

    try:
        for frame in read_mjpeg_frames(args.url):
            cv2.imshow(args.window, frame)
            key = cv2.waitKey(1) & 0xFF
            if key == ord('q'):
                break
            if key == ord('s'):
                filename = time.strftime("greifbandit_%Y%m%d_%H%M%S.jpg")
                cv2.imwrite(filename, frame)
                print("Saved:", filename)
    except Exception as exc:
        print("Connection error:", exc)
        print("Check the ESP32 address and the camera stream URL.")
        print("Example: http://192.168.4.1/stream")
        sys.exit(1)
    finally:
        cv2.destroyAllWindows()


if __name__ == "__main__":
    main()
