import RPi.GPIO as GPIO  
import time  
  
bcmledpin = 27    # pin13, bcm27
phyledpin = 13    # pin13, bcm27

def setup_bcm():
  GPIO.setmode(GPIO.BCM)
  GPIO.setup(bcmledpin, GPIO.OUT)
  
def setup_board():
  GPIO.setmode(GPIO.BOARD)
  GPIO.setup(phyledpin, GPIO.OUT)

def blink_bcm():  
  while True:  
    GPIO.output(bcmledpin, GPIO.HIGH)
    print("********set bcm pin ", bcmledpin, " High********")
    time.sleep(1)  
    GPIO.output(bcmledpin, GPIO.LOW)
    print("********set bcm pin ", bcmledpin, " Low********")
    time.sleep(1)

def blink_board():
  while True:
    GPIO.output(phyledpin, GPIO.HIGH)
    print("********set phy pin ", phyledpin, " High********")
    time.sleep(1)  
    GPIO.output(phyledpin, GPIO.LOW)
    print("********set phy pin ", phyledpin, " Low********")
    time.sleep(1)

def shutdown():
    GPIO.cleanup()
  
if __name__ == '__main__':     # Program start  
  print('To read output correctly, jumper pin 13 (bcm27) to pin 31 (bcm6)')
  print('Press Ctrl-C to exit') 
  setup_bcm()
  #setup_board()
  print("Hardware information: ", GPIO.RPI_INFO)
  try:  
    blink_bcm()
    #blink_board()
  except KeyboardInterrupt:
    shutdown()
