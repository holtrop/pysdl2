
#include "Palette.h"
#include "Color.h"

static PyMemberDef sdl_Palette_members[] = {
    {"ncolors", T_INT, offsetof(sdl_Palette, ncolors), 0, "number of colors"},
    {"colors", T_OBJECT_EX, offsetof(sdl_Palette, colors), 0, "list of colors"},
    {NULL}
};

static int
sdl_Palette_init(sdl_Palette *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = {"ncolors", "colors", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "iO", kwlist,
                &self->ncolors, &self->colors))
        return -1;
    return 0;
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
    0,                              /* tp_getattro */
    0,                              /* tp_setattro */
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
    sdl_Palette_members,            /* tp_members */
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

PyObject *sdl_Palette_from_SDL_Palette(SDL_Palette *p)
{
    PyObject *colors = PyList_New(p->ncolors);
    for (int i = 0; i < p->ncolors; i++)
    {
        PyObject *color = sdl_Color_from_SDL_Color(&p->colors[i]);
        PyList_SetItem(colors, i, color);
    }
    PyObject *args = Py_BuildValue("iO", p->ncolors, colors);
    PyObject *palette = PyObject_CallObject((PyObject *) &sdl_PaletteType,
            args);
    Py_DECREF(args);
    Py_DECREF(colors);
    return palette;
}
