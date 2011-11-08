
#include <Python.h>
#include <SDL/SDL.h>
#include "sdl_module.h"

#define _MAKE_CONST(m, pfx, sym) \
    do { \
        PyObject *io = PyInt_FromLong(pfx ## sym); \
        PyDict_SetItemString((m), #sym, io); \
        Py_DECREF(io); \
    } while(0)
#define MAKE_CONST(m, sym) _MAKE_CONST(m, SDL_, sym)
#define MAKE_CONSTK(m, sym) _MAKE_CONST(m, SDL, sym)

#define PYFUNC(name) static PyObject *name(PyObject *self, PyObject *args)

PYFUNC(sdl_init)
{
    Uint32 flags;
    if (!PyArg_ParseTuple(args, "I", &flags))
        return NULL;
    int rc = SDL_Init(flags);
    return Py_BuildValue("i", rc);
}

PYFUNC(sdl_initsubsystem)
{
    Uint32 flags;
    if (!PyArg_ParseTuple(args, "I", &flags))
        return NULL;
    int rc = SDL_InitSubSystem(flags);
    return Py_BuildValue("i", rc);
}

PYFUNC(sdl_quit)
{
    SDL_Quit();
    Py_RETURN_NONE;
}

static PyMethodDef sdl_methods[] = {
    {"Init", sdl_init, METH_VARARGS, "Initialize PySDL"},
    {"InitSubSystem", sdl_initsubsystem, METH_VARARGS, "Initialize subsystems"},
    {"Quit", sdl_quit, METH_VARARGS, "Uninitialize PySDL"},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initSDL(void)
{
    PyObject *m = Py_InitModule(PYSDL_MODULE_NAME, sdl_methods);
    if (m == NULL)
        return;

    PyObject *md = PyModule_GetDict(m);
    if (md == NULL)
        return;

    MAKE_CONST(md, INIT_TIMER);
    MAKE_CONST(md, INIT_VIDEO);
    MAKE_CONST(md, INIT_AUDIO);
    MAKE_CONST(md, INIT_CDROM);
    MAKE_CONST(md, INIT_JOYSTICK);
    MAKE_CONST(md, INIT_EVERYTHING);
    MAKE_CONST(md, INIT_NOPARACHUTE);
    MAKE_CONST(md, INIT_EVENTTHREAD);
}
