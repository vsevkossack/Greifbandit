# Arduino-IDE-Konfiguration in VS Code für ESP32-S3

Diese Anleitung beschreibt, wie du das Projekt Greifbandit mit Visual Studio Code und einer ESP32-S3-Platine kompilieren und flashen kannst. Die Konfiguration wurde für den Board-Typ `ESP32-S3 Dev Module` und den Port `/dev/ttyACM0` vorbereitet.

## 1. Voraussetzungen

Stelle sicher, dass folgende Programme verfügbar sind:

- VS Code
- Arduino CLI
- Erweiterung für Arduino in VS Code
- ESP32-Core für Arduino

## 2. Arduino CLI installieren

Falls `arduino-cli` noch nicht installiert ist, kann es über das Installationsskript installiert werden:

```bash
mkdir -p "$HOME/.local/bin"
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR="$HOME/.local/bin" sh
export PATH="$HOME/.local/bin:$PATH"
```

Prüfen:

```bash
arduino-cli version
```

## 3. ESP32-Board-Package hinzufügen

```bash
export PATH="$HOME/.local/bin:$PATH"
arduino-cli config set board_manager.additional_urls "https://espressif.github.io/arduino-esp32/package_esp32_index.json"
arduino-cli core update-index
arduino-cli core install esp32:esp32
```

Danach ist das ESP32-Board-Package installiert.

## 4. Bibliothek installieren

Für den Sketch wird die Bibliothek `ESP32Servo` benötigt:

```bash
export PATH="$HOME/.local/bin:$PATH"
arduino-cli lib install "ESP32Servo"
```

## 5. Projekt auf ESP32-S3 kompilieren

Das Sketch-Projekt liegt hier:

```text
firmware/roboarm
```

Kompilieren mit:

```bash
export PATH="$HOME/.local/bin:$PATH"
arduino-cli compile -b esp32:esp32:esp32s3 /home/seva/Projects/Greifbandit/firmware/roboarm
```

Wenn der Build erfolgreich ist, erscheint eine Ausgabe wie:

```text
Der Sketch verwendet 327925 Bytes (25%) des Programmspeicherplatzes.
```

## 6. Port prüfen

Der ESP32 ist in Linux meistens als `/dev/ttyACM0` sichtbar:

```bash
ls -l /dev/ttyACM0
```

Wenn der Port vorhanden ist, kann das Gerät erkannt werden.

## 7. In VS Code einrichten

Im Projektordner befindet sich die Konfigurationsdatei:

```text
.vscode/arduino.json
```

Beispielkonfiguration:

```json
{
    "board": "esp32:esp32:esp32s3",
    "port": "/dev/ttyACM0",
    "compiler": {
        "warnings": "all",
        "verbose": false
    }
}
```

Zusätzlich gibt es in `.vscode/tasks.json` vorkonfigurierte Aufgaben für:

- `Build RoboArm (ESP32-S3)`
- `Upload RoboArm (ESP32-S3)`

## 8. Firmware flashen

Mit dem Upload-Befehl in VS Code oder mit der Arduino-CLI:

```bash
export PATH="$HOME/.local/bin:$PATH"
arduino-cli compile --upload -b esp32:esp32:esp32s3 -p /dev/ttyACM0 /home/seva/Projects/Greifbandit/firmware/roboarm
```

## 9. Wenn Upload fehlschlägt

Falls die Meldung erscheint:

```text
Failed to connect to ESP32-S3: No serial data received.
```

Dann ist die Platine wahrscheinlich nicht im Flash-Modus.

### Lösung:

1. BOOT-Taste auf der ESP32-S3 gedrückt halten
2. RESET-Taste kurz drücken
3. BOOT-Taste loslassen
4. Upload erneut starten

Manchmal hilft auch ein kurzer Abstecken und Wiederanschließen des USB-Kabels.

## 10. Empfehlung

Für dieses Projekt ist die Kombination aus:

- VS Code
- Arduino CLI
- ESP32-Core
- `ESP32Servo`

am besten geeignet, da der Sketch direkt als `.ino`-Projekt kompiliert wird und die Firmware auf der ESP32-S3 sehr zuverlässig hochgeladen werden kann.

## 11. Wichtige Pfade im Projekt

```text
/home/seva/Projects/Greifbandit/
├── Docs/
├── firmware/
│   └── roboarm/
│       └── roboarm.ino
├── .vscode/
│   ├── arduino.json
│   ├── tasks.json
│   └── settings.json
└── Readme.md
```

## 12. Kurzfassung

Der Standard-Workflow für dieses Projekt ist:

```bash
export PATH="$HOME/.local/bin:$PATH"
arduino-cli compile -b esp32:esp32:esp32s3 /home/seva/Projects/Greifbandit/firmware/roboarm
arduino-cli compile --upload -b esp32:esp32:esp32s3 -p /dev/ttyACM0 /home/seva/Projects/Greifbandit/firmware/roboarm
```

Damit kann das Projekt direkt aus VS Code oder über die Konsole auf die ESP32-S3 geladen werden.
