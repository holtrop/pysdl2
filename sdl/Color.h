
#ifndef COLOR_H
#define COLOR_H

#include <Python.h>
#include <pysdl.h>
#include <structmember.h>

typedef struct
{
    PyObject_HEAD
    SDL_Color color;
} sdl_Color;

void sdl_Color_register_type(PyObject *module);
PyObject *sdl_Color_from_SDL_Color(SDL_Color *color);

#endif /* COLOR_H */
