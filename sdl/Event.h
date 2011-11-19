
#ifndef EVENT_H
#define EVENT_H

#include <Python.h>
#include <pysdl.h>
#include <structmember.h>

typedef struct
{
    PyObject_HEAD
    /* If subevent_type if SDL_NOEVENT, then this is the top-level event
     * object. Otherwise this is a subevent object, and toplevel points
     * to the top-level event object. */
    int subevent_type;
    SDL_Event *event;
    PyObject *toplevel;
} sdl_Event;

void sdl_Event_register_type(PyObject *module);
PyObject *sdl_Event_from_SDL_Event(SDL_Event *event);
PyObject *sdl_Event_get_type(void);
SDL_Event *sdl_Event_get_SDL_Event(PyObject *event);

#endif /* EVENT_H */
