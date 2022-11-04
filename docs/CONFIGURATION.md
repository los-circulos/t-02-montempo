# t-02 active digital field-programmable electric control line timer with OLED display and optional metrics

Note: this document describes saved configuration and not pre-flicht programming. For latter, check FLYING.md

Saved configuration includes items which rarely change, eg. setting voltage cut-off, number of motor poles etc.
The configuration utility allows setting precise (digital) values for each setting and save them. Settings are
preserved, no battery needed.

The configuration utility also contains a test spin mode. While test spinning, available metrics data is displayed.
The throttle can be adjusted even while the engine is spinning so finding the correct throttle setting is easy.
This tool can also be used to verify the metrics data, especially RPMs which are based on saved motor pole count.

Pre-flight programming, on the other hand, lets fine tune throttle, RPM or power and also fly time before each flight.

# Configuration with 8bit DIP switches

Altough using the 8bit DIP switches is optional, currently this is the only way to configure saved settings.

Also, at least one pushbutton is needed to save settings.

## Entering the configuration utility

 - Set all DIP switches to low/up/on/ground (*) before connecting battery.
 - Release any push buttons
 - Connect battery
 - "MOTOR 0%" and "PUSH TO SPIN" should be displayed. Test spin (motor) mode is the default configuration screen.

## Changing a setting

### Select a setting

The setting is selected with the rightmost DIP switches (pins 0, 1, etc). Some modes require setting just 2 DIP
switches, while others require setting the 3 or 4 rightmost switches. Eg. "UP UP" means setting the 2 rightmost (last)
DIP switches to low/up/on/ground (*).

When a mode is selected, the currently saved value is displayed as "<OLD" with medium size fonts. The actual value is
displayed as "^NEW" and with large fonts.

### Change the value

The DIP switches not used for selecting a setting are used to set its value. Eg. if "UP UP" was used, which 
selects test spin (motor) mode, then the first 6 DIP switches are available to set the desired throttle.

### Save new value

Saving a value is done by holding both push buttons (or one, if the other is disabled). While holding both buttons,
the LED will flash, an exclamation mark "!" is shown on the screen, as well as running dots in the bottom. A big
"SAVED" title appears once the save is done, and the timer will wait until all buttons are released before continuing.
If any of the buttons is released while the dots are still running, the setting is not saved.

## Available settings and what they do

### Setting format explained

(insert image here explaining . - U - D)

### List of available settings

 - "MOTOR" - it is not a setting but a motor test spin can be activated by setting a throttle output and pushing
    both buttons (like when saving).
   - . . . . . . U U
   - value: throttle output, in percentage %, 2% steps 0-98%
   - push both buttons to start spinning (there will be a dot running and releasing any button will cancel)
   - one of the 2 push buttons can be released while spinning
   - release both buttons to stop spinning
   - selecting another setting will also stop spinning
 - "SMART" - smart throttle end value (see FLYING.md about SMART mode and how to set a good value)
   - . . . . . . D D
   - value: throttle value at the end of a "SMART" mode fly, in percentage %, 2% steps 0-98%
   - code default: 90%
 - "T1CUT" - temperature sensor #1 cutoff value
   - . . . . . D D U
   - value: temperature, in °C, 2°C steps 28...90°C
   - code default: 90
 - "T2CUT" - temperature sensor #2 cutoff value
   - . . . . . U D U
   - value: temperature, in °C, 2°C steps 28...90°C
   - code default: 90
 - "MODE" - select hold (govern) mode, 4 modes to select
   - . . - - U U U D
   - value: the selected mode. See description of each mode in "FLYING.md"
   - code default: constant throttle
 - "POLES" - set motor poles for correct RPM sensing
   - . . . . D U U D
   - value: number of motor poles (pairs*2), 2 poles steps, 4...34P
   - code default: 4 (a bad default value for outrunners but will cause the least problems as default value)
 - "V CUT" - voltage cut off
   - . . . . D D U D
   - value: value when motor will be cut, in volts V, 0.1V steps
   - default: 3.7V
 - "A CUT" - current cut off
   - . . . . U D U D
   - value: amps when motor will be cut, in amps A, 2A steps, 20...50A
   - default: 30A
 - "LOGS" reserved for viewing saved logs (not yet implemented)
   - D D D D D D D D
   - use push button A and B to switch between logs


* - low/up/on/ground is explained in the 8bit DIP switch description in the scratch build doc.

@los.circulos
github.com/los-circulos
#controlline #vuelocircular #uコン #opensource
