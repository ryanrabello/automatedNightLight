//Initialize led pins
const int led[] = {9,5,6};

//Other config options see https://github.com/asteroidice/automatedNightLight/ for detailed info on what they do.
const int photocellPin = 0;
const int timeOn = 1800;
const int ledOff = 255;     //Led full off pwm value.
const int ledOn = 150;        //led full pwm brightness.
const int ledSwitch = 5;    //photo sensor reading for toggling the led on/off state
const int ledBuffer = 7;    //photo sensor sensitivity when light is on. (this will hopefully reduce repeated on/off toggle.)

//Don Touch it!!!
//These values are global variables becuase they need to be referenced later.
//they are set to ledOff so RGB led's are off.
int ledStates[] = {
  ledOff,ledOff,ledOff};
//More global variables
int currentLedSwitch = ledSwitch;
int time = 0;
int mainDelay = 1000;
char mode = 'a';
int prevAnalogRead;

void setup() {
  //Initialize those output ports.
  pinMode(led[0],OUTPUT);
  analogWrite(led[0],ledOff);
  pinMode(led[1],OUTPUT);
  analogWrite(led[1],ledOff);
  pinMode(led[2],OUTPUT);
  analogWrite(led[2],ledOff);
  //Get random brightness and use it as the seed.
  randomSeed(analogRead(0));
}

//set all the led's to the inputed value
void writeAll(int pwm){
  writeAll(pwm,pwm,pwm);
}
void writeAll(int r, int g, int b) {
  analogWrite(led[0],r);
  ledStates[0] = r;
  analogWrite(led[1],g);
  ledStates[1] = g;
  analogWrite(led[2],b);
  ledStates[2] = b;
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


void lightOn() {
  //Turn light on for X minutes and then fade it to dark then turn off light and goto sleep mode (all the while checking if the lights came back on)
  if(ledStates[0] != ledOff || ledStates[1] != ledOff || ledStates[2] != ledOff) { //if light is on
    time++;
    if(time % 10 == 0)
      changeLed(true);
  }else {
    changeLed();
    currentLedSwitch = ledBuffer;
  }

}
void changeLed(){
  changeLed(false);
}
void changeLed(boolean slow) {
  int ranLed[3];
  if((int)(random(0,4) + .5) == 0){
    //25% of time.
    for (int i = 0; i < 3; i ++){
      ranLed[i] = (int)(random(ledOn,ledOff) + .5);
    }
  }else{
    //75% of time.
    while(true){
      for (int i = 0; i < 3; i ++){
        ranLed[i] = (ledOff-ledOn)*(int)(random(0,2)) + ledOn;
      }
      if(ranLed[0] != ledOff || ranLed[1] != ledOff || ranLed[2] != ledOff)
        break;
    }
  }
  if(slow)
    fadeTo(ranLed[0],ranLed[1],ranLed[2],3000);
  else
    fadeTo(ranLed[0],ranLed[1],ranLed[2],1000);
}

void randomPattern() {
  int count = 5;
  int ranLed[3];
  while(count > 0) {
    //Configure led array
    if((int)(random(0,4) + .5) == 0){
      //25% of time.
      for (int i = 0; i < 3; i ++){
        ranLed[i] = (int)(random(ledOn,ledOff) + .5);
      }
    }else{
      //75% of time.
      while(true){
        for (int i = 0; i < 3; i ++){
          ranLed[i] = (ledOff-ledOn)*(int)(random(0,2)) + ledOn;
        }
        if(ranLed[0] != ledOff || ranLed[1] != ledOff || ranLed[2] != ledOff)
          break;
      }
    }
    fadeTo(ranLed[0],ranLed[1],ranLed[2],75);
    //writeAll(ranLed[0],ranLed[1],ranLed[2]);

    delay(500);
    //run atleast twice then there exists a probability of exiting.
    if(count < (int)(random(0,5)+.5))
      break;
    count--;
  }
  fadeTo(ledOff,ledOff,ledOff,250);
}

void loop() {
  int a = analogRead(photocellPin);
  if(currentLedSwitch < a){
    fadeTo(ledOff,ledOff,ledOff,1000);
    mode = 'a';
    mainDelay = 1000;
    currentLedSwitch = ledSwitch;
    time = 0;
  }
  switch(mode) {
    case 'a'://awake check if lights go off
      if(currentLedSwitch >= a){
        lightOn();
      }else{
        //add some fun on abiance changes
        if(abs(a-prevAnalogRead) > 5)
          randomPattern();
        else
          prevAnalogRead = a;
      }
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
