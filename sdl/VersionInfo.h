
#ifndef VERSIONINFO_H
#define VERSIONINFO_H

#include <Python.h>
#include <structmember.h>

typedef struct {
    PyObject_HEAD
    long major;
    long minor;
    long patch;
} sdl_VersionInfo;

extern PyTypeObject sdl_VersionInfoType;

PyObject *
sdl_VersionInfo_new(PyTypeObject *type, PyObject *args, PyObject *kwargs);

#endif /* VERSIONINFO_H */
