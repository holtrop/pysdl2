#!/usr/bin/env python

import math
import os
import sys
for d in filter(lambda x: x.startswith('lib.'), os.listdir('build')):
    sys.path = [os.path.sep.join([os.getcwd(), 'build', d])] + sys.path
import SDL

def make_cursor():
    SZ = 32
    data = [0] * (SZ * SZ / 8)
    mask = [0] * (SZ * SZ / 8)
    mx = SZ / 2
    my = SZ / 2
    idx = 0
    for y in range(SZ):
        for x in range(SZ):
            xd = x - mx
            yd = y - my
            dist = math.sqrt(xd * xd + yd * yd)
            if dist < 3.5:
                d, m = 1, 0
            elif dist < 8.5:
                d, m = 1, 1
            elif dist < 13.5:
                d, m = 0, 1
            else:
                d, m = 0, 0
            data[idx] |= d
            mask[idx] |= m
            if x % 8 == 7:
                idx += 1
            else:
                data[idx] = data[idx] << 1
                mask[idx] = mask[idx] << 1
    return SDL.CreateCursor(data, mask, SZ, SZ, mx, my)

SDL.Init(SDL.INIT_EVERYTHING)

surf = SDL.SetVideoMode(200, 200, 32, 0)

origcur = SDL.GetCursor()

cur2 = make_cursor()
SDL.SetCursor(cur2)

while True:
    evt = SDL.WaitEvent()
    if evt.type == SDL.KEYDOWN:
        if evt.key.keysym.sym == SDL.K_1:
            SDL.SetCursor(origcur)
        elif evt.key.keysym.sym == SDL.K_2:
            SDL.SetCursor(cur2)
        elif evt.key.keysym.sym == SDL.K_q:
            break
    elif evt.type == SDL.QUIT:
        break

SDL.Quit()
