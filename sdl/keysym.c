
#include "keysym.h"

static int
sdl_keysym_init(sdl_keysym *self, PyObject *args, PyObject *kwargs)
{
    self->event = NULL;
    PyErr_SetString(PyExc_TypeError,
            "keysym objects are not constructible");
    return -1;
}

static int
sdl_keysym_setattro(sdl_keysym *self, PyObject *attr, PyObject *val)
{
    if (!PyString_Check(attr) || !PyInt_Check(val))
    {
        PyErr_SetString(PyExc_ValueError, "Bad parameters");
        return -1;
    }
    const char *aname = PyString_AsString(attr);
    int v = PyInt_AsLong(val);
    if (!strcmp(aname, "scancode"))
        self->ks->scancode = v;
    else if (!strcmp(aname, "sym"))
        self->ks->sym = v;
    else if (!strcmp(aname, "mod"))
        self->ks->mod = v;
    else if (!strcmp(aname, "unicode"))
        self->ks->unicode = v;
    else
    {
        PyErr_SetString(PyExc_AttributeError, "Invalid attribute");
        return -1;
    }
    return 0;
}

static PyObject *
sdl_keysym_getattro(sdl_keysym *self, PyObject *attr)
{
    if (!PyString_Check(attr))
    {
        PyErr_SetString(PyExc_AttributeError, "Invalid attribute name");
        return NULL;
    }
    const char *aname = PyString_AsString(attr);
    if (!strcmp(aname, "scancode"))
        return PyInt_FromLong(self->ks->scancode);
    else if (!strcmp(aname, "sym"))
        return PyInt_FromLong(self->ks->sym);
    else if (!strcmp(aname, "mod"))
        return PyInt_FromLong(self->ks->mod);
    else if (!strcmp(aname, "unicode"))
        return PyInt_FromLong(self->ks->unicode);
    PyErr_SetString(PyExc_AttributeError, "Invalid attribute");
    return NULL;
}

static void
sdl_keysym_dealloc(sdl_keysym *self)
{
    Py_XDECREF(self->event);
}

PyTypeObject sdl_keysymType = {
    PyObject_HEAD_INIT(NULL)
    0,                              /* ob_size */
    "SDL.keysym",                   /* tp_name */
    sizeof(sdl_keysym),             /* tp_basicsize */
    0,                              /* tp_itemsize */
    (destructor)sdl_keysym_dealloc, /* tp_dealloc */
    0,                              /* tp_print */
    0,                              /* tp_getattr */
    0,                              /* tp_setattr */
    0,                              /* tp_compare */
    0,                              /* tp_repr */
    0,                              /* tp_as_number */
    0,                              /* tp_as_sequence */
    0,                              /* tp_as_mapping */
    0,                              /* tp_hash  */
    0,                              /* tp_call */
    0,                              /* tp_str */
    (getattrofunc)sdl_keysym_getattro,/* tp_getattro */
    (setattrofunc)sdl_keysym_setattro,/* tp_setattro */
    0,                              /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   /* tp_flags */
    "SDL keysym Structure",         /* tp_doc */
    0,                              /* tp_traverse */
    0,                              /* tp_clear */
    0,                              /* tp_richcompare */
    0,                              /* tp_weaklistoffset */
    0,                              /* tp_iter */
    0,                              /* tp_iternext */
    0,                              /* tp_methods */
    0,                              /* tp_members */
    0,                              /* tp_getset */
    0,                              /* tp_base */
    0,                              /* tp_dict */
    0,                              /* tp_descr_get */
    0,                              /* tp_descr_set */
    0,                              /* tp_dictoffset */
    (initproc)sdl_keysym_init,      /* tp_init */
    0,                              /* tp_alloc */
    PyType_GenericNew,              /* tp_new */
};

void sdl_keysym_register_type(PyObject *module)
{
    if (PyType_Ready(&sdl_keysymType) < 0)
    {
        fprintf(stderr, "Error initializing sdl_keysymType\n");
        return;
    }

    Py_INCREF(&sdl_keysymType);
    PyModule_AddObject(module, "keysym", (PyObject *) &sdl_keysymType);
}

PyObject *sdl_keysym_from_SDL_keysym(SDL_keysym *ks)
{
    if (ks == NULL)
        Py_RETURN_NONE;
    sdl_keysym *k = PyObject_New(sdl_keysym, &sdl_keysymType);
    k->ks = ks;
    k->event = NULL;
    return (PyObject *) k;
}

PyObject *sdl_keysym_from_SDL_keysym_and_event(SDL_keysym *ks, sdl_Event *evt)
{
    PyObject *k = sdl_keysym_from_SDL_keysym(ks);
    ((sdl_keysym *) k)->event = (PyObject *) evt;
    Py_INCREF((PyObject *) evt);
    return k;
}

PyObject *sdl_keysym_get_type(void)
{
    return (PyObject *) &sdl_keysymType;
}

SDL_keysym *sdl_keysym_get_SDL_keysym(PyObject *ko)
{
    return ((sdl_keysym *) ko)->ks;
}
