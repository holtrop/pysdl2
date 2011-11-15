
#ifndef PIXELFORMAT_H
#define PIXELFORMAT_H

#include <Python.h>
#include <pysdl.h>
#include <structmember.h>

typedef struct
{
    PyObject_HEAD
    PyObject *palette;
    SDL_PixelFormat pf;
} sdl_PixelFormat;

extern PyTypeObject sdl_PixelFormatType;

#endif /* PIXELFORMAT_H */
