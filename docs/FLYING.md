# t-02 active digital field-programmable electric control line timer with OLED display and optional "telemetry"

Note: this document describes pre-flicht programming and not saved configuration. For latter, check CONFIGURATION.md

Pre-flight programming lets setting the throttle (rpm, power, whatever) and flight time before each flight.

Also, the screen can be fixed to show a given info screen or leave it cycling between available modes.

## Pre-flight settings

After the logo screen and self-initialization, the pre-flight screen is shown.

### Pre-flight screen

... @todo

### Available pre-flight settings

 - throttle/rpm/power - based on the saved flight MODE, the applied throttle, the target rpm, or the target power
 - flight time - selected from predefined options
   - flight time "0:00" can be selected and means that the motor will spin until a cutoff threshold is met. Selecting
       "0:00" is only available if battery voltage sensing is active.
 - screen cycle - sets whether info screens are cycled during flight, or fixed for a given screen
   - to select a fix screen, activate cycling and start the countdown. Screens will cycle. When the desired screen
       is shown, deactivate cycling. Countdown will restart. The selected screen will be remember for later flights
       until another screen is selected (upcoming, to do)

## Configuration with 8bit DIP switches

Altough using the 8bit DIP switches is optional, currently this is the only way to set pre-flight settings.

A push button is not required, but then countdown will start automatically (if possible, eg. no cutoff is active)

### Setting throttle/rpm/power

Use the first 4 DIP switches to change the value. The actual value will be reflected on the screen.

Values:
 - for throttle, constant throttle and SMART modes: 68...98% in 2% steps, 98% default
   - for SMART mode this is the initial throttle. End target throttle remains the same constant
   - default: 98% throttle (all DIPs "down")
 - for RPM: 8000...12500, 300 1/min steps
   - default: 8000 (all DIPs "down")
 - for power: 120...500W with 20W increments between 120...300W, and 30W increments between 300...480W 
   - default: 120 (all DIPs "down")

### Setting fly time

Use DIP switches 5, 6, 7 to select fly time from a set of predefined values.

Values:
 - 0:00 - run until cutoff
   - U U U
 - 1:00 - a general test run to try settings etc. default
   - D D D
 - 3:00, 4:00, 5:00 - general "training" times
 - 5:40, 5:55, 5:70 - allows for a complete F2B routine

## Setting screen cycling, selecting scree

...


* - low/up/on/ground is explained in the 8bit DIP switch description in the scratch build doc.

@los.circulos
github.com/los-circulos
#controlline #vuelocircular #uコン #opensource
