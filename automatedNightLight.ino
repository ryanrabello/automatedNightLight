//Initialize led pins
int led[] = {9,5,6};

//Other config options see https://github.com/asteroidice/automatedNightLight/ for detailed info on what they do.
const int photocellPin = 0;
const int timeDim = 120;
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
int time = 0;
int mainDelay = 1000;
char mode = 'a';

void setup() {
  //Initialize those output ports.
  for(int i = 0; i < (sizeof(led)/sizeof(int)); i++) {
    pinMode(led[i],OUTPUT);
    analogWrite(led[i],255);//Turn off all RGB led pins
  }
  //Get random brightness and use it as the seed.
  randomSeed(analogRead(0));
  Serial.begin(9600);
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
void lightOn() {
  //Turn light on for X minutes and then fade it to dark then turn off light and goto sleep mode (all the while checking if the lights came back on)
  if(ledStates[0] != 255 || ledStates[1] != 255 || ledStates[2] != 255) { //if light is on
    time++;
    if(time >= timeDim){
      mode = 'n';
      fadeTo(220,240,255,3000);
    }
    if(time % 10 == 0)
      changeLed();
  }else {
    changeLed();
    currentLedSwitch = ledBuffer;
  }
  
}
void changeLed() {
  int ranLed[3];
  if((int)(random(0,4) + .5) == 0){
    //25% of time. 
    for (int i = 0; i < 3; i ++){
      ranLed[i] = (int)(random(150,255) + .5);
    }
  }else{
    //75% of time. 
    while(true){
      for (int i = 0; i < 3; i ++){
        ranLed[i] = 105*(int)(random(0,2)) + 150;
      }
      if(ranLed[0] != 255 || ranLed[1] != 255 || ranLed[2] != 255)
        break;
    }
  }
  fadeTo(ranLed[0],ranLed[1],ranLed[2],1000);
}
  
void nap() {
  time++;
  if(time >= timeOn){
    mode = 's';
    fadeTo(255,255,255,3000);
    mainDelay = 10000;
  }
}
void reset() {
  fadeTo(255,255,255,1000);
  mode = 'a';
  mainDelay = 1000;
  currentLedSwitch = ledSwitch;
  time = 0;
}
void loop() {
  if(currentLedSwitch < analogRead(photocellPin)){
    reset();
  }
  switch(mode) {
    case 'a'://awake check if lights go off
      if(currentLedSwitch >= analogRead(photocellPin)){
        lightOn();
      }
      break;
    case 'n':
      nap();
      break;
    case 's':
      //sleep mode
      delay(10000);
      break;
    default:
      mode = 'n';
      break;
  }
  delay(mainDelay);
}

