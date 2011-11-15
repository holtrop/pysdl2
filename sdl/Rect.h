
#ifndef RECT_H
#define RECT_H

#include <Python.h>
#include <pysdl.h>
#include <structmember.h>

typedef struct
{
    PyObject_HEAD
    SDL_Rect rect;
} sdl_Rect;

extern PyTypeObject sdl_RectType;

#endif /* RECT_H */
