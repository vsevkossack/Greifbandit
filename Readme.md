# Greifbandit 🤖

## Der freundliche Schatzsucher für den Alltag

**_Chaos ade - Greifbandit im Haus_**

Greifbandit ist ein experimenteller mobiler Assistenzroboter, der verlorene oder abgelegte Gegenstände im Haushalt finden, greifen und zurückbringen kann.

Der Name ist eine spielerische Kombination aus:

- **Greifer** – die Fähigkeit, Gegenstände zu greifen
- **Bandit** – ein kleiner humorvoller "Dieb", der aber nur Gutes tut:
  Er stiehlt keine Dinge, sondern sammelt verlorene "Schätze" und hilft Menschen im Alltag.

**Greifbandit hilft dem Menschen kleine alltägliche Probleme zu lösen** – wie das Finden einer Fernbedienung, verlorener Schlüssel oder einer verlegten Brille. Mit seinem Roboterarm kann er diese Gegenstände greifen und zurückbringen, ohne dass der Mensch danach suchen und sich bücken muss.

---

# Inhaltsverzeichnis

- [Mission](#mission)
- [Vision](#vision)
- [MVP Ziel](#mvp-ziel)
- [Systemübersicht](#systemübersicht)
- [Technologie](#technologie)
- [Product Life Cycle](#product-life-cycle)
- [Entwicklungs-Meilensteine](#entwicklungs-meilensteine)
- [MVP – Sock Pickup](#mvp--sock-pickup)
- [Agile Entwicklung](#agile-entwicklung)
- [Projektstruktur](#projektstruktur)
- [Langfristige Vision](#langfristige-vision)

---

# Mission

**Greifbandit hilft dem Menschen, kleine alltägliche Probleme zu lösen** – wie das Finden einer Fernbedienung, verlorener Schlüssel oder einer verlegten Brille. Mit seinem Roboterarm kann er diese Gegenstände greifen und zurückbringen, ohne dass der Mensch danach suchen und sich bücken muss.

---

# Vision

Ein autonomer Haushaltsroboter, der selbstständig:

- Gegenstände erkennt
- deren Position bestimmt
- sich dorthin bewegt
- Gegenstände greift
- sie zurückbringt

Beispiele:

- Socken
- Fernbedienung
- Brille
- Schlüssel
- kleine Alltagsgegenstände

---

# MVP Ziel

## "Greifbandit findet einen echten Socken"

Das erste Minimal Viable Product:

> Der Roboter erkennt einen echten einzelnen Socken auf einem sauberen Boden, fährt autonom dorthin und greift ihn ohne menschliche Hilfe.

Nicht Bestandteil des MVP:

- komplette Wohnung reinigen
- mehrere Räume
- automatische Ladestation
- Sprachsteuerung
- perfekte Navigation

Der Fokus liegt auf der kompletten technischen Kette:

**Sehen → Verstehen → Navigieren → Greifen**

---

# Systemübersicht
```
          Stereo Kamera
               |
               |
      Computer Vision System
               |
               |
          ROS2 Steuerung
               |
    +----------+----------+
    |                     |
  Mobile Plattform Roboterarm
    |                     |
Motoren Greifer    Encoder Aktoren
```

---

# Technologie

## Hardware

Geplante Komponenten:

- Mobile Roboterplattform
- Differential Drive mit zwei Antriebsrädern
- Stützrad oder alternative 4-Rad-Plattform
- Roboterarm
- Greifer
- Stereo-Kamera
- ESP32 / STM32 Controller
- Onboard Computer

## Software

- Linux
- ROS2
- Computer Vision
- Stereo Vision
- Robot Control
- GitLab Repository Management

---

# Product Life Cycle

- [ ] Phase 1 – Vision
- [ ] Phase 2 – Konzeptvalidierung
- [ ] Phase 3 – Prototype
- [ ] Phase 4 – MVP
- [ ] Phase 5 – Alpha
- [ ] Phase 6 – Beta
- [ ] Phase 7 – Version 1.0
- [ ] Phase 8 – Product Growth

## Phase 1 – Vision

Ziel:

Produktidee und Benutzerproblem definieren.

Ergebnisse:

- Produktvision
- Zielgruppe
- technische Anforderungen
- MVP Definition

---

## Phase 2 – Konzeptvalidierung

Ziel:

Technische Machbarkeit nachweisen.

Aktivitäten:

- Mechanisches Konzept
- Stabilitätsanalyse
- Auswahl der Hardware
- Auswahl der Sensorik

Ergebnis:

Die technische Umsetzung ist realistisch.

---

## Phase 3 – Prototype

Ziel:

Erster funktionsfähiger Roboter-Prototyp.

Komponenten:

- mobile Plattform
- Elektronik
- Roboterarm
- Kamera

Noch ohne vollständige Autonomie.

---

## Phase 4 – MVP

Der Roboter kann:

1. Objekt erkennen
2. Position bestimmen
3. hinfahren
4. greifen

---

## Phase 5 – Alpha

Erweiterungen:

- verschiedene Gegenstände
- verschiedene Lichtbedingungen
- verschiedene Bodenarten
- robustere Greifstrategien

---

## Phase 6 – Beta

Tests mit realen Benutzern:

- Zuverlässigkeit
- Bedienbarkeit
- Fehleranalyse
- Benutzerfeedback

---

## Phase 7 – Version 1.0

Stabile Produktversion:

- zuverlässige Navigation
- sichere Greiffunktion
- einfache Bedienung
- Fehlerbehandlung

---

## Phase 8 – Product Growth

Neue Funktionen:

- mehrere Gegenstände sammeln
- Smart Home Integration
- Sprachsteuerung
- Benutzerprofile

---

# Entwicklungs-Meilensteine

- [ ] M0 – Architektur
- [ ] M1 – Mechanical Proof of Concept
- [ ] M2 – Electronics & Bring-up
- [ ] M3 – Mobile Plattform
- [ ] M4 – Manipulator
- [ ] M5 – Stereo Vision
- [ ] M6 – Integration

## M0 – Architektur

Ziel:

Grundlagen schaffen.

Tasks:

- Systemarchitektur
- Hardwareauswahl
- Softwarestruktur
- Repository Setup

Definition of Done:

Projektstruktur und technische Architektur sind dokumentiert.

---

## M1 – Mechanical Proof of Concept

Ziel:

Nachweis der mechanischen Machbarkeit.

Tests:

- Stabilität der Plattform
- Schwerpunkt
- Belastbarkeit
- Montage des Roboterarms
- Vibrationen

Definition of Done:

Der Roboterarm kann sicher auf der mobilen Plattform betrieben werden.

---

## M2 – Electronics & Bring-up

Ziel:

Erste vollständige Hardwarekontrolle.

Funktionen:

- Motorsteuerung
- Armsteuerung
- Greifersteuerung
- Telemetrie
- Not-Aus
- Fernsteuerung

Definition of Done:

Der komplette Roboter kann manuell gesteuert werden.

---

## M3 – Mobile Plattform

Ziel:

Fahrfähige autonome Basis.

Funktionen:

- Vorwärts / Rückwärts
- Drehen
- Geschwindigkeitsregelung
- Encoder
- Odometry

---

## M4 – Manipulator

Ziel:

Greifen ohne Computer Vision.

Funktionen:

- Home Position
- Pick Position
- Place Position
- Greifersteuerung

---

## M5 – Stereo Vision

Ziel:

Objekte erkennen und Position bestimmen.

Funktionen:

- Kamerakalibrierung
- Stereo Rectification
- Tiefenberechnung
- Objekterkennung

---

## M6 – Integration

Ziel:

Alle Subsysteme verbinden.

Ablauf:
Objekt erkennen
↓
Position berechnen
↓
Roboter fährt hin
↓
Arm greift Objekt

---

# MVP – Sock Pickup

Definition of Done:

✅ echter Socken  
✅ echter Boden  
✅ autonome Erkennung  
✅ autonome Annäherung  
✅ erfolgreicher Greifvorgang  

---

# Agile Entwicklung

Projektmanagement:

- GitLab Issues
- GitLab Issue Boards
- Milestones
- Labels
- Repository Dokumentation

Kanban Board:
Backlog
↓
Ready
↓
In Progress
↓
Testing
↓
Done

---

# Projektstruktur
Greifbandit/

├── README.md
├── Docs/
├── mechanics/
├── electronics/
├── firmware/
├── ros2/
├── vision/
├── manipulator/
└── tests/


---

# Langfristige Vision

Greifbandit soll ein persönlicher Haushaltshelfer werden.

Ein kleiner Roboter, der Menschen unterstützt und alltägliche Aufgaben erleichtert.

Ein freundlicher "Bandit", der nicht Dinge stiehlt, sondern:

**die kleinen Probleme des Alltags.**





