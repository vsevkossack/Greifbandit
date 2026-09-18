#include <ESP32Servo.h>

Servo s[6];
int pos[6] = {90, 90, 90, 90, 90, 90};

// ESP32-S3 Servo-Pins
// Reale Hardware-Reihenfolge:
// Servo 0 = Finger -> GPIO 4
// Servo 1 = Handgelenk -> GPIO 5
// Servo 2 = Ellbogen 1 -> GPIO 6
// Servo 3 = Ellbogen 2 -> GPIO 7
// Servo 4 = Schulter -> GPIO 8
// Servo 5 = Schulter drehen -> GPIO 9
const int SERVO_PINS[6] = {4, 5, 6, 7, 8, 9};
const int SERVO_FALLBACK_PINS[6] = {4, 5, 6, 7, 18, 19};
const int SERVO_COUNT = 6;

// HW-95 / Motorsteuerung (Tank-Style: links/rechts separat)
// Wähle hier freie GPIOs, die nicht in SERVO_PINS liegen.
const int MOTOR_LEFT_FWD = 10;
const int MOTOR_LEFT_BWD = 11;
const int MOTOR_RIGHT_FWD = 12;
const int MOTOR_RIGHT_BWD = 13;
const int MOTOR_LEFT_PWM = 14;
const int MOTOR_RIGHT_PWM = 15;

bool servoValid[6] = {false, false, false, false, false, false};
int activeServoCount = 0;

const int FINGER_SERVO_INDEX = 0;
// Mechanischer Sicherheitsbereich des Fingers: nicht 180°! Nimm den echten
// Öffnungswert deiner Mechanik und halte dich daran.
const int FINGER_OPEN_ANGLE = 25;
const int FINGER_CLOSE_ANGLE = 110;
const int FINGER_GRIP_ANGLE = 100;
const int FINGER_STEP_SIZE = 5;
const int FINGER_STEP_DELAY_MS = 40;
const int FINGER_MAX_STEP_COUNT = 18;
const int SERVO_STEP_SIZE = 3;
const int SERVO_STEP_DELAY_MS = 25;
bool fingerClosed = false;

bool tryAttachServoAtPin(int index, int pin) {
  if (pin < 0 || pin > 47) {
    Serial.printf("Servo %d: ungültiger GPIO %d\n", index, pin);
    return false;
  }

  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);

  s[index].setPeriodHertz(50);
  bool ok = s[index].attach(pin, 500, 2400);

  if (!ok) {
    Serial.printf("Servo %d an GPIO %d nicht nutzbar.\n", index, pin);
    return false;
  }

  servoValid[index] = true;
  activeServoCount++;
  s[index].write(pos[index]);
  Serial.printf("Servo %d an GPIO %d erfolgreich aktiviert.\n", index, pin);
  return true;
}

bool attachServo(int index) {
  if (index < 0 || index >= SERVO_COUNT) {
    Serial.printf("Servo %d: ungültiger Index\n", index);
    return false;
  }

  const int primaryPin = SERVO_PINS[index];
  if (tryAttachServoAtPin(index, primaryPin)) {
    return true;
  }

  const int fallbackPin = SERVO_FALLBACK_PINS[index];
  if (fallbackPin != primaryPin && fallbackPin >= 0 && fallbackPin <= 47) {
    Serial.printf("Servo %d: wechsle auf sicheren Fallback-GPIO %d\n", index, fallbackPin);
    if (tryAttachServoAtPin(index, fallbackPin)) {
      return true;
    }
  }

  servoValid[index] = false;
  Serial.printf("Servo %d wurde deaktiviert, weil kein passender GPIO verfügbar ist.\n", index);
  return false;
}

void setup() {
  Serial.begin(115200);
  activeServoCount = 0;

  pinMode(MOTOR_LEFT_FWD, OUTPUT);
  pinMode(MOTOR_LEFT_BWD, OUTPUT);
  pinMode(MOTOR_RIGHT_FWD, OUTPUT);
  pinMode(MOTOR_RIGHT_BWD, OUTPUT);
  pinMode(MOTOR_LEFT_PWM, OUTPUT);
  pinMode(MOTOR_RIGHT_PWM, OUTPUT);

  analogWrite(MOTOR_LEFT_PWM, 0);
  analogWrite(MOTOR_RIGHT_PWM, 0);
  stopRobot();

  for (int i = 0; i < SERVO_COUNT; i++) {
    attachServo(i);
  }

  Serial.printf("Aktive Servos: %d von %d\n", activeServoCount, SERVO_COUNT);
  delay(500);
  printHelp();
}

void loop() {
  if (Serial.available() > 0) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    executeCommand(cmd);
  }
}

void printHelp() {
  Serial.println("---- Servo Controller ----");
  Serial.println("Befehle:");
  Serial.println("  HOME     -> alle Servos auf 90");
  Serial.println("  START    -> Position 1");
  Serial.println("  GRAB     -> Position 2");
  Serial.println("  LIFT     -> Position 3");
  Serial.println("  DROP     -> Position 4");
  Serial.println("  SEQ      -> Servos nacheinander bewegen");
  Serial.println("  OPEN     -> Finger bis mechanisch offen");
  Serial.println("  CLOSE    -> Finger bis geschlossen");
  Serial.println("  GRIP     -> Finger kontrolliert greifen");
  Serial.println("  AUTO     -> adaptives Greifen mit sicheren Schritten");
  Serial.println("  RELEASE  -> Finger wieder offen");
  Serial.println("  P 30 60 90 120 150 90");
  Serial.println("           -> alle Servos gleichzeitig setzen");
  Serial.println("  S 2 45   -> Servo 2 auf 45");
  Serial.println("  M 2 45   -> gleicher Befehl wie S: einzelner Motor");
  Serial.println("  F        -> vorwaerts");
  Serial.println("  B        -> rueckwaerts");
  Serial.println("  L        -> links drehen");
  Serial.println("  R        -> rechts drehen");
  Serial.println("  STOP     -> motoren aus");
  Serial.println("  HELP     -> Hilfe anzeigen");
  Serial.println("-------------------------");
}

void stopRobot() {
  digitalWrite(MOTOR_LEFT_FWD, LOW);
  digitalWrite(MOTOR_LEFT_BWD, LOW);
  digitalWrite(MOTOR_RIGHT_FWD, LOW);
  digitalWrite(MOTOR_RIGHT_BWD, LOW);
  analogWrite(MOTOR_LEFT_PWM, 0);
  analogWrite(MOTOR_RIGHT_PWM, 0);
  Serial.println("Roboter gestoppt");
}

void moveRobot(int leftDir, int rightDir, int speedLeft, int speedRight) {
  // leftDir / rightDir: 1 vorwaerts, -1 rueckwaerts, 0 stop
  digitalWrite(MOTOR_LEFT_FWD, leftDir == 1 ? HIGH : LOW);
  digitalWrite(MOTOR_LEFT_BWD, leftDir == -1 ? HIGH : LOW);
  digitalWrite(MOTOR_RIGHT_FWD, rightDir == 1 ? HIGH : LOW);
  digitalWrite(MOTOR_RIGHT_BWD, rightDir == -1 ? HIGH : LOW);

  analogWrite(MOTOR_LEFT_PWM, abs(speedLeft));
  analogWrite(MOTOR_RIGHT_PWM, abs(speedRight));
}

void executeCommand(String command) {
  if (command.length() == 0) {
    return;
  }

  command.toUpperCase();

  if (command == "HOME") {
    setAllServos(90, 90, 90, 90, 90, 90);
    Serial.println("Home gesetzt");
    return;
  }

  if (command == "START") {
    setAllServos(90, 90, 90, 90, 90, 90);
    Serial.println("Start gesetzt");
    return;
  }

  if (command == "GRAB") {
    setAllServos(30, 60, 90, 120, 150, 90);
    Serial.println("Grab gesetzt");
    return;
  }

  if (command == "LIFT") {
    setAllServos(50, 75, 110, 100, 130, 80);
    Serial.println("Lift gesetzt");
    return;
  }

  if (command == "DROP") {
    setAllServos(150, 120, 90, 60, 30, 90);
    Serial.println("Drop gesetzt");
    return;
  }

  if (command == "SEQ") {
    int seqValues[6] = {90, 90, 90, 90, 90, 90};
    moveServoSequenceDescending(seqValues, 250);
    Serial.println("Servos in Reihenfolge 9->4 nacheinander bewegt");
    return;
  }

  if (command == "OPEN") {
    moveFingerTo(FINGER_OPEN_ANGLE);
    Serial.println("Finger offen");
    return;
  }

  if (command == "CLOSE") {
    moveFingerTo(FINGER_CLOSE_ANGLE);
    Serial.println("Finger geschlossen");
    return;
  }

  if (command == "GRIP") {
    moveFingerTo(FINGER_GRIP_ANGLE);
    Serial.println("Finger greift kontrolliert zu");
    return;
  }

  if (command == "AUTO") {
    adaptiveGripFinger();
    return;
  }

  if (command == "RELEASE") {
    moveFingerTo(FINGER_OPEN_ANGLE);
    Serial.println("Finger freigegeben");
    return;
  }

  if (command == "F") {
    moveRobot(1, 1, 200, 200);
    Serial.println("Vorwaerts");
    return;
  }

  if (command == "B") {
    moveRobot(-1, -1, 200, 200);
    Serial.println("Rueckwaerts");
    return;
  }

  if (command == "L") {
    moveRobot(-1, 1, 180, 180);
    Serial.println("Links");
    return;
  }

  if (command == "R") {
    moveRobot(1, -1, 180, 180);
    Serial.println("Rechts");
    return;
  }

  if (command == "STOP") {
    stopRobot();
    return;
  }

  if (command == "HELP" || command == "?") {
    printHelp();
    return;
  }

  if (command.startsWith("P ")) {
    setAllFromSerial(command);
    return;
  }

  if (command.startsWith("S ") || command.startsWith("M ")) {
    setSingleServoFromSerial(command);
    return;
  }

  Serial.println("Unbekannter Befehl");
  printHelp();
}

int clampServoAngle(int index, int angle) {
  if (index == 3) { // GPIO 7 / Servo 3
    return constrain(angle, 0, 40);
  }
  return constrain(angle, 0, 180);
}

void setAllServos(int a, int b, int c, int d, int e, int f) {
  pos[0] = clampServoAngle(0, a);
  pos[1] = clampServoAngle(1, b);
  pos[2] = clampServoAngle(2, c);
  pos[3] = clampServoAngle(3, d);
  pos[4] = clampServoAngle(4, e);
  pos[5] = clampServoAngle(5, f);

  for (int i = 0; i < 6; i++) {
    if (!servoValid[i]) {
      continue;
    }
    s[i].write(pos[i]);
  }
}

void moveServoSequenceDescending(int values[6], int stepDelayMs) {
  // Reihenfolge: Servo 5 -> 4 -> 3 -> 2 -> 1 -> 0
  // Das entspricht den Pins 9, 8, 7, 6, 5, 4.
  for (int i = 5; i >= 0; i--) {
    if (!servoValid[i]) {
      continue;
    }
    pos[i] = clampServoAngle(i, values[i]);
    s[i].write(pos[i]);
    Serial.printf("Servo %d auf %d gesetzt\n", i, pos[i]);
    delay(stepDelayMs);
  }
}

void moveServoSmooth(int index, int targetAngle, int stepSize, int stepDelayMs) {
  if (index < 0 || index >= SERVO_COUNT) {
    Serial.println("Servo-Index außerhalb des gültigen Bereichs.");
    return;
  }

  if (!servoValid[index]) {
    Serial.printf("Servo %d ist nicht aktiv.\n", index);
    return;
  }

  targetAngle = constrain(targetAngle, 0, 180);
  int current = pos[index];

  if (targetAngle > current) {
    for (int angle = current; angle <= targetAngle; angle += stepSize) {
      pos[index] = angle;
      s[index].write(pos[index]);
      delay(stepDelayMs);
    }
  } else {
    for (int angle = current; angle >= targetAngle; angle -= stepSize) {
      pos[index] = angle;
      s[index].write(pos[index]);
      delay(stepDelayMs);
    }
  }

  pos[index] = targetAngle;
  s[index].write(pos[index]);
}

void moveFingerTo(int targetAngle) {
  if (!servoValid[FINGER_SERVO_INDEX]) {
    Serial.println("Finger-Servo ist nicht aktiv.");
    return;
  }

  targetAngle = constrain(targetAngle, FINGER_OPEN_ANGLE, FINGER_CLOSE_ANGLE);
  moveServoSmooth(FINGER_SERVO_INDEX, targetAngle, SERVO_STEP_SIZE, SERVO_STEP_DELAY_MS);

  fingerClosed = (pos[FINGER_SERVO_INDEX] >= FINGER_GRIP_ANGLE);
  Serial.printf("Finger auf %d Grad -> %s\n",
                pos[FINGER_SERVO_INDEX],
                fingerClosed ? "gegriffen" : "offen");
}

void adaptiveGripFinger() {
  if (!servoValid[FINGER_SERVO_INDEX]) {
    Serial.println("Finger-Servo ist nicht aktiv.");
    return;
  }

  int current = pos[FINGER_SERVO_INDEX];
  int target = FINGER_GRIP_ANGLE;

  Serial.println("Adaptive Greifung gestartet: sichere kleine Schritte");

  for (int step = 0; step < FINGER_MAX_STEP_COUNT; step++) {
    if (current >= target) {
      break;
    }

    current = min(current + FINGER_STEP_SIZE, target);
    pos[FINGER_SERVO_INDEX] = constrain(current, FINGER_OPEN_ANGLE, FINGER_CLOSE_ANGLE);
    s[FINGER_SERVO_INDEX].write(pos[FINGER_SERVO_INDEX]);
    Serial.printf("Finger schliesst -> %d Grad\n", pos[FINGER_SERVO_INDEX]);
    delay(FINGER_STEP_DELAY_MS);
  }

  if (pos[FINGER_SERVO_INDEX] >= FINGER_GRIP_ANGLE) {
    fingerClosed = true;
    Serial.println("Objekt sicher erfasst: Griffpunkt erreicht");
  } else {
    fingerClosed = false;
    Serial.println("Greifung abgebrochen: sicherer Endpunkt erreicht");
  }
}

void setSingleServoFromSerial(String command) {
  char buffer[64];
  command.toCharArray(buffer, sizeof(buffer));

  char *token = strtok(buffer, " ");
  if (token == NULL) {
    Serial.println("Format: S <servoIndex> <winkel>");
    return;
  }

  int index = atoi(token);
  token = strtok(NULL, " ");
  if (token == NULL) {
    Serial.println("Format: S <servoIndex> <winkel>");
    return;
  }

  int angle = atoi(token);

  if (index < 0 || index >= 6) {
    Serial.println("Servo-Index muss zwischen 0 und 5 liegen");
    return;
  }

  if (!servoValid[index]) {
    Serial.printf("Servo %d ist auf diesem Board nicht nutzbar.\n", index);
    return;
  }

  pos[index] = clampServoAngle(index, angle);
  s[index].write(pos[index]);

  if (index == FINGER_SERVO_INDEX) {
    fingerClosed = (pos[index] >= FINGER_GRIP_ANGLE);
  }

  Serial.print("Servo ");
  Serial.print(index);
  Serial.print(" -> ");
  Serial.println(pos[index]);
}

void setAllFromSerial(String command) {
  char buffer[64];
  command.toCharArray(buffer, sizeof(buffer));

  char *token = strtok(buffer, " ");
  if (token == NULL) {
    Serial.println("Format: P <w1> <w2> <w3> <w4> <w5> <w6>");
    return;
  }

  int values[6] = {0, 0, 0, 0, 0, 0};
  int count = 0;

  while (token != NULL && count < 6) {
    values[count] = atoi(token);
    count++;
    token = strtok(NULL, " ");
  }

  if (count != 6) {
    Serial.println("Es müssen genau 6 Winkel angegeben werden.");
    return;
  }

  setAllServos(values[0], values[1], values[2], values[3], values[4], values[5]);
  Serial.println("Alle Servos gesetzt");
}
