#include <Servo.h>


int Index;
int rCnt = 0;
int lCnt = 0;
int P1 = 0;
int P2 = 0;
int P3 = 0;
int P4 = 0;
int P5 = 0;
int P6 = 0;
int P7 = 0;
int P8 = 0;
int P9 = 0;
int attack = 0;
double CurrentPosition = 0;
int stp = 7;  //connect pin 13 to step
int dir = 8;  // connect pin 12 to dir
int stp2 = 12;  //connect pin 13 to step
int dir2 = 5;  // connect pin 12 to dir
int a = 0;     //  gen counter


int incomingByte = 0;   // for incoming serial data

void setup()
{
  Serial.begin(9600); // // opens serial port, sets data rate to 9600 bps
 
  // STEPPER PINS
   
  pinMode(6, OUTPUT); //Enable
  pinMode(5, OUTPUT); //Step
  pinMode(4, OUTPUT); //Direction

  pinMode(12, OUTPUT); //Enable
  pinMode(11, OUTPUT); //Step
  pinMode(10, OUTPUT); //dir

  digitalWrite(6,LOW);
  digitalWrite(12,LOW); 

}

void loop()
{  
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
   if(incomingByte == 'a' && attack == 0 ) 
    { 
                   
             digitalWrite(4,HIGH);//LOW = LEFT
              digitalWrite(10,LOW);//LOW = LEFT
            
              for(Index = 0; Index < 150; Index++)
              {
                digitalWrite(5,HIGH);
                digitalWrite(11,HIGH);
                delayMicroseconds(500);
                digitalWrite(5,LOW);
                digitalWrite(11,LOW);
                delayMicroseconds(500);
              }
              delay(500);
              digitalWrite(4,LOW);//LOW = LEFT
              digitalWrite(10,HIGH);//LOW = LEFT
            
              for(Index = 0; Index < 150; Index++)
              {
                digitalWrite(5,HIGH);
                digitalWrite(11,HIGH);
                delayMicroseconds(500);
                digitalWrite(5,LOW);
                digitalWrite(11,LOW);
                delayMicroseconds(500);
              }
            
          attack++;
          delay(1000);
    }
    else if(incomingByte == 'd')
    {
      attack = 0;
    }

      else if(incomingByte == '2' && P2 == 0 ){
      Serial.print("MOVE LEFT");
          
        if(P3 == 1){
        digitalWrite(4,LOW);//LOW = LEFT
        digitalWrite(10,LOW);//LOW = LEFT
      
        for(Index = 0; Index < 150; Index++)
        {
          digitalWrite(5,HIGH);
          digitalWrite(11,HIGH);
          delayMicroseconds(500);
          digitalWrite(5,LOW);
          digitalWrite(11,LOW);
          delayMicroseconds(500);
        }
     
        }
        else if(P1==1)
        {
          digitalWrite(4,HIGH);
            digitalWrite(10,HIGH);
          
            for(Index = 0; Index < 150; Index++)
            {
              digitalWrite(5,HIGH);
              digitalWrite(11,HIGH);
              delayMicroseconds(500);
              digitalWrite(5,LOW);
              digitalWrite(11,LOW);
              delayMicroseconds(500);
            }
           
        }
       
        P2++;
        P3=0;
        delayMicroseconds(150);
     }

    else  if(incomingByte == '3' && P3 == 0 ){
      Serial.print("MOVE LEFT");
          
        if (P4 ==1){
        digitalWrite(4,LOW);
        digitalWrite(10,LOW);
      
        for(Index = 0; Index < 150; Index++)
        {
          digitalWrite(5,HIGH);
          digitalWrite(11,HIGH);
          delayMicroseconds(500);
          digitalWrite(5,LOW);
          digitalWrite(11,LOW);
          delayMicroseconds(500);
        }
     
        }
        else if(P2 == 1)
        {
           digitalWrite(4,HIGH);
            digitalWrite(10,HIGH);
          
            for(Index = 0; Index < 150; Index++)
            {
              digitalWrite(5,HIGH);
              digitalWrite(11,HIGH);
              delayMicroseconds(500);
              digitalWrite(5,LOW);
              digitalWrite(11,LOW);
              delayMicroseconds(500);
            }
       
        }
       
        P3++;
        P2=0;
        P4=0;
       delayMicroseconds(150);
     }

   else  if(incomingByte == '4' && P4 == 0 ){
      Serial.print("MOVE LEFT");
        if(P5 == 1 ){   
        digitalWrite(4,LOW);
        digitalWrite(10,LOW);
      
        for(Index = 0; Index < 150; Index++)
        {
          digitalWrite(5,HIGH);
          digitalWrite(11,HIGH);
          delayMicroseconds(500);
          digitalWrite(5,LOW);
          digitalWrite(11,LOW);
          delayMicroseconds(500);
        }
  
        }
        else if(P3==1)
        {
          digitalWrite(4,HIGH);
            digitalWrite(10,HIGH);
          
            for(Index = 0; Index < 150; Index++)
            {
              digitalWrite(5,HIGH);
              digitalWrite(11,HIGH);
              delayMicroseconds(500);
              digitalWrite(5,LOW);
              digitalWrite(11,LOW);
              delayMicroseconds(500);
            }
       
        }
        P4++;
        P3=0;
        P5=0;
       delayMicroseconds(150);
     }
     else  if(incomingByte == '5' && P5 == 0 ){
      Serial.print("MOVE LEFT");

        if(P6 ==1){  
        digitalWrite(4,LOW);
        digitalWrite(10,LOW);
      
        for(Index = 0; Index < 150; Index++)
        {
          digitalWrite(5,HIGH);
          digitalWrite(11,HIGH);
          delayMicroseconds(500);
          digitalWrite(5,LOW);
          digitalWrite(11,LOW);
          delayMicroseconds(500);
        }
         
        }
        else if(P4 == 1)
        {
          digitalWrite(4,HIGH);
            digitalWrite(10,HIGH);
          
            for(Index = 0; Index < 150; Index++)
            {
              digitalWrite(5,HIGH);
              digitalWrite(11,HIGH);
              delayMicroseconds(500);
              digitalWrite(5,LOW);
              digitalWrite(11,LOW);
              delayMicroseconds(500);
            }
                 
        }
       
        P5++;
        P4=0;
        P6=0;
      delayMicroseconds(150);
     }
  else if(incomingByte == '6' && P6 == 0){
       Serial.print("MOVE LEFT");
         
          if(P7 ==1){  
        digitalWrite(4,LOW);
        digitalWrite(10,LOW);
      
        for(Index = 0; Index < 150; Index++)
        {
          digitalWrite(5,HIGH);
          digitalWrite(11,HIGH);
          delayMicroseconds(500);
          digitalWrite(5,LOW);
          digitalWrite(11,LOW);
          delayMicroseconds(500);
        }
             
        }
        else if(P5 == 1)
        {
          digitalWrite(4,HIGH);
            digitalWrite(10,HIGH);
          
            for(Index = 0; Index < 150; Index++)
            {
              digitalWrite(5,HIGH);
              digitalWrite(11,HIGH);
              delayMicroseconds(500);
              digitalWrite(5,LOW);
              digitalWrite(11,LOW);
              delayMicroseconds(500);
            }
                
        }

         
        P6++;
        P5=0;
        P7=0;
        delayMicroseconds(150);
        }
          else if(incomingByte == '7' && P7 == 0){
       Serial.print("MOVE LEFT");
         
        if(P8 ==1){  
        digitalWrite(4,LOW);
        digitalWrite(10,LOW);
      
        for(Index = 0; Index < 150; Index++)
        {
          digitalWrite(5,HIGH);
          digitalWrite(11,HIGH);
          delayMicroseconds(500);
          digitalWrite(5,LOW);
          digitalWrite(11,LOW);
          delayMicroseconds(500);
        }
             
        }
        else if(P6 == 1)
        {
          digitalWrite(4,HIGH);
            digitalWrite(10,HIGH);
          
            for(Index = 0; Index < 150; Index++)
            {
              digitalWrite(5,HIGH);
              digitalWrite(11,HIGH);
              delayMicroseconds(500);
              digitalWrite(5,LOW);
              digitalWrite(11,LOW);
              delayMicroseconds(500);
            }
                 
        }

         
        P7++;
        P6=0;
        P8=0;
        delayMicroseconds(150);
        }
          else if(incomingByte == '8' && P8 == 0){
       Serial.print("MOVE LEFT");
         
       if(P7 == 1)
        {
          digitalWrite(4,HIGH);
            digitalWrite(10,HIGH);
          
            for(Index = 0; Index < 150; Index++)
            {
              digitalWrite(5,HIGH);
              digitalWrite(11,HIGH);
              delayMicroseconds(500);
              digitalWrite(5,LOW);
              digitalWrite(11,LOW);
              delayMicroseconds(500);
            }
                
        }
        else{
          digitalWrite(4,LOW);
        digitalWrite(10,LOW);
      
        for(Index = 0; Index < 150; Index++)
        {
          digitalWrite(5,HIGH);
          digitalWrite(11,HIGH);
          delayMicroseconds(500);
          digitalWrite(5,LOW);
          digitalWrite(11,LOW);
          delayMicroseconds(500);
        }
        }

         
        P8++;
        P7=0;
       
        delayMicroseconds(150);
        }
         
  }
}

