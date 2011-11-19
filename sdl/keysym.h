
#ifndef KEYSYM_H
#define KEYSYM_H

#include <Python.h>
#include <pysdl.h>
#include <structmember.h>

typedef struct
{
    PyObject_HEAD
    SDL_keysym *ks;
} sdl_keysym;

void sdl_keysym_register_type(PyObject *module);
PyObject *sdl_keysym_from_SDL_keysym(SDL_keysym *ks);
PyObject *sdl_keysym_get_type(void);
SDL_keysym *sdl_keysym_get_SDL_keysym(PyObject *ko);

#endif /* KEYSYM_H */
