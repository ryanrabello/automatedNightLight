//Constants
const int led = 13;
const int ledSwitch = 
//GLobalVars

void setup() {
  //Initialize those output ports.
  pinMode(led,OUTPUT);
  analogWrite(led,0);
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

}

void loop() {
  int a = analogRead(photocellPin);
  if(currentLedSwitch < a){
    //RESET
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
