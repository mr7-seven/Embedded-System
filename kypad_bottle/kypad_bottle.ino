#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x20, 16, 2);

#define LED_START 2
#define LED_STOP 3
#define BUTTON_START 4
#define BUTTON_STOP 5
const byte ROWS = 4;
const byte COLS = 3;

char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {12, 11, 10, 9};
byte colPins[COLS] = {8, 7, 6};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
char key;


typedef enum {
  VIEW,
  SET
} Menu;
Menu mode;

char buf[32];
int volume, set_volume;
int botol, set_botol;
bool START, STOP, CR, last_cr; 

void setup() {
  Serial.begin(9600);
  pinMode(LED_START, OUTPUT);
  pinMode(LED_STOP, OUTPUT);
  pinMode(BUTTON_START, INPUT_PULLUP);
  pinMode(BUTTON_STOP, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
}

void loop() {

  if (mode == VIEW) {
    key = keypad.getKey();
    START = !digitalRead(BUTTON_START);
    STOP = !digitalRead(BUTTON_STOP);

    CR = (START || CR) && (!STOP);
    digitalWrite(LED_START, CR);
    digitalWrite(LED_STOP, !CR);

    lcd.setCursor(0, 0);
    sprintf(buf, "Volume : %d mL", volume);
    lcd.print(buf);
    lcd.setCursor(0, 1);
    sprintf(buf, "Total  : %d Botol", botol);
    lcd.print(buf);
    if (key == '*') {
      mode = SET;
      lcd.clear();
    }

    if (CR != last_cr) {
      Serial.print(CR);
      Serial.print(",");
      Serial.print(volume);
      Serial.print(",");
      Serial.println(botol);
      delay(100);
    }
    last_cr = CR;
  }
  else if (mode == SET) {
    do {
      key = keypad.getKey();
      if (key >= '0' && key <= '9') {
        set_volume = set_volume * 10 + (key - '0');
        volume = set_volume;
      }

      if (key == '#') set_volume = 0;
      lcd.setCursor(0, 0);
      sprintf(buf, "SET  : %.4d mL", set_volume);
      lcd.print(buf);
    } while (key != '*');

    do {
      key = keypad.getKey();
      if (key >= '0' && key <= '9') {
        set_botol = set_botol * 10 + (key - '0');
        botol = set_botol;
      }

      if (key == '#') set_botol = 0;
      lcd.setCursor(0, 1);
      sprintf(buf, "SET  : %.2d Botol", set_botol);
      lcd.print(buf);
    } while (key != '*');

    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("SAVING...!");
    for (byte i = 0; i < 16; i++) {
      lcd.setCursor(i, 1);
      lcd.print(">");
      delay(10);
    }

    set_volume = 0;
    set_botol = 0;
    lcd.clear();
    mode = VIEW;
  }
}
