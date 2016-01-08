//Initialize led pins
int led[] = {
  9,5,6};  //For 

//Other config options see https://github.com/asteroidice/automatedNightLight/ for detailed info on what they do.
const int photocellPin = 0;
const int timeDim = 60;
const int timeOn = 1800;    
const int ledMaxBright = 255;
const int ledDim = 150;     //led brightness after timeDim seconds.
const int ledSwitch = 5;    //photo sensor reading for toggling the led on/off state
const int ledBuffer = 7;    //photo sensor sensitivity when light is on. (this will hopefully reduce repeated on/off toggle.)

//Don Touch it!!!
//These values are global variables becuase they need to be referenced later.
//they are set to 255 so RGB led's are off.
int ledStates[] = {
  255,255,255};
//More global variables
int currentLedSwitch = ledSwitch;
int photocellBrightness;
int time = 0;
int mainDelay = 1000;
float pulseX = 0;

void setup() {
  //Initialize those output ports.
  for(int i = 0; i < (sizeof(led)/sizeof(int)); i++) {
    pinMode(led[i],OUTPUT);
    analogWrite(led[i],255);//Turn off all RGB led pins
  }
  //Get random brightness
  randomSeed(analogRead(0));
}

//set all the led's to the inputed value
void writeAll(int pwm){
  for(int i = 0; i < (sizeof(led)/sizeof(int)); i++) {
    analogWrite(led[i],pwm);
    ledStates[i] = pwm;
  }
}

//Transistion from current to inputed values 
void fadeTo(int red, int green, int blue,int transitionTime) {
  //Want 20 fps
  float spf = 1.0/((float)transitionTime*9.0);
  //int time = transitionTime / (transitionTime*20); //Get time percycle in microseconds.
  float change[] = {
    red - ledStates[0],green - ledStates[1],blue - ledStates[2]  }; //Difference in all the values.
  //Run transition
  for(float i=0; i <= 1.00;) {
    i += spf;
    //Set all Led to the right value
    for(int j=0; j < 3; j++){
      analogWrite(led[j], i*change[j] + (float)ledStates[j]);
    }
  }
  ledStates[0] = red;
  ledStates[1] = green;
  ledStates[2] = blue;
}

//Transition with sine curve
void sineFadeTo(int red, int green, int blue,int transitionTime) {
  //Want 20 fps
  float spf = 3.14159265/((float)transitionTime*9.0);
  //int time = transitionTime / (transitionTime*20); //Get time percycle in microseconds.
  float change[] = {
    red - ledStates[0],green - ledStates[1],blue - ledStates[2]  }; //Difference in all the values.
  //Run transition
  float sineI;
  for(float i=0; i <= 1.00;) {
    i += spf;
    sineI=(sin(PI*i-PI*.5)+1)/2;
    //Set all Led to the right value
    for(int j=0; j < 3; j++){
      analogWrite(led[j], sineI*change[j] + (float)ledStates[j]);
    }
  }
  ledStates[0] = red;
  ledStates[1] = green;
  ledStates[2] = blue;
}

void LEDstate(int  s) {
  int rand = (int)(random(0,3) + .5);
  if (s == 1) {
    //FADE ON LED
    currentLedSwitch = ledBuffer;
    switch(rand) {
    case 0:
      fadeTo(150,255,255,1000);
      break;
    case 1:
      fadeTo(255,255,150,1000);
      break;
    default:
      fadeTo(255,150,255,1000);
      break;
    }
    if( time ==0)
      time = 1;
  }
  else {
    //FADE OFF LED
    currentLedSwitch = ledSwitch;
    mainDelay = 1000;
    fadeTo(255,255,255,1000);
    time = 0;
  }
}

void loop() {
  if(time > 0)
    time++;
  if(time > timeOn){
    //Put light into sleep mode
    time = -1;
    mainDelay = 10000;
    fadeTo(255,255,255,10000);
    ;//Slow off LED
  }
  //grab info
  photocellBrightness = analogRead(photocellPin);
  //range for photo sensor.
  if(time == timeDim){
    fadeTo(220,240,255,3000);//LED dim
  }
  if(time !=-1)
  {
    if (photocellBrightness <= currentLedSwitch)
    {
      if(!(time > 0))
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

