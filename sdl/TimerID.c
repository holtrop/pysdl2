
#include "TimerID.h"
#include "Color.h"

static int
sdl_TimerID_init(sdl_TimerID *self, PyObject *args, PyObject *kwargs)
{
    PyErr_SetString(PyExc_TypeError, "TimerID objects are not constructible");
    return -1;
}

static int
sdl_TimerID_setattro(sdl_TimerID *self, PyObject *attr, PyObject *val)
{
    PyErr_SetString(PyExc_TypeError, "TimerID types are immutable");
    return -1;
}

static PyObject *
sdl_TimerID_getattro(sdl_TimerID *self, PyObject *attr)
{
    PyErr_SetString(PyExc_AttributeError, "Invalid attribute name");
    return NULL;
}

static void
sdl_TimerID_dealloc(sdl_TimerID *self)
{
    PyObject_Del(self);
}

PyTypeObject sdl_TimerIDType = {
    PyObject_HEAD_INIT(NULL)
    0,                              /* ob_size */
    "SDL.TimerID",                  /* tp_name */
    sizeof(sdl_TimerID),            /* tp_basicsize */
    0,                              /* tp_itemsize */
    (destructor)sdl_TimerID_dealloc,/* tp_dealloc */
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
    (getattrofunc)sdl_TimerID_getattro,/* tp_getattro */
    (setattrofunc)sdl_TimerID_setattro,/* tp_setattro */
    0,                              /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   /* tp_flags */
    "SDL TimerID Structure",        /* tp_doc */
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
    (initproc)sdl_TimerID_init,     /* tp_init */
    0,                              /* tp_alloc */
    PyType_GenericNew,              /* tp_new */
};

void sdl_TimerID_register_type(PyObject *module)
{
    if (PyType_Ready(&sdl_TimerIDType) < 0)
    {
        fprintf(stderr, "Error initializing sdl_TimerIDType\n");
        return;
    }

    Py_INCREF(&sdl_TimerIDType);
    PyModule_AddObject(module, "TimerID", (PyObject *) &sdl_TimerIDType);
}

PyObject *sdl_TimerID_from_SDL_TimerID(SDL_TimerID timerid)
{
    if (timerid == NULL)
        Py_RETURN_NONE;
    sdl_TimerID *sdl_timerid = PyObject_New(sdl_TimerID, &sdl_TimerIDType);
    sdl_timerid->timerid = timerid;
    return (PyObject *) sdl_timerid;
}
