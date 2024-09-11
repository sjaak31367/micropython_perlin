import micropython

def reseed(seed):
  global p
  from random import seed as __seed, randint as __randint
  p = bytearray(512)
  perm = bytearray(256)
  __seed(seed)
  for i in range(256):
    perm[i] = i
  for i in range(256):
    swap = __randint(0,255)
    temp = perm[swap]
    perm[swap] = perm[i]
    perm[i] = temp
  for i in range(512):
    p[i] = perm[i%256]
  del perm, __seed, __randint

@micropython.native
def perlin(x, y, z):
  global p
  xi = int(x) & 255
  yi = int(y) & 255
  zi = int(z) & 255
  xf = x - int(x)
  yf = y - int(y)
  zf = z - int(z)
  u = xf * xf * xf * (xf * (xf * 6 - 15) + 10)
  v = yf * yf * yf * (yf * (yf * 6 - 15) + 10)
  w = zf * zf * zf * (zf * (zf * 6 - 15) + 10)
  a  = p[xi]     + yi;
  aa = p[a]      + zi;
  ab = p[a  + 1] + zi;
  b  = p[xi + 1] + yi;
  ba = p[b]      + zi;
  bb = p[b  + 1] + zi;

##  gradh = ph & 15
##  gradu = gradh < 8 ? xf : yf
##  gradv = gradh < 4 ? yf : (gradh==12 || gradh==14 ? xf : zf)
##  grad = ((gradh & 1) == - ? gradu : -gradu) + ((gradh & 2) == 0 ? gradv : -gradv)
  gradh = p[aa] & 15
  gradu = xf if gradh < 8 else yf
  gradv = yf if gradh < 4 else (xf if gradh==12 or gradh==14 else zf)
  lerpa = (gradu if (gradh & 1) == 0 else -gradu) + (gradv if (gradh & 2) == 0 else -gradv)
  gradh = p[ba] & 15
  gradu = xf-1 if gradh < 8 else yf
  gradv = yf if gradh < 4 else (xf-1 if gradh==12 or gradh==14 else zf)
  lerpb = (gradu if (gradh & 1) == 0 else -gradu) + (gradv if (gradh & 2) == 0 else -gradv)
  x1 = lerpa + u * (lerpb - lerpa)

  gradh = p[ab] & 15
  gradu = xf if gradh < 8 else yf-1
  gradv = yf-1 if gradh < 4 else (xf if gradh==12 or gradh==14 else zf)
  lerpa = (gradu if (gradh & 1) == 0 else -gradu) + (gradv if (gradh & 2) == 0 else -gradv)
  gradh = p[bb] & 15
  gradu = xf-1 if gradh < 8 else yf-1
  gradv = yf-1 if gradh < 4 else (xf-1 if gradh==12 or gradh==14 else zf)
  lerpb = (gradu if (gradh & 1) == 0 else -gradu) + (gradv if (gradh & 2) == 0 else -gradv)
  x2 = lerpa + u * (lerpb - lerpa)

  y1 = x1 + v *(x2 - x1)


  gradh = p[aa+1] & 15
  gradu = xf if gradh < 8 else yf
  gradv = yf if gradh < 4 else (xf if gradh==12 or gradh==14 else zf-1)
  lerpa = (gradu if (gradh & 1) == 0 else -gradu) + (gradv if (gradh & 2) == 0 else -gradv)
  gradh = p[ba+1] & 15
  gradu = xf-1 if gradh < 8 else yf
  gradv = yf if gradh < 4 else (xf-1 if gradh==12 or gradh==14 else zf-1)
  lerpb = (gradu if (gradh & 1) == 0 else -gradu) + (gradv if (gradh & 2) == 0 else -gradv)
  x1 = lerpa + u * (lerpb - lerpa)

  gradh = p[ab+1] & 15
  gradu = xf if gradh < 8 else yf-1
  gradv = yf-1 if gradh < 4 else (xf if gradh==12 or gradh==14 else zf-1)
  lerpa = (gradu if (gradh & 1) == 0 else -gradu) + (gradv if (gradh & 2) == 0 else -gradv)
  gradh = p[bb+1] & 15
  gradu = xf-1 if gradh < 8 else yf-1
  gradv = yf-1 if gradh < 4 else (xf-1 if gradh==12 or gradh==14 else zf-1)
  lerpb = (gradu if (gradh & 1) == 0 else -gradu) + (gradv if (gradh & 2) == 0 else -gradv)
  x2 = lerpa + u * (lerpb - lerpa)

  y2 = x1 + v *(x2 - x1)

  return ((y1 + w * (y2 - y1)) +1) / 2

@micropython.native
def octave_perlin(x, y, z, octaves=1, persistence=0.5):
  total = 0
  frequency = 1
  amplitude = 1
  max_value = 0
  i = 0
  while (i < octaves):
    total += perlin(x * frequency, y * frequency, z * frequency) * amplitude
    max_value += amplitude
    amplitude *= persistence
    frequency *= 2
    i += 1
  return total / max_value
