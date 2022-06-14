String moisture() {
  // Get sensor value and Convert it to percentage
  int value = analogRead(sensor_pin);
  value = map(value, 4095, 1500, 0, 100);
  return String(value);
}
