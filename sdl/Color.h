
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

extern PyTypeObject sdl_ColorType;

#endif /* COLOR_H */
