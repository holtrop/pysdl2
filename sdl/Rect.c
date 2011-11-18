
#include "Rect.h"

static int
sdl_Rect_init(sdl_Rect *self, PyObject *args, PyObject *kwargs)
{
    self->rect = (SDL_Rect *) calloc(1, sizeof(SDL_Rect));
    self->owner = 1;
    return 0;
}

static int
sdl_Rect_setattro(sdl_Rect *self, PyObject *attr, PyObject *val)
{
    if (!PyString_Check(attr) || !PyInt_Check(val))
    {
        PyErr_SetString(PyExc_ValueError, "Bad parameters");
        return -1;
    }
    const char *aname = PyString_AsString(attr);
    int v = PyInt_AsLong(val);
    if (!strcmp(aname, "x"))
        self->rect->x = v;
    else if (!strcmp(aname, "y"))
        self->rect->y = v;
    else if (!strcmp(aname, "w"))
        self->rect->w = v;
    else if (!strcmp(aname, "h"))
        self->rect->h = v;
    else
    {
        PyErr_SetString(PyExc_AttributeError, "Invalid attribute");
        return -1;
    }
    return 0;
}

static PyObject *
sdl_Rect_getattro(sdl_Rect *self, PyObject *attr)
{
    if (!PyString_Check(attr))
    {
        PyErr_SetString(PyExc_AttributeError, "Invalid attribute name");
        return NULL;
    }
    const char *aname = PyString_AsString(attr);
    if (!strcmp(aname, "x"))
        return PyInt_FromLong(self->rect->x);
    else if (!strcmp(aname, "y"))
        return PyInt_FromLong(self->rect->y);
    else if (!strcmp(aname, "w"))
        return PyInt_FromLong(self->rect->w);
    else if (!strcmp(aname, "h"))
        return PyInt_FromLong(self->rect->h);
    PyErr_SetString(PyExc_AttributeError, "Invalid attribute");
    return NULL;
}

static void
sdl_Rect_dealloc(sdl_Rect *self)
{
    if (self->owner)
        free(self->rect);
}

PyTypeObject sdl_RectType = {
    PyObject_HEAD_INIT(NULL)
    0,                              /* ob_size */
    "SDL.Rect",                     /* tp_name */
    sizeof(sdl_Rect),               /* tp_basicsize */
    0,                              /* tp_itemsize */
    (destructor)sdl_Rect_dealloc,   /* tp_dealloc */
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
    (getattrofunc)sdl_Rect_getattro,/* tp_getattro */
    (setattrofunc)sdl_Rect_setattro,/* tp_setattro */
    0,                              /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   /* tp_flags */
    "SDL Rect Structure",           /* tp_doc */
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
    (initproc)sdl_Rect_init,        /* tp_init */
    0,                              /* tp_alloc */
    PyType_GenericNew,              /* tp_new */
};

void sdl_Rect_register_type(PyObject *module)
{
    if (PyType_Ready(&sdl_RectType) < 0)
    {
        fprintf(stderr, "Error initializing sdl_RectType\n");
        return;
    }

    Py_INCREF(&sdl_RectType);
    PyModule_AddObject(module, "Rect", (PyObject *) &sdl_RectType);
}

PyObject *sdl_Rect_from_SDL_Rect(SDL_Rect *rect)
{
    if (rect == NULL)
        Py_RETURN_NONE;
    sdl_Rect *sdl_rect = PyObject_New(sdl_Rect, &sdl_RectType);
    sdl_rect->owner = 0;
    sdl_rect->rect = rect;
    return (PyObject *) sdl_rect;
}

PyObject *sdl_Rect_get_type(void)
{
    return (PyObject *) &sdl_RectType;
}

SDL_Rect *sdl_Rect_get_SDL_Rect(PyObject *rect)
{
    return ((sdl_Rect *) rect)->rect;
}
