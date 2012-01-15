#!/usr/bin/env python

import os
import sys
import time
for d in filter(lambda x: x.startswith('lib.'), os.listdir('build')):
    sys.path = [os.path.sep.join([os.getcwd(), 'build', d])] + sys.path
import SDL

def invert_surface(surf):
    i = 0
    for y in range(surf.h):
        for x in range(surf.w):
            surf.pixels[i] ^= 0xFFFFFFFF
            i += 1
    SDL.Flip(surf)

def main():
    SDL.Init(SDL.INIT_VIDEO)

    surf = SDL.SetVideoMode(256, 256, 32, SDL.DOUBLEBUF)

    if surf is not None:
        SDL.WM_SetCaption("Pixels test", "Pixels test")
        p = 0
        for y in range(256):
            for x in range(256):
                surf.pixels[p] = SDL.MapRGB(surf.format, x, 0, y)
                p += 1

        SDL.Flip(surf)

        while True:
            evt = SDL.WaitEvent()
            if evt.type == SDL.QUIT:
                break
            elif evt.type == SDL.KEYUP:
                if evt.key.keysym.sym == SDL.K_q:
                    break
                elif evt.key.keysym.sym == SDL.K_SPACE:
                    invert_surface(surf)
                elif evt.key.keysym.sym == SDL.K_F1:
                    SDL.WM_ToggleFullScreen(surf)
                elif evt.key.keysym.sym == SDL.K_F2:
                    grab = SDL.WM_GrabInput(SDL.GRAB_QUERY)
                    if grab == SDL.GRAB_ON:
                        grab = SDL.GRAB_OFF
                    else:
                        grab = SDL.GRAB_ON
                    SDL.WM_GrabInput(grab)

    SDL.Quit()

if __name__ == '__main__':
    main()
