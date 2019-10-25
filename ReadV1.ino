#include <SoftwareSerial.h>
SoftwareSerial mySerial(7,8); //Rx,Tx
//Sensor Parameters
#define trig 3
#define echo 4
int distance = 0;
char incomingByte; 
String inputString;
int relay = 13;

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW); // set relay to off
  digitalWrite(echo,HIGH);
  Serial.println("Starting...");
  delay(1000);
  mySerial.println("AT");
//  pollSms();
  mySerial.println("AT+CMGF=1");
//  pollSms();
  mySerial.println("AT+CNMI=1,2,0,0,0");
//  pollSms();
  mySerial.println("AT+CMGL=\"REC UNREAD\""); // Read Unread Messages
//  pollSms();
}
 
void loop()
{
  sensorValue();
//  sendText();
  pollSms();
  //clear_sim();
  delay(5000);
}
 
void pollSms()
{
//  delay(500);
//  while (Serial.available())
//  {
//    mySerial.write(Serial.read());
//  }
//  while(mySerial.available())
//  {
//    Serial.write(mySerial.read());
//  }

 Serial.write(".");
 if(mySerial.available()){
      delay(100);
      Serial.write("available now");
      // Serial Buffer
      while(mySerial.available()){
        Serial.write("pollsms myserial available");
        incomingByte = mySerial.read();
        inputString += incomingByte; 
        Serial.write("pollsms incomingbyte: " + incomingByte);
      }

      delay(10);      

      Serial.println(inputString);
      inputString.toUpperCase(); // Uppercase the Received Message

      //turn RELAY ON or OFF
      if (inputString.indexOf("ON") > -1){
        digitalWrite(relay, HIGH);
      }
      if (inputString.indexOf("OFF") > -1){
        digitalWrite(relay, LOW);
      }          

      delay(50);

      //Delete Messages & Save Memory
      if (inputString.indexOf("OK") == -1){
      mySerial.println("AT+CMGDA=\"DEL ALL\"");

      delay(1000);}

      inputString = "";
  }


}

void sendText(){
   mySerial.println("AT+CMGF=1");
  delay(500);
  mySerial.println("AT+CMGS=\"+233247017862\"\r");
  delay(500);
   if(distance < 30){
  mySerial.print("Tank is full, please OFF pump!");
  delay(500);
  }
  else if(distance > 150){
  mySerial.print("Tank is empty, please ON pump!");
  delay(500);
  }
 mySerial.write(26);
}

void clear_sim(){
  mySerial.println("AT+CMGD=1");
  delay(1000);
  mySerial.println("AT+CMGD=2");
  delay(1000);
  mySerial.println("AT+CMGD=3");
  delay(1000);
  mySerial.println("AT+CMGD=4");
  delay(1000);
  mySerial.println("AT+CMGD=5");
  delay(1000);
}

void sensorValue(){
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);

  distance = pulseIn(echo,HIGH, 26000); //Read in times pulses
  distance = distance/58;
}
