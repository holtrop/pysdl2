
#ifndef RECT_H
#define RECT_H

#include <Python.h>
#include <pysdl.h>
#include <structmember.h>

typedef struct
{
    PyObject_HEAD
    char owner;
    SDL_Rect *rect;
} sdl_Rect;

void sdl_Rect_register_type(PyObject *module);
PyObject *sdl_Rect_from_SDL_Rect(SDL_Rect *rect);

#endif /* RECT_H */
