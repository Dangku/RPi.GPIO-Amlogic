
# RPi.GPIO-Amlogic  
  
RPi.GPIO port with support for Bananapi M5/M2Pro/M2S/CM4/CM5IO
  

## Based on:  
  
RPi.GPIO by Ben Croston  
  https://sourceforge.net/projects/raspberry-gpio-python/  

Bananapi M5/M2Pro/M2S/CM4/CM5IO WiringPi port  
  https://github.com/Dangku/amlogic-wiringPi  

awesometic's Odroid RPi.GPIO port  
  https://github.com/awesometic/RPi.GPIO-Odroid  
  
  
## License  
RPi.GPIO is distributed under MIT license, but wiringPi uses LGPL 3.  Since this project  
contains code from both projects, it is licensed under the slightly more restrictive LGPL v3.  
  
  
## Status  
Working for simple IO on Bananapi M5/M2Pro/M2S/CM4/CM5IO
PWM, events, analog read, ... not implemented  
  
  
## Building  
1) Install build-essential, python, python-dev, python-setuptools, git  
2) git clone https://github.com/Dangku/RPi.GPIO-Amlogic.git  
3) cd RPi.GPIO-Amlogic  
4) sudo python setup.py clean --all  
5) sudo python setup.py build install  

## Notes  
Apps require root (eg 'sudo python testapp.py')  
When using BCM mode, RPi BCM numbers are passed to GPIO.xxx and translated internally to Amlogic GPIO numbers  
Compare RPi connector pinout / BCM chart and Amlogic pinout to match RPi BCM with Amlogic pins  
