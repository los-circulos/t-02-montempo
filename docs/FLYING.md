# t-02 active digital field-programmable electric control line timer with OLED display and optional metrics

Note: this document describes pre-flicht programming and not saved configuration. For latter, check CONFIGURATION.md

Pre-flight programming lets setting the throttle (rpm, power, whatever) and flight time before each flight.

Also, the screen can be fixed to show a given info screen or leave it cycling between available modes.

## Pre-flight settings

After the logo screen and self-initialization, the pre-flight screen is shown.

### Pre-flight screen

... @todo

### Available pre-flight settings

 - throttle/rpm - based on the saved flight MODE, the applied throttle, the target rpm, or the target power
 - flight time - selected from predefined options
   - flight time "0:00" can be selected and means that the motor will spin until a cutoff threshold is met. Selecting
       "0:00" is only available if battery voltage sensing is active.

## Configuration with 8bit DIP switches

Adjusting pre-flight settings requires DIP switches and at least one push button.

A push button is not required, but then countdown will start automatically (if possible)

### Setting throttle/rpm/power

Use the first 5 (9bit) or first 4 (8bit) DIP switches to change the value. The actual value will be shown on the screen.

Values:
 - for constant throttle and SMART throttle modes: 53...98% in 3% steps
   - for SMART mode this is the initial throttle. End target throttle remains the same constant set in configuration
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
