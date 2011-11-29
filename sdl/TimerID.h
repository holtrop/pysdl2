
#ifndef TIMERID_H
#define TIMERID_H

#include <Python.h>
#include <pysdl.h>
#include <structmember.h>

typedef struct
{
    PyObject_HEAD
    SDL_TimerID timerid;
    PyObject *evt;
    char ok_to_dealloc;
} sdl_TimerID;

void sdl_TimerID_register_type(PyObject *module);
PyObject *sdl_TimerID_build(PyObject *evt);
void sdl_TimerID_set_TimerID(PyObject *sdl_timerid, SDL_TimerID timerid);
PyObject *sdl_TimerID_get_type(void);
SDL_TimerID sdl_TimerID_get_SDL_TimerID(PyObject *sdl_timerid);

#endif /* TIMERID_H */
