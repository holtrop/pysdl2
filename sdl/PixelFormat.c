
#include "PixelFormat.h"
#include "Palette.h"

static int
sdl_PixelFormat_init(sdl_PixelFormat *self, PyObject *args, PyObject *kwargs)
{
    self->ok_to_dealloc = 0;
    PyErr_SetString(PyExc_TypeError,
            "PixelFormat objects are not constructible");
    return -1;
}

static int
sdl_PixelFormat_setattro(sdl_PixelFormat *self, PyObject *attr, PyObject *val)
{
    PyErr_SetString(PyExc_TypeError, "PixelFormat types are immutable");
    return -1;
}

static PyObject *
sdl_PixelFormat_getattro(sdl_PixelFormat *self, PyObject *attr)
{
    if (!PyString_Check(attr))
    {
        PyErr_SetString(PyExc_AttributeError, "Invalid attribute name");
        return NULL;
    }
    const char *aname = PyString_AsString(attr);
    if (!strcmp(aname, "palette"))
    {
        Py_INCREF(self->palette);
        return self->palette;
    }
    else if (!strcmp(aname, "BitsPerPixel"))
        return Py_BuildValue("I", self->pf->BitsPerPixel);
    else if (!strcmp(aname, "BytesPerPixel"))
        return Py_BuildValue("I", self->pf->BytesPerPixel);
    else if (!strcmp(aname, "Rloss"))
        return Py_BuildValue("I", self->pf->Rloss);
    else if (!strcmp(aname, "Gloss"))
        return Py_BuildValue("I", self->pf->Gloss);
    else if (!strcmp(aname, "Bloss"))
        return Py_BuildValue("I", self->pf->Bloss);
    else if (!strcmp(aname, "Aloss"))
        return Py_BuildValue("I", self->pf->Aloss);
    else if (!strcmp(aname, "Rshift"))
        return Py_BuildValue("I", self->pf->Rshift);
    else if (!strcmp(aname, "Gshift"))
        return Py_BuildValue("I", self->pf->Gshift);
    else if (!strcmp(aname, "Bshift"))
        return Py_BuildValue("I", self->pf->Bshift);
    else if (!strcmp(aname, "Ashift"))
        return Py_BuildValue("I", self->pf->Ashift);
    else if (!strcmp(aname, "Rmask"))
        return Py_BuildValue("I", self->pf->Rmask);
    else if (!strcmp(aname, "Gmask"))
        return Py_BuildValue("I", self->pf->Gmask);
    else if (!strcmp(aname, "Bmask"))
        return Py_BuildValue("I", self->pf->Bmask);
    else if (!strcmp(aname, "Amask"))
        return Py_BuildValue("I", self->pf->Amask);
    else if (!strcmp(aname, "colorkey"))
        return Py_BuildValue("I", self->pf->colorkey);
    else if (!strcmp(aname, "alpha"))
        return Py_BuildValue("I", self->pf->alpha);
    PyErr_SetString(PyExc_AttributeError, "Invalid attribute");
    return NULL;
}

static void
sdl_PixelFormat_dealloc(sdl_PixelFormat *self)
{
    if (self->ok_to_dealloc)
        Py_DECREF(self->palette);
    PyObject_Del(self);
}

PyTypeObject sdl_PixelFormatType = {
    PyObject_HEAD_INIT(NULL)
    0,                              /* ob_size */
    "SDL.PixelFormat",              /* tp_name */
    sizeof(sdl_PixelFormat),        /* tp_basicsize */
    0,                              /* tp_itemsize */
    (destructor)sdl_PixelFormat_dealloc,/* tp_dealloc */
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
    (getattrofunc)sdl_PixelFormat_getattro,/* tp_getattro */
    (setattrofunc)sdl_PixelFormat_setattro,/* tp_setattro */
    0,                              /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   /* tp_flags */
    "SDL PixelFormat Structure",    /* tp_doc */
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
    (initproc)sdl_PixelFormat_init, /* tp_init */
    0,                              /* tp_alloc */
    PyType_GenericNew,              /* tp_new */
};

void sdl_PixelFormat_register_type(PyObject *module)
{
    if (PyType_Ready(&sdl_PixelFormatType) < 0)
    {
        fprintf(stderr, "Error initializing sdl_PixelFormatType\n");
        return;
    }

    Py_INCREF(&sdl_PixelFormatType);
    PyModule_AddObject(module, "PixelFormat",
            (PyObject *) &sdl_PixelFormatType);
}

PyObject *sdl_PixelFormat_from_SDL_PixelFormat(SDL_PixelFormat *pf)
{
    if (pf == NULL)
        Py_RETURN_NONE;
    PyObject *palette = sdl_Palette_from_SDL_Palette(pf->palette);
    sdl_PixelFormat *pixelformat = PyObject_New(sdl_PixelFormat,
            &sdl_PixelFormatType);
    pixelformat->palette = palette;
    pixelformat->pf = pf;
    pixelformat->ok_to_dealloc = 1;
    return (PyObject *) pixelformat;
}

PyObject *sdl_PixelFormat_get_type(void)
{
    return (PyObject *) &sdl_PixelFormatType;
}

SDL_PixelFormat *sdl_PixelFormat_get_SDL_PixelFormat(PyObject *pf)
{
    return ((sdl_PixelFormat *) pf)->pf;
}
