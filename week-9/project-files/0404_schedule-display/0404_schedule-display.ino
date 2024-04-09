//library
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Arduino.h>
//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

#define DATABASE_URL "https://ps70-assignment-9-default-rtdb.firebaseio.com/" 
#define API_KEY "AIzaSyDQLa0yHfCj-yF4qb3oo5hCBFqBPJmPdmw"

#define WIFI_SSID "MAKERSPACE"
#define WIFI_PASSWORD "12345678"
unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;
unsigned long prevMillis = 0;
const long interval = 5000; // Check every 5 seconds

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;



#include <LiquidCrystal_I2C.h>
 
// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;
 
// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the API key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("OK");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  // put your setup code here, to run once:
  if (!Firebase.RTDB.beginStream(&fbdo, "/events"))
  {
    Serial.println(fbdo.errorReason());
  }


     // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!Firebase.RTDB.readStream(&fbdo))
  {
    Serial.println(fbdo.errorReason());
  }

  if (fbdo.streamTimeout())
  {
    Serial.println("Stream timeout, resume streaming...");
    Serial.println();
  }

  if (fbdo.streamAvailable())
  {
    if (fbdo.dataTypeEnum() == firebase_rtdb_data_type_integer)
      Serial.println(fbdo.to<int>());
    else if (fbdo.dataTypeEnum() == firebase_rtdb_data_type_float)
      Serial.println(fbdo.to<float>(), 5);
    else if (fbdo.dataTypeEnum() == firebase_rtdb_data_type_double)
      printf("%.9lf\n", fbdo.to<double>());
    else if (fbdo.dataTypeEnum() == firebase_rtdb_data_type_boolean)
      Serial.println(fbdo.to<bool>() ? "true" : "false");
    else if (fbdo.dataTypeEnum() == firebase_rtdb_data_type_string)
      Serial.println(fbdo.to<String>());
    else if (fbdo.dataTypeEnum() == firebase_rtdb_data_type_json)
    {
        Serial.println("in the json for loop");
        FirebaseJson *json = fbdo.to<FirebaseJson *>();
        Serial.println(json->raw());
        FirebaseJsonData event;
        json->get(event, "event");
        Serial.println(event.to<String>());
        FirebaseJsonData date;
        json->get(date, "date");
        Serial.println(date.to<String>());
        
      lcd.setCursor(0, 0);
  // print message
  lcd.print(date.to<String>());
  delay(1000);
  // clears the display to print new message
  
  // set cursor to first column, second row
  lcd.setCursor(0,1);
  lcd.print(event.to<String>());
  delay(3000);
  lcd.clear(); 
    }
    else if (fbdo.dataTypeEnum() == firebase_rtdb_data_type_array)
    {
        FirebaseJsonArray *arr = fbdo.to<FirebaseJsonArray *>();
        Serial.println(arr->raw());
    }

  }


}
