
#include "Surface.h"
#include "PixelFormat.h"
#include "Rect.h"

static int
sdl_Surface_init(sdl_Surface *self, PyObject *args, PyObject *kwargs)
{
    self->ok_to_dealloc = 0;
    PyErr_SetString(PyExc_TypeError,
            "Surface objects are not constructible");
    return -1;
}

static Py_ssize_t
sdl_Surface_len(sdl_Surface *self)
{
    return self->surface->w * self->surface->h;
}

PyObject *
sdl_Surface_getitem(sdl_Surface *self, Py_ssize_t i)
{
    if (i < 0 || i >= self->surface->w * self->surface->h)
    {
        PyErr_SetString(PyExc_ValueError, "pixel index out of range");
        return NULL;
    }
    int bpp = self->surface->format->BytesPerPixel;
    uint8_t *pxl = ((uint8_t *) self->surface->pixels) + i * bpp;
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
    if (i < 0 || i >= self->surface->w * self->surface->h)
    {
        PyErr_SetString(PyExc_ValueError, "pixel index out of range");
        return -1;
    }
    if (!PyInt_Check(v))
    {
        PyErr_SetString(PyExc_ValueError, "pixel value is not an integer");
        return -1;
    }
    int bpp = self->surface->format->BytesPerPixel;
    uint8_t *pxl = ((uint8_t *) self->surface->pixels) + i * bpp;
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

static int
sdl_Surface_setattro(sdl_Surface *self, PyObject *attr, PyObject *val)
{
    PyErr_SetString(PyExc_TypeError, "Surface types are immutable");
    return -1;
}

static PyObject *
sdl_Surface_getattro(sdl_Surface *self, PyObject *attr)
{
    if (!PyString_Check(attr))
    {
        PyErr_SetString(PyExc_AttributeError, "Invalid attribute name");
        return NULL;
    }
    const char *aname = PyString_AsString(attr);
    if (!strcmp(aname, "format"))
    {
        Py_INCREF(self->format);
        return self->format;
    }
    else if (!strcmp(aname, "pixels"))
    {
        Py_INCREF(self->pixels);
        return self->pixels;
    }
    else if (!strcmp(aname, "clip_rect"))
    {
        Py_INCREF(self->clip_rect);
        return self->clip_rect;
    }
    else if (!strcmp(aname, "flags"))
        return Py_BuildValue("I", self->surface->flags);
    else if (!strcmp(aname, "w"))
        return Py_BuildValue("i", self->surface->w);
    else if (!strcmp(aname, "h"))
        return Py_BuildValue("i", self->surface->h);
    else if (!strcmp(aname, "pitch"))
        return Py_BuildValue("I", self->surface->pitch);
    else if (!strcmp(aname, "refcount"))
        return Py_BuildValue("i", self->surface->refcount);
    PyErr_SetString(PyExc_AttributeError, "Invalid attribute");
    return NULL;
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

static void
sdl_Surface_dealloc(sdl_Surface *self)
{
    if (self->ok_to_dealloc)
    {
        Py_DECREF(self->format);
        Py_DECREF(self->clip_rect);
    }
    PyObject_Del(self);
}

PyTypeObject sdl_SurfaceType = {
    PyObject_HEAD_INIT(NULL)
    0,                              /* ob_size */
    "SDL.Surface",                  /* tp_name */
    sizeof(sdl_Surface),            /* tp_basicsize */
    0,                              /* tp_itemsize */
    (destructor)sdl_Surface_dealloc,/* tp_dealloc */
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
    (getattrofunc)sdl_Surface_getattro,/* tp_getattro */
    (setattrofunc)sdl_Surface_setattro,/* tp_setattro */
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
    0,                              /* tp_members */
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

void sdl_Surface_register_type(PyObject *module)
{
    if (PyType_Ready(&sdl_SurfaceType) < 0)
    {
        fprintf(stderr, "Error initializing sdl_SurfaceType\n");
        return;
    }

    Py_INCREF(&sdl_SurfaceType);
    PyModule_AddObject(module, "Surface", (PyObject *) &sdl_SurfaceType);
}

PyObject *sdl_Surface_from_SDL_Surface(SDL_Surface *surface)
{
    if (surface == NULL)
        Py_RETURN_NONE;
    PyObject *format = sdl_PixelFormat_from_SDL_PixelFormat(surface->format);
    PyObject *clip_rect = sdl_Rect_from_SDL_Rect(&surface->clip_rect);
    sdl_Surface *sdl_surface = PyObject_New(sdl_Surface, &sdl_SurfaceType);
    sdl_surface->surface = surface;
    sdl_surface->format = format;
    sdl_surface->pixels = (PyObject *) sdl_surface;
    sdl_surface->clip_rect = clip_rect;
    sdl_surface->ok_to_dealloc = 1;
    return (PyObject *) sdl_surface;
}

SDL_Surface *sdl_Surface_get_SDL_Surface(PyObject *sdl_surface)
{
    return ((sdl_Surface *) sdl_surface)->surface;
}

PyObject *sdl_Surface_get_type(void)
{
    return (PyObject *) &sdl_SurfaceType;
}
