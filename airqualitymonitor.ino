//Tai-Hsien Ou Yang
//2017 Mar. 4th

#include <ESP8266WiFi.h> 
#include <HTTPSRedirect.h>

//Create a Google Sheet
//Tools->Script Editor, paste code.gs into it
//File->save the projecct
//Publish->Deploy as web app->Current web app URL
//Set "Who has access to the app:" as everyone, even anonymous
//GScriptId is in Google App URL: /https://script.google.com/macros/s/==GScriptId==/exec

#define PIN_SENSOR_LED    16 //D2, Don't use D7
#define PIN_ADC           0

const char* ssid = "*****WIFI SSID*****";
const char* password = "*****WIFI PASSWORD*****";

const char* host = "script.google.com"; 
const char* googleRedirHost = "script.googleusercontent.com" ;
const int httpsPort = 443; 
const char *GScriptId = "*****GScriptId*****";

HTTPSRedirect client(httpsPort);

String url = String("/macros/s/") + GScriptId + "/exec?"; 
String strLat = "*****latitude*****";
String strLong = "*****longitude*****";

IPAddress ip(192, 168, 1, 5); 
IPAddress gateway(192, 168, 1, 1);

void setup() { 

  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(PIN_SENSOR_LED, OUTPUT);
  pinMode(PIN_ADC, INPUT);

  Serial.begin(115200); 
  Serial.print(String("\nBooting..."));

  WiFi.begin(ssid, password); 
      while (WiFi.status() != WL_CONNECTED) { 
            delay(1000); 
            Serial.print("."); 
    } 

  Serial.print(String("\nWiFi connection is established"));  
}

int readSensor(){ 
  int analogData=0;

  while(analogData<2){ //repeat measuring until it gets a value >2
    digitalWrite(PIN_SENSOR_LED, 1);
    delayMicroseconds(280); //light the LED, according to the datasheet

    analogData = analogRead(PIN_ADC);
    delayMicroseconds(40);

    digitalWrite(PIN_SENSOR_LED, 0);
    delayMicroseconds(9600);
  }

  return analogData; 
}

float toDensity( int intValue ){
  float fltDensity = 0;

  //I use a 2k/1k voltage divider to convert 5V to 1.65 V, scale the value by 100, and mg to ug, so
  //0.13*1.65/1024*100*1000 = 0.21
  fltDensity = 0.01*(int(21*intValue)); 
  return fltDensity; 
}

void sendPayload(String strLat, String strLong, float value){ 
  
    if (!client.connected()){ 
            Serial.println("."); 
            client.connect(host, httpsPort); 
    } 

    String urlFinal = url + "&lat="  + strLat + "&long="  + strLong + "&value=" + String(value); 
    client.printRedir(urlFinal, host, googleRedirHost); 
}

void loop(){
  int intRawRead = 0;
  float fltDensity = 0;  

  digitalWrite(BUILTIN_LED, 0); //0=LED ON; 

  intRawRead = readSensor();
  Serial.println(String(intRawRead) ); 
  fltDensity = toDensity(intRawRead);

  sendPayload(strLat, strLong, fltDensity ); 

  digitalWrite(BUILTIN_LED, 1); //1=LED OFF; 
  delay(600000); //10min
}


