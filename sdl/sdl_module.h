
#ifndef SDL_MODULE_H
#define SDL_MODULE_H

#include <Python.h>

#define PYSDL_MODULE_NAME "SDL"

PyMODINIT_FUNC
initSDL(void);

#endif /* SDL_MODULE_H */
