
#include "VideoInfo.h"
#include "PixelFormat.h"

static int
sdl_VideoInfo_init(sdl_VideoInfo *self, PyObject *args, PyObject *kwargs)
{
    self->ok_to_dealloc = 0;
    PyErr_SetString(PyExc_TypeError,
            "VideoInfo objects are not constructible");
    return -1;
}

static int
sdl_VideoInfo_setattro(sdl_VideoInfo *self, PyObject *attr, PyObject *val)
{
    PyErr_SetString(PyExc_TypeError, "VideoInfo types are immutable");
    return -1;
}

static PyObject *
sdl_VideoInfo_getattro(sdl_VideoInfo *self, PyObject *attr)
{
    if (!PyString_Check(attr))
    {
        PyErr_SetString(PyExc_AttributeError, "Invalid attribute name");
        return NULL;
    }
    const char *aname = PyString_AsString(attr);
    if (!strcmp(aname, "vfmt"))
    {
        Py_INCREF(self->vfmt);
        return self->vfmt;
    }
    else if (!strcmp(aname, "hw_available"))
        return Py_BuildValue("I", self->vi->hw_available);
    else if (!strcmp(aname, "wm_available"))
        return Py_BuildValue("I", self->vi->wm_available);
    else if (!strcmp(aname, "blit_hw"))
        return Py_BuildValue("I", self->vi->blit_hw);
    else if (!strcmp(aname, "blit_hw_CC"))
        return Py_BuildValue("I", self->vi->blit_hw_CC);
    else if (!strcmp(aname, "blit_hw_A"))
        return Py_BuildValue("I", self->vi->blit_hw_A);
    else if (!strcmp(aname, "blit_sw"))
        return Py_BuildValue("I", self->vi->blit_sw);
    else if (!strcmp(aname, "blit_sw_CC"))
        return Py_BuildValue("I", self->vi->blit_sw_CC);
    else if (!strcmp(aname, "blit_sw_A"))
        return Py_BuildValue("I", self->vi->blit_sw_A);
    else if (!strcmp(aname, "blit_fill"))
        return Py_BuildValue("I", self->vi->blit_fill);
    else if (!strcmp(aname, "video_mem"))
        return Py_BuildValue("I", self->vi->video_mem);
    else if (!strcmp(aname, "current_w"))
        return Py_BuildValue("i", self->vi->current_w);
    else if (!strcmp(aname, "current_h"))
        return Py_BuildValue("i", self->vi->current_h);
    PyErr_SetString(PyExc_AttributeError, "Invalid attribute");
    return NULL;
}

static void
sdl_VideoInfo_dealloc(sdl_VideoInfo *self)
{
    if (self->ok_to_dealloc)
        Py_DECREF(self->vfmt);
}

PyTypeObject sdl_VideoInfoType = {
    PyObject_HEAD_INIT(NULL)
    0,                              /* ob_size */
    "SDL.VideoInfo",                /* tp_name */
    sizeof(sdl_VideoInfo),          /* tp_basicsize */
    0,                              /* tp_itemsize */
    (destructor)sdl_VideoInfo_dealloc,/* tp_dealloc */
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
    (getattrofunc)sdl_VideoInfo_getattro,/* tp_getattro */
    (setattrofunc)sdl_VideoInfo_setattro,/* tp_setattro */
    0,                              /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   /* tp_flags */
    "SDL VideoInfo Structure",      /* tp_doc */
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
    (initproc)sdl_VideoInfo_init,   /* tp_init */
    0,                              /* tp_alloc */
    PyType_GenericNew,              /* tp_new */
};

void sdl_VideoInfo_register_type(PyObject *module)
{
    if (PyType_Ready(&sdl_VideoInfoType) < 0)
    {
        fprintf(stderr, "Error initializing sdl_VideoInfoType\n");
        return;
    }

    Py_INCREF(&sdl_VideoInfoType);
    PyModule_AddObject(module, "VideoInfo", (PyObject *) &sdl_VideoInfoType);
}

PyObject *sdl_VideoInfo_from_SDL_VideoInfo(const SDL_VideoInfo *vi)
{
    if (vi == NULL)
        Py_RETURN_NONE;
    PyObject *vfmt = sdl_PixelFormat_from_SDL_PixelFormat(vi->vfmt);
    sdl_VideoInfo *sdl_vi = PyObject_New(sdl_VideoInfo, &sdl_VideoInfoType);
    sdl_vi->vfmt = vfmt;
    sdl_vi->vi = vi;
    sdl_vi->ok_to_dealloc = 1;
    return (PyObject *) sdl_vi;
}
