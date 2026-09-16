#include <ESP32Servo.h>

Servo s[6];
int pos[6] = {90, 90, 90, 90, 90, 90};

// ESP32-S3 Servo-Pins
// GPIO 9, 10 und 11 sind auf vielen ESP32-S3-Boards Flash-/Boot-Pins und
// daher für Servos nicht zuverlässig. Saubere PWM-Pins sind z. B. 12..17.
// Wenn am Board andere Pins verwendet werden, hier gezielt umkonfigurieren.
const int SERVO_PINS[6] = {12, 13, 14, 15, 16, 17};
const int SERVO_COUNT = 6;

bool attachServo(int index) {
  const int pin = SERVO_PINS[index];

  if (pin < 0 || pin > 47) {
    Serial.printf("Servo %d: ungültiger GPIO %d\n", index, pin);
    return false;
  }

  s[index].setPeriodHertz(50);
  bool ok = s[index].attach(pin, 500, 2400);

  if (!ok) {
    Serial.printf("Servo %d konnte nicht an GPIO %d gehängt werden.\n", index, pin);
    return false;
  }

  s[index].write(pos[index]);
  return true;
}

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < SERVO_COUNT; i++) {
    attachServo(i);
  }

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
  Serial.println("  P 30 60 90 120 150 90");
  Serial.println("           -> alle Servos einzeln setzen");
  Serial.println("  S 2 45   -> Servo 2 auf 45");
  Serial.println("  HELP     -> Hilfe anzeigen");
  Serial.println("-------------------------");
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

  if (command == "HELP" || command == "?") {
    printHelp();
    return;
  }

  if (command.startsWith("P ")) {
    setAllFromSerial(command);
    return;
  }

  if (command.startsWith("S ")) {
    setSingleServoFromSerial(command);
    return;
  }

  Serial.println("Unbekannter Befehl");
  printHelp();
}

void setAllServos(int a, int b, int c, int d, int e, int f) {
  pos[0] = constrain(a, 0, 180);
  pos[1] = constrain(b, 0, 180);
  pos[2] = constrain(c, 0, 180);
  pos[3] = constrain(d, 0, 180);
  pos[4] = constrain(e, 0, 180);
  pos[5] = constrain(f, 0, 180);

  for (int i = 0; i < 6; i++) {
    s[i].write(pos[i]);
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

  pos[index] = constrain(angle, 0, 180);
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
