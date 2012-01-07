#!/usr/bin/env python

import sys
import SDL
from OpenGL.GL import *

def redraw():
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glColor(1.0, 0.6, 0.0)
    glBegin(GL_QUADS)
    glNormal3f(0, 0, 1)
    glVertex3f(0.5, 0.5, 0)
    glVertex3f(-0.5, 0.5, 0)
    glVertex3f(-0.5, -0.5, 0)
    glVertex3f(0.5, -0.5, 0)
    glEnd()
    SDL.GL_SwapBuffers()

def main(argv):
    SDL.Init(SDL.INIT_EVERYTHING)

    SDL.GL_SetAttribute(SDL.GL_DOUBLEBUFFER, 1)

    screen = SDL.SetVideoMode(800, 600, 32, SDL.OPENGL)
    SDL.WM_SetCaption("PySDL OpenGL demo", "PySDL OpenGL demo")

    glEnable(GL_LIGHTING)
    glEnable(GL_LIGHT0)
    glEnable(GL_COLOR_MATERIAL)
    glShadeModel(GL_SMOOTH)

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
