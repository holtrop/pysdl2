
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

#define PYFUNC(name, desc) \
    static const char name ## _doc[] = desc; \
    static PyObject *pysdl_ ## name(PyObject *self, PyObject *args)

PYFUNC(GetError, "Get SDL error string")
{
    char *s = SDL_GetError();
    if (s == NULL)
        Py_RETURN_NONE;
    return Py_BuildValue("s", s);
}

PYFUNC(Init, "Initialize PySDL")
{
    Uint32 flags;
    if (!PyArg_ParseTuple(args, "I", &flags))
        return NULL;
    int rc = SDL_Init(flags);
    return Py_BuildValue("i", rc);
}

PYFUNC(InitSubSystem, "Initialize subsystems")
{
    Uint32 flags;
    if (!PyArg_ParseTuple(args, "I", &flags))
        return NULL;
    int rc = SDL_InitSubSystem(flags);
    return Py_BuildValue("i", rc);
}

PYFUNC(Quit, "Uninitialize PySDL")
{
    SDL_Quit();
    Py_RETURN_NONE;
}

PYFUNC(QuitSubSystem, "Shut down a subsystem")
{
    Uint32 flags;
    if (!PyArg_ParseTuple(args, "I", &flags))
        return NULL;
    SDL_QuitSubSystem(flags);
    Py_RETURN_NONE;
}

PYFUNC(SetError, "Set the SDL error string")
{
    const char *s;
    if (!PyArg_ParseTuple(args, "s", &s))
        return NULL;
    SDL_SetError("%s", s);
    Py_RETURN_NONE;
}

PYFUNC(WasInit, "Check which subsystems are initialized")
{
    Uint32 flags;
    if (!PyArg_ParseTuple(args, "I", &flags))
        return NULL;
    int rc = SDL_WasInit(flags);
    return Py_BuildValue("i", rc);
}

#define METH_REF(name) {#name, pysdl_ ## name, METH_VARARGS, name ## _doc}
static PyMethodDef sdl_methods[] = {
    METH_REF(GetError),
    METH_REF(Init),
    METH_REF(InitSubSystem),
    METH_REF(Quit),
    METH_REF(QuitSubSystem),
    METH_REF(SetError),
    METH_REF(WasInit),
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

#undef ENOMEM
    MAKE_CONST(md, ENOMEM);
    MAKE_CONST(md, EFREAD);
    MAKE_CONST(md, EFWRITE);
    MAKE_CONST(md, EFSEEK);
    MAKE_CONST(md, UNSUPPORTED);
    MAKE_CONST(md, LASTERROR);
}
