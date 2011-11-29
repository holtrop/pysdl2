
#ifndef TIMERID_H
#define TIMERID_H

#include <Python.h>
#include <pysdl.h>
#include <structmember.h>

typedef struct
{
    PyObject_HEAD
    SDL_TimerID timerid;
} sdl_TimerID;

void sdl_TimerID_register_type(PyObject *module);
PyObject *sdl_TimerID_from_SDL_TimerID(SDL_TimerID timerid);

#endif /* TIMERID_H */
