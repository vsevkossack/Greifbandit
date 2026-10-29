import argparse
import sys
import time
import urllib.request
import numpy as np
import cv2

def main():
    parser = argparse.ArgumentParser(description="Greifbandit ESP32 camera viewer")
    parser.add_argument("--url", default="http://192.168.4", help="URL to the camera stream")
    parser.add_argument("--window", default="Greifbandit Camera", help="Window title")
    args = parser.parse_args()

    print("Connecting to:", args.url)
    print("Press 'q' to quit, 's' to save snapshot.")

    # Створюємо вікно OpenCV
    cv2.namedWindow(args.window, cv2.WINDOW_AUTOSIZE)

    # Лічильник для розрахунку реальних FPS
    last_time = time.time()
    frame_count = 0

    while True:
        try:
            # Робимо швидкий одиночний запит до плати
            req = urllib.request.Request(args.url, headers={"User-Agent": "GreifbanditCam/1.0"})
            with urllib.request.urlopen(req, timeout=2.0) as response:
                # Читаємо весь отриманий JPEG-файл у пам'ять
                img_bytes = response.read()
                
                if not img_bytes:
                    continue
                
                # Декодуємо байтовий масив у картинку OpenCV
                arr = np.frombuffer(img_bytes, dtype=np.uint8)
                frame = cv2.imdecode(arr, cv2.IMREAD_COLOR)
                
                if frame is not None:
                    # Виводимо картинку у вікно
                    cv2.imshow(args.window, frame)
                    
                    # Рахуємо FPS для діагностики
                    frame_count += 1
                    if time.time() - last_time >= 1.0:
                        print(f"Diagnostics: Streaming at {frame_count} FPS", flush=True)
                        frame_count = 0
                        last_time = time.time()

        except Exception as exc:
            # Виводимо помилку підключення, якщо плата не відповіла
            print(f"Network diagnostic: {exc}", flush=True)
            time.sleep(0.5) # Пауза перед наступною спробою, щоб не заспамити мережу
            continue

        # Обробка кнопок (чекаємо 1 мілісекунду)
        key = cv2.waitKey(1) & 0xFF
        if key == ord('q'):
            print("Quitting...")
            break
        if key == ord('s'):
            filename = time.strftime("greifbandit_%Y%m%d_%H%M%S.jpg")
            cv2.imwrite(filename, frame)
            print("Saved snapshot:", filename)

    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
