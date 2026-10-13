# ESP32-S3-DEVKITC-1-N8R2 Library

Diese Bibliothek stammt aus dem lokalen USB-Ordner und wurde in das Projekt verschoben, damit sie sauber unter der Elektronik-Struktur des Repositories liegt.

## Inhalt

- `ESP32-S3-DEVKITC-1-N8R2.kicad_sym` – symbol library für den Schaltplan
- `XCVR_ESP32-S3-DEVKITC-1-N8R2.kicad_mod` – footprint library für das PCB
- `ESP32-S3-DEVKITC-1-N8R2.step` – 3D-Model des Moduls
- `License.txt` – Lizenzinformationen
- `how-to-import.htm` – Import-Hinweis aus SnapEDA

## Verwendet in KiCad

Das Projekt in `electronics/greifbandit_esp32s3/` referenziert diese Bibliothek über:

- `sym-lib-table`
- `fp-lib-table`

Damit kann das ESP32-S3-Modul im Schaltplan platziert und später mit dem passenden Footprint verknüpft werden.
