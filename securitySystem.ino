#include <Servo.h>
#include <Keypad.h>
#include <LiquidCrystal.h>

#define passwordLength 5
#define greenLedPin 10
#define redLedPin 12

Servo myServo;
bool doorOpen = false;
byte count = 0;

// LCD Configuration
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

// Keypad Configuration
char keyPressed;
char password[passwordLength] = "1234";
char userInput[passwordLength];
const byte columns = 4;
const byte rows = 4;

char keys[rows][columns] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[rows] = {0, 1, 2, 3};
byte columnPins[columns] = {4, 5, 6, 7};
Keypad myKeyPad = Keypad(makeKeymap(keys), rowPins, columnPins, rows, columns);

void setup() {
  closeServo();
  lcd.begin(16, 2);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  setLocked(true);
  lcd.print(" Door is locked");
  delay(2000);
  lcd.clear();
  digitalWrite(redLedPin, HIGH);
  digitalWrite(greenLedPin, LOW);
}

void loop() {
  if (doorOpen == true) {
    keyPressed = myKeyPad.getKey();
    if (keyPressed == '#') {
      setLocked(true);
      lcd.clear();
      closeServo();
      lcd.print(" Door is closed");
      delay(3000);
      doorOpen = false;
    }
  } else {
    openDoor();
  }
}

void loadingMessage(char message[]) {
  lcd.setCursor(0, 1);
  lcd.print(message);
  for (int i = 0; i < 15; i++) {
    delay(300);
    lcd.print(".");
  }
}

void clearSceen() {
  while (count != 0) {
    userInput[count--] = 0;
  }
  return;
}

void closeServo() {
  myServo.attach(9);
  for (int position = 90; position >= 0; position -= 10) {
    myServo.write(position);
    delay(15);
  }
}

void openServo() {
  myServo.attach(9);
  for (int position = 0; position <= 90; position += 10) {
    myServo.write(position);
    delay(15);
  }
}

void openDoor() {
  lcd.setCursor(0, 0);
  lcd.print("Enter Password: ");

  keyPressed = myKeyPad.getKey();
  if (keyPressed) {
    if (!isKeyValid(keyPressed)) {
      lcd.clear();
      lcd.print("Invalid Key!");
      delay(1000);
      lcd.clear();
      clearSceen();
      return;
    }

    userInput[count] = keyPressed;
    lcd.setCursor(count, 1);
    lcd.print(userInput[count]);
    count++;
  }

  if (count == passwordLength - 1) {
    if (!strcmp(userInput, password)) {
      setLocked(false);
      lcd.clear();
      lcd.print(" ** Verifying **");
      delay(1000);
      lcd.clear();
      openServo();
      lcd.print("  Door is Open ");
      doorOpen = true;
      delay(1000);
      loadingMessage(" ");
      lcd.clear();
      lcd.print("  Door is Closed! ");
      delay(1000);
      closeServo();
      doorOpen = false;
    } else {
      lcd.clear();
      lcd.print("  Wrong Password! ");
      doorOpen = false;
    }
    delay(1000);
    lcd.clear();
    clearSceen();
  }
}

void setLocked(int locked) {
  if (locked) {
    digitalWrite(redLedPin, HIGH);
    digitalWrite(greenLedPin, LOW);
  } else {
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, HIGH);
  }
}

bool isKeyValid(char key) {
  char invalidKeys[] = {'A', 'B', 'C', 'D', '*', '#'};
  for (int i = 0; i < sizeof(invalidKeys) / sizeof(char); i++) {
    if (invalidKeys[i] == key) {
      return false;
    }
  }
  return true;
}
