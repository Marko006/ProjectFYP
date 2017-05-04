int Index;

void setup() 
{
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
 


 digitalWrite(4,LOW);
  digitalWrite(10,HIGH);

  for(Index = 0; Index < 300; Index++)
  {
    digitalWrite(5,HIGH);
    digitalWrite(11,HIGH);
    delayMicroseconds(500);
    digitalWrite(5,LOW);
    digitalWrite(11,LOW);
    delayMicroseconds(500);
  }
delay(1000);

  digitalWrite(4,HIGH);
  digitalWrite(10,LOW);

  for(Index = 0; Index < 300; Index++)
  {
    digitalWrite(5,HIGH);
    digitalWrite(11,HIGH);
    delayMicroseconds(500);
    digitalWrite(5,LOW);
    digitalWrite(11,LOW);
    delayMicroseconds(500);
  }
 delay(1000);
}

