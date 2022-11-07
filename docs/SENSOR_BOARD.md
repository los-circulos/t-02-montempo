# t-02 active digital field-programmable electric control line timer with OLED display and optional metrics

## Sensor board documentation -  

Sensor board is to be mounted on the ESC and has input for battery voltage, motor signal (RPM), temperature sensing of
   ESC with optional 2nd temperature sensor, and external current sensor board. The current sensor board is optional, 
   and the timer will work without the sensor board itself, just won't measure anything :)

## Metrics

### Battery voltage

Connect to battery + lead. Can connect into current sensor's battery bypass, practically either side

The voltage sensing must be calibrated with the P1 potenciometer manually.

### Current sensing 

Current sensing is done by "Arduino current sensor board" based on ACS712 or similar. Only the 30A version (or any
   compatible with 0.066V/A coefficient) is supported.

Cut the ESC's RED battery lead and run it through the sensor module terminal. Connect the sensor module pins to the
   sensor board's cable or pins.


## Files provided:
...

## Bill of materials:
### sensor board:
### recommended current sensing
 - ACS712 (or compatible) 30A current sensor module, with 0.066V/A coefficient


@los.circulos
github.com/los-circulos
#controlline #vuelocircular #uコン #opensource

