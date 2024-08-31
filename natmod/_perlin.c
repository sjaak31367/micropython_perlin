// Include the header file to get access to the MicroPython API
#include "py/dynruntime.h"
#include <math.h>
#include <stdio.h>
#include "_noise.h"

#define lerp(t, a, b) ((a) + (t) * ((b) - (a)))

static inline float grad2(const int hash, const float x, const float y) {
    const int h = hash & 15;
    return x * GRAD3[h][0] + y * GRAD3[h][1];
}

float noise2(float x, float y, const float repeatx, const float repeaty, const int base) {
    float fx, fy;
    int A, AA, AB, B, BA, BB;
    int i = (int)floorf(fmodf(x, repeatx));
    int j = (int)floorf(fmodf(y, repeaty));
    int ii = (int)fmodf(i + 1, repeatx);
    int jj = (int)fmodf(j + 1, repeaty);
    i = (i & 255) + base;
    j = (j & 255) + base;
    ii = (ii & 255) + base;
    jj = (jj & 255) + base;

    x -= floorf(x); y -= floorf(y);
    fx = x*x*x * (x * (x * 6 - 15) + 10);
    fy = y*y*y * (y * (y * 6 - 15) + 10);

    A  = PERM[i];
    AA = PERM[A + j];
    AB = PERM[A + jj];
    B  = PERM[ii];
    BA = PERM[B + j];
    BB = PERM[B + jj];
        
    return lerp(fy, lerp(fx, grad2(PERM[AA], x, y),
                             grad2(PERM[BA], x - 1, y)),
                    lerp(fx, grad2(PERM[AB], x, y - 1),
                             grad2(PERM[BB], x - 1, y - 1)));
}

static mp_obj_t py_noise2(size_t n_args, const mp_obj_t* args) {
    //if (n_args == 3 || n_args == 4) {
        //mp_obj_t self = args[0];
        mp_float_t _x = mp_obj_get_float(args[1]);
        mp_float_t _y = mp_obj_get_float(args[2]);
        mp_int_t _octaves = 1;
        if (n_args == 4) {
            _octaves = mp_obj_get_int(args[3]);
        }
    //} else {
    //    mp_raise_ValueError(MP_ERROR_TEXT("Function requires 3 or 4 arguments!"));
    //    return NULL;
    //}
    
    float x = _x;
    float y = _y;
    int octaves = (mp_int_t) _octaves;
    float persistence = 0.5f;
    float lacunarity = 2.0f;
    float repeatx = 1024; // arbitrary
    float repeaty = 1024; // arbitrary
    int base = 0;

    if (octaves == 1) {
        // Single octave, return simple noise
        return (mp_obj_t *) mp_obj_new_float_from_d((double) noise2(x, y, repeatx, repeaty, base));
    } else if (octaves > 1) {
        int i;
        float freq = 1.0f;
        float amp = 1.0f;
        float max = 0.0f;
        float total = 0.0f;

        for (i = 0; i < octaves; i++) {
            total += noise2(x * freq, y * freq, repeatx * freq, repeaty * freq, base) * amp;
            max += amp;
            freq *= lacunarity;
            amp *= persistence;
        }
        return (mp_obj_t *) mp_obj_new_float_from_d((double) (total / max));
    } else {
        mp_raise_ValueError(MP_ERROR_TEXT("Expected octaves value > 0"));
        return NULL;
    }
}
// Define a Python reference to the function above
static MP_DEFINE_CONST_FUN_OBJ_VAR(py_noise2_obj, 4, py_noise2);

// This is the entry point and is called when the module is imported
mp_obj_t mpy_init(mp_obj_fun_bc_t *self, size_t n_args, size_t n_kw, mp_obj_t *args) {
    // This must be first, it sets up the globals dict and other things
    MP_DYNRUNTIME_INIT_ENTRY

    // Make the function available in the module's namespace
    mp_store_global(MP_QSTR_noise2, MP_OBJ_FROM_PTR(&py_noise2_obj));

    // This must be last, it restores the globals dict
    MP_DYNRUNTIME_INIT_EXIT
}
