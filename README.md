## Micropython Perlin  
----
#### A module for micropython which adds the ability to generate Perlin noise  

Developed against micropython [v1.24.0-preview.218.gb704ff66c.dirty](https://github.com/micropython/micropython/commit/b704ff66c3e034c36e548eb0b9874871b5f3b5d0) with a WEMOS S2 Mini (esp32s2) and ESP-IDF [v5.2.2](https://github.com/espressif/esp-idf/releases/tag/v5.2.2) with Python [3.8.10](https://www.python.org/downloads/release/python-3810/).  

#### How to:  
###### Compile (firmware version):  
1. You need the micropython repo.  
2. Clone this repo into `micropython/ports/{port}/boards/{board}/modules`  
I'm using a Lolin S2 Mini, so I'll end up with `/home/sjaak/.../micropython/ports/esp32/boards/LOLIN_S2_MINI/modules/micropython_perlin/`.  
3. (A) Compile the firmware:  
`cd` to `micropython/ports/{port}`  
`make USER_C_MODULES=./boards/{board}/modules/micropython_perlin/firmware/perlin.cmake BOARD={board} all`  
(B) Or you can add it to `micropython/ports/{port}/boards/{board}/mpconfigboard.cmake` with `set(USER_C_MODULES ${MICROPY_BOARD_DIR}/modules/micropython_perlin/firmware/perlin.cmake)`  
4. Flash your new firmware, and you should have a functioning perlin noise generator installed!  

###### Usage:
```py
import perlin

#perlin.reseed(int seed)
perlin.reseed(31367)

#perlin.octave_perlin(float x, float y, float z, [int octaves, [float persistence]])
perlin.octave_perlin(0.5, 0.5, 0.5, 6)  # 0.531746
```  

----

#### Credits  
Firmware version is based off Adrian Biagioli's [Perlin.cs](https://gist.github.com/Flafla2/1a0b9ebef678bbce3215).  
Early Natmod version was based off Casey Duncan's [Noise](https://github.com/caseman/noise) (MIT License).  
