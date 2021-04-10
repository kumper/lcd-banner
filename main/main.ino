#include <LiquidCrystal.h>

LiquidCrystal lcd(8,9,4,5,6,7);

const String lSpawserwis  = "  SPAW-SERWIS   ";
//const String lMode0       = "  CZYSZCZENIE   ";
//const String lMode1       = "  POLEROWANIE   ";
//const String lMode2       = "  ZNAKOWANIE    ";
const String lMode0       = "CZYSZCZENIE   0%%";
const String lMode55      = "CZYSZCZENIE  55%%";
const String lMode75      = "CZYSZCZENIE  75%%";
const String lMode100     = "CZYSZCZENIE 100%%";
//const String lWorkReady   = "Gotowy do pracy ";
//const char*  lWork        = " Praca:    %3d%% ";

int resistorPin = A1;
int currentResistorValue = 0;
int lastResistorValue = currentResistorValue;
const int resistorMaxValue = 870;

int keyPadPin = A0;
int currentKeyPad = 0;
int oldKeyPad = currentKeyPad;

// 0 - CZYSZCZENIE
// 1 - POLEROWANIE
// 2 - ZNAKOWANIE
int mode = 0;
int mode0Pin = 11; //numer pinu na Czyszczenie
int mode1Pin = 12; //numer pinu na Polerowanie
int mode2Pin = 13; //numer pinu na Znakowanie

int banerClock = 10000; //co jaki czas (ms) ma się pojawiać baner Spaw-serwis
int banerDuration = 2000; // jak długo (ms) ma się wyświetlać baner

void setup() {
  int start = millis();
  Serial.begin(9600);
  Serial.println("Starting");
  
  lcd.begin(16, 2);

  pinMode(resistorPin, INPUT);
  pinMode(keyPadPin, INPUT);
  
  pinMode(mode0Pin, OUTPUT);
  pinMode(mode1Pin, OUTPUT);
  pinMode(mode2Pin, OUTPUT);

  screen0_animate();
  screen0_main();
//  screen1_readyToWork();
  int end = millis();
  
  Serial.println("Started in " + String(end - start) + " ms");
}

void loop() {
//  handleResistor();
  handleKeyPad();
  handleScreen0();
  handleScreen1();
}

//void handleResistor() {
//  const int tolerance = 10;
//  currentResistorValue = analogRead(resistorPin);
//  if ( abs(lastResistorValue - currentResistorValue) > tolerance ) {
//    lastResistorValue = currentResistorValue;
//    Serial.print("currentResistorValue: ");
//    Serial.println(currentResistorValue);
//    Serial.print("lastResistorValue: ");
//    Serial.println(lastResistorValue);
//  }
//}

void handleScreen0() {
  int currentMillis = millis() % banerClock;
  if ( (currentMillis / (banerClock - banerDuration)) > 0) {
    screen0_main();
    if ( (currentMillis / banerClock) > 0) {
      currentMillis = 0;
    }
  } 
  else {
//    if (mode == 0) {
//      screen0_mode0();
//    } else if (mode == 1) {
//      screen0_mode1();
//    } else if (mode == 2) {
//      screen0_mode2();
//    } else {
//      Serial.println("[ERROR] Unsupported mode: " + String(mode) + "; Reseting to 0"); 
//      mode = 0;
//    }
    handleScreen1();
  }
}

void handleScreen1() {
//  if (mapResistorValueToPercent() == 0) {
//    screen1_readyToWork();
//  } else {
//    screen1_work(); 
//  }
    if (mode == 0) {
      screen1_mode0();
    } else if (mode == 55) {
      screen1_mode55();
    } else if (mode == 75) {
      screen1_mode75();
    } else if (mode == 100) {
      screen1_mode100();
    } else {
      Serial.println("[ERROR] Unsupported mode: " + String(mode) + "; Reseting to 0"); 
      mode = 0;
    }
}

void screen0_animate() {
  int delayTime = 20;
  String label = lSpawserwis;
  lcd.clear();
  int i, j;
  for (i = 0; i < 16; i++) {
    char currentChar = label[i];
    for (j = 15; j > i-1; j--) {
      lcd.print(" ");
      lcd.setCursor(j, 0);
      lcd.print(currentChar);
      if (currentChar != ' ') delay(delayTime);
    }
    if ( (i > 5) && (i < 13) && (i % 2 == 0) ) delayTime *= 2;
  }
  screen0_main();
  delay(1000);
}

void screen0_main() {
  lcd.setCursor(0, 0);
  lcd.print(String(lSpawserwis));
}

void screen1_mode100() {
  lcd.setCursor(0, 1);
  lcd.print(String(lMode100));
}
void screen1_mode75() {
  lcd.setCursor(0, 1);
  lcd.print(String(lMode75));
}
void screen1_mode55() {
  lcd.setCursor(0, 1);
  lcd.print(String(lMode55));
}
void screen1_mode0() {
  lcd.setCursor(0, 1);
  lcd.print(String(lMode0));
}

//void screen1_readyToWork() {
//  lcd.setCursor(0, 1);
//  lcd.print(String(lWorkReady));
//}

//void screen1_work() {
//  char buffer[16];
//  sprintf(buffer, lWork, mapResistorValueToPercent());
//  lcd.setCursor(0, 1);
//  lcd.print(buffer);
//}

int mapResistorValueToPercent() {
  int tmp = ((long)currentResistorValue * 100) / resistorMaxValue;
  if (tmp > 100) return 100;
  return tmp;
}

void handleKeyPad() {
  const int tolerance = 10;
  currentKeyPad = analogRead(keyPadPin);
//  Serial.println("[TRACE] currentKeyPad: " + String(currentKeyPad));
  if ( abs(oldKeyPad - currentKeyPad) > tolerance ) {
    oldKeyPad = currentKeyPad;
    if ( currentKeyPad >= 0 && currentKeyPad < 50 ) pressedRight();
    if ( currentKeyPad >= 50 && currentKeyPad < 170 ) pressedUp();
    if ( currentKeyPad >= 170 && currentKeyPad < 330 ) pressedDown();
    if ( currentKeyPad >= 330 && currentKeyPad < 520 ) pressedLeft();
    if ( currentKeyPad >= 520 && currentKeyPad < 800 ) pressedSelect();
    if ( currentKeyPad >= 800 ) idle();
  }
}

void pressedRight() {
  Serial.println("Prawo");
  setMode(55);
}

void pressedUp() {
  Serial.println("Gora");
  setMode(100);
}
void pressedDown() {
  Serial.println("Dol");
  setMode(75);
}
void pressedLeft() {
  Serial.println("Lewo");
}
void pressedSelect() {
  Serial.println("Select / Enter");
}
void idle() {
  Serial.println("Idle");
}

void setMode(int modeTmp) {
  mode = modeTmp;
  if (mode == 100) {
    Serial.println("Set mode: " + String(mode));
    digitalWrite(mode0Pin, HIGH);
    digitalWrite(mode1Pin, LOW);
    digitalWrite(mode2Pin, LOW);
  } else if (mode == 75) {
    Serial.println("Set mode: " + String(mode));
    digitalWrite(mode0Pin, HIGH);
    digitalWrite(mode1Pin, HIGH);
    digitalWrite(mode2Pin, LOW);
  } else if (mode == 55) {
    Serial.println("Set mode: " + String(mode));
    digitalWrite(mode0Pin, HIGH);
    digitalWrite(mode1Pin, HIGH);
    digitalWrite(mode2Pin, HIGH);
  } else {
    Serial.println("[ERROR] Unsupported mode: " + String(mode) + "; Reseting to 0"); 
    mode = 0;
  }
}
