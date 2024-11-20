# Importing libraries
import lvgl as lv
import lv_utils
from ili9XXX import ili9341
from gt911 import GT911
import gt911_constants as gt
from time import sleep
from machine import UART

# Event loop for catching events
event_loop=lv_utils.event_loop()

# Display orientation (landscape)
rot=0xF0

# Initialization of LVGL library
lv.init()

# Initialization of TFT display, SPI pins used by the Cheap Yellow Display specified
disp = ili9341(width=480, height=320, mhz=20, miso=12, mosi=13, clk=14, dc=2, cs=15, power=-1, backlight=27, backlight_on=1, hybrid=False, factor=8, rot=rot)
disp.init()

# Initialization of capacitive touch screen
tp = GT911(sda=33, scl=32, interrupt=21,reset=0)
tp.width=480
tp.height=320
tp.begin(93)

# Example of screen definition
scr = lv.obj() # Variable scr contains screen object

# Example of button definition
btn = lv.button(scr) # Variable btn contains button, which is on screen scr
btn.set_size(110,110) # Setting button size (width, height)
btn.set_pos(330, 60) # Setting button position (from_left, from_top)   
btn.add_event_cb(btn_press, lv.EVENT.CLICKED, None) # Connecting btn_press function to on_click event

# Example of button which is executed on click
def btn_press(t):
    # Do whatever
    return

#Example of text label on button - text displayed on button
label1 = lv.label(btn) # Variable label contains text label, which is on screen scr
label1.set_text("Any text") # Setting text for text label
label1.align(lv.ALIGN.CENTER,0,0) # Aligning text on center of buton
    
# Example of text label on screen scr
label2 = lv.label(scr) # Variable label contains text label, which is on screen scr
label2.set_text("Any text") # Setting text for text label
label2.set_pos(30,15) # Setting text position (from_left, from_top)

# Example of list on screen scr
lst=lv.list(scr) # Variable lst contains list
lst.set_width(270) # Setting width of list lst
lst.set_height(260) # Setting height of list lst
lst.set_pos(30,50) # Setting position of list lst (from_left, from_top)

# Example of LED on screen scr
led = lv.led(scr) # Variable led contains LED
led.set_brightness(100) # Setting brightness of LED
led.set_pos(370,255) # Setting position of LED (from_left, from_top)
led.set_size(30,30) # Setting size of LED
led.off() # Turning LED off
led.on() # Turning LED on

# Loading the screen scr
lv.screen_load(scr) # Screen scr will be displayed after execution

# Infinite loop - to keep application running
while True:
    sleep(0.1)
    pass



