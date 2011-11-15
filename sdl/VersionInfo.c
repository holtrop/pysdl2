
#include "VersionInfo.h"

static PyMemberDef sdl_VersionInfo_members[] = {
    {"major", T_UBYTE, offsetof(sdl_VersionInfo, ver.major), 0, "major"},
    {"minor", T_UBYTE, offsetof(sdl_VersionInfo, ver.minor), 0, "minor"},
    {"patch", T_UBYTE, offsetof(sdl_VersionInfo, ver.patch), 0, "patch"},
    {NULL}
};

static int
sdl_VersionInfo_init(sdl_VersionInfo *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = {"major", "minor", "patch", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|iii", kwlist,
                &self->ver.major, &self->ver.minor, &self->ver.patch))
        return -1;
    return 0;
}

PyTypeObject sdl_VersionInfoType = {
    PyObject_HEAD_INIT(NULL)
    0,                              /* ob_size */
    "SDL.VersionInfo",              /* tp_name */
    sizeof(sdl_VersionInfo),        /* tp_basicsize */
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
    "SDL VersionInfo Structure",    /* tp_doc */
    0,                              /* tp_traverse */
    0,                              /* tp_clear */
    0,                              /* tp_richcompare */
    0,                              /* tp_weaklistoffset */
    0,                              /* tp_iter */
    0,                              /* tp_iternext */
    0,                              /* tp_methods */
    sdl_VersionInfo_members,        /* tp_members */
    0,                              /* tp_getset */
    0,                              /* tp_base */
    0,                              /* tp_dict */
    0,                              /* tp_descr_get */
    0,                              /* tp_descr_set */
    0,                              /* tp_dictoffset */
    (initproc)sdl_VersionInfo_init, /* tp_init */
    0,                              /* tp_alloc */
    0,                              /* tp_new */
};
