
#ifndef SURFACE_H
#define SURFACE_H

#include <Python.h>
#include <pysdl.h>
#include <structmember.h>

typedef struct
{
    PyObject_HEAD
    SDL_Surface *surface;
    PyObject *format;
    PyObject *pixels;
    PyObject *clip_rect;
    char ok_to_dealloc;
} sdl_Surface;

void sdl_Surface_register_type(PyObject *module);
PyObject *sdl_Surface_from_SDL_Surface(SDL_Surface *surface);
SDL_Surface *sdl_Surface_get_SDL_Surface(PyObject *sdl_surface);
PyObject *sdl_Surface_get_type(void);

#endif /* SURFACE_H */
