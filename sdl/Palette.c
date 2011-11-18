
#include "Palette.h"
#include "Color.h"

static int
sdl_Palette_init(sdl_Palette *self, PyObject *args, PyObject *kwargs)
{
    PyErr_SetString(PyExc_TypeError, "Palette objects are not constructible");
    return -1;
}

static int
sdl_Palette_setattro(sdl_Palette *self, PyObject *attr, PyObject *val)
{
    PyErr_SetString(PyExc_TypeError, "Palette types are immutable");
    return -1;
}

static PyObject *
sdl_Palette_getattro(sdl_Palette *self, PyObject *attr)
{
    if (!PyString_Check(attr))
    {
        PyErr_SetString(PyExc_AttributeError, "Invalid attribute name");
        return NULL;
    }
    const char *aname = PyString_AsString(attr);
    if (!strcmp(aname, "ncolors"))
        return PyInt_FromLong(self->palette->ncolors);
    else if (!strcmp(aname, "colors"))
        return self->colors;
    PyErr_SetString(PyExc_AttributeError, "Invalid attribute");
    return NULL;
}

PyTypeObject sdl_PaletteType = {
    PyObject_HEAD_INIT(NULL)
    0,                              /* ob_size */
    "SDL.Palette",                  /* tp_name */
    sizeof(sdl_Palette),            /* tp_basicsize */
    0,                              /* tp_itemsize */
    0,                              /* tp_dealloc */
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
    (getattrofunc)sdl_Palette_getattro,/* tp_getattro */
    (setattrofunc)sdl_Palette_setattro,/* tp_setattro */
    0,                              /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   /* tp_flags */
    "SDL Palette Structure",        /* tp_doc */
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
    (initproc)sdl_Palette_init,     /* tp_init */
    0,                              /* tp_alloc */
    PyType_GenericNew,              /* tp_new */
};

void sdl_Palette_register_type(PyObject *module)
{
    if (PyType_Ready(&sdl_PaletteType) < 0)
    {
        fprintf(stderr, "Error initializing sdl_PaletteType\n");
        return;
    }

    Py_INCREF(&sdl_PaletteType);
    PyModule_AddObject(module, "Palette", (PyObject *) &sdl_PaletteType);
}

PyObject *sdl_Palette_from_SDL_Palette(SDL_Palette *p)
{
    if (p == NULL)
        Py_RETURN_NONE;
    PyObject *colors = PyList_New(p->ncolors);
    for (int i = 0; i < p->ncolors; i++)
    {
        PyObject *color = sdl_Color_from_SDL_Color(&p->colors[i]);
        PyList_SetItem(colors, i, color);
    }
    sdl_Palette *palette = PyObject_New(sdl_Palette, &sdl_PaletteType);
    palette->palette = p;
    palette->colors = colors;
    return (PyObject *) palette;
}
