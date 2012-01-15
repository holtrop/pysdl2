#!/usr/bin/env python

import sys
import SDL
from OpenGL.GL import *

def init(surf):
    glShadeModel(GL_SMOOTH)
    aspect = surf.w / float(surf.h)
    glOrtho(-aspect, aspect, -1.0, 1.0, -1.0, 1.0)

def redraw():
    glClear(GL_COLOR_BUFFER_BIT)
    glPushMatrix()
    glRotatef(SDL.GetTicks() * 90.0 / 1000, 0, 0, 1)
    glBegin(GL_QUADS)
    glColor(1.0, 1.0, 1.0)
    glVertex3f(0.5, 0.5, 0)
    glColor(1.0, 0.0, 0.0)
    glVertex3f(-0.5, 0.5, 0)
    glColor(0.0, 1.0, 0.0)
    glVertex3f(-0.5, -0.5, 0)
    glColor(0.0, 0.0, 1.0)
    glVertex3f(0.5, -0.5, 0)
    glEnd()
    glPopMatrix()
    SDL.GL_SwapBuffers()

def main(argv):
    SDL.Init(SDL.INIT_EVERYTHING)

    SDL.GL_SetAttribute(SDL.GL_DOUBLEBUFFER, 1)

    screen = SDL.SetVideoMode(800, 600, 32, SDL.OPENGL)
    SDL.WM_SetCaption("PySDL2 OpenGL demo", "PySDL2 OpenGL demo")

    init(screen)

    timer_event = SDL.Event()
    timer_event.type = SDL.USEREVENT
    draw_timer = SDL.AddTimer(1000 / 30, timer_event)

    while True:
        evt = SDL.WaitEvent()
        if evt.type == SDL.QUIT:
            break
        elif evt.type == SDL.USEREVENT:
            redraw()

    SDL.Quit()

if __name__ == '__main__':
    sys.exit(main(sys.argv))
