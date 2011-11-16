
#include "Color.h"

static PyMemberDef sdl_Color_members[] = {
    {"r", T_UBYTE, offsetof(sdl_Color, color.r), 0, "r"},
    {"g", T_UBYTE, offsetof(sdl_Color, color.g), 0, "g"},
    {"b", T_UBYTE, offsetof(sdl_Color, color.b), 0, "b"},
    {NULL}
};

static int
sdl_Color_init(sdl_Color *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = {"r", "g", "b", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "iii", kwlist,
                &self->color.r, &self->color.g, &self->color.b))
        return -1;
    return 0;
}

PyTypeObject sdl_ColorType = {
    PyObject_HEAD_INIT(NULL)
    0,                              /* ob_size */
    "SDL.Color",                    /* tp_name */
    sizeof(sdl_Color),              /* tp_basicsize */
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
    "SDL Color Structure",          /* tp_doc */
    0,                              /* tp_traverse */
    0,                              /* tp_clear */
    0,                              /* tp_richcompare */
    0,                              /* tp_weaklistoffset */
    0,                              /* tp_iter */
    0,                              /* tp_iternext */
    0,                              /* tp_methods */
    sdl_Color_members,              /* tp_members */
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
