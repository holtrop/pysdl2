
#ifndef VIDEOINFO_H
#define VIDEOINFO_H

#include <Python.h>
#include <pysdl.h>
#include <structmember.h>

typedef struct
{
    PyObject_HEAD
    const SDL_VideoInfo *vi;
    PyObject *vfmt;
    char ok_to_dealloc;
} sdl_VideoInfo;

void sdl_VideoInfo_register_type(PyObject *module);
PyObject *sdl_VideoInfo_from_SDL_VideoInfo(const SDL_VideoInfo *vi);

#endif /* VIDEOINFO_H */
