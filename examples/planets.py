from math import sqrt
import perlin, gc
from random import randint
from time import ticks_ms, sleep_ms

class planet:
  def __init__(self, radius, seed):
    self.r = radius
    perlin.reseed(seed)
    temp = []
    for i in range(radius * 2 + 2):  # Make an array of (radius*2 +2) squared
      temp.append([0] * (radius * 2 + 2))
    self.cache = temp                # Set as Cache
  def generate(self, octaves):
    x,y,z = 0, 0, 0.0
    r = self.r
    for pixel_y in range(r*2):
      for pixel_x in range(r*2):
        x = pixel_x - r
        y = pixel_y - r
        if ((x * x + y * y) <= r * r):     # Honestly I wrote this code almost 4 years ago. I understand it but couldn't explain it to ya.
          z = sqrt(r * r - x * x - y * y)  # Best I can do is say "check for every pixel if it's within r of the centre of the planet. And if so, calculate how far out of the screen (up) you have to go for it to be exactly r."
          self.cache[pixel_y][pixel_x] = perlin.octave_perlin((x + r) / 20, (y + r) / 20, z / 10, octaves)
    gc.collect()                           # Then carve this semi-sphere out of perlin noise, and store that in cache
  def rotate(self, deg):
    pass  # Would like this, but would require regenerating, and also math.
  def render(self, lcd, waterlevel):
    for y in range(len(self.cache)):
      for x in range(len(self.cache[0])):  # Center planet on screen. if heightvalue (perlin) < waterlevel, render black, else, white.
        lcd.pixel(x+(lcd.width//2-self.r), y+(lcd.height//2-self.r), 0 if self.cache[y][x] < waterlevel else 1)
    lcd.show()

# show(screen_object, range(start_seed, stop_seed), number_of_perlin_octaves, (size_min, size_max), display_time_in_ms)
#   e.g. show(ssd1306.SSD1306_I2C(128,64,I2C(1,scl=Pin(2),sda=Pin(15))), range(64), 6, (15,30), 1000)
def show(lcd, iterable, octaves, randrange, displaytime):
  prevtime=0
  for seed in iterable:
    start = ticks_ms()
    r = randint(randrange[0],randrange[1])  # Get radius
    print("planet")        # Prints required to stop WDT from dying?
    p = planet(r, seed)    # Start planet, planet-cache, and perlin
    print("generate")
    p.generate(octaves)    # Generate terrain
    lcd.fill(0)            # Remove previous planet image
    stop = ticks_ms()
    sleep_ms(displaytime)  # Allow the previous planet some time to be displayed.
    print("render")
    p.render(lcd, 0.5)     # Show planet
    lcd.text(f"s={seed}",0,0,1)
    lcd.text(f"r={r}",128-(4*8),0,1)
    lcd.text(f"{stop-start}ms",0,56,1)
    lcd.show()             # Show planet + stats
    del p
    gc.collect()
    prevtime=(stop-start)
    ##  Remnant of code to render a planet with shifting waterlevel, controlled with a knob.
    #a = Pin(14, Pin.OUT, value=0)
    #b = ADC(Pin(26, Pin.IN), atten=ADC.ATTN_11DB)
    #c = Pin(33, Pin.OUT, value=1)
    #while True:
    #  p.render(lcd, 1 / 4096 * b.read())
    #  gc.collect()

def demo():
  import ssd1306
  from machine import Pin, I2C
  lcd = ssd1306.SSD1306_I2C(128,64,I2C(1,scl=Pin(2),sda=Pin(15)))
  show(lcd, range(64), 6, (15,28), 1000)
