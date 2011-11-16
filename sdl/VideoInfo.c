
#include "VideoInfo.h"
#include "PixelFormat.h"

static PyMemberDef sdl_VideoInfo_members[] = {
    {"hw_available", T_BOOL, offsetof(sdl_VideoInfo, hw_available), 0,
        "hw_available"},
    {"wm_available", T_BOOL, offsetof(sdl_VideoInfo, wm_available), 0,
        "wm_available"},
    {"blit_hw", T_BOOL, offsetof(sdl_VideoInfo, blit_hw), 0,
        "blit_hw"},
    {"blit_hw_CC", T_BOOL, offsetof(sdl_VideoInfo, blit_hw_CC), 0,
        "blit_hw_CC"},
    {"blit_hw_A", T_BOOL, offsetof(sdl_VideoInfo, blit_hw_A), 0,
        "blit_hw_A"},
    {"blit_sw", T_BOOL, offsetof(sdl_VideoInfo, blit_sw), 0,
        "blit_sw"},
    {"blit_sw_CC", T_BOOL, offsetof(sdl_VideoInfo, blit_sw_CC), 0,
        "blit_sw_CC"},
    {"blit_sw_A", T_BOOL, offsetof(sdl_VideoInfo, blit_sw_A), 0,
        "blit_sw_A"},
    {"blit_fill", T_BOOL, offsetof(sdl_VideoInfo, blit_fill), 0, "blit_fill"},
    {"video_mem", T_UINT, offsetof(sdl_VideoInfo, video_mem), 0, "video_mem"},
    {"vfmt", T_OBJECT_EX, offsetof(sdl_VideoInfo, vfmt), 0, "vfmt"},
    {"current_w", T_INT, offsetof(sdl_VideoInfo, current_w), 0, "current_w"},
    {"current_h", T_INT, offsetof(sdl_VideoInfo, current_h), 0, "current_h"},
    {NULL}
};

static int
sdl_VideoInfo_init(sdl_VideoInfo *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = {
        "hw_available", "wm_available",
        "blit_hw", "blit_hw_CC", "blit_hw_A",
        "blit_sw", "blit_sw_CC", "blit_sw_A",
        "blit_fill", "video_mem", "vfmt",
        "current_w", "current_h",
        NULL
    };
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "iiiiiiiiiiOii", kwlist,
                &self->hw_available, &self->wm_available,
                &self->blit_hw, &self->blit_hw_CC, &self->blit_hw_A,
                &self->blit_sw, &self->blit_sw_CC, &self->blit_sw_A,
                &self->blit_fill, &self->video_mem, &self->vfmt,
                &self->current_w, &self->current_h))
        return -1;
    return 0;
}

PyTypeObject sdl_VideoInfoType = {
    PyObject_HEAD_INIT(NULL)
    0,                              /* ob_size */
    "SDL.VideoInfo",                /* tp_name */
    sizeof(sdl_VideoInfo),          /* tp_basicsize */
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
    "SDL VideoInfo Structure",      /* tp_doc */
    0,                              /* tp_traverse */
    0,                              /* tp_clear */
    0,                              /* tp_richcompare */
    0,                              /* tp_weaklistoffset */
    0,                              /* tp_iter */
    0,                              /* tp_iternext */
    0,                              /* tp_methods */
    sdl_VideoInfo_members,          /* tp_members */
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

PyObject *sdl_VideoInfo_from_SDL_VideoInfo(const SDL_VideoInfo *vi)
{
    if (vi == NULL)
        Py_RETURN_NONE;
    PyObject *vfmt = sdl_PixelFormat_from_SDL_PixelFormat(vi->vfmt);
    PyObject *args = Py_BuildValue("iiiiiiiiIOii",
            vi->hw_available, vi->wm_available,
            vi->blit_hw, vi->blit_hw_CC, vi->blit_hw_A,
            vi->blit_sw, vi->blit_sw_CC, vi->blit_sw_A,
            vi->video_mem, vfmt,
            vi->current_w, vi->current_h);
    PyObject *sdl_vi = PyObject_CallObject((PyObject *) &sdl_VideoInfoType,
            args);
    Py_DECREF(args);
    Py_DECREF(vfmt);
    return sdl_vi;
}
