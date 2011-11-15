
#ifndef PALETTE_H
#define PALETTE_H

#include <Python.h>
#include <pysdl.h>
#include <structmember.h>

typedef struct
{
    PyObject_HEAD
    int ncolors;
    PyObject *colors;
} sdl_Palette;

extern PyTypeObject sdl_PaletteType;

#endif /* PALETTE_H */
