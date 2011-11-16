
#ifndef VERSIONINFO_H
#define VERSIONINFO_H

#include <Python.h>
#include <pysdl.h>
#include <structmember.h>

typedef struct {
    PyObject_HEAD
    SDL_version ver;
} sdl_VersionInfo;

void sdl_VersionInfo_register_type(PyObject *module);
PyObject *sdl_VersionInfo_from_SDL_VersionInfo(const SDL_version *ver);

#endif /* VERSIONINFO_H */
