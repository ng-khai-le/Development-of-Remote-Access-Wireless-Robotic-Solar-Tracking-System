#include <SoftwareSerial.h>
#include <Servo.h>
#include <math.h>
int light_1;
int light_2;
int light_3;
int light_4;
static int pos1=90;
static int pos2=90;
const int range=20;
int Vo_AADAT;
int Vo_Fixed;
double Vin_AADAT;
double Vin_Fixed;
double real_Vo_AADAT;
double real_Vo_Fixed;
double C_AADAT;
double C_Fixed;
double P_AADAT;
double P_Fixed;
String receive;
Servo Servo_1;
Servo Servo_2;
SoftwareSerial HC06(11, 12); //HC06-TX Pin 11, HC06-RX to Arduino Pin 12
////////////////////////////////////////////////////////////////////////////////////////////////////////
int horizontal_right (int pos2){           
  Servo_2.attach(10);
  while( pos2 < 145) { // goes from 180 degrees to 0 degrees
    
   int light_2=analogRead(A1);
   int light_4=analogRead(A3);
  
   if( ((light_4-light_2)<range) && ((light_4-light_2)>-range)){  //-range< x <range, means balance
     break;}   
   else if(pos1>90){
      if((light_4-light_2)<-range){
          break;} }
   else{ //pos1<=90
      if((light_4-light_2)>range){
          break;} }                
   
    pos2+= 1;
    Servo_2.write(pos2);              // tell servo 2 to go to position in variable 'pos'
    delay(15);                        // waits 15 ms for the servo to reach the position  
  
  }//end while
  Servo_2.detach();
  return pos2;
}//end void

////////////////////////////////////////////////////////////////////////////////////////////////////////
int horizontal_left (int pos2){
  Servo_2.attach(10);
  while( pos2 > 35) { // goes from 180 degrees to 0 degrees

   int light_2=analogRead(A1);
   int light_4=analogRead(A3);

   if( ((light_4-light_2)<range) && ((light_4-light_2)>-range)){  //-range< x <range, means balance
      break;}
   else if(pos1>90){
      if((light_4-light_2)>range){
          break;} }
   else{ //pos1<=90
       if((light_4-light_2)<-range){
          break;} }

   pos2-= 1;
   Servo_2.write(pos2);              // tell servo 2 to go to position in variable 'pos'
   delay(15);                        // waits 15 ms for the servo to reach the position
   
  }//end while
  Servo_2.detach();
  return pos2;
}//end void

////////////////////////////////////////////////////////////////////////////////////////////////////////
int vertical_up (int pos1){
  Servo_1.attach(9);
  while(pos1 < 145){ // goes from 0 degrees to 180 degrees

   int light_1=analogRead(A0);
   int light_3=analogRead(A2);

   if( ((light_1-light_3)<range) && ((light_1-light_3)>-range)){  //-range< x <range, means balance
    break;}
   else if((light_1-light_3)<-range){ 
    break;} 
   else{
    pos1 += 1;
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

   int light_1=analogRead(A0);
   int light_3=analogRead(A2);

   if( ((light_1-light_3)<range) && ((light_1-light_3)>-range)){  //-range< x <range, means balance
    break;}
   else if((light_1-light_3)>range){
    break;}
   else{
    pos1 -= 1;
    Servo_1.write(pos1);      // tell servo 1 to go to position in variable 'pos'
    delay(15); }               // waits 15 ms for the servo to reach the position
   
  }//end while
  Servo_1.detach();
  return pos1;
}//end function

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Servo_1.attach(9);
  Servo_2.attach(10);
  Servo_1.write(pos1);    //Reset Servo1 to original location
  Servo_2.write(pos2);    //Reset Servo1 to original location
  delay(15);
  Servo_1.detach();
  Servo_2.detach();
  HC06.begin(115200); //Baudrate 9600 , Choose your own baudrate 
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
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
    delay(15);
//////////////////////////////////////////////////   
    light_2=analogRead(A1);
    light_4=analogRead(A3);
    if(  (light_4-light_2)>range   ){
      if(pos1>90){
        pos2 = horizontal_right(pos2);}else{
        pos2 = horizontal_left(pos2);}
      };//end condtion 1
///////////////////////////////////////
    light_2=analogRead(A1);
    light_4=analogRead(A3);
    if(  (light_4-light_2)<-range   ){
      if(pos1>90){
        pos2 = horizontal_left(pos2);}else{ 
        pos2 = horizontal_right(pos2);}
      };//end condtion 2
///////////////////////////////////////     
    Vo_AADAT  = analogRead(A5);
    Vo_Fixed  = analogRead(A4);
    real_Vo_AADAT = (Vo_AADAT * 5) /1023;
    real_Vo_Fixed = (Vo_Fixed * 5) /1023;
    C_AADAT   = real_Vo_AADAT/ 12250;
    C_Fixed   = real_Vo_Fixed/ 12250;
    Vin_AADAT  = (( C_AADAT * 22750 ) + real_Vo_AADAT);
    Vin_Fixed  = (( C_Fixed * 22750 ) + real_Vo_Fixed);
    P_AADAT   = pow(Vin_AADAT,2)/35000;
    P_Fixed   = pow(Vin_Fixed,2)/35000;
//////////////////////////////////////////////////
    HC06.print(pos1);
    HC06.print(",");
    HC06.print(pos2);
    HC06.print(",");
    HC06.print(light_1);
    HC06.print(",");
    HC06.print(light_3);
    HC06.print(",");
    HC06.print(light_2);
    HC06.print(",");
    HC06.print(light_4);
    HC06.print(",");
    HC06.print(Vo_AADAT);
    HC06.print(",");
    HC06.println(Vo_Fixed);
  }
//////////////////////////////////////////////////////////////////////////////////////////////////  
  else if (receive == "0"){ //reset angle if receive '0'
    Servo_1.attach(9);
    Servo_2.attach(10);
    pos1 = 90; 
    pos2 = 90;
    Servo_1.write(pos1);
    Servo_2.write(pos2);
    delay(15);
    Servo_1.detach();
    Servo_2.detach();
    }
  }
}//end void loop
