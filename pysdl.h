
#include <SDL/SDL.h>

#define MAKE_PY_CONST(m, name, sym) \
    do { \
        PyObject *io = PyInt_FromLong(sym); \
        PyDict_SetItemString((m), (name), io); \
        Py_DECREF(io); \
    } while(0)

#define PYFUNC(name, desc) \
    static const char name ## _doc[] = desc; \
    static PyObject *pysdl_ ## name(PyObject *self, PyObject *args)
#define PYFUNC_REF(name) {#name, pysdl_ ## name, METH_VARARGS, name ## _doc}

