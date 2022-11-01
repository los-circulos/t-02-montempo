# t-02 active digital field-programmable electric control line timer with OLED display and optional "telemetry"

## Hardware Build from Scratch -  

## Files provided:
...

## Bill of materials:
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

