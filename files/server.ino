int logged = 0; // Define Variables
void data_post() {
  if (now.hour() % 2 == 0 and logged == 0) { // Check if it's the time to post data
    if (WiFi.status() == WL_CONNECTED) { // Check for Wi-Fi
      // Define Clients
      WiFiClient client;
      HTTPClient http;

      // Begin HTTP Server
      http.begin(client, serverName);

      // Define Strings
      String weight = "weight=" + String(analogRead(forcePin));
      String humidity = "humidity=" + moisture();
      String height = "height=" + get_distance();

      // Define Request Data
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      String httpRequestData = humidity + "&" + height + "&" + weight;
      int httpResponseCode = http.POST(httpRequestData);

      // Print Responce Code to Console
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      logged = 1;
      http.end();
    }
    else { // Wi-Fi Disconnected
      Serial.println("WiFi Disconnected");
    }
  } else if (now.hour() % 2 == 1) { // Check for Odd Hour
    logged = 0;
  }
}
