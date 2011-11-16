
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

PyObject *sdl_Rect_from_SDL_Rect(SDL_Rect *rect);

#endif /* RECT_H */
