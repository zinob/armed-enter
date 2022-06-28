#include <DigiKeyboard.h>

unsigned int button, n;
bool pressed;

enum state {
  idling,
  arming,
  ready,
  armed
};

state c_state = idling;

void setup()
{
  pinMode(0, INPUT_PULLUP);
  pinMode(1, OUTPUT);
  DigiKeyboard.delay(500);
  DigiKeyboard.sendKeyStroke(0);
}

void loop()
{
  n++;
  pressed = digitalRead(0) == 0;

  if (c_state == idling){
    is_idling(pressed);
  } else if (c_state == arming) {
    is_arming(pressed);
  } else if ( (c_state == ready) and (not pressed) ) {
    c_state = armed;
    DigiKeyboard.delay(200);
  } else if ( c_state == armed ) {
    is_armed(pressed);
  }
}

void is_idling(bool pressed){
  DigiKeyboard.delay(100);

  analogWrite(1,10);
  if (pressed){
    c_state = arming;
    n = 0;
  }
}
void is_arming(bool pressed){
    DigiKeyboard.delay(10);
    if ( n == (1 << 9) ) {
      c_state = ready;
      analogWrite(1,10);
    } else if (not pressed) {
      c_state = idling;
    } else {
      analogWrite(1,64);
      
    }
  } 
  
void is_armed(bool pressed){
  if (pressed) {
    fire();
    analogWrite(1,0);
    c_state = idling;
  } else {
    if (n%255 == 0){
      
      analogWrite(1,n>>8);
    }
  }
}

void fire(){
  analogWrite(1,254);
  DigiKeyboard.sendKeyStroke(0);
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(500);
  analogWrite(1,0);
  DigiKeyboard.delay(1500);
}
