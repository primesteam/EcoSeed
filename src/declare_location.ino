// Define Variables
int stops = 10;
int ldrmax = 0;
int steppos = 0;
int current = 0;

void declare_location() {
  // Rotate 1st Stepper Clockwise
  for (int i = 0; i < (stops / 2 + 1); i++) {
    int l = analogRead(ldrPin);
    if (l > ldrmax) {
      ldrmax = l;
      steppos = (stepsPerRevolution / stops) * i;
    }
    ldrStepper.step(stepsPerRevolution / stops);
    delay(500);
  }
  ldrStepper.step(-(stepsPerRevolution / stops) * 5);

  // Rotate 1st Stepper Counterclockwise
  for (int i = 0; i < (stops / 2 + 1); i++) {
    int l = analogRead(ldrPin);
    if (l > ldrmax) {
      ldrmax = l;
      steppos = (-stepsPerRevolution / stops) * i;
    }
    ldrStepper.step(-stepsPerRevolution / stops);
    delay(500);
  }
  ldrStepper.step((stepsPerRevolution / stops) * 5);

  // Turn 2nd Stepper For the First Time
  if (current == 0) {
    plantStepper.step(steppos * 1.8);
    current = steppos * 1.8;
  }
  else { // Turn 2nd Stepper
    plantStepper.step(-current);
    current = steppos * 1.8;
    plantStepper.step(steppos * 1.8);
  }
}
