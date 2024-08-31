## Micropython Perlin  
----
#### A module for micropython which adds the ability to generate Perlin noise  

Developed against micropython [v1.24.0-preview.218.gb704ff66c.dirty](https://github.com/micropython/micropython/commit/b704ff66c3e034c36e548eb0b9874871b5f3b5d0) with a WEMOS S2 Mini (esp32s2) and ESP-IDF [v5.2.2](https://github.com/espressif/esp-idf/releases/tag/v5.2.2) with Python [3.12.5](https://www.python.org/downloads/release/python-3125/).  

#### How to:  
###### Compile (firmware version):  
Firstly, you need the micropython repo.  
Then, clone this repo into `micropython/ports/{PORT}/boards/{BOARD}/modules`, I'm using a Lolin S2 Mini, so I'll end up with `/home/sjaak/.../micropython/ports/esp32/boards/LOLIN_S2_MINI/modules/micropython_perlin/`.  
Compile the firmware:  
`cd micropython/ports/{port}` and `make USER_C_MODULES=./boards/LOLIN_S2_MINI/modules/micropython_perlin/firmware/perlin.cmake BOARD={BOARD} all`  
Or you can add it to `mpconfigboard.cmake` with `set(USER_C_MODULES ${MICROPY_BOARD_DIR}/modules/micropython_perlin/firmware/perlin.cmake)`  
Flash your new firmware, and you should have a functioning perlin noise generator installed!  

###### Usage:
```py
import perlin

#perlin.reseed(int seed)
perlin.reseed(31367)

#perlin.octave_perlin(float x, float y, float z, [int octaves, [float persistence]])
perlin.octave_perlin(0.5, 0.5, 0.5, 6)  # 0.531746```  

----

#### Credits  
Based off Flafla2's [Perlin.cs](https://gist.github.com/Flafla2/1a0b9ebef678bbce3215)  