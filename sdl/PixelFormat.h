
#ifndef PIXELFORMAT_H
#define PIXELFORMAT_H

#include <Python.h>
#include <pysdl.h>
#include <structmember.h>

typedef struct
{
    PyObject_HEAD
    PyObject *palette;
    SDL_PixelFormat *pf;
    char ok_to_dealloc;
} sdl_PixelFormat;

void sdl_PixelFormat_register_type(PyObject *module);
PyObject *sdl_PixelFormat_from_SDL_PixelFormat(SDL_PixelFormat *pf);
PyObject *sdl_PixelFormat_get_type(void);
SDL_PixelFormat *sdl_PixelFormat_get_SDL_PixelFormat(PyObject *pf);

#endif /* PIXELFORMAT_H */
