# t-02 semi-active digital field-programmable electric control line timer with OLED display and optional metrics

## What it does

 - safe startup sequence
 - easily change throttle and flight time before flight
 - start countdown with a long button press
 - 25sec countdown before motor starts
 - configurable soft start
 - flight on fixed or compensated throttle level
 - status led flashes tell what's happening all the time, fast flashes notice before motor start and before motor stop

### What it does not

 - it does not actively compensate for maneuvers or other flight speed changes
 - RPM, temperature and current sensing and related functions are not fully implemented currently (thus power related 
   neither as it would depend on current)

## General documentation,

Program and schematics to build a timer based on Arduino Nano

 - semi-active: SMART throttle, voltage compensation ("govi"), constant RPM ("governor")
 - digital: settings are precise and repeatable
 - field programmable: easily change configuration before each flight with a simple DIP switch
   - 8 or 9 bit DIP switch for programming (8bit gives less value resolution in setups)
 - OLED display: shows time, configuration, metrics information. 
   - flight time (countdown) is displayed during flight, along with a progress bar
   - after flight summary screen - just a photo as flight log
   - easy to write "plugins" for different screens 
   - OLED display is optional. Might save a few grams, but no metrics info, and configuration is difficult.
 - metrics: base board has connectors for some sensors
   - voltage: requires a simple voltage divider. Required for voltage cut-off and constant power.
   - RPM: requires a simple voltage divider and calibration. Required for constant RPM.
   - current (NOT IMPLEMENTED): requires current sensing board. Required for discharge metering and constant power.
   - temperature (NOT IMPLEMENTED): up to 3 points of measurement with simple sensors. Required for temperature cut-off. Configurable 28-90C
   - without any sensors, SMART and constant throttle still work
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
