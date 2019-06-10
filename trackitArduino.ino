#include <Sim800L.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
TinyGPSPlus gps;

String textMessage = "";
//Create software serial object to communicate with SIM800L
SoftwareSerial mySerial(2, 3); //SIM800L Tx & Rx is connected to Arduino #3 & #2

void setup()
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);

  //Begin serial communication with Arduino and SIM800L
  mySerial.begin(9600);

  Serial.println("Initializing...");
  delay(1000);

  mySerial.println("AT");
    updateSerial();

  mySerial.println("AT+CMGF=1"); // TEXT mode
  updateSerial();
  mySerial.println("AT+CNMI=2,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
   updateSerial();
}

void loop()
{

  while (Serial.available() > 0) {
    gps.encode(Serial.read());
    //if (gps.location.isUpdated()) {
    //  Serial.print("Latitude= ");
    //  Serial.print(gps.location.lat(), 6);
    //  Serial.print(" Longitude= ");
    //  Serial.println(gps.location.lng(), 6);
   // }
  }
  
  if (mySerial.available() > 0) {
    textMessage = mySerial.readString();
    //Serial.print(textMessage);
    delay(10);


    // updateSerial();
  }
  if (textMessage.indexOf("gps") >= 0) {
    // Turn on relay and save current state
    updateDatabase();
    //Serial.println("Uruchamianie gps");
    textMessage = "";

  }
  if (textMessage.indexOf("sms") >= 0) {
    // Turn on relay and save current state

    //Serial.println("Wysylanie sms");
    textMessage = "";
  }
    if (textMessage.indexOf("delete") >= 0) {
    // Turn on relay and save current state

    //Serial.println("Usuwanie sms");
    textMessage = "";
    deleteSms();
  }
}

void updateDatabase() {
  mySerial.println("AT");
  delay(1000);
  mySerial.println("AT+CPIN?");
  delay(1000);
  mySerial.println("AT+CREG?");
  delay(1000);
  mySerial.println("AT+CGATT?");
  delay(1000);
  mySerial.println("AT+CSQ");
  delay(1000);
  mySerial.println("AT+SAPBR=3,1,CONTYPE,GPRS");
  delay(1000);
  mySerial.println("AT+SAPBR=3,1,APN,INTERNET");
  delay(1000);
  mySerial.println("AT+SAPBR=1,1");
  delay(1000);
  mySerial.println("AT+SAPBR=2,1");
  delay(1000);
  mySerial.println("AT+HTTPINIT");
  delay(1000);
  mySerial.println("AT+HTTPPARA=CID,1");
  delay(1000);
  mySerial.println("AT+HTTPPARA=URL,http://arduinoandroidtrackit.000webhostapp.com/upload1.php?deviceid=731536061&lat=" + String(gps.location.lat(), 6) + "&lon=" + String(gps.location.lng(), 6));
  delay(1000);
  mySerial.println("AT+HTTPACTION=0");
  delay(1000);

}

void deleteSms() {
  mySerial.println("AT");
  delay(1000);
  mySerial.println("AT+CMGD=1,4");
  delay(1000);
  Serial.println("Sms memory cleared");
}

void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (mySerial.available())
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
