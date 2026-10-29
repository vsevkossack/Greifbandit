# Kamera-Subsystem

## Ziel

Die ESP32-S3-CAM liefert ein Livebild an den PC.

Das ist nur das Auge des Roboters. Es überträgt nur Live-Bilder. Alles andere, also Navigation, Entscheidung, Logik und mögliche Objekterkennung, läuft auf dem anderen ESP.

---

## 🔨 Was ist zu tun?

- Kamera starten
- Firmware / Beispielprogramm testen
- WLAN-Verbindung herstellen
- Bildstream aufrufen
- Bildqualität prüfen
- Verzögerung grob beobachten

Noch keine KI.
Noch keine Objekterkennung.

Nur:

Kamera → WLAN → PC

---

## 📦 Ergebnis

Livebild auf dem PC.

Der PC kann das aktuelle Kamerabild des Roboters anzeigen.

---

## ✅ Definition of Done

- Der PC kann zuverlässig das aktuelle Kamerabild des Roboters anzeigen.
- Die Kamera arbeitet als reines Bild-Subsystem.
- Die Bildübertragung funktioniert stabil über WLAN.
- Keine Bildverarbeitung, keine KI und keine Objekterkennung in diesem Subsystem.
- Der andere ESP übernimmt die Entscheidung und die Logik.

---

## Architektur-Hinweis

Dieses Subsystem ist nur das "Auge".

- Kamera liefert Bilder
- WLAN sendet die Bilder
- PC zeigt sie an
- anderer ESP entscheidet, was als Nächstes passiert

Damit bleibt die Trennung klar:

- ESP32-S3-CAM = Kamera und Bildübertragung
- zweiter ESP = Steuerung, Logik und Robotik
