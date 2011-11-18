
#ifndef PALETTE_H
#define PALETTE_H

#include <Python.h>
#include <pysdl.h>
#include <structmember.h>

typedef struct
{
    PyObject_HEAD
    SDL_Palette *palette;
    PyObject *colors;
} sdl_Palette;

void sdl_Palette_register_type(PyObject *module);
PyObject *sdl_Palette_from_SDL_Palette(SDL_Palette *p);

#endif /* PALETTE_H */
