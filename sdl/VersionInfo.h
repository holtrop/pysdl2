
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

PyObject *sdl_VersionInfo_from_SDL_VersionInfo(const SDL_version *ver);

#endif /* VERSIONINFO_H */
