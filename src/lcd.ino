void lcd_welcome() {
  lcd.clear(); // Clear LCD Screen
  // Set Cursor and Write Text
  lcd.setCursor(4, 0);
  lcd.print("Eco Seed");
  lcd.setCursor(3, 1);
  lcd.print("Initialize");
  delay(1000);
}

void lcd_stats(){
  lcd.clear(); // Clear LCD Screen
  // Set Cursor and write the Weight
  lcd.setCursor(0, 0);
  lcd.print("W: " + String(analogRead(forcePin)));
  // Set Cursor and Write the Humidity
  lcd.setCursor(8,0);
  lcd.print("H: " + moisture() + "%");
  // Set Cursor and Write the Height
  lcd.setCursor(0,1);
  lcd.print("H: " + get_distance() + "cm");
}
