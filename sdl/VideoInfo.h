
#ifndef VIDEOINFO_H
#define VIDEOINFO_H

#include <Python.h>
#include <pysdl.h>
#include <structmember.h>

typedef struct
{
    PyObject_HEAD
    char hw_available;
    char wm_available;
    char blit_hw;
    char blit_hw_CC;
    char blit_hw_A;
    char blit_sw;
    char blit_sw_CC;
    char blit_sw_A;
    char blit_fill;
    uint32_t video_mem;
    PyObject *vfmt;
    int current_w;
    int current_h;
} sdl_VideoInfo;

void sdl_VideoInfo_register_type(PyObject *module);
PyObject *sdl_VideoInfo_from_SDL_VideoInfo(const SDL_VideoInfo *vi);

#endif /* VIDEOINFO_H */
