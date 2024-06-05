// Button pins
#define BUTTON_UP 5
#define BUTTON_DOWN 4
#define BUTTON_ENTER 18

bool BUTTON_UP_Pressed = false;
bool BUTTON_DOWN_Pressed = false;
bool BUTTON_ENTER_Pressed = false;

void handleUpButtonPress() {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();

  // Debouncing logic: Ignore the interrupt if it occurs within 50 ms of the last interrupt
  if (interruptTime - lastInterruptTime > 150) {
    BUTTON_UP_Pressed = true;           // Set the flag to indicate a button press
    lastInterruptTime = interruptTime;  // Update the last interrupt time
  }
}

void handleDownButtonPress() {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();

  // Debouncing logic: Ignore the interrupt if it occurs within 50 ms of the last interrupt
  if (interruptTime - lastInterruptTime > 150) {
    BUTTON_DOWN_Pressed = true;         // Set the flag to indicate a button press
    lastInterruptTime = interruptTime;  // Update the last interrupt time
  }
}


void handleEnterButtonPress() {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();

  // Debouncing logic: Ignore the interrupt if it occurs within 50 ms of the last interrupt
  if (interruptTime - lastInterruptTime > 150) {
    BUTTON_ENTER_Pressed = true;        // Set the flag to indicate a button press
    lastInterruptTime = interruptTime;  // Update the last interrupt time
  }
}

//Button go acction after preassing the button. 
