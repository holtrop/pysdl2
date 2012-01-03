
#include "Cursor.h"

static int
sdl_Cursor_init(sdl_Cursor *self, PyObject *args, PyObject *kwargs)
{
    PyErr_SetString(PyExc_TypeError, "Cursor objects are not constructible");
    return -1;
}

static int
sdl_Cursor_setattro(sdl_Cursor *self, PyObject *attr, PyObject *val)
{
    PyErr_SetString(PyExc_TypeError, "Cursor types are immutable");
    return -1;
}

static PyObject *
sdl_Cursor_getattro(sdl_Cursor *self, PyObject *attr)
{
    PyErr_SetString(PyExc_TypeError, "Cursor types have no accessible members");
    return NULL;
}

static void
sdl_Cursor_dealloc(sdl_Cursor *self)
{
    PyObject_Del(self);
}

PyTypeObject sdl_CursorType = {
    PyObject_HEAD_INIT(NULL)
    0,                              /* ob_size */
    "SDL.Cursor",                   /* tp_name */
    sizeof(sdl_Cursor),             /* tp_basicsize */
    0,                              /* tp_itemsize */
    (destructor)sdl_Cursor_dealloc, /* tp_dealloc */
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
    (getattrofunc)sdl_Cursor_getattro,/* tp_getattro */
    (setattrofunc)sdl_Cursor_setattro,/* tp_setattro */
    0,                              /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   /* tp_flags */
    "SDL Cursor Structure",         /* tp_doc */
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
    (initproc)sdl_Cursor_init,      /* tp_init */
    0,                              /* tp_alloc */
    PyType_GenericNew,              /* tp_new */
};

void sdl_Cursor_register_type(PyObject *module)
{
    if (PyType_Ready(&sdl_CursorType) < 0)
    {
        fprintf(stderr, "Error initializing sdl_CursorType\n");
        return;
    }

    Py_INCREF(&sdl_CursorType);
    PyModule_AddObject(module, "Cursor", (PyObject *) &sdl_CursorType);
}

PyObject *sdl_Cursor_from_SDL_Cursor(SDL_Cursor *c)
{
    if (c == NULL)
        Py_RETURN_NONE;
    sdl_Cursor *cursor = PyObject_New(sdl_Cursor, &sdl_CursorType);
    cursor->cursor = c;
    return (PyObject *) cursor;
}

PyObject *sdl_Cursor_get_type(void)
{
    return (PyObject *) &sdl_CursorType;
}

SDL_Cursor *sdl_Cursor_get_SDL_Cursor(PyObject *cursor)
{
    return ((sdl_Cursor *) cursor)->cursor;
}
