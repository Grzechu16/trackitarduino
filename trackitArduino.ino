#include <Timers.h>
#include <TinyGPS++.h>

TinyGPSPlus gps;
Timer timer;

String textMessage = "";
float latitude = 0;
float longitude = 0;
int counter = 0;
boolean gsmConnection = false;
String gsmStatus = "";
bool correctCoordinates = false;

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial3.begin(9600);
  initializeGsm();
  timer.begin(5000);
}

void loop()
{
  getCoordinates();

  if (Serial3.available() > 0) {
    textMessage = Serial3.readString();
    textMessage.toUpperCase();
    delay(10);
  } 
  if (textMessage.indexOf("GPS1") >= 0) {
    updateDatabase(1);
    textMessage = "";
  } else if (textMessage.indexOf("GPS10") >= 0) {
    updateDatabase(10);
    textMessage = "";
  } else if (textMessage.indexOf("GPS30") >= 0) {
    updateDatabase(30);
    textMessage = "";
  } else if (textMessage.indexOf("GPS60") >= 0) {
    updateDatabase(60);
    textMessage = "";
  }
  if(timer.available()){
    //setGsmMode();
  }
}

void updateDatabase(int counter) {
  while (counter > 0) {
    Serial.println("Database update requested");
    Serial.println("...");
    if (correctCoordinates == true) {
      Serial.println("Coordinates OK");
      Serial.println("...");
    }
    else {
      Serial.println("Coordinates INVALID");
      Serial.println("...");
    }

    Serial3.println("AT");
    delay(600);
    Serial3.println("AT+CSQ");
    delay(600);
    Serial3.println("AT+SAPBR=3,1,CONTYPE,GPRS");
    delay(600);
    Serial3.println("AT+SAPBR=3,1,APN,INTERNET");
    delay(600);
    Serial3.println("AT+SAPBR=1,1");
    delay(600);
    Serial3.println("AT+SAPBR=2,1");
    delay(600);
    Serial3.println("AT+HTTPINIT");
    delay(600);
    Serial3.println("AT+HTTPPARA=CID,1");
    delay(600);

    if (correctCoordinates) {
      Serial3.println("AT+HTTPPARA=URL,http://arduinoandroidtrackit.000webhostapp.com/uploadWithCoordinates.php?deviceid=731536061&lat=" + String(latitude, 6) + "&lon=" + String(longitude, 6) + "&valgps=1");
      delay(600);
    }
    else {
      Serial3.println("AT+HTTPPARA=URL,http://arduinoandroidtrackit.000webhostapp.com/uploadWithoutCoordinates.php?deviceid=731536061&valgps=0");
      delay(600);
    }

    Serial3.println("AT+HTTPACTION=0");
    delay(600);
    Serial.println("Update completed");
    Serial.println("...");
    if (counter == 1) {
      counter--;
    } else {
      delay(60000);
      counter--;
    }
  }
}

void initializeGsm() {
  Serial.println("Checking connection");
  Serial.println("...");

  while (gsmConnection != true) {
    Serial3.println("AT");
    delay(100);
    gsmStatus = Serial3.readString();
    delay(100);

    if (gsmStatus.indexOf("OK") > 0) {
      gsmConnection = true;
      Serial.println("Gsm connection OK");
      Serial.println("...");
      delay(100);
    }
    else {
      gsmConnection = false;
      gsmStatus = "";
      Serial.println("Waiting for GSM module");
      Serial.println("...");
    }
    delay(100);
  }

  Serial3.println("AT");
  delay(600);
  Serial3.println("AT+CMGF=1");
  delay(600);
  Serial3.println("AT+CNMI=2,2,0,0,0");
  delay(600);
  Serial3.println("AT+CMGD=1,4");
  delay(600);
  Serial.println("Device initialized, sms memory cleared");
  Serial.println("...");
  Serial.println("Waiting for order");
  Serial.println("...");
}

void setGsmMode() {
  Serial3.println("AT+CMGF=1");
  delay(600);
  Serial3.println("AT+CNMI=2,2,0,0,0");
  delay(600);
}

void getCoordinates() {
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
