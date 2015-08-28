//change these values to your liking. 
int LEDpin[3] = {3,5,6};
int photocellPin = 0;
int timeDim = 300;
int timeOn = 1800;
int ledMaxBright = 255;
//photo sensor reading for toggling the led on/off state
int ledSwitch = 5;
//photo sensor sensitivity when light is on. (this will hopefully reduce repeated on/off toggle.)
int ledBuffer = 7;
//led brightness after 5 minutes
int ledDim = 150;

//variables you should not mess with.(they change as the program runs)
int currentLedSwitch = ledSwitch;
int photocellBrightness;
int LEDbrightness;
int time = 0;
int mainDelay = 1000;
float pulseX = 0;

void setup(void) {
  //Start debug link.
  Serial.begin(9600);
  pinMode(LEDpin[0], OUTPUT);
  pinMode(LEDpin[1], OUTPUT);
  pinMode(LEDpin[2], OUTPUT);
}

void loop(void) {
   //For debug
   /*Serial.print(" Analog reading = ");
   Serial.print(photocellBrightness);
   Serial.print(" LEDbrightness = ");
   Serial.print(LEDbrightness);
   Serial.print("Time = ");
   Serial.print(time);
   Serial.print("ledSwitch=");
   Serial.println(currentLedSwitch);*/
   
  if(time > 0)
    time++;
  if(time > timeOn)
    slowFadeOff();
  //grab info
  photocellBrightness = analogRead(photocellPin);
  //range for photo sensor.
  if(time == timeDim)
    dimLed();
  if(time !=-1)
  {
    if (photocellBrightness <= currentLedSwitch)
    {
      if(LEDbrightness != ledDim)
        LEDstate(1);
    }
    else
      LEDstate(-1);
  }
  else
    if(photocellBrightness > currentLedSwitch)
      LEDstate(-1);

  delay(mainDelay);
}
void pulse()
{
  //Not implimented
}
void slowFadeOff()
{
  //Serial.println("running Slow off");
  while (LEDbrightness != 0) {
    LEDbrightness = LEDbrightness - .25;
    analogWrite(LEDpin[0], LEDbrightness);
    analogWrite(LEDpin[1], LEDbrightness);
    analogWrite(LEDpin[2], LEDbrightness);
    delay(25);
  }
  //Put into sleep mode
  time = -1;
  mainDelay = 10000;
}
void dimLed()
{
  //Serial.println("DIM LED");
  while (LEDbrightness > ledDim) {
  LEDbrightness -= .25;
  analogWrite(LEDpin[0], LEDbrightness);
  analogWrite(LEDpin[1], LEDbrightness);
  analogWrite(LEDpin[2], LEDbrightness);
  delay(25);
  }
}

void LEDstate(int  s) {
      //Serial.print("LEDstate s=");
      //Serial.println(s);
  if (s == 1) {
    //FADE ON LED
    currentLedSwitch = ledBuffer;
    while (LEDbrightness != ledMaxBright) {
      LEDbrightness += 1;
      analogWrite(LEDpin[0], LEDbrightness);
      analogWrite(LEDpin[1], LEDbrightness);
      analogWrite(LEDpin[2], LEDbrightness);
      delay(5);
    }
    if( time ==0)
      time = 1;
  } 
  else {
    //FADE OFF LED
    currentLedSwitch = ledSwitch;
    mainDelay = 1000;
    while (LEDbrightness > 0) {
      LEDbrightness -= 1;
      analogWrite(LEDpin[0], LEDbrightness);
      analogWrite(LEDpin[1], LEDbrightness);
      analogWrite(LEDpin[2], LEDbrightness);
      delay(5);
    }
    time = 0;
  }
}



