
#include <Python.h>
#include <SDL/SDL.h>
#include "sdl_module.h"

static PyObject *
sdl_init(PyObject *self, PyObject *args)
{
    Py_RETURN_NONE;
}

static PyMethodDef sdl_methods[] = {
    {"Init", sdl_init, METH_VARARGS, "Initialize PySDL"},
};

PyMODINIT_FUNC
initSDL(void)
{
    PyObject *module = Py_InitModule(PYSDL_MODULE_NAME, sdl_methods);

    if (module == NULL)
        return;
}
