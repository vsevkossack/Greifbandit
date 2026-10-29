# Firmware

## Ziel

Die Firmware steuert den Roboter und die Hardware. Sie ist die Steuerzentrale für Bewegung, Arm, Greifer und Kommunikation zwischen den Systemen.

Der Fokus liegt auf der realen Ausführung auf dem ESP bzw. den eingebetteten Controllern.

---

## 🔧 Aufgaben

- Motorsteuerung
- Ansteuerung des Roboterarms
- Steuerung des Greifers
- Verbindung mit Sensoren und Peripherie
- Zustandslogik für Bewegung und Aktionen
- Datenaustausch mit dem Kamera-Subsystem und anderen ESPs

---

## 🧩 Aufgabenteilung

Die Firmware ist nicht für die Bildverarbeitung zuständig.

- Kamera-ESP: Livebild über WLAN an den PC
- Steuer-ESP: Motorik, Arm, Greifer, Logik

Damit bleibt die Trennung sauber:

- Kamerasystem = Sehen
- Firmware = Handeln

---

## 📦 Ergebnis

Die Roboterplattform kann über die Firmware zuverlässig gesteuert werden.

- vorwärts / rückwärts
- drehen
- Arm bewegen
- Greifer kontrollieren
- Zustände ausführen

---

## ✅ Definition of Done

- Die Firmware startet zuverlässig auf dem ESP.
- Motoren und Aktoren reagieren korrekt.
- Der Roboterarm kann in definierte Positionen fahren.
- Der Greifer kann geöffnet und geschlossen werden.
- Die Steuerlogik arbeitet ohne Bildverarbeitung im Hintergrund.
- Die Kamera liefert nur Bilddaten; die Firmware übernimmt die echte Aktion.

---

## Hinweis

Diese Firmware bildet die Basis für den nächsten Schritt: autonome Bewegung und Greiflogik mit der Kamera als Sensorik.
