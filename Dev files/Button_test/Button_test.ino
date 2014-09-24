#include <LiquidTWI.h>
#include <Wire.h>
#include <ClickEncoder.h>
#include <TimerOne.h>
LiquidTWI lcd(0);
ClickEncoder *encoder1, *encoder2;
int16_t last1, value1, last2, value2;
void timerIsr() {
  encoder1->service();
  encoder2->service();
}
void setup(){
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  encoder1 = new ClickEncoder(8, 7, 9);
  encoder2 = new ClickEncoder(5, 4);
  encoder1->setAccelerationEnabled(0);
  encoder2->setAccelerationEnabled(0);
  lcd.begin(16, 2);
  lcd.clear();
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);
  pinMode(6, INPUT_PULLUP);}
int toppin = 6;

void loop(){
    //top encoder read
  value1 += encoder1->getValue();
  if (value1 == last1) {
  }
  else if (value1 < last1) {
    last1 = value1;
    lcd.setCursor(0, 0);
    lcd.print("             ");
    lcd.setCursor(0, 0);
    lcd.print("CCW");
  }
  else if (value1 > last1) {
    last1 = value1;
    lcd.setCursor(0, 0);
    lcd.print("             ");
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
          lcd.print("             ");
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
    lcd.print("             ");
    lcd.setCursor(0, 1);
    lcd.print("2CCW");
  }
  else if (value2 > last2) {
    last2 = value2;
    lcd.setCursor(0, 1);
    lcd.print("             ");
    lcd.setCursor(0, 1);
    lcd.print("2CW");
  }
  
  int top = digitalRead(toppin);
  if (top == LOW) {
     lcd.setCursor(0, 1);
     lcd.print("             ");
     lcd.setCursor(0, 1);
     lcd.print("2button!");
  }
  lcd.setCursor(0, 0);
  int button1 = digitalRead(10);
  int button2 = digitalRead(A0);
  int button3 = digitalRead(A2);
  int button4 = digitalRead(A1);
  int button5 = digitalRead(11);
  int button6 = digitalRead(12);
  int button7 = digitalRead(A4);
  int button8 = digitalRead(A3);
  if (button1 == LOW) {
    lcd.print("Button 1");}
  else if (button2 == LOW) {
    lcd.print("Button 2");}
  else if (button3 == LOW) {
    lcd.print("Button 3");}
  else if (button4 == LOW) {
    lcd.print("Button 4");}
  else if (button5 == LOW) {
    lcd.print("Button 5");}
  else if (button6 == LOW) {
    lcd.print("Button 6");}
  else if (button7 == LOW) {
    lcd.print("Button 7");}
  else if (button8 == LOW) {
    lcd.print("Button 8");}
}
