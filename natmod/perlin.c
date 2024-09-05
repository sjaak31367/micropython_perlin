// Include the header file to get access to the MicroPython API
//#include "py/dynruntime.h"
#include "py/obj.h"
#include "py/dynruntime.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

static int p[512];

void reseed(int seed) {
    srand(seed);
    for (int i = 0; i < 256; ++i){
        p[i] = i;
    }
    for (int i = 0; i < 256; ++i) {
        int swap = rand() % 256;
        int temp = p[swap];
        p[swap]  = p[i];
        p[i]     = temp;
    }
    for (int i = 0; i < 256; ++i) {
        p[i+256] = p[i];
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

// Wrapper for perlin functions
static mp_obj_t mod_perlin_reseed(mp_obj_t seed_obj) {
    reseed(mp_obj_get_int(seed_obj));
    return mp_const_none;
}
static mp_obj_t mod_perlin_perlin(mp_obj_t x_obj, mp_obj_t y_obj, mp_obj_t z_obj) {
    float x = mp_obj_get_float(x_obj);
    float y = mp_obj_get_float(y_obj);
    float z = mp_obj_get_float(z_obj);
    return mp_obj_new_float(perlin(x, y, z));
}
static mp_obj_t mod_perlin_octave_perlin(size_t n_args, const mp_obj_t* args) {
    float x = mp_obj_get_float(args[0]);
    float y = mp_obj_get_float(args[1]);
    float z = mp_obj_get_float(args[2]);
    int octaves = 1;
    float persistence = 0.5f;
    if (n_args >= 4) {
        octaves = mp_obj_get_int(args[3]);
    }
    if (n_args >= 5) {
        persistence = mp_obj_get_float(args[4]);
    }
    return mp_obj_new_float(octave_perlin(x, y, z, octaves, persistence));
}

// Define a Python reference to the function above
static MP_DEFINE_CONST_FUN_OBJ_1(mod_perlin_reseed_obj, mod_perlin_reseed);
static MP_DEFINE_CONST_FUN_OBJ_3(mod_perlin_perlin_obj, mod_perlin_perlin);
static MP_DEFINE_CONST_FUN_OBJ_VAR(mod_perlin_octave_perlin_obj, 3, mod_perlin_octave_perlin);

// This is the entry point and is called when the module is imported
mp_obj_t mpy_init(mp_obj_fun_bc_t *self, size_t n_args, size_t n_kw, mp_obj_t *args) {
    // This must be first, it sets up the globals dict and other things
    MP_DYNRUNTIME_INIT_ENTRY

    // Make the function available in the module's namespace
    mp_store_global(MP_QSTR_reseed, MP_OBJ_FROM_PTR(&mod_perlin_reseed_obj));
    mp_store_global(MP_QSTR_perlin, MP_OBJ_FROM_PTR(&mod_perlin_perlin_obj));
    mp_store_global(MP_QSTR_octave_perlin, MP_OBJ_FROM_PTR(&mod_perlin_octave_perlin_obj));

    // This must be last, it restores the globals dict
    MP_DYNRUNTIME_INIT_EXIT
}
