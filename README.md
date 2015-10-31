# automatedNightLight
An arduino project for an elegant simple night light.I thought that this would help me out in my busy college life and I think it has brought much warmth to my dorm room and has detered any monsters from under my bed.
##What it's supposed to do. 
The amazing automated night light is supposed to be a night light that doesn't get noticed. Once you flip off the lights the inteligent night light will fade on to full brightness. This gives you the required lumens to find your bed without loosing any important organs, limbs, or self esteem. Once your in bed you'll probably need to plug in your phone, check the time and ponder the pros and cons of getting up and using the restroom. After a while (about 5 minutes by default) the night light will dim to about 60% brighness and will go off by default after a total of 30 minutes at which point any thought of going to the restroom should be replaced with the knowlege of how exposed your toes (and corners) are. 
##Setup
- **1.**click on the .ino file
- **2.**Click raw and then file->save to get the file.
- **3.**Open the downloaded file with the Arduino IDE
- **4.**The IDE will ask if you want to make a folder around the file click 'yes'.
- **5.**Change the led pins or any other configuration to your liking.
- **6.**Upload to your arduino.

##Configuration
- `LedPins[3]`  is an array of the pins used for lighting up leds. (currenlty only supports up to three #needAForLoop).
- `photocellPin` is the pin number for the photocell. Wired like [this](https://learn.adafruit.com/system/assets/assets/000/000/459/medium800/light_cdspulldowndiag.gif?1396763222).
- `timeDim` is the time (in seconds) it takes for the led to dim to desired dim brightness. 
- `timeOn` the amount of time (overall) that the night light will remain on after darkness fell upon the land. 
- `ledMaxBright` the `anologeWrite()` output for the LED (255 is full blast).
- `ledDim` the `anologeWrite()` input for Dim stage (the stage after the light has been on for timeDim seconds).
- `ledSwitch` the `anologeRead()` value that is used as the threshold between night and day.
- `ledBuffer` is the amount of light required to turn the night light back off after it has been turned on. (this is used to fix an issue where the night light would switch on and off due to fluxuating light values at dusk). 

##What to do if this repo gives you the hebejebes.
1. Let me know what it is you don't like, think could use improvement, or have a seriouse problem with.
2. Find a hammer.
3. Hit something with it.(It's bound to at least solve one problem). 
Enjoy
