#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); //Informacja o podlaczeniu nowego wyswietlacza

int resistorPin = 6;
int currentResistorValue = 0;
int lastResistorValue = currentResistorValue;
const int resistorMaxValue = 870;

void setup() {
  Serial.begin(9600);
  Serial.println("Started");
  
  lcd.begin(16, 2);

  pinMode(resistorPin, INPUT);

  Serial.println(millis());
  screen_animate();
  Serial.println(millis());
  screen_main();
  screen_readyToWork();
}

void loop() {
  currentResistorValue = readAnalogResistor();
  if (resistorValueChanged()) {
    lastResistorValue = currentResistorValue;
    Serial.print("currentResistorValue: ");
    Serial.println(currentResistorValue);
    Serial.print("lastResistorValue: ");
    Serial.println(lastResistorValue);
    if (mapResistorValueToPercent() == 0) {
      screen_readyToWork();
    } else {
      screen_work(); 
    }
  }
//  delay(1000);
}

void screen_animate() {
  int delayTime = 20;
  String label = String("  SPAW-SERWIS   ");
  lcd.clear();
  int i, j;
  for (i = 2; i < 13; i++) {
    char currentChar = label[i];
    for (j = 15; j > i-1; j--) {
      lcd.print(" ");
      lcd.setCursor(j, 0);
      lcd.print(currentChar);
      delay(delayTime);
    }
    if ( (i > 5) && (i % 2 == 0) ) delayTime *= 2;
  }
  screen_main();
  delay(1000);
}

boolean resistorValueChanged() {
  int tolerance = 10;
  int lo = lastResistorValue - tolerance;
  int hi = lastResistorValue + tolerance;
  return (currentResistorValue < lo) || (currentResistorValue > hi);
}

void screen_main() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  SPAW-SERWIS   ");
}

void screen_readyToWork() {
  lcd.setCursor(0, 1);
  lcd.print("GOTOWY DO PRACY ");
}

void screen_work() {
//  screen_main();

  char buffer[16];
  sprintf(buffer,"  PRACA    %3d%% ", mapResistorValueToPercent());
  lcd.setCursor(0, 1);
  lcd.print(buffer);
}

int readAnalogResistor() {
  int tmp = analogRead(A0);
//    Serial.print("tmp: ");
//    Serial.println(tmp);
//    Serial.print("resMax: ");
//    Serial.println(resMax);
  return tmp;
}

int mapResistorValueToPercent() {
  int tmp = ((long)currentResistorValue * 100) / resistorMaxValue;
  if (tmp > 100) return 100;
  return tmp;
}
