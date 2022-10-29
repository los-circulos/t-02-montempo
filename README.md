# t-02 active digital field-programmable electric control line timer with OLED display and optional telemetry

Program and schematics to build a timer based on Arduino Nano

 - active: SMART throttle, constant RPM ("governor"), constant power (flat throttle is available)
 - digital: settings are precise and repeatable
 - field programmable: easily change configuration before each flight with a simple DIP switch
   - DIP switch is optional, but then timer will work only in default mode: 98% constant throttle, 3:00 flight time, 
       And cutoffs, is sensors connected: 90C, ...V, ...A
 - OLED display: shows time, configuration, telemetry information. 
   - Depending on display size and line length, flight time might be seen during flight
   - various info screens to chose from or to rotate them
   - very easy way to record discharge and voltage drop during flight
   - easy to write "plugins" for different screens 
   - OLED display is optional. Saves a few grams, but no telemetry info, and configuration is difficult.
 - telemetry: simple sensor boards ad voltage dividers can provide live data
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
 - test mode: adjust throttle and see realtime telemetry info on screen

## Files provided:


## Bill of material:
### bare minimum:
 - U0 Arduino nano 5V - or any pin compatible similar board. Depending on the ESC used, a 3V board might work as well
 - J1 3pin servo header (or reuse ISP header)
 - R1, R2 100k pullup resistor
### recommended for basic functionality
 - SW1 8bit DIP switch (typically the white switches-blue housing) RECOMMENDED
 - SW2, SW3 - momentary push switches / microswitches. Push buttons with integrated LEDs are recommended
 - D1, D2 - LEDs (optional)
 - U1 display (currently only one type supported)
   - 128*32 OLED with 1306 driver and I2C communication
 - J2 2*12 header (mostly not used but the plastic strip holds the used pins well)
### recommended commodity
 - SW2, SW3 - use push buttons with integrated LEDs (replaces D1, D2)
 - PCB - use optional PCBs (schematics etc included as available)
### required for voltage sensing
 - R3 xk resistor
 - R4 xk resistor
### current sensing
 - U2 20A or 30A current sensing module
 - ...
### RPM sensing
 - ...

### optional, not recommended
 - R3, R4 100k pulldown resistor to replace SW2, SW3 respectively (none, one, or both)
 - J3 2*8 header with jumpers - to replace SW1 DIP switch. Not recommended.


@los.circulos
github.com/los-circulos
#controlline #vuelocircular #uコン #opensource

