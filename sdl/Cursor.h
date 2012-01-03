
#ifndef CURSOR_H
#define CURSOR_H

#include <Python.h>
#include <pysdl.h>
#include <structmember.h>

typedef struct
{
    PyObject_HEAD
    SDL_Cursor *cursor;
} sdl_Cursor;

void sdl_Cursor_register_type(PyObject *module);
PyObject *sdl_Cursor_from_SDL_Cursor(SDL_Cursor *p);
PyObject *sdl_Cursor_get_type(void);
SDL_Cursor *sdl_Cursor_get_SDL_Cursor(PyObject *cursor);

#endif /* CURSOR_H */
