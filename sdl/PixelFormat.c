
#include "PixelFormat.h"
#include "Palette.h"

static PyMemberDef sdl_PixelFormat_members[] = {
    {"palette", T_OBJECT_EX, offsetof(sdl_PixelFormat, palette), 0,
        "palette"},
    {"BitsPerPixel", T_UBYTE, offsetof(sdl_PixelFormat, pf.BitsPerPixel), 0,
        "BitsPerPixel"},
    {"BytesPerPixel", T_UBYTE, offsetof(sdl_PixelFormat, pf.BytesPerPixel), 0,
        "BytesPerPixel"},
    {"Rloss", T_UBYTE, offsetof(sdl_PixelFormat, pf.Rloss), 0, "Rloss"},
    {"Gloss", T_UBYTE, offsetof(sdl_PixelFormat, pf.Gloss), 0, "Gloss"},
    {"Bloss", T_UBYTE, offsetof(sdl_PixelFormat, pf.Bloss), 0, "Bloss"},
    {"Aloss", T_UBYTE, offsetof(sdl_PixelFormat, pf.Aloss), 0, "Aloss"},
    {"Rshift", T_UBYTE, offsetof(sdl_PixelFormat, pf.Rshift), 0, "Rshift"},
    {"Gshift", T_UBYTE, offsetof(sdl_PixelFormat, pf.Gshift), 0, "Gshift"},
    {"Bshift", T_UBYTE, offsetof(sdl_PixelFormat, pf.Bshift), 0, "Bshift"},
    {"Ashift", T_UBYTE, offsetof(sdl_PixelFormat, pf.Ashift), 0, "Ashift"},
    {"Rmask", T_UINT, offsetof(sdl_PixelFormat, pf.Rmask), 0, "Rmask"},
    {"Gmask", T_UINT, offsetof(sdl_PixelFormat, pf.Gmask), 0, "Gmask"},
    {"Bmask", T_UINT, offsetof(sdl_PixelFormat, pf.Bmask), 0, "Bmask"},
    {"Amask", T_UINT, offsetof(sdl_PixelFormat, pf.Amask), 0, "Amask"},
    {"colorkey", T_UINT, offsetof(sdl_PixelFormat, pf.colorkey), 0, "colorkey"},
    {"alpha", T_UBYTE, offsetof(sdl_PixelFormat, pf.alpha), 0, "alpha"},
    {NULL}
};

static int
sdl_PixelFormat_init(sdl_PixelFormat *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = {
        "palette", "BitsPerPixel", "BytesPerPixel",
        "Rloss", "Gloss", "Bloss", "Aloss",
        "Rshift", "Gshift", "Bshift", "Ashift",
        "Rmask", "Gmask", "Bmask", "Amask",
        "colorkey",
        "alpha",
        NULL
    };
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "Oiiiiiiiiiiiiiiii", kwlist,
                &self->palette,
                &self->pf.BitsPerPixel, &self->pf.BytesPerPixel,
                &self->pf.Rloss, &self->pf.Gloss,
                &self->pf.Bloss, &self->pf.Aloss,
                &self->pf.Rshift, &self->pf.Gshift,
                &self->pf.Bshift, &self->pf.Ashift,
                &self->pf.Rmask, &self->pf.Gmask,
                &self->pf.Bmask, &self->pf.Amask,
                &self->pf.colorkey, &self->pf.alpha))
        return -1;
    Py_INCREF(self->palette);
    return 0;
}

PyTypeObject sdl_PixelFormatType = {
    PyObject_HEAD_INIT(NULL)
    0,                              /* ob_size */
    "SDL.PixelFormat",              /* tp_name */
    sizeof(sdl_PixelFormat),        /* tp_basicsize */
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
    "SDL PixelFormat Structure",    /* tp_doc */
    0,                              /* tp_traverse */
    0,                              /* tp_clear */
    0,                              /* tp_richcompare */
    0,                              /* tp_weaklistoffset */
    0,                              /* tp_iter */
    0,                              /* tp_iternext */
    0,                              /* tp_methods */
    sdl_PixelFormat_members,        /* tp_members */
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
    PyObject *args = Py_BuildValue("Oiiiiiiiiiiiiiiii",
            palette, pf->BitsPerPixel, pf->BytesPerPixel,
            pf->Rloss, pf->Gloss, pf->Bloss, pf->Aloss,
            pf->Rshift, pf->Gshift, pf->Bshift, pf->Ashift,
            pf->Rmask, pf->Gmask, pf->Bmask, pf->Amask,
            pf->colorkey, pf->alpha);
    PyObject *pixelformat = PyObject_CallObject(
            (PyObject *) &sdl_PixelFormatType,
            args);
    Py_DECREF(args);
    Py_DECREF(palette);
    return pixelformat;
}
