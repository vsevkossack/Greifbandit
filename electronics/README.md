# Elektronik – Greifbandit

Dieser Ordner enthält die Hardware- und KiCad-Struktur für die Steuerungs-Elektronik des Greifbandit.

## Inhalt

- ESP32-S3 Basisplatine
- Stromversorgung und Ground-Verteilung
- serielle Schnittstellen und IO-Anbindung
- externe Bibliothek für ESP32-S3-Module
- eigenes Projekt für den Greifbandit-Schaltplan

## Ordnerstruktur

- electronics/greifbandit_esp32s3/
  - KiCad-Projekt für den eigenen Schaltplan
- electronics/libraries/ESP32-S3-DEVKITC-1-N8R2/
  - kompilierte Bibliothek mit Symbol, Footprint und 3D-Modell für das ESP32-S3-DevKitC-1-N8R2

## KiCad-Projekt

Das aktive Schaltungsprojekt liegt in:

- electronics/greifbandit_esp32s3/

Es enthält unter anderem:

- Greifbandit_ESP32S3.kicad_pro
- Greifbandit_ESP32S3.kicad_sch
- Greifbandit_ESP32S3.kicad_sym
- sym-lib-table
- fp-lib-table

## Bibliothek

Die ESP32-S3-Bibliothek wurde aus dem USB-Ordner in den Projektordner kopiert und in `electronics/libraries/ESP32-S3-DEVKITC-1-N8R2/` abgelegt.

Wichtige Dateien:

- `ESP32-S3-DEVKITC-1-N8R2.kicad_sym` – Symbol für den Schaltplan
- `XCVR_ESP32-S3-DEVKITC-1-N8R2.kicad_mod` – Footprint für das PCB
- `ESP32-S3-DEVKITC-1-N8R2.step` – 3D-Modell
- `License.txt` – Lizenzhinweis

Diese Struktur erlaubt es, das Modul im Schaltplan zu platzieren und anschließend direkt mit dem passenden Footprint auf der Platine zuzuordnen.
