=Copter Lights

This Arduino sketch implements running lights for a hex copter.

This sketch runs on a Teensy 3.1

It employs an Analog Devices ADXL335 accelerometer.

The hardware consists of the Teensy and the ADXL225 connected together as implied by the code. As is also implied by the code, an output channel of the RC receiver is connected to pin 2.

Configure your transmitter to assign a three-position switch to the output channel connected to pin 2.
* When the switch is in the "0" (off) position, the LEDs will be dark.
* When the switch is in the "1" (on) position, the LEDs strobe.
* When the switch is in the "2" (auto) position, the LEDs play the apppropriate sequence based on the output of the accelerometer.
