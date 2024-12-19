#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <math.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1', '2', '3', '/'},
  {'4', '5', '6', '*'},
  {'7', '8', '9', '-'},
  {'C', '0', '=', '+'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

String input = "";
float result = 0;
bool calculated = false;
float operandsStack[50];
char operatorsStack[50];
int operandIndex = 0;
int operatorIndex = 0;
bool clearScreen = true;

unsigned long lastKeyPressTime = 0;
const unsigned long screenClearDelay = 2000;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Calculator:");
  lcd.setCursor(0, 1);
  lcd.print("Press 'C' to Clear");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Input: ");
}

void loop() {
  char key = keypad.getKey();
  
  if (key) {
    lastKeyPressTime = millis();
    
    if (key == '=') {
      if (input.length() > 0 && !calculated) {
        result = evaluateExpression(input);
        lcd.setCursor(0, 1);
        lcd.print("Result: ");
        lcd.print(result, 2);
        calculated = true;
      }
    } else if (key == 'C') {
      clearScreen = true; 
      clearInput();
    } else if (isdigit(key) || key == '.' || (key == '-' && input.length() == 0)) {
      input += key;
      lcd.print(key);
    } else if (isOperator(key) && input.length() > 0 && !calculated) {
      input += key;
      lcd.print(key);
    } else if (key == '(') {
      input += key;
      lcd.print(key);
    } else if (key == ')') {
      input += key;
      lcd.print(key);
    }
  }

  
  if (clearScreen && millis() - lastKeyPressTime >= screenClearDelay) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Input: ");
    clearScreen = false; 
  }
}

bool isOperator(char key) {
  return (key == '+' || key == '-' || key == '*' || key == '/');
}

float fastSqrt(float x) {
  return sqrt(x);
}

float fastPow(float base, float exponent) {
  return pow(base, exponent);
}

float evaluateExpression(String expression) {
 
  int len = expression.length();
  char tokens[len + 1];
  expression.toCharArray(tokens, len + 1);

  
  operandIndex = 0;
  operatorIndex = 0;

  for (int i = 0; i < len; i++) {
    char token = tokens[i];
    if (isdigit(token) || token == '.') {
      float number = atof(tokens + i);
      operandsStack[operandIndex++] = number;
      while (i < len && (isdigit(tokens[i]) || tokens[i] == '.')) {
        i++;
      }
      i--;
    } else if (isOperator(token)) {
      while (operatorIndex > 0 && hasPrecedence(operatorsStack[operatorIndex - 1], token)) {
        float operand2 = operandsStack[--operandIndex];
        float operand1 = operandsStack[--operandIndex];
        char op = operatorsStack[--operatorIndex];
        operandsStack[operandIndex++] = applyOperator(op, operand1, operand2);
      }
      operatorsStack[operatorIndex++] = token;
    } else if (token == '(') {
      operatorsStack[operatorIndex++] = token;
    } else if (token == ')') {
      while (operatorIndex > 0 && operatorsStack[operatorIndex - 1] != '(') {
        float operand2 = operandsStack[--operandIndex];
        float operand1 = operandsStack[--operandIndex];
        char op = operatorsStack[--operatorIndex];
        operandsStack[operandIndex++] = applyOperator(op, operand1, operand2);
      }
      operatorIndex--; // Pop '('
    }
  }

  while (operatorIndex > 0) {
    float operand2 = operandsStack[--operandIndex];
    float operand1 = operandsStack[--operandIndex];
    char op = operatorsStack[--operatorIndex];
    operandsStack[operandIndex++] = applyOperator(op, operand1, operand2);
  }

  return operandsStack[0];
}

bool hasPrecedence(char op1, char op2) {
  if ((op1 == '*' || op1 == '/') && (op2 == '+' || op2 == '-')) {
    return true;
  }
  return false;
}

float applyOperator(char op, float operand1, float operand2) {
  switch (op) {
    case '+':
      return operand1 + operand2;
    case '-':
      return operand1 - operand2;
    case '*':
      return operand1 * operand2;
    case '/':
      if (operand2 != 0) {
        return operand1 / operand2;
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Error: Division by 0");
        delay(2000);
        clearInput();
        return 0;
      }
    default:
      return 0;
  }
}

void clearInput() {
  input = "";
  result = 0;
  calculated = false;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Input: ");
}