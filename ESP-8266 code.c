/** Project Name  : ZIOT Controller
/** Project Name  : ZIOT Controller
 *  Created By    : Md. Zahirul Islam
 *  Published     : https://voltagefacts.blogspot.com
 *  
 ** Library Imported  : Firebase-ESP8266 by Modbizt
 *  Source of Library : https://github.com/mobizt/Firebase-ESP8266 which is MIT licensed. 
 *  
 * 
 ** User License : This code is free to use but this piece of docment must be included.
 */

#include <ESP8266WiFi.h>		
#include <FirebaseESP8266.h>	

#define WIFI_SSID "YOUR WiFi NAME"                       /* 1. Define the WiFi credentials */
#define WIFI_PASSWORD "YOUR WiFi PASSWORD"
#define USER_EMAIL "YOUR ZIOT EMAIL"        /* 4. Define the user Email and */
#define USER_PASSWORD "YOUR ZIOT PASSWORD"  /* password that you have used to registerd*/
#define FIREBASE_HOST "fir-and-nodemcu-default-rtdb.firebaseio.com" /* 2. Define the Firebase project host name (required) */
#define API_KEY "AIzaSyDfAnoVad2UPCUHKD5katzo6l-dUHQ5hSI"           /* 3. Define the API key*/
#define output_1 16 
#define output_2 5
#define output_3 4
#define output_4 2

void collectData(FirebaseData &data);        /* The function to print the operating results */

FirebaseData fbdo;    /* 6. Define the Firebase Data object */
FirebaseAuth auth;    /* 7. Define the FirebaseAuth data for authentication data */
FirebaseConfig config;/* 8. Define the FirebaseConfig data for config data */

String path = "/user/";
unsigned long dataMillis = 0;
int count = 0;

bool switch1, switch2, switch3, switch4;

void setup()
{
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  pinMode(output_1, OUTPUT);
  pinMode(output_2, OUTPUT);
  pinMode(output_3, OUTPUT);
  pinMode(output_4, OUTPUT);

  config.host = FIREBASE_HOST;        /* Assign the project host and api key (required) */
  config.api_key = API_KEY;

  auth.user.email = USER_EMAIL;       /* Assign the user sign in credentials */
  auth.user.password = USER_PASSWORD;
  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  Firebase.begin(&config, &auth);  /* Initialize the library with the Firebase authen and config */
  path += auth.token.uid.c_str();  /* path for user data is now "/user/YOUR_USER_UID"*/
             /*where YOUR_USER_UID = auth.token.uid*/  
}

void loop()
{
    if (millis() - dataMillis > 3000)
    {
        dataMillis = millis();
        Serial.println("------------------------------------");
  
        if (Firebase.get(fbdo, path + "/switches"))  /*Reading data form firebase*/
        {
            Serial.println("PASSED");
            Serial.println("PATH: " + fbdo.dataPath());
            Serial.println("TYPE: " + fbdo.dataType());
            Serial.println("ETag: " + fbdo.ETag());
            /*Serial.print("VALUE: ");
            Serial.println(fbdo.boolData());
            if(fbdo.boolData()){
                digitalWrite(inLED, LOW);
            } else
            {
                digitalWrite(inLED, HIGH);
            }
            */
            collectData(fbdo);
            
            Serial.println();
        }
        else
        {
            Serial.println("FAILED");
            Serial.println("REASON: " + fbdo.errorReason());
            Serial.println();
        }
    }
}

void collectData(FirebaseData &data)
{
    if (data.dataType() == "json")
    {
        Serial.println();
        FirebaseJson &json = data.jsonObject();
        
        size_t len = json.iteratorBegin();
        String key, value = "";
        int type = 0;
        for (size_t i = 0; i < len; i++)
        {
            json.iteratorGet(i, type, key, value);
            Serial.print(i);
            Serial.print(", ");
            Serial.print("Type: ");
            Serial.print(type == FirebaseJson::JSON_OBJECT ? "object" : "array");
            if (type == FirebaseJson::JSON_OBJECT)
            {
                Serial.print(", Key: ");
                Serial.print(key);

                if(key == "switch1"){
                    if(value == "true"){
                        switch1 = true;
                    }
                    if(value == "false"){
                        switch1 = false;
                    }
                }
                    
                if(key == "switch2"){
                    if(value == "true"){
                        switch2 = true;
                    }
                    if(value == "false"){
                        switch2 = false;
                    }
                }
                    
                if(key == "switch3"){
                    if(value == "true"){
                        switch3 = true;
                    }
                    if(value == "false"){
                        switch3 = false;
                    }
                }
                    
                if(key == "switch4"){
                    if(value == "true"){
                        switch4 = true;
                    }
                    if(value == "false"){
                        switch4 = false;
                    }
                }            
            }

            ///..................
   if(switch1){
        digitalWrite(output_1, LOW);
    } 
    else
    {
        digitalWrite(output_1, HIGH);
    }
    
    if(switch2){
        digitalWrite(output_2, LOW);
    } 
    else
    {
        digitalWrite(output_2, HIGH);
    }
    
    if(switch3){
        digitalWrite(output_3, LOW);
    } 
    else
    {
        digitalWrite(output_3, HIGH);
    }
    
    if(switch4){
        digitalWrite(output_4, LOW);
    } 
    else
    {
        digitalWrite(output_4, HIGH);
    }
            ///...............
            
            Serial.print(", Value: ");
            Serial.println(value);
        }
        json.iteratorEnd();
    }
    
    else
    {
        Serial.println(data.payload());
    }
}
