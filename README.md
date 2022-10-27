# t-02 active digital field-programmable electric control line timer with OLED display and optional telemetry

Program and schematics to build a timer based on Arduino Nano

 - active: SMART throttle, constant RPM ("governor"), constant power (flat throttle is available)
 - digital: settings are precise and repeatable
 - field programmable: easily change configuration before each flight with a simple DIP switch
 - OLED display: shows time, configuration, telemetry information. 
   - Depending on display size and line length, flight time might be seen during flight
   - various info screens to chose from or to rotate them
   - very easy way to record discharge and voltage drop during flight
   - easy to write "plugins" for different screens 
   - can be omitted and keep functionality (saves a few grams, but no telemetry info)
 - telemetry: simple sensor boards ad voltage dividers can provide live data
   - voltage: requires a simple voltage divider. Required for voltage cut-off and constant power.
   - RPM: requires a simple voltage divider and calibration. Required for constant RPM.
   - current: requires current sensing board. Required for discharge metering and constant power.
   - temperature: up to 3 points of measurement with simple sensors. Required for temperature cut-off.
   - without any sensors, SMART and constant throttle still work
   - NOTE some of these features are "coming up"
 - open source: you can build your own :)
 - support for up to 2 push buttons
   - used to initialize flight
   - both work as kill switch during countdown and flight
   - both can be disabled with an extra resistor (if no buttons are present, flight is initiated automatically, but some functions might be disabled, eg. rpm calibration)
 - test mode: adjust throttle and see realtime telemetry info on screen

Components needed:
...

## Files provided:



@los.circulos
github.com/los-circulos
#controlline #vuelocircular #uコン #opensource

