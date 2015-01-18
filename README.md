= Copter Lights

This Arduino sketch implements automatic, motion-sensitive running lights for a hex copter.

== Overview

I built a DJI F550 and wanted some cool running lights for it. I found NeoPixels at Adafruit Industries. 

I built the copter using the eight-channel Spektrum AR8000 that came with the transmitter. Because the DJI Naza-M v2 uses only seven channels, I decided to use the spare channel to control the running lights. While shopping for parts I found the Analog Devices ADXL335 accelerometer and though it would be cool to have the lights change based on how the copter was moving.

I ended up building an Arduino-powered setup with the following features:

* Running lights can be set to off, on, or auto using a transmitter switch
** When the switch is in the "0" (off) position, the LEDs will be dark.
** When the switch is in the "1" (on) position, the LEDs strobe.
** When the switch is in the "2" (auto) position, the LEDs play the apppropriate sequence based on the output of the accelerometer.

== Hardware Components

This sketch runs on a Teensy 3.1.

http://www.pjrc.com/teensy/
https://www.adafruit.com/products/1625

It employs an Analog Devices ADXL335 accelerometer.

http://www.analog.com/en/sensors/inertial-sensors/adxl335/products/product.html
https://www.adafruit.com/products/163

The LEDs are Flora RGB Smart NeoPixels from Adafruit Industries.

https://www.adafruit.com/products/1260

== Hardware Configuration

The hardware consists of the Teensy and the ADXL225 connected together as implied by the code. Additionally, the Vo of the ADXL335 is connected to AREF of the Teensy.

As is also implied by the code, an output channel of the RC receiver is connected to the CONTROL_PIN.

== Setup
# Construct the device according to the schematic.
# Find the calibration values using the method described at https://learn.adafruit.com/adafruit-analog-accelerometer-breakouts and enter them in the appropriate place in the code.
# Configure your transmitter to assign a three-position switch to the output channel connected to CONTROL_PIN.


