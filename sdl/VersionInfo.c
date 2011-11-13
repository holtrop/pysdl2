
#include "VersionInfo.h"

static PyMemberDef sdl_VersionInfo_members[] = {
    {"major", T_INT, offsetof(sdl_VersionInfo, major), 0, "major"},
    {"minor", T_INT, offsetof(sdl_VersionInfo, minor), 0, "minor"},
    {"patch", T_INT, offsetof(sdl_VersionInfo, patch), 0, "patch"},
    {NULL}
};

PyTypeObject sdl_VersionInfoType = {
    PyObject_HEAD_INIT(NULL)
    0,                              /* ob_size */
    "sdl.VersionInfo",              /* tp_name */
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
    Py_TPFLAGS_DEFAULT,             /* tp_flags */
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
    0,                              /* tp_init */
    0,                              /* tp_alloc */
    sdl_VersionInfo_new,            /* tp_new */
};

PyObject *
sdl_VersionInfo_new(PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
    sdl_VersionInfo *self = (sdl_VersionInfo *) type->tp_alloc(type, 0);
    if (self != NULL)
    {
        printf("GOT HERE\n");
    }

    return (PyObject *) self;
}
