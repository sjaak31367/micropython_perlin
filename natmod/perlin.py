from random import seed as __seed, randint as __randint

def reseed(i):
  perm = [0]*256
  __seed(i)
  for i in range(256):
    perm[i] = i
  for i in range(256):
    swap = __randint(0,255)
    temp = perm[swap]
    perm[swap] = perm[i]
    perm[i] = temp
  __setperm(perm)
  del perm
