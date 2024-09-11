from random import seed as __seed, randint as __randint

class perlin(object):
  def __init__():
    self.p = bytearray(512)
  def reseed(seed):
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
      self.p[i] = perm[i%256]
    del perm
  def perlin(x, y, z):
    xi = int(x) & 255
    yi = int(y) & 255
    zi = int(z) & 255
    xf = x - int(x)
    yf = y - int(y)
    zf = z - int(z)
    u = xf * xf * xf * (xf * (xf * 6 - 15) + 10)
    v = yf * yf * yf * (yf * (yf * 6 - 15) + 10)
    w = zf * zf * zf * (zf * (zf * 6 - 15) + 10)
    a  = self.p[xi] + yi;
    aa = self.p[a] + zi;
    ab = self.p[a + 1] + zi;
    b  = self.p[xi + 1] + yi;
    ba = self.p[b] + zi;
    bb = self.p[b + 1] + zi;

##    gradh = ph & 15
##    gradu = gradh < 8 ? xf : yf
##    gradv = gradh < 4 ? yf : (gradh==12 || gradh==14 ? xf : zf)
##    grad = ((gradh & 1) == - ? gradu : -gradu) + ((gradh & 2) == 0 ? gradv : -gradv)
    gradh = p[aa] & 15
    gradu = gradh < 8 ? xf : yf
    gradv = gradh < 4 ? yf : (gradh==12 || gradh==14 ? xf : zf)
    lerpa = ((gradh & 1) == - ? gradu : -gradu) + ((gradh & 2) == 0 ? gradv : -gradv)
    gradh = p[ba] & 15
    gradu = gradh < 8 ? xf-1 : yf
    gradv = gradh < 4 ? yf : (gradh==12 || gradh==14 ? xf-1 : zf)
    lerpb = ((gradh & 1) == - ? gradu : -gradu) + ((gradh & 2) == 0 ? gradv : -gradv)
    x1 = lerpa + u * (lerpb - lerpa)

    gradh = p[ab] & 15
    gradu = gradh < 8 ? xf : yf-1
    gradv = gradh < 4 ? yf-1 : (gradh==12 || gradh==14 ? xf : zf)
    lerpa = ((gradh & 1) == - ? gradu : -gradu) + ((gradh & 2) == 0 ? gradv : -gradv)
    gradh = p[bb] & 15
    gradu = gradh < 8 ? xf-1 : yf-1
    gradv = gradh < 4 ? yf-1 : (gradh==12 || gradh==14 ? xf-1 : zf)
    lerpb = ((gradh & 1) == - ? gradu : -gradu) + ((gradh & 2) == 0 ? gradv : -gradv)
    x2 = lerpa + u * (lerpb - lerpa)

    y1 = x1 + v *(x2 - x1)


    gradh = p[aa+1] & 15
    gradu = gradh < 8 ? xf : yf
    gradv = gradh < 4 ? yf : (gradh==12 || gradh==14 ? xf : zf-1)
    lerpa = ((gradh & 1) == - ? gradu : -gradu) + ((gradh & 2) == 0 ? gradv : -gradv)
    gradh = p[ba+1] & 15
    gradu = gradh < 8 ? xf-1 : yf
    gradv = gradh < 4 ? yf : (gradh==12 || gradh==14 ? xf-1 : zf-1)
    lerpb = ((gradh & 1) == - ? gradu : -gradu) + ((gradh & 2) == 0 ? gradv : -gradv)
    x1 = lerpa + u * (lerpb - lerpa)

    gradh = p[ab+1] & 15
    gradu = gradh < 8 ? xf : yf-1
    gradv = gradh < 4 ? yf-1 : (gradh==12 || gradh==14 ? xf : zf-1)
    lerpa = ((gradh & 1) == - ? gradu : -gradu) + ((gradh & 2) == 0 ? gradv : -gradv)
    gradh = p[bb+1] & 15
    gradu = gradh < 8 ? xf-1 : yf-1
    gradv = gradh < 4 ? yf-1 : (gradh==12 || gradh==14 ? xf-1 : zf-1)
    lerpb = ((gradh & 1) == - ? gradu : -gradu) + ((gradh & 2) == 0 ? gradv : -gradv)
    x2 = lerpa + u * (lerpb - lerpa)

    y2 = x1 + v *(x2 - x1)

    return ((y1 + w * (y2 - y1)) +1) / 2
