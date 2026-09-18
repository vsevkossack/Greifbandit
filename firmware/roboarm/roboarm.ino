#include <ESP32Servo.h>

Servo s[6];
int pos[6] = {90, 90, 90, 90, 90, 90};

// ESP32-S3 Servo-Pins
// Reihenfolge wie gewünscht: 8, 7, 6, 5, 4, 9
// Pin 8 ist auf vielen ESP32-S3-Boards problematisch und kann den Servo-Stack
// blockieren. Wenn ein Servo nicht nutzbar ist, werden die übrigen trotzdem
// weiter nacheinander angesteuert.
const int SERVO_PINS[6] = {8, 7, 6, 5, 4, 9};
const int SERVO_FALLBACK_PINS[6] = {18, 19, 6, 5, 4, 9};
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
    int seqValues[6] = {15, 30, 60, 90, 120, 150};
    moveMultiServoSequence(seqValues, 120);
    Serial.println("Servos nacheinander bewegt");
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

void moveMultiServoSequence(int values[6], int stepDelayMs) {
  for (int i = 0; i < 6; i++) {
    if (!servoValid[i]) {
      continue;
    }
    pos[i] = clampServoAngle(i, values[i]);
    s[i].write(pos[i]);
    delay(stepDelayMs);
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
