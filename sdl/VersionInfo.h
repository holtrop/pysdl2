
#ifndef VERSIONINFO_H
#define VERSIONINFO_H

#include <Python.h>
#include <pysdl.h>
#include <structmember.h>

typedef struct {
    PyObject_HEAD
    SDL_version ver;
} sdl_VersionInfo;

extern PyTypeObject sdl_VersionInfoType;

#endif /* VERSIONINFO_H */
