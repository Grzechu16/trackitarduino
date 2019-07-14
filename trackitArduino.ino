#include <Sim800L.h>
#include <TinyGPS++.h>
TinyGPSPlus gps;

String textMessage = "";
float latitude = 0;
float longitude = 0;
int counter = 0;
bool correctCoordinates = false;

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial3.begin(9600);
  initializeGps();
}

void loop()
{

 getCoordinates();

  if (Serial3.available() > 0) {
    textMessage = Serial3.readString();
    textMessage.toUpperCase();
    delay(10);
  }
  if (textMessage.indexOf("GPS") >= 0) {
    if (counter == 0) {
      updateDatabase();
      textMessage = "";
    }
    else
      onlyUpdate();
      textMessage = "";
  }
}

void updateDatabase() {
  Serial.println("Database update requested");
  Serial.println("...");
  if(correctCoordinates==true){
  Serial.println("Coordinates OK");
  Serial.println("...");
  }
  counter = 1;

  Serial3.println("AT");
  delay(100);
  Serial3.println("AT+CSQ");
  delay(100);
  Serial3.println("AT+SAPBR=3,1,CONTYPE,GPRS");
  delay(100);
  Serial3.println("AT+SAPBR=3,1,APN,INTERNET");
  delay(100);
  Serial3.println("AT+SAPBR=1,1");
  delay(100);
  Serial3.println("AT+SAPBR=2,1");
  delay(100);
  Serial3.println("AT+HTTPINIT");
  delay(100);
  Serial3.println("AT+HTTPPARA=CID,1");
  delay(100);
  Serial3.println("AT+HTTPPARA=URL,http://arduinoandroidtrackit.000webhostapp.com/upload1.php?deviceid=731536061&lat=" + String(latitude, 6) + "&lon=" + String(longitude, 6));
  delay(100);
  Serial3.println("AT+HTTPACTION=0");
  delay(100);
  Serial.println("Update completed");
  Serial.println("...");
}

void onlyUpdate() {
  Serial.println("Database update requested");
  Serial.println("...");
  
  Serial3.println("AT+HTTPINIT");
  delay(100);
  Serial3.println("AT+HTTPPARA=CID,1");
  delay(100);
  Serial3.println("AT+HTTPPARA=URL,http://arduinoandroidtrackit.000webhostapp.com/upload1.php?deviceid=731536061&lat=" + String(latitude, 6) + "&lon=" + String(longitude, 6));
  delay(100);
  Serial3.println("AT+HTTPACTION=0");
  delay(100);
  Serial.println("Update completed");
  Serial.println("...");
}

void initializeGps() {
  Serial.println("Initializing module");
  Serial.println("...");
  
  Serial3.println("AT");
  delay(100);
  Serial3.println("AT+CMGF=1"); 
  delay(100);
  Serial3.println("AT+CNMI=2,2,0,0,0"); 
  delay(100);
  Serial3.println("AT+CMGD=1,4");
  delay(100);
  Serial.println("Device initialized, sms memory cleared");
  Serial.println("...");
  Serial.println("Waiting for order");
  Serial.println("...");
}

void updateSerial() {
  delay(500);
  //if(Serial1.available()){
  //  Serial.write(Serial1.read());
  //}
  while (Serial3.available()) {
    Serial.write(Serial3.read());
  }
}

void getCoordinates(){
   while (Serial1.available() > 0) {
    if (gps.encode(Serial1.read()))
      parseGps();
  }
}

void parseGps()
{
  if (gps.location.isValid())
  {
    correctCoordinates = true;
    longitude = gps.location.lng();
    latitude = gps.location.lat();
    delay(1000);
  }
  else
  {
    correctCoordinates = false;
    delay(1000);
  }
}
