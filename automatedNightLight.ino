//Constants
const int led = 13;
const int ledSwitch = 5;    //photo sensor reading for toggling the led on/off state
const int ledBuffer = 7;    //photo sensor sensitivity when light is on. (this will hopefully reduce repeated on/off toggle.)
const int ledOn = 255;
const int ledOff = 0;
const int timeToSleep = 10 * 60; //secconds
const int photocellPin = 0;


//GLobalVars
int currentLedSwitch = ledSwitch;
int sleepTime = 0;
int ledSetting = 0;
char mode = 'a';

void setup() {
  //Initialize those output ports.
  //pinMode(led,OUTPUT);
  analogWrite(led,ledOff);
  for(int i = 0; i<=255; i++){
    analogWrite(led,i);
    delay(100);
  }
}


//Transistion from current to inputed values
void fadeTo(int state,int transitionTime) {
  int delay = abs(transitionTime/(state - ledSetting));
  int steps = state - ledSetting;
  if(steps < 0){
    for(steps; steps <=0; steps++){
      ledSetting--;
      analogWrite(led, ledSetting);
    }
  }

}


void lightOn() {
  if(ledSetting != ledOn){
    fadeTo(ledOn, 1000);
    ledSetting = ledOn;
    sleepTime = 0;
  }else {
    sleepTime++;
    if(sleepTime >= timeToSleep){
      fadeTo(ledOff, 5000);
      mode = 's';
    }
  }
}

void loop() {
  if(currentLedSwitch < analogRead(photocellPin)){
    //RESET
    fadeTo(ledOff,1000);
    mode = 'a';
    currentLedSwitch = ledSwitch;
    sleepTime = 0;
  }
  switch(mode) {
    case 'a'://awake check if lights go off
      if(currentLedSwitch >= analogRead(photocellPin)){
        lightOn();
      }
      break;
    case 's':
      //sleep mode
      delay(10000);
      break;
  }
  delay(1000);
}
