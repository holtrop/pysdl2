
#include "Surface.h"
#include "PixelFormat.h"

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
        "flags", "format", "w", "h", "pitch", "pixels", "clip_rect", "refcount",
        NULL
    };
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "IOiiIIOi", kwlist,
                &self->surface.flags, &self->format,
                &self->surface.w, &self->surface.h,
                &self->surface.pitch, &self->surface.pixels,
                &self->clip_rect, &self->surface.refcount))
        return -1;
    self->pixels = (PyObject *) self;
    return 0;
}

static Py_ssize_t
sdl_Surface_len(sdl_Surface *self)
{
    return self->surface.w * self->surface.h;
}

PyObject *
sdl_Surface_getitem(sdl_Surface *self, Py_ssize_t i)
{
    if (i < 0 || i >= self->surface.w * self->surface.h)
        Py_RETURN_NONE;
    int bpp = ((sdl_PixelFormat *) self->format)->pf.BytesPerPixel;
    uint8_t *pxl = ((uint8_t *) self->surface.pixels) + i * bpp;
    uint32_t pval = 0;
    switch (bpp)
    {
        case 1:
            pval = *pxl;
            break;
        case 2:
            pval = *(uint16_t *) pxl;
            break;
        case 3:
            pval = *(uint32_t *) pxl;
#if __BYTE_ORDER == __LITTLE_ENDIAN
            pval &= 0xFFFFFF;
#else
            pval >>= 8;
#endif
            break;
        case 4:
            pval = *(uint32_t *) pxl;
            break;
    }
    return Py_BuildValue("I", pval);
}

int
sdl_Surface_setitem(sdl_Surface *self, Py_ssize_t i, PyObject *v)
{
    if (i < 0 || i >= self->surface.w * self->surface.h)
        return -1;
    if (!PyInt_Check(v))
        return -1;
    int bpp = ((sdl_PixelFormat *) self->format)->pf.BytesPerPixel;
    uint8_t *pxl = ((uint8_t *) self->surface.pixels) + i * bpp;
    uint32_t pval = PyInt_AsSsize_t(v);
    switch (bpp)
    {
        case 1:
            *pxl = pval;
            break;
        case 2:
            *(uint16_t *) pxl = pval;
            break;
        case 3:
#if __BYTE_ORDER == __LITTLE_ENDIAN
            pxl[0] = (uint8_t) (pval & 0xFF);
            pxl[1] = (uint8_t) ((pval >> 8) & 0xFF);
            pxl[2] = (uint8_t) ((pval >> 16) & 0xFF);
#else
            pxl[0] = (uint8_t) ((pval >> 16) & 0xFF);
            pxl[1] = (uint8_t) ((pval >> 8) & 0xFF);
            pxl[2] = (uint8_t) (pval & 0xFF);
#endif
            break;
        case 4:
            *(uint32_t *) pxl = pval;
            break;
    }
    return 0;
}

static PySequenceMethods sdl_Surface_seqitems = {
    (lenfunc)sdl_Surface_len,               /* sq_length */
    0,                                      /* sq_concat */
    0,                                      /* sq_repeat */
    (ssizeargfunc)sdl_Surface_getitem,      /* sq_item */
    0,                                      /* sq_slice */
    (ssizeobjargproc)sdl_Surface_setitem,   /* sq_ass_item */
    0,                                      /* sq_contains */
    0,                                      /* sq_inplace_concat */
    0,                                      /* sq_inplace_repeat */
};

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
    &sdl_Surface_seqitems,          /* tp_as_sequence */
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
