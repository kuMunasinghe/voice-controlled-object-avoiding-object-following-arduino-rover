#include <Servo.h>
#include <NewPing.h>


#define echo A0  // Define UltraSonic echo & trig pins 
#define trig A1
#define max_dis 300 // max distance 300cm

NewPing sonar(trig, echo, max_dis); //assigning lib.

#define ir 12 // Proximity Sensor

#define led 2 //LED pin

#define buzz 7 // Buzzer pin

#define lf 3 //turn leftside wheels to move Forward
#define lb 11  //turn leftside wheels to move Backward
#define rf 5  //turn rightside wheels to move Forward
#define rb 6  //turn rightside wheels to move Backward



Servo servo; // defining servo

String cmd;
// char c;
int distance;


int range=100; // range declaration for obj_avoid


boolean goesForward = false; 

void setup()
{ 
  Serial.begin(9600);
  pinMode(lf, OUTPUT);
  pinMode(lb, OUTPUT);
  pinMode(rf, OUTPUT);
  pinMode(rb, OUTPUT);

  pinMode(led, OUTPUT);

  pinMode(buzz, OUTPUT);


  pinMode(ir, INPUT);

  servo.attach(9);
  test();
  servo.write(90);
  int rightdis=0;
  int leftdis=0;

}
void loop()
{
  
  distance=sonar.ping_cm();
  
  
  if(Serial.available()>0)
  {
    cmd="";
    delay(2);
    cmd=Serial.readString();
    delay(2);

    Serial.println(cmd);

    if(cmd=="left"||cmd=="turn left"||cmd=="L")
    {
      tone(buzz,2500,150);
      left();
      
    }
   else if(cmd=="right"||cmd=="turn right"||cmd=="R")
    {
      tone(buzz,2500,150);
      right();
    }
    else if (cmd=="stop"||cmd=="break")
    {
      tone(buzz,2500,300);
      servo.write(90);
      digitalWrite(led,LOW);
      Stop();
    }
    
    else 
    {
      Stop();
    }
    
  }
  if(cmd=="forward"||cmd=="move forward"||cmd=="F")
  {
    forward();
  }
  if(cmd=="backward"||cmd=="move backward"||cmd=="B")
  {

    backward();
  }
  if(cmd=="follow me"||cmd=="follow")
  {
    follow();
  }
  if(cmd=="auto")
  {
    obj_avoid();
  }

} 
  




/*-----------------------------------------------------------------------------follow mode-----------------------------------------------------------------------------------------*/

void follow()
{
 int dist=readPing();

  if(dist>30)
  {
    Stop();
  }
  if(dist<25 && dist>20)
  {
   
    analogWrite(lf, 100);
    analogWrite(rf, 100);
    digitalWrite(lb, LOW);
    digitalWrite(rb, LOW);
    delay(100);
  }
  if(dist<20 && dist>15 )
  {
    
    analogWrite(lf, 100);
    analogWrite(rf, 100);
    digitalWrite(lb, LOW);
    digitalWrite(rb, LOW);
    delay(100);
  }
  if (dist<15 && dist>12)
  {
    digitalWrite(led,HIGH);
    delay(60);
    digitalWrite(led,LOW);
    delay(60);
    tone(buzz,2500,100);
    analogWrite(lb, LOW);
    analogWrite(rb, LOW);
    digitalWrite(lf, LOW);
    digitalWrite(rf, LOW);
    delay(100);
  }
  if(dist<12 && dist>2)
  {
    analogWrite(lb, 100);
    analogWrite(rb, 100);
    digitalWrite(lf, LOW);
    digitalWrite(rf, LOW);
    delay(100);
  }
}

/*---------------------------------------------------------------------------Motor Movement---------------------------------------------------------------------------------------- */


void Stop()
{
  digitalWrite(lf, LOW);
  digitalWrite(rf, LOW);
  digitalWrite(lb, LOW);
  digitalWrite(rb, LOW);
}

void forward()
{
  int distance = readPing();
  if(distance < 15)
  {
    digitalWrite(led,HIGH);
    Stop();
    cmd="";
  }
 else
 {
  digitalWrite(led,LOW);
  digitalWrite(lf, HIGH);
  digitalWrite(rf, HIGH);
  digitalWrite(lb, LOW);
  digitalWrite(rb, LOW);
}
}

void backward()
{
  
  int ir_sensor=digitalRead(ir);
  if(ir_sensor==0)
  {
    Stop();
    cmd="";
  }
  else
  {
   digitalWrite(led,HIGH);
  delay(250);
  digitalWrite(led,LOW);
  delay(250);
  analogWrite(lb, 150);
  analogWrite(rb, 150);
  digitalWrite(lf, LOW);
  digitalWrite(rf, LOW);
  }
}

void left()
{ 
  servo.write(30);
  delay(300);
  int look=readPing();
  delay(300);
  servo.write(90);
  delay(300);
  if(look < 15)
  {
    Stop();
    digitalWrite(led,HIGH);
    delay(3000);
    digitalWrite(led,LOW);
    
  }
  else
  {
    analogWrite(rf, 150);
    analogWrite(lb, 150);
    digitalWrite(lf, LOW);
    digitalWrite(rb, LOW);
    delay(600);
    digitalWrite(lf, HIGH);
    digitalWrite(rf, HIGH);
    digitalWrite(lb, LOW);
    digitalWrite(rb, LOW);
  }
}

void right()
{ 
  servo.write(160);
  delay(300);
  int look=readPing();
  delay(300);
  servo.write(90);
  delay(300);
  if(look < 15)
  {
    Stop();
    digitalWrite(led,HIGH);
    delay(3000);
    digitalWrite(led,LOW);
    
  }
  else
  {
  analogWrite(rb, 150);
  analogWrite(lf, 150);
  digitalWrite(rf, LOW);
  digitalWrite(lb, LOW);
  delay(750);
  forward();
  }
}

void moveforward(){

  if(!goesForward)
  {

    goesForward=true;
    analogWrite(lf, 100);
    analogWrite(rf, 100);
    digitalWrite(lb, LOW);
    digitalWrite(rb, LOW);
    
    }
}
/*---------------------------------------------------------------------------------------Read Ping----------------------------------------------------------------------------------- */
int readPing() { 
  delay(70);
  int cm = sonar.ping_cm();
  if(cm==0)
  {
    cm = 250;
  }
  return cm;
}

/*-----------------------------------------------------------------------------servo+ping left--------------------------------------------------------------------------------------*/

int lookLeft()
{
   servo.write(30); 
   delay(300);
   int distance = readPing();
   delay(100);
   servo.write(90); 
   return distance;
   delay(100);
} 


/*-----------------------------------------------------------------------------servo+ping right--------------------------------------------------------------------------------------*/
int lookRight()
{
   servo.write(160); 
   delay(300);
   int distance = readPing();
   delay(100);
   servo.write(90); 
   return distance;
   delay(100);
}

/*----------------------------------------------------------------------------------object avoid--------------------------------------------------------------------------------------*/

void obj_avoid()
{
 int rightdis=0;  
 int leftdis=0;
 delay(10);



 if (range<=15)
 {
  Stop();
  delay(300);
  backward();
  delay(300);
  Stop();
  delay(300);
  rightdis=lookRight();
  delay(300);
  leftdis=lookLeft();
  delay(300);

  if(range>=leftdis)
  {
    analogWrite(rb, 100);
    analogWrite(lf, 100);
    digitalWrite(rf, LOW);
    digitalWrite(lb, LOW);
    delay(600);
    analogWrite(lf, 100);
    analogWrite(rf, 100);
    digitalWrite(lb, LOW);
    digitalWrite(rb, LOW);
    Stop();
  }
  else
  {
    analogWrite(rf, 100);
    analogWrite(lb, 100);
    digitalWrite(lf, LOW);
    digitalWrite(rb, LOW);
    delay(600);
    analogWrite(lf, 100);
    analogWrite(rf, 100);
    digitalWrite(lb, LOW);
    digitalWrite(rb, LOW);
    Stop();
  }
 }
 else
 {
  moveforward();
 }
  range=readPing();
}


/*-----------------------------------------------------------------------------setup test-----------------------------------------------------------------------------------------*/

void test()
{ 
  tone(buzz,2500,200);
  for(int x=0;x<=5;++x)
  {
    digitalWrite(led,HIGH);
    delay(100);
    digitalWrite(led,LOW);
    delay(100);
  }

}
