
#include "Surface.h"

static PyMemberDef sdl_Surface_members[] = {
    {"flags", T_UINT, offsetof(sdl_Surface, surface.flags), 0, "flags"},
    {"format", T_OBJECT_EX, offsetof(sdl_Surface, format), 0, "format"},
    {"w", T_INT, offsetof(sdl_Surface, surface.w), 0, "width"},
    {"h", T_INT, offsetof(sdl_Surface, surface.h), 0, "height"},
    {"pitch", T_USHORT, offsetof(sdl_Surface, surface.pitch), 0, "pitch"},
    {"pixels", T_OBJECT_EX, offsetof(sdl_Surface, pixels), 0, "pixels"},
    {"clip_rect", T_OBJECT_EX, offsetof(sdl_Surface, clip_rect), 0,
        "clip_rect"},
    {"refcount", T_INT, offsetof(sdl_Surface, surface.refcount), 0,
        "refcount"},
    {NULL}
};

static int
sdl_Surface_init(sdl_Surface *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = {
        "flags", "format", "w", "h", "pitch", "clip_rect", "refcount",
        NULL
    };
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "IOiiIOi", kwlist,
                &self->surface.flags, &self->format,
                &self->surface.w, &self->surface.h,
                &self->surface.pitch,
                &self->clip_rect, &self->surface.refcount))
        return -1;
    self->pixels = (PyObject *) self;
    return 0;
}

PyTypeObject sdl_SurfaceType = {
    PyObject_HEAD_INIT(NULL)
    0,                              /* ob_size */
    "SDL.Surface",                  /* tp_name */
    sizeof(sdl_Surface),            /* tp_basicsize */
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
    "SDL Surface Structure",        /* tp_doc */
    0,                              /* tp_traverse */
    0,                              /* tp_clear */
    0,                              /* tp_richcompare */
    0,                              /* tp_weaklistoffset */
    0,                              /* tp_iter */
    0,                              /* tp_iternext */
    0,                              /* tp_methods */
    sdl_Surface_members,            /* tp_members */
    0,                              /* tp_getset */
    0,                              /* tp_base */
    0,                              /* tp_dict */
    0,                              /* tp_descr_get */
    0,                              /* tp_descr_set */
    0,                              /* tp_dictoffset */
    (initproc)sdl_Surface_init,     /* tp_init */
    0,                              /* tp_alloc */
    PyType_GenericNew,              /* tp_new */
};
