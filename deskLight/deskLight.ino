/**
 * Hi! 
 * This program controls an RGB LED with some fade animations. 
 * You can use this by adjusting the LED pins below and adjusting 
 * the value you'd like to use for on and off. 
 **/


//Initialize led pins
int led[] = {6, 5, 3};

const int ledOff = 255; //Led full off pwm value.
const int ledOn = 175;  //led full on pwm brightness.
const int mainDelay = 1000;

void setup()
{
    //Initialize those output ports.
    for (int i = 0; i < (sizeof(led) / sizeof(int)); i++)
    {
        pinMode(led[i], OUTPUT);
        analogWrite(led[i], ledOff); //Turn off all RGB led pins
    }
    //Get random brightness and use it as the seed.
    randomSeed(analogRead(0));
}

/**
 * Fade from the current LED state to the inputed one
 **/
int time = 0;
int ledStates[] = {ledOff, ledOff, ledOff};
void fadeTo(int red, int green, int blue, int transitionTime)
{
    //Want 20 fps
    float spf = 1.0 / ((float)transitionTime * 9.0);
    //int time = transitionTime / (transitionTime*20); //Get time percycle in microseconds.
    float change[] = {
        red - ledStates[0], green - ledStates[1], blue - ledStates[2]}; //Difference in all the values.
    //Run transition
    for (float i = 0; i <= 1.00;)
    {
        i += spf;
        //Set all Led to the right value
        for (int j = 0; j < 3; j++)
        {
            analogWrite(led[j], i * change[j] + (float)ledStates[j]);
        }
    }
    ledStates[0] = red;
    ledStates[1] = green;
    ledStates[2] = blue;
}

/**
 * Function for changing the RGB LED to a random value (either a solid color or a mix of colors)
 **/
void changeLed() {
    changeLed(false);
}
void changeLed(boolean slow)
{
    int ranLed[3];
    if ((int)(random(0, 4) + .5) == 0)
    {
        //25% of time.
        for (int i = 0; i < 3; i++)
        {
            ranLed[i] = (int)(random(ledOn, ledOff) + .5);
        }
    }
    else
    {
        //75% of time.
        while (true)
        {
            for (int i = 0; i < 3; i++)
            {
                ranLed[i] = (ledOff - ledOn) * (int)(random(0, 2)) + ledOn;
            }
            if (ranLed[0] != ledOff || ranLed[1] != ledOff || ranLed[2] != ledOff)
                break;
        }
    }

    fadeTo(ranLed[0], ranLed[1], ranLed[2], slow ? 3000 : 1000);
}

/**
 * Create a pattern of lights for some fun action
 **/
int timeDelays[] = {2, 3, 4, 8, 16, 32};
void randomPattern()
{
    int randWait = 1000 / timeDelays[(int)(random(0, 4) + .5)];
    int count = 7;
    int ranLed[3];
    while (count > 0)
    {
        //Configure led array
        if ((int)(random(0, 4) + .5) == 0)
        {
            //25% of time.
            for (int i = 0; i < 3; i++)
            {
                ranLed[i] = (int)(random(ledOn, ledOff) + .5);
            }
        }
        else
        {
            //75% of time.
            while (true)
            {
                for (int i = 0; i < 3; i++)
                {
                    ranLed[i] = (ledOff - ledOn) * (int)(random(0, 2)) + ledOn;
                }
                if (ranLed[0] != ledOff || ranLed[1] != ledOff || ranLed[2] != ledOff)
                    break;
            }
        }
        fadeTo(ranLed[0], ranLed[1], ranLed[2], 100);

        delay(randWait * .75);
        //run atleast 4 times then there exists a probability of exiting.
        if (count < (int)(random(0, 5) + .5))
            break;
        count--;
    }
}

void loop()
{
    randomPattern();

    int slowCycles = random(50, 100);
    for (int i = 0; i < slowCycles; i++)
    {
        changeLed();
        delay(5000);
    }
    
    delay(mainDelay);
}
