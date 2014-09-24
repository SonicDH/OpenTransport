#include <ClickEncoder.h>
#include <TimerOne.h>
#include <LiquidTWI.h>
#include <Wire.h>
LiquidTWI lcd(0);
ClickEncoder *encoder1, *encoder2;
int16_t last1, value1, last2, value2;
void timerIsr() {
  encoder1->service();
  encoder2->service();
}
void setup() {
  Serial.begin(9600);
  encoder1 = new ClickEncoder(5, 4, 6);
  encoder2 = new ClickEncoder(8, 7, 9);
  encoder1->setAccelerationEnabled(0);
  encoder2->setAccelerationEnabled(0);
  lcd.begin(16, 2);
  lcd.clear();
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);}

void loop() {
  //top encoder read
  value1 += encoder1->getValue();
  if (value1 == last1) {
  }
  else if (value1 < last1) {
    last1 = value1;
    lcd.setCursor(0, 0);
    lcd.print("CCW");
  }
  else if (value1 > last1) {
    last1 = value1;
    lcd.setCursor(0, 0);
    lcd.print("CW");
  }
  
  ClickEncoder::Button b = encoder1->getButton();
  if (b != ClickEncoder::Open) {
    Serial.print("Button: ");
    #define VERBOSECASE(label) case label: Serial.println(#label); break;
    switch (b) {
      VERBOSECASE(ClickEncoder::Pressed);
      VERBOSECASE(ClickEncoder::Held)
      VERBOSECASE(ClickEncoder::Released)
      VERBOSECASE(ClickEncoder::Clicked)
      case ClickEncoder::DoubleClicked:
          lcd.setCursor(0, 0);
          lcd.print("button!");
      break;
    }
  }    
  //bottom encoder read
  value2 += encoder2->getValue();
  if (value2 == last2) {
  }
  else if (value2 < last2) {
    last2 = value2;
    lcd.setCursor(0, 1);
    lcd.print("2CCW");
  }
  else if (value2 > last2) {
    last2 = value2;
    lcd.setCursor(0, 1);
    lcd.print("2CW");
  }
  
  ClickEncoder::Button c = encoder2->getButton();
  if (c != ClickEncoder::Open) {
    Serial.print("Button: ");
    #define VERBOSECASE(label) case label: Serial.println(#label); break;
    switch (c) {
      VERBOSECASE(ClickEncoder::Pressed);
      VERBOSECASE(ClickEncoder::Held)
      VERBOSECASE(ClickEncoder::Released)
      VERBOSECASE(ClickEncoder::Clicked)
      case ClickEncoder::DoubleClicked:
          lcd.setCursor(0, 1);
          lcd.print("2button!");
      break;
    }
  }
}

