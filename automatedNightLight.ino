//Constants
const int led = 13;
const int ledSwitch = 5;    //photo sensor reading for toggling the led on/off state
const int ledBuffer = 7;    //photo sensor sensitivity when light is on. (this will hopefully reduce repeated on/off toggle.)
const int ledOn = 255;
const int ledOff = 0;
const int timeToSleep = 10 * 60; //secconds


//GLobalVars
int currentLedSwitch = ledSwitch;
int sleepTime = 0;
int ledSetting = 0;
char mode = 'a';
int prevAnalogRead = 0;

void setup() {
  //Initialize those output ports.
  pinMode(led,OUTPUT);

  //Give the user a test flash.
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(150);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(150);              // wait for a second
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(150);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(150);              // wait for a second

  analogWrite(led,ledOff);
}


//Transistion from current to inputed values
void fadeTo(int state,int transitionTime) {
  int wait = abs(transitionTime/(state - ledSetting));
  int steps = state - ledSetting;
  if(steps < 0){
    for(steps; steps <=0; steps++){
      ledSetting--;
      analogWrite(led, ledSetting);
      delay(wait);
    }
  }else{
    for(steps; steps >=0; steps--){
      ledSetting++;
      analogWrite(led, ledSetting);
      delay(wait);
    }
  }
  analogWrite(led, state);
  ledSetting = state;
}


void lightOn() {
  if(ledSetting != ledOn){
    fadeTo(ledOn, 500);
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
  int a = analogRead(0);
  if(currentLedSwitch < a){
    //RESET
    fadeTo(ledOff,500);
    mode = 'a';
    currentLedSwitch = ledSwitch;
    sleepTime = 0;
  }
  switch(mode) {
    case 'a'://awake check if lights go off
      if(currentLedSwitch >= a){
        lightOn();
      }
      break;
    case 's':
      //sleep mode
      delay(100);
      break;
  }
  delay(100);
}
