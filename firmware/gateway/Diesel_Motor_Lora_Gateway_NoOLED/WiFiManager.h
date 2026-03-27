void WifiManager(){
  
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  // it is a good practice to make sure your code sets wifi mode how you want it.
  ////////////////WIFI MANAGER CODE///////////////
  //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around

  WiFiManager wm;
  // Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the specified name ("AutoConnectAP"),
  // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
  // then goes into a blocking loop awaiting configuration and will return success result
  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  res = wm.autoConnect("AutoConnectAP"); // anonymous ap
   // id/name, placeholder/prompt, default, length

  // res = wm.autoConnect("AutoConnectAP","password"); // password protected ap
  if (!res) {
    Serial.println("Failed to connect");
    // ESP.restart();
  }
  else {
    //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  Serial.print(" SSDI:  ");
  Serial.println(WiFi.SSID());
  Serial.print(" Pass:  ");
  Serial.println(WiFi.psk());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  }
}
