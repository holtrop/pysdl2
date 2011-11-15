
#include "Rect.h"

static PyMemberDef sdl_Rect_members[] = {
    {"x", T_UBYTE, offsetof(sdl_Rect, rect.x), 0, "x"},
    {"y", T_UBYTE, offsetof(sdl_Rect, rect.y), 0, "y"},
    {"w", T_UBYTE, offsetof(sdl_Rect, rect.w), 0, "width"},
    {"h", T_UBYTE, offsetof(sdl_Rect, rect.h), 0, "height"},
    {NULL}
};

static int
sdl_Rect_init(sdl_Rect *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = {"x", "y", "w", "h", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|iiii", kwlist,
                &self->rect.x, &self->rect.y, &self->rect.w, &self->rect.h))
        return -1;
    return 0;
}

PyTypeObject sdl_RectType = {
    PyObject_HEAD_INIT(NULL)
    0,                              /* ob_size */
    "SDL.Rect",                     /* tp_name */
    sizeof(sdl_Rect),               /* tp_basicsize */
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
    "SDL Rect Structure",           /* tp_doc */
    0,                              /* tp_traverse */
    0,                              /* tp_clear */
    0,                              /* tp_richcompare */
    0,                              /* tp_weaklistoffset */
    0,                              /* tp_iter */
    0,                              /* tp_iternext */
    0,                              /* tp_methods */
    sdl_Rect_members,               /* tp_members */
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
