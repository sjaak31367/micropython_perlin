// Include the header file to get access to the MicroPython API
#include "py/dynruntime.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

static const int permutation[] = {
    151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
    190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,88,237,149,56,87,174,20,
    125,136,171,168,68,175,74,165,71,134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,
    105,92,41,55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208,89,18,169,200,196,
    135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,250,124,123,5,202,38,147,118,126,255,
    82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,
    153,101,155,167,43,172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,228,
    251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,107,49,192,214,31,181,199,
    106,157,184,84,204,176,115,121,50,45,127,4,150,254,138,236,205,93,222,114,67,29,24,72,243,141,128,
    195,78,66,215,61,156,180
};

static int p[512];

void reseed(int seed) {
    srand(seed);
    for (int i = 0; i < 256; i++) {
        int swap = rand() % 256;
        int temp = ((int*)permutation)[i];
        ((int*)permutation)[i] = ((int*)permutation)[swap];
        ((int*)permutation)[swap] = temp;
    }
    for (int i = 0; i < 512; i++) {
        p[i] = permutation[i % 256];
    }
}

static inline float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

static inline float lerp(float a, float b, float x) {
    return a + x * (b - a);
}

static inline float grad(int hash, float x, float y, float z) {
    int h = hash & 15;
    float u = h < 8 ? x : y;
    float v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float perlin(float x, float y, float z) {
    int xi = (int)x & 255;
    int yi = (int)y & 255;
    int zi = (int)z & 255;

    float xf = x - (int)x;
    float yf = y - (int)y;
    float zf = z - (int)z;

    float u = fade(xf);
    float v = fade(yf);
    float w = fade(zf);

    int a  = p[xi] + yi;
    int aa = p[a] + zi;
    int ab = p[a + 1] + zi;
    int b  = p[xi + 1] + yi;
    int ba = p[b] + zi;
    int bb = p[b + 1] + zi;

    float x1 = lerp(grad(p[aa], xf, yf, zf), grad(p[ba], xf - 1, yf, zf), u);
    float x2 = lerp(grad(p[ab], xf, yf - 1, zf), grad(p[bb], xf - 1, yf - 1, zf), u);
    float y1 = lerp(x1, x2, v);

    x1 = lerp(grad(p[aa + 1], xf, yf, zf - 1), grad(p[ba + 1], xf - 1, yf, zf - 1), u);
    x2 = lerp(grad(p[ab + 1], xf, yf - 1, zf - 1), grad(p[bb + 1], xf - 1, yf - 1, zf - 1), u);
    float y2 = lerp(x1, x2, v);

    return (lerp(y1, y2, w) + 1) / 2;
}

float octave_perlin(float x, float y, float z, int octaves, float persistence) {
    float total = 0;
    float frequency = 1;
    float amplitude = 1;
    float max_value = 0;
    for (int i = 0; i < octaves; i++) {
        total += perlin(x * frequency, y * frequency, z * frequency) * amplitude;
        max_value += amplitude;
        amplitude *= persistence;
        frequency *= 2;
    }
    return total / max_value;
}

#include "py/obj.h"
#include "py/runtime.h"

// Wrapper for perlin function
static mp_obj_t mod_perlin_perlin(mp_obj_t x_obj, mp_obj_t y_obj, mp_obj_t z_obj) {
    float x = mp_obj_get_float(x_obj);
    float y = mp_obj_get_float(y_obj);
    float z = mp_obj_get_float(z_obj);
    return mp_obj_new_float(perlin(x, y, z));
}
static MP_DEFINE_CONST_FUN_OBJ_3(mod_perlin_perlin_obj, mod_perlin_perlin);

// Add additional bindings similarly for other functions

static const mp_rom_map_elem_t perlin_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR_perlin), MP_ROM_PTR(&mod_perlin_perlin_obj) },
    // Other function bindings go here
};

static MP_DEFINE_CONST_DICT(perlin_module_globals, perlin_module_globals_table);

const mp_obj_module_t perlin_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&perlin_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_perlin, perlin_user_cmodule, MODULE_PERLIN_ENABLED);

/*// Define a Python reference to the function above
static MP_DEFINE_CONST_FUN_OBJ_VAR(py_noise2_obj, 4, py_noise2);

// This is the entry point and is called when the module is imported
mp_obj_t mpy_init(mp_obj_fun_bc_t *self, size_t n_args, size_t n_kw, mp_obj_t *args) {
    // This must be first, it sets up the globals dict and other things
    MP_DYNRUNTIME_INIT_ENTRY

    // Make the function available in the module's namespace
    mp_store_global(MP_QSTR_noise2, MP_OBJ_FROM_PTR(&py_noise2_obj));

    // This must be last, it restores the globals dict
    MP_DYNRUNTIME_INIT_EXIT
}*/
