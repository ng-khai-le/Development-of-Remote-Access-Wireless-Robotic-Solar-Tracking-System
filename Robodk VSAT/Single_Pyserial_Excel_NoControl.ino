#include <SoftwareSerial.h>
#include <Servo.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
int light_1;
int light_3;
static int pos1=90;
const int range=20;
int Vo_Single;
int Vo_Fixed;
double Vin_Single;
double Vin_Fixed;
double real_Vo_Single;
double real_Vo_Fixed;
double C_Single;
double C_Fixed;
double P_Single;
double P_Fixed;
String receive;

Adafruit_SSD1306 display(OLED_RESET);
Servo Servo_1;
SoftwareSerial HC06(10, 11); //HC06-TX Pin 10, HC06-RX to Arduino Pin 11
////////////////////////////////////////////////////////////////////////////////////////////////////////
int vertical_up (int pos1){
  Servo_1.attach(9);
  while(pos1 < 145){ // goes from 0 degrees to 180 degrees
   
   light_1=analogRead(A0);
   light_3=analogRead(A2);

   if( ((light_1-light_3)<range) && ((light_1-light_3)>-range)){  //-range< x <range, means balance
    break;}
   else if((light_1-light_3)<-range){ 
    break;} 
   else{pos1 += 1;
    Servo_1.write(pos1);              // tell servo 1 to go to position in variable 'pos'
    delay(15); }                      // waits 15 ms for the servo to reach the position
   
  }//end while
  Servo_1.detach();
  return pos1;
}//end function

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int vertical_down (int pos1){
  Servo_1.attach(9);
  while(pos1 > 35){ // goes from 0 degrees to 180 degrees

   light_1=analogRead(A0);
   light_3=analogRead(A2);

   if( ((light_1-light_3)<range) && ((light_1-light_3)>-range)){  //-range< x <range, means balance
    break;}
   else if((light_1-light_3)>range){
    break;}
   else{pos1 -= 1;
    Servo_1.write(pos1);      // tell servo 1 to go to position in variable 'pos'
    delay(15); }               // waits 15 ms for the servo to reach the position
   
  }//end while
  Servo_1.detach();
  return pos1;
}//end function

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Servo_1.attach(9);
  Servo_1.write(pos1);    //Reset Servo1 to original location
  delay(15);
  Servo_1.detach();
  HC06.begin(9600); //Baudrate 9600 , Choose your own baudrate 
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  display.begin(SSD1306_SWITCHCAPVCC,0x3C);
  display.clearDisplay();
} //end void setup
//////////////////////////////////////////////////

void loop() {
  if(HC06.available() > 0) //When HC06 receive something
  {
    receive = HC06.readStringUntil('\r'); //Read from Serial Communication
  if(receive == "r"){
    light_1=analogRead(A0);
    light_3=analogRead(A2);
    if(  (light_1 - light_3)>range  ){   
      pos1 = vertical_up(pos1); };//end condtion 3
//////////////////////////////////////////////////
    light_1=analogRead(A0);
    light_3=analogRead(A2);
    if(  (light_1 - light_3)<-range  ){ 
      pos1 = vertical_down(pos1); };//end condtion 4
//////////////////////////////////////////////////
    Vo_Single = analogRead(A3);
    Vo_Fixed  = analogRead(A1);
    real_Vo_Single = (Vo_Single * 5) /1023;
    real_Vo_Fixed  = (Vo_Fixed * 5) /1023;
    C_Single   = real_Vo_Single / 12250;
    C_Fixed    = real_Vo_Fixed  / 12250 ;
    Vin_Single = (( C_Single * 22750) + real_Vo_Single) ;
    Vin_Fixed  = (( C_Fixed * 22750 ) + real_Vo_Fixed) ;
    P_Single   = (pow(Vin_Single,2))/35000;
    P_Fixed    = (pow(Vin_Fixed,2))/35000;
//////////////////////////////////////////////////
    HC06.print(pos1);
    HC06.print(",");
    HC06.print(light_1);
    HC06.print(",");
    HC06.print(light_3);
    HC06.print(",");
    HC06.print(Vo_Single);
    HC06.print(",");
    HC06.println(Vo_Fixed);
    
    display.clearDisplay();
    display.setTextSize(0);             
    display.setTextColor(WHITE);        
    display.setCursor(0,8);     
          
    display.print("P_S=");
    display.print(P_Single*1000);
    display.print("mW Vi=");
    display.print(Vin_Single);
    display.println("V");
  
    display.print("P_F=");
    display.print(P_Fixed*1000);
    display.print("mW Vi=");
    display.print(Vin_Fixed);
    display.println("V");

    display.print("Angle = ");
    display.println(pos1);
    display.display();
  }
//////////////////////////////////////////////////////////////////////////////////////////////////  
  else if (receive == "0"){ //reset angle if receive '0'
    pos1 = 90; 
    Servo_1.attach(9);
    Servo_1.write(pos1);
    delay(15);
    Servo_1.detach();
    }
  }
}//end void loop
