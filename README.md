# t-02 active digital field-programmable electric control line timer with OLED display and optional metrics

## General documentation, 

Program and schematics to build a timer based on Arduino Nano

 - active: SMART throttle, constant RPM ("governor"), constant power (flat throttle is available)
 - digital: settings are precise and repeatable
 - field programmable: easily change configuration before each flight with a simple DIP switch
   - DIP switch is optional, but then timer will work only in default mode: 98% constant throttle, 3:00 flight time, 
       And cutoffs, is sensors connected: 90C, ...V, ...A
 - OLED display: shows time, configuration, metrics information. 
   - Depending on display size and line length, flight time might be seen during flight
   - various info screens to chose from or to rotate them
   - very easy way to record discharge and voltage drop during flight
   - easy to write "plugins" for different screens 
   - OLED display is optional. Saves a few grams, but no metrics info, and configuration is difficult.
 - metrics: simple sensor boards ad voltage dividers can provide live data
   - voltage: requires a simple voltage divider. Required for voltage cut-off and constant power.
   - RPM: requires a simple voltage divider and calibration. Required for constant RPM.
   - current: requires current sensing board. Required for discharge metering and constant power.
   - temperature: up to 3 points of measurement with simple sensors. Required for temperature cut-off. Configurable 28-90C
   - without any sensors, SMART and constant throttle still work
   - NOTE some of these features are "coming up"
 - open source: you can build your own and compile the code yourself. Make any changes :)
 - support for up to 2 push buttons
   - used to initialize flight and for configuration
   - both work as kill switch during countdown and flight
   - both can be disabled with an extra resistor (if no buttons are present, flight is initiated automatically, but some functions might be disabled, eg. rpm calibration)
 - test mode: adjust throttle and see realtime metrics info on screen

## Files provided:

This is a CLion/platformio project. (I'll try to provide easy to use .ino files with functional releases)

How to set up the compiler and builder for your system is something I cannot help much with (that's why the .ino those
should be ready to load into Arduino IDE, compile, and burn)

 - docs - documentation
 - src - project files to be compiled
 - test - sadly empty for now
 - CMakeLists.txt - needed for compilation
 - CMakeListsPrivate.txt - same
 - LICENSE - read it
 - platformio.ini - needed for compilation
 - README.md - you're reading it

@los.circulos
github.com/los-circulos
#controlline #vuelocircular #uコン #opensource
