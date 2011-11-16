
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

PyObject *sdl_PixelFormat_from_SDL_PixelFormat(SDL_PixelFormat *pf);

#endif /* PIXELFORMAT_H */
