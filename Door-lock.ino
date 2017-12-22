/////////////////////////////////////////
/*
NFC breakout Module
-IRG to D12
-RESET to D3
-SCL to A5
-SDA to A4

Wifi Module
-Rx to D5
-Tx to D6

Servo motor
-PWN to D11

RGB
-Blue to D9 // Or you can use the color you want
*/
/////////////////////////////////////////
#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#define IRQ   (12)
#define RESET (3)
SoftwareSerial Wifi(6,5); //RX,TX(5,6)
#define SSID "****"        //WIFI ID
#define PASSWD "****" //WIFI PASSWORD
#define DST_IP "www.YOUR_PROJECT_ID.appspot.com" //Google App Engine domain
#define BLUE 9 //LED to indicate state

#define BTN 2
Adafruit_NFCShield_I2C NFC(IRQ, RESET);
Servo servo;
int state=0; //state info 0: usally state, 1: register state
volatile int b=0;
void NFC_module(){  //NFC module setting
  servo.attach(11);
  servo.write(20);
  Serial.begin(115200);
  NFC.begin();
  
  uint32_t versiondata = NFC.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    sound_error();
    while (1); // halt
  }

  NFC.setPassiveActivationRetries(0xFF);
  NFC.SAMConfig();
  
  Serial.println("NFC Ready");
}
void Wifi_module() //Wifi module setting
{
  String cmd;
  Wifi.begin(9600);
  Wifi.setTimeout(2000);
  delay(1000);
  
  Wifi.print("AT+CWJAP=");
  //"ssid"
  Wifi.print("\"");    
  Wifi.print(SSID);  
  Wifi.print("\"");
  Wifi.print(","); 
    //"pwd"
  Wifi.print("\"");      
  Wifi.print(PASSWD); 
  Wifi.println("\"");
  
  Serial.println("Start Wifi module..");
  Wifi.println("AT");
  if(Wifi.find("OK")){
      Serial.println("Wifi Ready");
     sound_ready();
  }
  else{
      Serial.println("Wifi No respone");
  }
  cmd = "AT+CIPSTART=\"TCP\",\""; //connect Google App Engine
  cmd += DST_IP;
  cmd += "\",80\r\n";
  Wifi.println(cmd);
  if(Wifi.find("OK")){
    Serial.println("LINK SUCCESS");
  }
  else{
    Serial.println("LINK ERROR");
  }
}
void Open_door(String nfc_id)// open door
{
  String cmd;
  Wifi.println("AT+CIPSTATUS");
  if(Wifi.find("STATUS:4")){
    Serial.println("Try Reconnect");  
    cmd = "AT+CIPSTART=\"TCP\",\"";
    cmd += DST_IP;
    cmd += "\",80\r\n";
    Wifi.println(cmd);
    if(Wifi.find("OK")){
     Serial.println("LINK SUCCESS");
    }
    else{
     Serial.println("LINK ERROR");
    }
   }
  else{
    Serial.println("Link SUCCESS");
  }
  String nfc_info="GET /open.php?nfc_id=";
  nfc_info+=nfc_id;
  nfc_info+=" HTTP/1.1\r\nHOST: ";
  nfc_info+=DST_IP;
  nfc_info+="\r\n\r\n";
  
  cmd="AT+CIPSEND=";
  cmd+=String(nfc_info.length());
  Wifi.println(cmd);
  Wifi.print(nfc_info);
  if(Wifi.findUntil("ooopen","Unlink")){
    Serial.println("Open door");
    servo.write(100);
    sound_open();
    delay(3000);
    servo.write(20);
    sound_close();
    delay(2000);
    //return;
  }
  else if(Wifi.find("noregister","Unlink")){ 
    Serial.println("noregister");
    delay(2000);
  }
  else{   
    Serial.println("Don't open door");
    sound_nocard();
    delay(2000);
  }
  
}

void Register_user(String nfc_id) //register user
{
  
  String cmd;
  Wifi.println("AT+CIPSTATUS");
   if(Wifi.find("STATUS:4")){
    Serial.println("Try Reconnect");  
    cmd = "AT+CIPSTART=\"TCP\",\"";
    cmd += DST_IP;
    cmd += "\",80\r\n";
    Wifi.println(cmd);
    if(Wifi.find("OK")){
     Serial.println("LINK SUCCESS");
    }
    else{
     Serial.println("LINK ERROR");
    }
   }
  else{
    Serial.println("Link SUCCESS");
  }
  String user_info="GET /register.php?nfc_id=";
  user_info+=nfc_id;
  user_info+=" HTTP/1.1\r\nHOST: ";
  user_info+=DST_IP;
  user_info+="\r\n\r\n";

  cmd="AT+CIPSEND=";
  cmd+=String(user_info.length());
  Wifi.println(cmd);
  Wifi.print(user_info);
  if(Wifi.findUntil("clear","Unlink")){
    Serial.println("REGISTER SUCCESS");
    sound_register();
    delay(2000);
    //return;
  }
  else if(Wifi.findUntil("already","Unlink")){
    Serial.println("REGISTER ERROR");
    sound_nocard();
    delay(2000);
    //return;
  }
  else{
    Serial.println("WHAT?");
    sound_nocard();
  }
}
void blink(){
  if(b==0)
    b=255;
  else
    b=0;
  digitalWrite(BLUE, b);    
}
void sound_open(){
  tone(8,400);delay(100);tone(8,450);delay(100);tone(8,500);delay(300);noTone(8);
}
void sound_close(){
  tone(8,500);delay(100);tone(8,450);delay(100);tone(8,400);delay(300);noTone(8);
}
void sound_nocard(){
  tone(8,900);delay(500);noTone(8);delay(200);tone(8,900);delay(800);noTone(8);
}
void sound_register(){
  tone(8,400);delay(300);tone(8,200);delay(200);tone(8,700);delay(600);noTone(8);
}
void sound_ready(){
  tone(8,700); delay(50);tone(8,900);delay(100);tone(8,700);delay(50);tone(8,900);delay(100);noTone(8);
}
void sound_error(){
  while(1){
  tone(8,500);
  delay(100);
  noTone(8);
  delay(1000);
  }
}
void setup() {
  // put your setup code here, to run once:
  pinMode(BTN,OUTPUT);
  attachInterrupt(0,blink,RISING);
  NFC_module();
  Wifi_module();
  //MsTimer2::set(5000,changeflag);
  //MsTimer2::start();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  uint8_t nfc_id[]={0,0,0,0,0,0,0};
  uint8_t nfc_id_length;
  boolean check; //Check card tag
  String uid; 

 
     check=NFC.readPassiveTargetID(PN532_MIFARE_ISO14443A,&nfc_id[0],&nfc_id_length);
      
      if(b==255)
        state=1;
        
      if(check){
        
        for(int i=0;i<nfc_id_length;i++){
          uid+=nfc_id[i];
        }
        if(state==0){
         Open_door(uid);
         //Wifi.flush();
        }
        else if(state==1)
         Register_user(uid);
         //Wifi.flush();
         b=0;
         state=0;
         digitalWrite(BLUE, b); 
      }
}
