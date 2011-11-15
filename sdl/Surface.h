
#ifndef SURFACE_H
#define SURFACE_H

#include <Python.h>
#include <pysdl.h>
#include <structmember.h>

typedef struct
{
    PyObject_HEAD
    SDL_Surface surface;
    PyObject *format;
    PyObject *pixels;
    PyObject *clip_rect;
} sdl_Surface;

extern PyTypeObject sdl_SurfaceType;

#endif /* SURFACE_H */
