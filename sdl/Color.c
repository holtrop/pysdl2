
#include "Color.h"

static int
sdl_Color_init(sdl_Color *self, PyObject *args, PyObject *kwargs)
{
    self->owner = 1;
    self->color = calloc(1, sizeof(sdl_Color));
    return 0;
}

static int
sdl_Color_setattro(sdl_Color *self, PyObject *attr, PyObject *val)
{
    if (!PyString_Check(attr) || !PyInt_Check(val))
    {
        PyErr_SetString(PyExc_ValueError, "Bad parameters");
        return -1;
    }
    const char *aname = PyString_AsString(attr);
    int v = PyInt_AsLong(val);
    if (!strcmp(aname, "r"))
        self->color->r = v;
    else if (!strcmp(aname, "g"))
        self->color->g = v;
    else if (!strcmp(aname, "b"))
        self->color->b = v;
    else
    {
        PyErr_SetString(PyExc_AttributeError, "Invalid attribute");
        return -1;
    }
    return 0;
}

static PyObject *
sdl_Color_getattro(sdl_Color *self, PyObject *attr)
{
    if (!PyString_Check(attr))
    {
        PyErr_SetString(PyExc_AttributeError, "Invalid attribute name");
        return NULL;
    }
    const char *aname = PyString_AsString(attr);
    if (!strcmp(aname, "r"))
        return PyInt_FromLong(self->color->r);
    else if (!strcmp(aname, "g"))
        return PyInt_FromLong(self->color->g);
    else if (!strcmp(aname, "b"))
        return PyInt_FromLong(self->color->b);
    PyErr_SetString(PyExc_AttributeError, "Invalid attribute");
    return NULL;
}

static void
sdl_Color_dealloc(sdl_Color *self)
{
    if (self->owner)
        free(self->color);
}

PyTypeObject sdl_ColorType = {
    PyObject_HEAD_INIT(NULL)
    0,                              /* ob_size */
    "SDL.Color",                    /* tp_name */
    sizeof(sdl_Color),              /* tp_basicsize */
    0,                              /* tp_itemsize */
    (destructor)sdl_Color_dealloc,  /* tp_dealloc */
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
    (getattrofunc)sdl_Color_getattro,/* tp_getattro */
    (setattrofunc)sdl_Color_setattro,/* tp_setattro */
    0,                              /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   /* tp_flags */
    "SDL Color Structure",          /* tp_doc */
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
    (initproc)sdl_Color_init,       /* tp_init */
    0,                              /* tp_alloc */
    PyType_GenericNew,              /* tp_new */
};

void sdl_Color_register_type(PyObject *module)
{
    if (PyType_Ready(&sdl_ColorType) < 0)
    {
        fprintf(stderr, "Error initializing sdl_ColorType\n");
        return;
    }

    Py_INCREF(&sdl_ColorType);
    PyModule_AddObject(module, "Color", (PyObject *) &sdl_ColorType);
}

PyObject *sdl_Color_from_SDL_Color(SDL_Color *color)
{
    if (color == NULL)
        Py_RETURN_NONE;
    PyObject *args = Py_BuildValue("iii", color->r, color->g, color->b);
    PyObject *sdl_color = PyObject_CallObject((PyObject *) &sdl_ColorType,
            args);
    Py_DECREF(args);
    return sdl_color;
}
