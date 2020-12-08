/*
a0 - rw
a1 - play
a2 - stop
a3 - ff
a4 - Bottom enc button
a5 - top enc button
2 - sda
3 - scl
4 - 2_1
5 - rec
6 - 2_3
7 - 2_4
8 - top enc 1
9 - top enc 2
10 - btm enc 1
11 - btm enc 2
*/

//#include <LiquidTWI.h>
//#include <Wire.h>
#include <ClickEncoder.h>
#include <TimerOne.h>
#include "Keyboard.h"
//LiquidTWI lcd(0);
int toppin = A5;
int ffstate;
int rwstate;
int jogstate = 1;
int topstate = 0; //default mode for the top encoder is zoom control (0), clicking the button switches to nudge mode (1)
int lightstate = 1;
ClickEncoder *encoder1, *encoder2;
int16_t last1, value1, last2, value2;
void timerIsr() {
  encoder1->service();
  encoder2->service();
}
void setup(){
 //Nice looking boot sequence
 //lcd.begin(16, 2);
 // lcd.clear();
 // lcd.setCursor(0,0);
 // lcd.print("  OpenTransport");
// Setup all the stuff
  pinMode(A0, INPUT_PULLUP); //rw
  pinMode(A1, INPUT_PULLUP); //play
  pinMode(A2, INPUT_PULLUP); //stop
  pinMode(A3, INPUT_PULLUP); //ff
  pinMode(A5, INPUT_PULLUP); //top enc buttom
  pinMode(4, INPUT_PULLUP); //2_1
  pinMode(5, INPUT_PULLUP); //rec
  pinMode(6, INPUT_PULLUP); //2_3
  pinMode(7, INPUT_PULLUP); //2_4
  pinMode(12, OUTPUT); //LED Light
  pinMode(13, INPUT_PULLUP); //lightswitch
  encoder2 = new ClickEncoder(9, 8);
  encoder1 = new ClickEncoder(10, 11, A4); //button support broken when using more than one encoder with this library. Compensated for in the runtime code by treating it like a normal button.
  encoder1->setAccelerationEnabled(0);
  encoder2->setAccelerationEnabled(0);
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);
 // lcd.setCursor(0,1);
 // lcd.print("Ready");
  delay(2000);
  //lcd.clear();
  Keyboard.begin();
  digitalWrite(12, HIGH);
}
void loop(){
  // lcd.setCursor(0,0);
  //set lights on or off
  int light = digitalRead(13); //light click
  if (light == LOW) {
  //  lcd.clear();
    if (lightstate == 1) {
   //  lcd.print("Lights Off");
     digitalWrite(12, LOW);
   //  lcd.setBacklight(LOW);
     lightstate = 0;
     delay(500);}
    else if (lightstate == 0) {
   //  lcd.print("Lights On");
     digitalWrite(12, HIGH);
   //  lcd.setBacklight(HIGH);
     lightstate = 1;
     delay(500);}
    }
//------------------------------------------------------------------ 
//Jog Wheel (bottom encoder) portion.

  //bottom encoder read
  value1 += encoder1->getValue();
  if (value1 == last1) {
  }
  else if (value1 < last1) {  //counter-clockwise
    last1 = value1;
  //  lcd.clear();
  //  lcd.print("Jog Left");
    for (int x = 1; x <= jogstate; x++){
      Keyboard.write(KEY_LEFT_ARROW);}
  }
  else if (value1 > last1) {  //clockwise bottom
    last1 = value1;
//lcd.clear();
  //  lcd.print("Jog Right");
    for (int x = 1; x <= jogstate; x++){
    Keyboard.write(KEY_RIGHT_ARROW);}
  }
  // Jog Wheel Button not yet implemented.  
  ClickEncoder::Button b = encoder1->getButton();
  if (b != ClickEncoder::Open) {  //bottom encoder double click
    #define VERBOSECASE(label) case label: Serial.println(#label); break;
    switch (b) {
      case ClickEncoder::DoubleClicked:
          jogstate = 1;
     //    lcd.clear();
     //     lcd.print("Jog Set to 1x");
      break;
    }
  }
//------------------------------------------------------------------ 
//top encoder read
  value2 += encoder2->getValue();
  if (value2 == last2) { //top encoder ccw
  }
  else if (value2 < last2) {
    last2 = value2;
  //  lcd.clear();
    if (topstate == 1) {
  //    lcd.print("Nudge Left");
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press(KEY_LEFT_ARROW);
      Keyboard.releaseAll();
     }
    else if (topstate == 0) {
   //   lcd.print("Zoom Out");
      Keyboard.write('-');
    delay(150);
     }
  }
  else if (value2 > last2) { //top encoder cw
    last2 = value2;
  //  lcd.clear();
    if (topstate == 1) {
  //    lcd.print("Nudge Right");
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press(KEY_RIGHT_ARROW);
      Keyboard.releaseAll();
     }
    else if (topstate == 0) {
  //    lcd.print("Zoom In");
      Keyboard.write('=');
    delay(150);
     }
  }
  int top = digitalRead(toppin); //top click
  if (top == LOW) {
  //  lcd.clear();
    if (topstate == 1) {
  //   lcd.print("Zoom mode!");
     topstate = 0;
     delay(500);}
    else if (topstate == 0) {
  //   lcd.print("Nudge mode!");
     topstate = 1;
     delay(500);}
    }
//------------------------------------------------------------------ 

//Clear the screen and check the status of all the control buttons
  int button1 = digitalRead(A0);
  int button2 = digitalRead(A1);
  int button3 = digitalRead(A2);
  int button4 = digitalRead(A3);
  int button5 = digitalRead(4);
  int button6 = digitalRead(5);
  int button7 = digitalRead(6);
  int button8 = digitalRead(7);
  int jogbutton = digitalRead(A4);
  
// -----------------------------------------------------------------------
  if (button4 == LOW) {
  //  lcd.clear();
 //   lcd.print("Fast Forward");
    Keyboard.write('l');
    delay(500);
  }
  else if (button1 == LOW) { 
 //   lcd.clear();
 //   lcd.print("Rewind");
    Keyboard.write('j');
    delay(500);
  }

//------------------------------------------------------------------------
  if (button2 == LOW) { 
  //  lcd.clear();
 //   lcd.print("Play");
    Keyboard.write(' ');
    delay(500);
  }
//keypress
  if (button3 == LOW) { 
 //   lcd.clear();
 //   lcd.print("Stop");
    Keyboard.write('k');
    delay(500);
//keypress
  }
// -----------------------------------------------------------------------
//This key holds the note on until you release button.
  if (button4 == LOW) {
     if (ffstate == 0) {
  //    lcd.print("Fast Forward");
//keypress
     ffstate = 1;}}
  else if (button4 == HIGH) {
     if (ffstate == 1){
 //     lcd.clear();
//keypress
      ffstate = 0;}}  
//------------------------------------------------------------------------
/* old record button code
  if (button5 == LOW) { //This key is sends out the Midi Code matching to the Loop button in the Mackie Keymap.
    lcd.clear();
    lcd.print("Delete");
    Keyboard.press(KEY_DELETE);
    delay(100);
    Keyboard.releaseAll();
    delay(500);
//keypress
}

  else if (button6 == LOW) { //This key is sends out the Midi Code matching to the Record button in the Mackie Keymap.
    lcd.clear();
    lcd.print("Record");
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(' ');
    delay(100);
    Keyboard.releaseAll();
    delay(500);
//keypress
  }
 */
   if (button6 == LOW) {
 //   lcd.clear();
//    lcd.print("Delete");
    Keyboard.press(KEY_DELETE);
    delay(100);
    Keyboard.releaseAll();
    delay(500);
//keypress
}  else if (button5 == LOW) { 
 //   lcd.clear();
//    lcd.print("Select at Head");
    Keyboard.write('d');
    delay(500);
//keypress
}
  else if (button7 == LOW) {
 //   lcd.clear();
 //   lcd.print("Ripple Delete");
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press(KEY_BACKSPACE);
    delay(100);
    Keyboard.releaseAll();
    delay(500);
//keypress
  }
  else if (button8 == LOW) {
//    lcd.clear();
//    lcd.print("Split");
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('k');
    delay(100);
    Keyboard.releaseAll();
    delay(500);  
//keypress
}
  else if (jogbutton == LOW) {
    jogstate++;
//    lcd.print("Jog Speed ");
//    lcd.print(jogstate);
//    lcd.print("x");
    delay(500);
//keypress
}
}
