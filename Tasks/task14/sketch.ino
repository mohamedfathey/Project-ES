#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte ROWS = 4; // four rows
const byte COLS = 4; // four columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; // connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int maxInput = 32;
const int halfMaxInput = maxInput / 2;

char inputArray[maxInput]; // Array to store entered characters
int arrayIndex = 0; // Index to keep track of the current position in the array
char firstLine[halfMaxInput];
char secondLine[halfMaxInput];

void setup() {
  lcd.init();
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    if (key == 'C') {
      // Clear the array
      clearArray();
      clearLCD();
    } else if (key == 'D') {
      // Delete the last character from the array
      deleteLastCharacter();
    } else {
      // Add the key to the array if it's not full
      if (arrayIndex < maxInput) {
        addCharacter(key);
      } else {
        displayErrorMessage("Input is full");
      }
    }

    splitArray(); // Split the input array into firstLine and secondLine
    printToLCD();
  }
}

void addCharacter(char key) {
  if (arrayIndex < maxInput) {
    inputArray[arrayIndex++] = key;
  }
}

void deleteLastCharacter() {
  if (arrayIndex > 0) {
    arrayIndex--;
    inputArray[arrayIndex] = '\0';
  }
}

void clearArray() {
  arrayIndex = 0;
  for (int i = 0; i < maxInput; i++) {
    inputArray[i] = '\0';
  }
}

void splitArray() {
  for (int i = 0; i < halfMaxInput; i++) {
    firstLine[i] = inputArray[i];
    secondLine[i] = inputArray[i + halfMaxInput];
  }
}

void printToLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(firstLine);
  lcd.setCursor(0, 1);
  lcd.print(secondLine);
}

void displayErrorMessage(const char *message) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Error: ");
  lcd.setCursor(0,1);
  lcd.print(message);
  delay(2000);
  lcd.clear();
}

void clearLCD(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Input ");
  lcd.setCursor(0,1);
  lcd.print("Cleared");
  delay(2000);
  lcd.clear();
}