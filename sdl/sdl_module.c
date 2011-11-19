
#include <Python.h>
#include "pysdl.h"
#include "sdl_module.h"
#include "VersionInfo.h"
#include "Color.h"
#include "Rect.h"
#include "Palette.h"
#include "PixelFormat.h"
#include "VideoInfo.h"
#include "Surface.h"
#include "keysym.h"

/**************************************************************************
 * SDL Core Functionality                                                 *
 *************************************************************************/
PYFUNC(ClearError, "Clear the current SDL error")
{
    SDL_ClearError();
    Py_RETURN_NONE;
}

PYFUNC(Error, "Set the SDL error message to one of several predefined strings.")
{
    int code;
    if (!PyArg_ParseTuple(args, "i", &code))
        return NULL;
    SDL_Error(code);
    Py_RETURN_NONE;
}

PYFUNC(GetError, "Get SDL error string")
{
    char *s = SDL_GetError();
    if (s == NULL)
        Py_RETURN_NONE;
    return Py_BuildValue("s", s);
}

PYFUNC(Init, "Initialize PySDL")
{
    Uint32 flags;
    if (!PyArg_ParseTuple(args, "I", &flags))
        return NULL;
    int rc = SDL_Init(flags);
    return Py_BuildValue("i", rc);
}

PYFUNC(InitSubSystem, "Initialize subsystems")
{
    Uint32 flags;
    if (!PyArg_ParseTuple(args, "I", &flags))
        return NULL;
    int rc = SDL_InitSubSystem(flags);
    return Py_BuildValue("i", rc);
}

PYFUNC(Linked_Version,
        "Retrieve the version of the dynamically linked SDL library")
{
    const SDL_version *ver = SDL_Linked_Version();
    return sdl_VersionInfo_from_SDL_VersionInfo(ver);
}

PYFUNC(Quit, "Uninitialize PySDL")
{
    SDL_Quit();
    Py_RETURN_NONE;
}

PYFUNC(QuitSubSystem, "Shut down a subsystem")
{
    Uint32 flags;
    if (!PyArg_ParseTuple(args, "I", &flags))
        return NULL;
    SDL_QuitSubSystem(flags);
    Py_RETURN_NONE;
}

PYFUNC(SetError, "Set the SDL error string")
{
    const char *s;
    if (!PyArg_ParseTuple(args, "s", &s))
        return NULL;
    SDL_SetError("%s", s);
    Py_RETURN_NONE;
}

PYFUNC(VERSION, "Retrieve compile-time version of the SDL library")
{
    SDL_version ver;
    SDL_VERSION(&ver);
    return sdl_VersionInfo_from_SDL_VersionInfo(&ver);
}

PYFUNC(WasInit, "Check which subsystems are initialized")
{
    Uint32 flags;
    if (!PyArg_ParseTuple(args, "I", &flags))
        return NULL;
    int rc = SDL_WasInit(flags);
    return Py_BuildValue("i", rc);
}

/**************************************************************************
 * SDL Video Functionality                                                *
 *************************************************************************/
PYFUNC(Flip, "swap SDL screen buffers")
{
    PyObject *surfo;
    if (!PyArg_ParseTuple(args, "O", &surfo))
        return NULL;
    if (!PyObject_IsInstance(surfo, sdl_Surface_get_type()))
    {
        PyErr_SetString(PyExc_ValueError, "Invalid parameter");
        return NULL;
    }
    SDL_Surface *ss = sdl_Surface_get_SDL_Surface(surfo);
    return Py_BuildValue("i", SDL_Flip(ss));
}

PYFUNC(FreeSurface, "delete an SDL.Surface")
{
    PyObject *surfo;
    if (!PyArg_ParseTuple(args, "O", &surfo))
        return NULL;
    if (!PyObject_IsInstance(surfo, sdl_Surface_get_type()))
    {
        PyErr_SetString(PyExc_ValueError, "Invalid parameter");
        return NULL;
    }
    SDL_Surface *ss = sdl_Surface_get_SDL_Surface(surfo);
    SDL_FreeSurface(ss);
    Py_RETURN_NONE;
}

PYFUNC(GetRGB, "get RGB values from a pixel in the specified pixel format")
{
    PyObject *formato;
    Uint32 pixel;
    if (!PyArg_ParseTuple(args, "IO", &pixel, &formato))
        return NULL;
    if (!PyObject_IsInstance(formato, sdl_PixelFormat_get_type()))
    {
        PyErr_SetString(PyExc_ValueError, "Invalid parameter");
        return NULL;
    }
    SDL_PixelFormat *format = sdl_PixelFormat_get_SDL_PixelFormat(formato);
    Uint8 r, g, b;
    SDL_GetRGB(pixel, format, &r, &g, &b);
    return Py_BuildValue("III", r, g, b);
}

PYFUNC(GetRGBA, "get RGBA values from a pixel in the specified pixel format")
{
    PyObject *formato;
    Uint32 pixel;
    if (!PyArg_ParseTuple(args, "IO", &pixel, &formato))
        return NULL;
    if (!PyObject_IsInstance(formato, sdl_PixelFormat_get_type()))
    {
        PyErr_SetString(PyExc_ValueError, "Invalid parameter");
        return NULL;
    }
    SDL_PixelFormat *format = sdl_PixelFormat_get_SDL_PixelFormat(formato);
    Uint8 r, g, b, a;
    SDL_GetRGBA(pixel, format, &r, &g, &b, &a);
    return Py_BuildValue("IIII", r, g, b, a);
}

PYFUNC(GetVideoInfo, "return information about current video hardware")
{
    const SDL_VideoInfo *vi = SDL_GetVideoInfo();
    return sdl_VideoInfo_from_SDL_VideoInfo(vi);
}

PYFUNC(GetVideoSurface, "return the current display surface")
{
    SDL_Surface *surf = SDL_GetVideoSurface();
    return sdl_Surface_from_SDL_Surface(surf);
}

PYFUNC(ListModes, "get a list of available screen dimensions for the "
        "given format and video flags")
{
    PyObject *formato;
    Uint32 flags;
    SDL_PixelFormat *format;
    if (!PyArg_ParseTuple(args, "OI", &formato, &flags))
        return NULL;
    if (PyObject_IsInstance(formato, sdl_PixelFormat_get_type()))
    {
        format = sdl_PixelFormat_get_SDL_PixelFormat(formato);
    }
    else if (formato == Py_None)
    {
        format = NULL;
    }
    else
    {
        PyErr_SetString(PyExc_ValueError, "Invalid parameter");
        return NULL;
    }
    SDL_Rect **modes = SDL_ListModes(format, flags);
    if (modes == NULL)
        Py_RETURN_NONE;
    if ((long) modes == -1)
        return PyInt_FromLong(-1);
    int len;
    for (len = 0; modes[len]; len++)
        ;
    PyObject *lst = PyList_New(len);
    for (int i = 0; i < len; i++)
    {
        PyObject *rect = sdl_Rect_from_SDL_Rect(modes[i]);
        PyList_SetItem(lst, i, rect);
    }
    return lst;
}

PYFUNC(MapRGB, "map an RGB color value to a pixel format")
{
    PyObject *formato;
    unsigned int r, g, b;
    if (!PyArg_ParseTuple(args, "OIII", &formato, &r, &g, &b))
        return NULL;
    if (!PyObject_IsInstance(formato, sdl_PixelFormat_get_type()))
    {
        PyErr_SetString(PyExc_ValueError, "Invalid parameter");
        return NULL;
    }
    SDL_PixelFormat *format = sdl_PixelFormat_get_SDL_PixelFormat(formato);
    return Py_BuildValue("I", SDL_MapRGB(format, r, g, b));
}

PYFUNC(MapRGBA, "map an RGBA color value to a pixel format")
{
    PyObject *formato;
    unsigned int r, g, b, a;
    if (!PyArg_ParseTuple(args, "OIIII", &formato, &r, &g, &b, &a))
        return NULL;
    if (!PyObject_IsInstance(formato, sdl_PixelFormat_get_type()))
    {
        PyErr_SetString(PyExc_ValueError, "Invalid parameter");
        return NULL;
    }
    SDL_PixelFormat *format = sdl_PixelFormat_get_SDL_PixelFormat(formato);
    return Py_BuildValue("I", SDL_MapRGBA(format, r, g, b, a));
}

PYFUNC(SetGamma, "set the color gamma function for the display")
{
    float r, g, b;
    if (!PyArg_ParseTuple(args, "fff", &r, &g, &b))
        return NULL;
    return Py_BuildValue("i", SDL_SetGamma(r, g, b));
}

PYFUNC(SetVideoMode, "set a video mode with the specified width, height, "
        "and bits-per-pixel values")
{
    int width, height, bpp;
    Uint32 flags;
    if (!PyArg_ParseTuple(args, "iiiI", &width, &height, &bpp, &flags))
        return NULL;
    SDL_Surface *surf = SDL_SetVideoMode(width, height, bpp, flags);
    return sdl_Surface_from_SDL_Surface(surf);
}

PYFUNC(UpdateRect, "update the given area of the screen")
{
    PyObject *surfo;
    Sint32 x, y, w, h;
    if (!PyArg_ParseTuple(args, "Oiiii", &surfo, &x, &y, &w, &h))
        return NULL;
    if (!PyObject_IsInstance(surfo, sdl_Surface_get_type()))
    {
        PyErr_SetString(PyExc_ValueError, "Invalid parameter");
        return NULL;
    }
    SDL_Surface *ss = sdl_Surface_get_SDL_Surface(surfo);
    SDL_UpdateRect(ss, x, y, w, h);
    Py_RETURN_NONE;
}

PYFUNC(UpdateRects, "update the given areas of the screen")
{
    PyObject *surfo;
    PyObject *rectso;
    if (!PyArg_ParseTuple(args, "OO", &surfo, &rectso))
        return NULL;
    if ( (!PyObject_IsInstance(surfo, sdl_Surface_get_type()))
            || (!PyList_Check(rectso)) )
    {
        PyErr_SetString(PyExc_ValueError, "Invalid parameter");
        return NULL;
    }
    SDL_Surface *ss = sdl_Surface_get_SDL_Surface(surfo);
    Py_ssize_t len = PyList_Size(rectso);
    SDL_Rect *rects = calloc(len, sizeof(SDL_Rect));
    for (Py_ssize_t i = 0; i < len; i++)
    {
        PyObject *o = PyList_GetItem(rectso, i);
        if (!PyObject_IsInstance(o, sdl_Rect_get_type()))
        {
            PyErr_SetString(PyExc_ValueError, "Invalid parameter in list");
            free(rects);
            return NULL;
        }
        memcpy(&rects[i], sdl_Rect_get_SDL_Rect(o), sizeof(SDL_Rect));
    }
    SDL_UpdateRects(ss, (int) len, rects);
    free(rects);
    Py_RETURN_NONE;
}

PYFUNC(VideoDriverName, "get the name of the video driver")
{
    const int len = 101;
    char namebuf[len];
    SDL_VideoDriverName(&namebuf[0], len);
    namebuf[len - 1] = '\0';
    return PyString_FromString(namebuf);
}

PYFUNC(VideoModeOK, "check to see if a video mode is supported")
{
    int width, height, bpp;
    Uint32 flags;
    if (!PyArg_ParseTuple(args, "iiiI", &width, &height, &bpp, &flags))
        return NULL;
    return Py_BuildValue("i", SDL_VideoModeOK(width, height, bpp, flags));
}

/**************************************************************************
 * SDL Video Functionality                                                *
 *************************************************************************/
PYFUNC(BUTTON, "return a mask for the specified mouse button")
{
    Uint32 button;
    if (!PyArg_ParseTuple(args, "I", &button))
        return NULL;
    return Py_BuildValue("I", SDL_BUTTON(button));
}

PYFUNC(EVENTMASK, "return a mask for the specified event")
{
    Uint32 evt;
    if (!PyArg_ParseTuple(args, "I", &evt))
        return NULL;
    return Py_BuildValue("I", SDL_EVENTMASK(evt));
}

PYFUNC(EnableKeyRepeat, "set keyboard repeat rate")
{
    int delay, interval;
    if (!PyArg_ParseTuple(args, "ii", &delay, &interval))
        return NULL;
    return Py_BuildValue("i", SDL_EnableKeyRepeat(delay, interval));
}

PYFUNC(EnableUNICODE, "enable UNICODE translation")
{
    int enable;
    if (!PyArg_ParseTuple(args, "i", &enable))
        return NULL;
    return Py_BuildValue("i", SDL_EnableUNICODE(enable));
}

PYFUNC(EventState, "set the state of processing certain events")
{
    unsigned int type;
    int state;
    if (!PyArg_ParseTuple(args, "Ii", &type, &state))
        return NULL;
    return Py_BuildValue("I", SDL_EventState(type, state));
}

PYFUNC(GetKeyName, "get the name of an SDL keysym")
{
    unsigned int key;
    if (!PyArg_ParseTuple(args, "I", &key))
        return NULL;
    return Py_BuildValue("s", SDL_GetKeyName(key));
}

PYFUNC(GetKeyState, "get a snapshot of the current keyboard state")
{
    int numkeys;
    Uint8 *keys = SDL_GetKeyState(&numkeys);
    PyObject *lst = PyList_New(numkeys);
    for (int i = 0; i < numkeys; i++)
    {
        PyList_SetItem(lst, i, Py_BuildValue("i", keys[i]));
    }
    return lst;
}

PYFUNC(GetModState, "get the state of modifier keys")
{
    return Py_BuildValue("I", SDL_GetModState());
}

PYFUNC(GetMouseState, "get the current state of the mouse")
{
    int x, y;
    Uint8 button_mask = SDL_GetMouseState(&x, &y);
    return Py_BuildValue("iiI", x, y, button_mask);
}

PYFUNC(GetRelativeMouseState, "get the relative change of the mouse")
{
    int x, y;
    Uint8 button_mask = SDL_GetRelativeMouseState(&x, &y);
    return Py_BuildValue("iiI", x, y, button_mask);
}

PYFUNC(PeepEvents,
        "check the event queue for events and optionally return them")
{
    int numevents;
    Uint32 action, mask;
    if (!PyArg_ParseTuple(args, "iII", &numevents, &action, &mask))
        return NULL;
    SDL_Event *events = malloc(numevents * sizeof(SDL_Event));
    int num_retrieved = SDL_PeepEvents(events, numevents, action, mask);
    if (num_retrieved < 0)
    {
        PyErr_SetString(PyExc_ValueError, "error in SDL_PeepEvents()");
        return NULL;
    }
    PyObject *eventso = PyList_New(num_retrieved);
    for (int i = 0; i < num_retrieved; i++)
    {
        SDL_Event *evt = malloc(sizeof(SDL_Event));
        memcpy(evt, &events[i], sizeof(SDL_Event));
        PyList_SetItem(eventso, i, sdl_Event_from_SDL_Event(evt));
    }
    free(events);
    return eventso;
}

PYFUNC(PollEvent, "poll for any currently pending event")
{
    SDL_Event *evt = malloc(sizeof(SDL_Event));
    int n = SDL_PollEvent(evt);
    if (n > 0)
    {
        /* transfer ownership of the evt pointer */
        return sdl_Event_from_SDL_Event(evt);
    }
    free(evt);
    Py_RETURN_NONE;
}

PYFUNC(PumpEvents,
        "pump the event loop, gathering events from the input devices")
{
    SDL_PumpEvents();
    Py_RETURN_NONE;
}

PYFUNC(PushEvent, "push an event onto the event queue")
{
    PyObject *evto;
    if (!PyArg_ParseTuple(args, "O", &evto))
        return NULL;
    if (!PyObject_IsInstance(evto, sdl_Event_get_type()))
    {
        PyErr_SetString(PyExc_ValueError, "Invalid parameter");
        return NULL;
    }
    return Py_BuildValue("i", SDL_PushEvent(sdl_Event_get_SDL_Event(evto)));
}

PYFUNC(SetModState, "set the current key modifier state")
{
    unsigned int modstate;
    if (!PyArg_ParseTuple(args, "I", &modstate))
        return NULL;
    SDL_SetModState(modstate);
    Py_RETURN_NONE;
}

PYFUNC(WaitEvent, "wait indefinitely for the next available event")
{
    SDL_Event *evt = malloc(sizeof(SDL_Event));
    int ret = SDL_WaitEvent(evt);
    if (ret == 0)
    {
        PyErr_SetString(PyExc_ValueError, "error in SDL_WaitEvent()");
        return NULL;
    }
    /* transfer ownership of the evt pointer */
    return sdl_Event_from_SDL_Event(evt);
}

/**************************************************************************
 * Python SDL Methods                                                     *
 *************************************************************************/
static PyMethodDef sdl_methods[] = {
    /* Core */
    PYFUNC_REF(ClearError),
    PYFUNC_REF(Error),
    PYFUNC_REF(GetError),
    PYFUNC_REF(Init),
    PYFUNC_REF(InitSubSystem),
    PYFUNC_REF(Linked_Version),
    PYFUNC_REF(Quit),
    PYFUNC_REF(QuitSubSystem),
    PYFUNC_REF(SetError),
    PYFUNC_REF(VERSION),
    PYFUNC_REF(WasInit),
    /* Video */
    PYFUNC_REF(Flip),
    PYFUNC_REF(FreeSurface),
    PYFUNC_REF(GetRGB),
    PYFUNC_REF(GetRGBA),
    PYFUNC_REF(GetVideoInfo),
    PYFUNC_REF(GetVideoSurface),
    PYFUNC_REF(ListModes),
    PYFUNC_REF(MapRGB),
    PYFUNC_REF(MapRGBA),
    PYFUNC_REF(SetGamma),
    PYFUNC_REF(SetVideoMode),
    PYFUNC_REF(UpdateRect),
    PYFUNC_REF(UpdateRects),
    PYFUNC_REF(VideoDriverName),
    PYFUNC_REF(VideoModeOK),
    /* Events */
    PYFUNC_REF(BUTTON),
    PYFUNC_REF(EVENTMASK),
    PYFUNC_REF(EnableKeyRepeat),
    PYFUNC_REF(EnableUNICODE),
    PYFUNC_REF(EventState),
    PYFUNC_REF(GetKeyName),
    PYFUNC_REF(GetKeyState),
    PYFUNC_REF(GetModState),
    PYFUNC_REF(GetMouseState),
    PYFUNC_REF(GetRelativeMouseState),
    PYFUNC_REF(PeepEvents),
    PYFUNC_REF(PollEvent),
    PYFUNC_REF(PumpEvents),
    PYFUNC_REF(PushEvent),
    PYFUNC_REF(SetModState),
    PYFUNC_REF(WaitEvent),
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initSDL(void)
{
    PyObject *m = Py_InitModule(PYSDL_MODULE_NAME, sdl_methods);
    if (m == NULL)
        return;

    MAKE_PY_CONST(m, "INIT_TIMER", SDL_INIT_TIMER);
    MAKE_PY_CONST(m, "INIT_VIDEO", SDL_INIT_VIDEO);
    MAKE_PY_CONST(m, "INIT_AUDIO", SDL_INIT_AUDIO);
    MAKE_PY_CONST(m, "INIT_CDROM", SDL_INIT_CDROM);
    MAKE_PY_CONST(m, "INIT_JOYSTICK", SDL_INIT_JOYSTICK);
    MAKE_PY_CONST(m, "INIT_EVERYTHING", SDL_INIT_EVERYTHING);
    MAKE_PY_CONST(m, "INIT_NOPARACHUTE", SDL_INIT_NOPARACHUTE);
    MAKE_PY_CONST(m, "INIT_EVENTTHREAD", SDL_INIT_EVENTTHREAD);

    MAKE_PY_CONST(m, "ENOMEM", SDL_ENOMEM);
    MAKE_PY_CONST(m, "EFREAD", SDL_EFREAD);
    MAKE_PY_CONST(m, "EFWRITE", SDL_EFWRITE);
    MAKE_PY_CONST(m, "EFSEEK", SDL_EFSEEK);
    MAKE_PY_CONST(m, "UNSUPPORTED", SDL_UNSUPPORTED);
    MAKE_PY_CONST(m, "LASTERROR", SDL_LASTERROR);

    MAKE_PY_CONST(m, "K_UNKNOWN", SDLK_UNKNOWN);
    MAKE_PY_CONST(m, "K_FIRST", SDLK_FIRST);
    MAKE_PY_CONST(m, "K_BACKSPACE", SDLK_BACKSPACE);
    MAKE_PY_CONST(m, "K_TAB", SDLK_TAB);
    MAKE_PY_CONST(m, "K_CLEAR", SDLK_CLEAR);
    MAKE_PY_CONST(m, "K_RETURN", SDLK_RETURN);
    MAKE_PY_CONST(m, "K_PAUSE", SDLK_PAUSE);
    MAKE_PY_CONST(m, "K_ESCAPE", SDLK_ESCAPE);
    MAKE_PY_CONST(m, "K_SPACE", SDLK_SPACE);
    MAKE_PY_CONST(m, "K_EXCLAIM", SDLK_EXCLAIM);
    MAKE_PY_CONST(m, "K_QUOTEDBL", SDLK_QUOTEDBL);
    MAKE_PY_CONST(m, "K_HASH", SDLK_HASH);
    MAKE_PY_CONST(m, "K_DOLLAR", SDLK_DOLLAR);
    MAKE_PY_CONST(m, "K_AMPERSAND", SDLK_AMPERSAND);
    MAKE_PY_CONST(m, "K_QUOTE", SDLK_QUOTE);
    MAKE_PY_CONST(m, "K_LEFTPAREN", SDLK_LEFTPAREN);
    MAKE_PY_CONST(m, "K_RIGHTPAREN", SDLK_RIGHTPAREN);
    MAKE_PY_CONST(m, "K_ASTERISK", SDLK_ASTERISK);
    MAKE_PY_CONST(m, "K_PLUS", SDLK_PLUS);
    MAKE_PY_CONST(m, "K_COMMA", SDLK_COMMA);
    MAKE_PY_CONST(m, "K_MINUS", SDLK_MINUS);
    MAKE_PY_CONST(m, "K_PERIOD", SDLK_PERIOD);
    MAKE_PY_CONST(m, "K_SLASH", SDLK_SLASH);
    MAKE_PY_CONST(m, "K_0", SDLK_0);
    MAKE_PY_CONST(m, "K_1", SDLK_1);
    MAKE_PY_CONST(m, "K_2", SDLK_2);
    MAKE_PY_CONST(m, "K_3", SDLK_3);
    MAKE_PY_CONST(m, "K_4", SDLK_4);
    MAKE_PY_CONST(m, "K_5", SDLK_5);
    MAKE_PY_CONST(m, "K_6", SDLK_6);
    MAKE_PY_CONST(m, "K_7", SDLK_7);
    MAKE_PY_CONST(m, "K_8", SDLK_8);
    MAKE_PY_CONST(m, "K_9", SDLK_9);
    MAKE_PY_CONST(m, "K_COLON", SDLK_COLON);
    MAKE_PY_CONST(m, "K_SEMICOLON", SDLK_SEMICOLON);
    MAKE_PY_CONST(m, "K_LESS", SDLK_LESS);
    MAKE_PY_CONST(m, "K_EQUALS", SDLK_EQUALS);
    MAKE_PY_CONST(m, "K_GREATER", SDLK_GREATER);
    MAKE_PY_CONST(m, "K_QUESTION", SDLK_QUESTION);
    MAKE_PY_CONST(m, "K_AT", SDLK_AT);

    MAKE_PY_CONST(m, "K_LEFTBRACKET", SDLK_LEFTBRACKET);
    MAKE_PY_CONST(m, "K_BACKSLASH", SDLK_BACKSLASH);
    MAKE_PY_CONST(m, "K_RIGHTBRACKET", SDLK_RIGHTBRACKET);
    MAKE_PY_CONST(m, "K_CARET", SDLK_CARET);
    MAKE_PY_CONST(m, "K_UNDERSCORE", SDLK_UNDERSCORE);
    MAKE_PY_CONST(m, "K_BACKQUOTE", SDLK_BACKQUOTE);
    MAKE_PY_CONST(m, "K_a", SDLK_a);
    MAKE_PY_CONST(m, "K_b", SDLK_b);
    MAKE_PY_CONST(m, "K_c", SDLK_c);
    MAKE_PY_CONST(m, "K_d", SDLK_d);
    MAKE_PY_CONST(m, "K_e", SDLK_e);
    MAKE_PY_CONST(m, "K_f", SDLK_f);
    MAKE_PY_CONST(m, "K_g", SDLK_g);
    MAKE_PY_CONST(m, "K_h", SDLK_h);
    MAKE_PY_CONST(m, "K_i", SDLK_i);
    MAKE_PY_CONST(m, "K_j", SDLK_j);
    MAKE_PY_CONST(m, "K_k", SDLK_k);
    MAKE_PY_CONST(m, "K_l", SDLK_l);
    MAKE_PY_CONST(m, "K_m", SDLK_m);
    MAKE_PY_CONST(m, "K_n", SDLK_n);
    MAKE_PY_CONST(m, "K_o", SDLK_o);
    MAKE_PY_CONST(m, "K_p", SDLK_p);
    MAKE_PY_CONST(m, "K_q", SDLK_q);
    MAKE_PY_CONST(m, "K_r", SDLK_r);
    MAKE_PY_CONST(m, "K_s", SDLK_s);
    MAKE_PY_CONST(m, "K_t", SDLK_t);
    MAKE_PY_CONST(m, "K_u", SDLK_u);
    MAKE_PY_CONST(m, "K_v", SDLK_v);
    MAKE_PY_CONST(m, "K_w", SDLK_w);
    MAKE_PY_CONST(m, "K_x", SDLK_x);
    MAKE_PY_CONST(m, "K_y", SDLK_y);
    MAKE_PY_CONST(m, "K_z", SDLK_z);
    MAKE_PY_CONST(m, "K_DELETE", SDLK_DELETE);

    MAKE_PY_CONST(m, "K_WORLD_0", SDLK_WORLD_0);
    MAKE_PY_CONST(m, "K_WORLD_1", SDLK_WORLD_1);
    MAKE_PY_CONST(m, "K_WORLD_2", SDLK_WORLD_2);
    MAKE_PY_CONST(m, "K_WORLD_3", SDLK_WORLD_3);
    MAKE_PY_CONST(m, "K_WORLD_4", SDLK_WORLD_4);
    MAKE_PY_CONST(m, "K_WORLD_5", SDLK_WORLD_5);
    MAKE_PY_CONST(m, "K_WORLD_6", SDLK_WORLD_6);
    MAKE_PY_CONST(m, "K_WORLD_7", SDLK_WORLD_7);
    MAKE_PY_CONST(m, "K_WORLD_8", SDLK_WORLD_8);
    MAKE_PY_CONST(m, "K_WORLD_9", SDLK_WORLD_9);
    MAKE_PY_CONST(m, "K_WORLD_10", SDLK_WORLD_10);
    MAKE_PY_CONST(m, "K_WORLD_11", SDLK_WORLD_11);
    MAKE_PY_CONST(m, "K_WORLD_12", SDLK_WORLD_12);
    MAKE_PY_CONST(m, "K_WORLD_13", SDLK_WORLD_13);
    MAKE_PY_CONST(m, "K_WORLD_14", SDLK_WORLD_14);
    MAKE_PY_CONST(m, "K_WORLD_15", SDLK_WORLD_15);
    MAKE_PY_CONST(m, "K_WORLD_16", SDLK_WORLD_16);
    MAKE_PY_CONST(m, "K_WORLD_17", SDLK_WORLD_17);
    MAKE_PY_CONST(m, "K_WORLD_18", SDLK_WORLD_18);
    MAKE_PY_CONST(m, "K_WORLD_19", SDLK_WORLD_19);
    MAKE_PY_CONST(m, "K_WORLD_20", SDLK_WORLD_20);
    MAKE_PY_CONST(m, "K_WORLD_21", SDLK_WORLD_21);
    MAKE_PY_CONST(m, "K_WORLD_22", SDLK_WORLD_22);
    MAKE_PY_CONST(m, "K_WORLD_23", SDLK_WORLD_23);
    MAKE_PY_CONST(m, "K_WORLD_24", SDLK_WORLD_24);
    MAKE_PY_CONST(m, "K_WORLD_25", SDLK_WORLD_25);
    MAKE_PY_CONST(m, "K_WORLD_26", SDLK_WORLD_26);
    MAKE_PY_CONST(m, "K_WORLD_27", SDLK_WORLD_27);
    MAKE_PY_CONST(m, "K_WORLD_28", SDLK_WORLD_28);
    MAKE_PY_CONST(m, "K_WORLD_29", SDLK_WORLD_29);
    MAKE_PY_CONST(m, "K_WORLD_30", SDLK_WORLD_30);
    MAKE_PY_CONST(m, "K_WORLD_31", SDLK_WORLD_31);
    MAKE_PY_CONST(m, "K_WORLD_32", SDLK_WORLD_32);
    MAKE_PY_CONST(m, "K_WORLD_33", SDLK_WORLD_33);
    MAKE_PY_CONST(m, "K_WORLD_34", SDLK_WORLD_34);
    MAKE_PY_CONST(m, "K_WORLD_35", SDLK_WORLD_35);
    MAKE_PY_CONST(m, "K_WORLD_36", SDLK_WORLD_36);
    MAKE_PY_CONST(m, "K_WORLD_37", SDLK_WORLD_37);
    MAKE_PY_CONST(m, "K_WORLD_38", SDLK_WORLD_38);
    MAKE_PY_CONST(m, "K_WORLD_39", SDLK_WORLD_39);
    MAKE_PY_CONST(m, "K_WORLD_40", SDLK_WORLD_40);
    MAKE_PY_CONST(m, "K_WORLD_41", SDLK_WORLD_41);
    MAKE_PY_CONST(m, "K_WORLD_42", SDLK_WORLD_42);
    MAKE_PY_CONST(m, "K_WORLD_43", SDLK_WORLD_43);
    MAKE_PY_CONST(m, "K_WORLD_44", SDLK_WORLD_44);
    MAKE_PY_CONST(m, "K_WORLD_45", SDLK_WORLD_45);
    MAKE_PY_CONST(m, "K_WORLD_46", SDLK_WORLD_46);
    MAKE_PY_CONST(m, "K_WORLD_47", SDLK_WORLD_47);
    MAKE_PY_CONST(m, "K_WORLD_48", SDLK_WORLD_48);
    MAKE_PY_CONST(m, "K_WORLD_49", SDLK_WORLD_49);
    MAKE_PY_CONST(m, "K_WORLD_50", SDLK_WORLD_50);
    MAKE_PY_CONST(m, "K_WORLD_51", SDLK_WORLD_51);
    MAKE_PY_CONST(m, "K_WORLD_52", SDLK_WORLD_52);
    MAKE_PY_CONST(m, "K_WORLD_53", SDLK_WORLD_53);
    MAKE_PY_CONST(m, "K_WORLD_54", SDLK_WORLD_54);
    MAKE_PY_CONST(m, "K_WORLD_55", SDLK_WORLD_55);
    MAKE_PY_CONST(m, "K_WORLD_56", SDLK_WORLD_56);
    MAKE_PY_CONST(m, "K_WORLD_57", SDLK_WORLD_57);
    MAKE_PY_CONST(m, "K_WORLD_58", SDLK_WORLD_58);
    MAKE_PY_CONST(m, "K_WORLD_59", SDLK_WORLD_59);
    MAKE_PY_CONST(m, "K_WORLD_60", SDLK_WORLD_60);
    MAKE_PY_CONST(m, "K_WORLD_61", SDLK_WORLD_61);
    MAKE_PY_CONST(m, "K_WORLD_62", SDLK_WORLD_62);
    MAKE_PY_CONST(m, "K_WORLD_63", SDLK_WORLD_63);
    MAKE_PY_CONST(m, "K_WORLD_64", SDLK_WORLD_64);
    MAKE_PY_CONST(m, "K_WORLD_65", SDLK_WORLD_65);
    MAKE_PY_CONST(m, "K_WORLD_66", SDLK_WORLD_66);
    MAKE_PY_CONST(m, "K_WORLD_67", SDLK_WORLD_67);
    MAKE_PY_CONST(m, "K_WORLD_68", SDLK_WORLD_68);
    MAKE_PY_CONST(m, "K_WORLD_69", SDLK_WORLD_69);
    MAKE_PY_CONST(m, "K_WORLD_70", SDLK_WORLD_70);
    MAKE_PY_CONST(m, "K_WORLD_71", SDLK_WORLD_71);
    MAKE_PY_CONST(m, "K_WORLD_72", SDLK_WORLD_72);
    MAKE_PY_CONST(m, "K_WORLD_73", SDLK_WORLD_73);
    MAKE_PY_CONST(m, "K_WORLD_74", SDLK_WORLD_74);
    MAKE_PY_CONST(m, "K_WORLD_75", SDLK_WORLD_75);
    MAKE_PY_CONST(m, "K_WORLD_76", SDLK_WORLD_76);
    MAKE_PY_CONST(m, "K_WORLD_77", SDLK_WORLD_77);
    MAKE_PY_CONST(m, "K_WORLD_78", SDLK_WORLD_78);
    MAKE_PY_CONST(m, "K_WORLD_79", SDLK_WORLD_79);
    MAKE_PY_CONST(m, "K_WORLD_80", SDLK_WORLD_80);
    MAKE_PY_CONST(m, "K_WORLD_81", SDLK_WORLD_81);
    MAKE_PY_CONST(m, "K_WORLD_82", SDLK_WORLD_82);
    MAKE_PY_CONST(m, "K_WORLD_83", SDLK_WORLD_83);
    MAKE_PY_CONST(m, "K_WORLD_84", SDLK_WORLD_84);
    MAKE_PY_CONST(m, "K_WORLD_85", SDLK_WORLD_85);
    MAKE_PY_CONST(m, "K_WORLD_86", SDLK_WORLD_86);
    MAKE_PY_CONST(m, "K_WORLD_87", SDLK_WORLD_87);
    MAKE_PY_CONST(m, "K_WORLD_88", SDLK_WORLD_88);
    MAKE_PY_CONST(m, "K_WORLD_89", SDLK_WORLD_89);
    MAKE_PY_CONST(m, "K_WORLD_90", SDLK_WORLD_90);
    MAKE_PY_CONST(m, "K_WORLD_91", SDLK_WORLD_91);
    MAKE_PY_CONST(m, "K_WORLD_92", SDLK_WORLD_92);
    MAKE_PY_CONST(m, "K_WORLD_93", SDLK_WORLD_93);
    MAKE_PY_CONST(m, "K_WORLD_94", SDLK_WORLD_94);
    MAKE_PY_CONST(m, "K_WORLD_95", SDLK_WORLD_95);

    MAKE_PY_CONST(m, "K_KP0", SDLK_KP0);
    MAKE_PY_CONST(m, "K_KP1", SDLK_KP1);
    MAKE_PY_CONST(m, "K_KP2", SDLK_KP2);
    MAKE_PY_CONST(m, "K_KP3", SDLK_KP3);
    MAKE_PY_CONST(m, "K_KP4", SDLK_KP4);
    MAKE_PY_CONST(m, "K_KP5", SDLK_KP5);
    MAKE_PY_CONST(m, "K_KP6", SDLK_KP6);
    MAKE_PY_CONST(m, "K_KP7", SDLK_KP7);
    MAKE_PY_CONST(m, "K_KP8", SDLK_KP8);
    MAKE_PY_CONST(m, "K_KP9", SDLK_KP9);
    MAKE_PY_CONST(m, "K_KP_PERIOD", SDLK_KP_PERIOD);
    MAKE_PY_CONST(m, "K_KP_DIVIDE", SDLK_KP_DIVIDE);
    MAKE_PY_CONST(m, "K_KP_MULTIPLY", SDLK_KP_MULTIPLY);
    MAKE_PY_CONST(m, "K_KP_MINUS", SDLK_KP_MINUS);
    MAKE_PY_CONST(m, "K_KP_PLUS", SDLK_KP_PLUS);
    MAKE_PY_CONST(m, "K_KP_ENTER", SDLK_KP_ENTER);
    MAKE_PY_CONST(m, "K_KP_EQUALS", SDLK_KP_EQUALS);

    MAKE_PY_CONST(m, "K_UP", SDLK_UP);
    MAKE_PY_CONST(m, "K_DOWN", SDLK_DOWN);
    MAKE_PY_CONST(m, "K_RIGHT", SDLK_RIGHT);
    MAKE_PY_CONST(m, "K_LEFT", SDLK_LEFT);
    MAKE_PY_CONST(m, "K_INSERT", SDLK_INSERT);
    MAKE_PY_CONST(m, "K_HOME", SDLK_HOME);
    MAKE_PY_CONST(m, "K_END", SDLK_END);
    MAKE_PY_CONST(m, "K_PAGEUP", SDLK_PAGEUP);
    MAKE_PY_CONST(m, "K_PAGEDOWN", SDLK_PAGEDOWN);

    MAKE_PY_CONST(m, "K_F1", SDLK_F1);
    MAKE_PY_CONST(m, "K_F2", SDLK_F2);
    MAKE_PY_CONST(m, "K_F3", SDLK_F3);
    MAKE_PY_CONST(m, "K_F4", SDLK_F4);
    MAKE_PY_CONST(m, "K_F5", SDLK_F5);
    MAKE_PY_CONST(m, "K_F6", SDLK_F6);
    MAKE_PY_CONST(m, "K_F7", SDLK_F7);
    MAKE_PY_CONST(m, "K_F8", SDLK_F8);
    MAKE_PY_CONST(m, "K_F9", SDLK_F9);
    MAKE_PY_CONST(m, "K_F10", SDLK_F10);
    MAKE_PY_CONST(m, "K_F11", SDLK_F11);
    MAKE_PY_CONST(m, "K_F12", SDLK_F12);
    MAKE_PY_CONST(m, "K_F13", SDLK_F13);
    MAKE_PY_CONST(m, "K_F14", SDLK_F14);
    MAKE_PY_CONST(m, "K_F15", SDLK_F15);

    MAKE_PY_CONST(m, "K_NUMLOCK", SDLK_NUMLOCK);
    MAKE_PY_CONST(m, "K_CAPSLOCK", SDLK_CAPSLOCK);
    MAKE_PY_CONST(m, "K_SCROLLOCK", SDLK_SCROLLOCK);
    MAKE_PY_CONST(m, "K_RSHIFT", SDLK_RSHIFT);
    MAKE_PY_CONST(m, "K_LSHIFT", SDLK_LSHIFT);
    MAKE_PY_CONST(m, "K_RCTRL", SDLK_RCTRL);
    MAKE_PY_CONST(m, "K_LCTRL", SDLK_LCTRL);
    MAKE_PY_CONST(m, "K_RALT", SDLK_RALT);
    MAKE_PY_CONST(m, "K_LALT", SDLK_LALT);
    MAKE_PY_CONST(m, "K_RMETA", SDLK_RMETA);
    MAKE_PY_CONST(m, "K_LMETA", SDLK_LMETA);
    MAKE_PY_CONST(m, "K_LSUPER", SDLK_LSUPER);
    MAKE_PY_CONST(m, "K_RSUPER", SDLK_RSUPER);
    MAKE_PY_CONST(m, "K_MODE", SDLK_MODE);
    MAKE_PY_CONST(m, "K_COMPOSE", SDLK_COMPOSE);

    MAKE_PY_CONST(m, "K_HELP", SDLK_HELP);
    MAKE_PY_CONST(m, "K_PRINT", SDLK_PRINT);
    MAKE_PY_CONST(m, "K_SYSREQ", SDLK_SYSREQ);
    MAKE_PY_CONST(m, "K_BREAK", SDLK_BREAK);
    MAKE_PY_CONST(m, "K_MENU", SDLK_MENU);
    MAKE_PY_CONST(m, "K_POWER", SDLK_POWER);
    MAKE_PY_CONST(m, "K_EURO", SDLK_EURO);
    MAKE_PY_CONST(m, "K_UNDO", SDLK_UNDO);

    MAKE_PY_CONST(m, "K_LAST", SDLK_LAST);

    MAKE_PY_CONST(m, "KMOD_NONE", KMOD_NONE);
    MAKE_PY_CONST(m, "KMOD_LSHIFT", KMOD_LSHIFT);
    MAKE_PY_CONST(m, "KMOD_RSHIFT", KMOD_RSHIFT);
    MAKE_PY_CONST(m, "KMOD_LCTRL", KMOD_LCTRL);
    MAKE_PY_CONST(m, "KMOD_RCTRL", KMOD_RCTRL);
    MAKE_PY_CONST(m, "KMOD_LALT", KMOD_LALT);
    MAKE_PY_CONST(m, "KMOD_RALT", KMOD_RALT);
    MAKE_PY_CONST(m, "KMOD_LMETA", KMOD_LMETA);
    MAKE_PY_CONST(m, "KMOD_RMETA", KMOD_RMETA);
    MAKE_PY_CONST(m, "KMOD_NUM", KMOD_NUM);
    MAKE_PY_CONST(m, "KMOD_CAPS", KMOD_CAPS);
    MAKE_PY_CONST(m, "KMOD_MODE", KMOD_MODE);
    MAKE_PY_CONST(m, "KMOD_RESERVED", KMOD_RESERVED);

    MAKE_PY_CONST(m, "KMOD_CTRL", KMOD_CTRL);
    MAKE_PY_CONST(m, "KMOD_SHIFT", KMOD_SHIFT);
    MAKE_PY_CONST(m, "KMOD_ALT", KMOD_ALT);
    MAKE_PY_CONST(m, "KMOD_META", KMOD_META);

    MAKE_PY_CONST(m, "ANYFORMAT", SDL_ANYFORMAT);
    MAKE_PY_CONST(m, "ASYNCBLIT", SDL_ASYNCBLIT);
    MAKE_PY_CONST(m, "DOUBLEBUF", SDL_DOUBLEBUF);
    MAKE_PY_CONST(m, "HWACCEL", SDL_HWACCEL);
    MAKE_PY_CONST(m, "HWPALETTE", SDL_HWPALETTE);
    MAKE_PY_CONST(m, "HWSURFACE", SDL_HWSURFACE);
    MAKE_PY_CONST(m, "FULLSCREEN", SDL_FULLSCREEN);
    MAKE_PY_CONST(m, "OPENGL", SDL_OPENGL);
    MAKE_PY_CONST(m, "OPENGLBLIT", SDL_OPENGLBLIT);
    MAKE_PY_CONST(m, "RESIZABLE", SDL_RESIZABLE);
    MAKE_PY_CONST(m, "RLEACCEL", SDL_RLEACCEL);
    MAKE_PY_CONST(m, "SRCALPHA", SDL_SRCALPHA);
    MAKE_PY_CONST(m, "SRCCOLORKEY", SDL_SRCCOLORKEY);
    MAKE_PY_CONST(m, "SWSURFACE", SDL_SWSURFACE);
    MAKE_PY_CONST(m, "PREALLOC", SDL_PREALLOC);

    MAKE_PY_CONST(m, "GL_RED_SIZE", SDL_GL_RED_SIZE);
    MAKE_PY_CONST(m, "GL_GREEN_SIZE", SDL_GL_GREEN_SIZE);
    MAKE_PY_CONST(m, "GL_BLUE_SIZE", SDL_GL_BLUE_SIZE);
    MAKE_PY_CONST(m, "GL_ALPHA_SIZE", SDL_GL_ALPHA_SIZE);
    MAKE_PY_CONST(m, "GL_BUFFER_SIZE", SDL_GL_BUFFER_SIZE);
    MAKE_PY_CONST(m, "GL_DOUBLEBUFFER", SDL_GL_DOUBLEBUFFER);
    MAKE_PY_CONST(m, "GL_DEPTH_SIZE", SDL_GL_DEPTH_SIZE);
    MAKE_PY_CONST(m, "GL_STENCIL_SIZE", SDL_GL_STENCIL_SIZE);
    MAKE_PY_CONST(m, "GL_ACCUM_RED_SIZE", SDL_GL_ACCUM_RED_SIZE);
    MAKE_PY_CONST(m, "GL_ACCUM_GREEN_SIZE", SDL_GL_ACCUM_GREEN_SIZE);
    MAKE_PY_CONST(m, "GL_ACCUM_BLUE_SIZE", SDL_GL_ACCUM_BLUE_SIZE);
    MAKE_PY_CONST(m, "GL_ACCUM_ALPHA_SIZE", SDL_GL_ACCUM_ALPHA_SIZE);
    MAKE_PY_CONST(m, "GL_STEREO", SDL_GL_STEREO);
    MAKE_PY_CONST(m, "GL_MULTISAMPLEBUFFERS", SDL_GL_MULTISAMPLEBUFFERS);
    MAKE_PY_CONST(m, "GL_MULTISAMPLESAMPLES", SDL_GL_MULTISAMPLESAMPLES);
    MAKE_PY_CONST(m, "GL_SWAP_CONTROL", SDL_GL_SWAP_CONTROL);
    MAKE_PY_CONST(m, "GL_ACCELERATED_VISUAL", SDL_GL_ACCELERATED_VISUAL);

    MAKE_PY_CONST(m, "NOEVENT", SDL_NOEVENT);
    MAKE_PY_CONST(m, "ACTIVEEVENT", SDL_ACTIVEEVENT);
    MAKE_PY_CONST(m, "KEYDOWN", SDL_KEYDOWN);
    MAKE_PY_CONST(m, "KEYUP", SDL_KEYUP);
    MAKE_PY_CONST(m, "MOUSEMOTION", SDL_MOUSEMOTION);
    MAKE_PY_CONST(m, "MOUSEBUTTONDOWN", SDL_MOUSEBUTTONDOWN);
    MAKE_PY_CONST(m, "MOUSEBUTTONUP", SDL_MOUSEBUTTONUP);
    MAKE_PY_CONST(m, "JOYAXISMOTION", SDL_JOYAXISMOTION);
    MAKE_PY_CONST(m, "JOYBALLMOTION", SDL_JOYBALLMOTION);
    MAKE_PY_CONST(m, "JOYHATMOTION", SDL_JOYHATMOTION);
    MAKE_PY_CONST(m, "JOYBUTTONDOWN", SDL_JOYBUTTONDOWN);
    MAKE_PY_CONST(m, "JOYBUTTONUP", SDL_JOYBUTTONUP);
    MAKE_PY_CONST(m, "VIDEORESIZE", SDL_VIDEORESIZE);
    MAKE_PY_CONST(m, "VIDEOEXPOSE", SDL_VIDEOEXPOSE);
    MAKE_PY_CONST(m, "QUIT", SDL_QUIT);
    MAKE_PY_CONST(m, "USEREVENT", SDL_USEREVENT);
    MAKE_PY_CONST(m, "SYSWMEVENT", SDL_SYSWMEVENT);
    MAKE_PY_CONST(m, "ALLEVENTS", SDL_ALLEVENTS);

    MAKE_PY_CONST(m, "ACTIVEEVENTMASK", SDL_ACTIVEEVENTMASK);
    MAKE_PY_CONST(m, "KEYDOWNMASK", SDL_KEYDOWNMASK);
    MAKE_PY_CONST(m, "KEYUPMASK", SDL_KEYUPMASK);
    MAKE_PY_CONST(m, "KEYEVENTMASK", SDL_KEYEVENTMASK);
    MAKE_PY_CONST(m, "MOUSEMOTIONMASK", SDL_MOUSEMOTIONMASK);
    MAKE_PY_CONST(m, "MOUSEBUTTONDOWNMASK", SDL_MOUSEBUTTONDOWNMASK);
    MAKE_PY_CONST(m, "MOUSEBUTTONUPMASK", SDL_MOUSEBUTTONUPMASK);
    MAKE_PY_CONST(m, "MOUSEEVENTMASK", SDL_MOUSEEVENTMASK);
    MAKE_PY_CONST(m, "JOYAXISMOTIONMASK", SDL_JOYAXISMOTIONMASK);
    MAKE_PY_CONST(m, "JOYBALLMOTIONMASK", SDL_JOYBALLMOTIONMASK);
    MAKE_PY_CONST(m, "JOYHATMOTIONMASK", SDL_JOYHATMOTIONMASK);
    MAKE_PY_CONST(m, "JOYBUTTONDOWNMASK", SDL_JOYBUTTONDOWNMASK);
    MAKE_PY_CONST(m, "JOYBUTTONUPMASK", SDL_JOYBUTTONUPMASK);
    MAKE_PY_CONST(m, "JOYEVENTMASK", SDL_JOYEVENTMASK);
    MAKE_PY_CONST(m, "VIDEORESIZEMASK", SDL_VIDEORESIZEMASK);
    MAKE_PY_CONST(m, "VIDEOEXPOSEMASK", SDL_VIDEOEXPOSEMASK);
    MAKE_PY_CONST(m, "QUITMASK", SDL_QUITMASK);
    MAKE_PY_CONST(m, "SYSWMEVENTMASK", SDL_SYSWMEVENTMASK);

    MAKE_PY_CONST(m, "ADDEVENT", SDL_ADDEVENT);
    MAKE_PY_CONST(m, "PEEKEVENT", SDL_PEEKEVENT);
    MAKE_PY_CONST(m, "GETEVENT", SDL_GETEVENT);

    MAKE_PY_CONST(m, "IGNORE", SDL_IGNORE);
    MAKE_PY_CONST(m, "ENABLE", SDL_ENABLE);
    MAKE_PY_CONST(m, "QUERY", SDL_QUERY);

    MAKE_PY_CONST(m, "BUTTON_LMASK", SDL_BUTTON_LMASK);
    MAKE_PY_CONST(m, "BUTTON_MMASK", SDL_BUTTON_MMASK);
    MAKE_PY_CONST(m, "BUTTON_RMASK", SDL_BUTTON_RMASK);
    MAKE_PY_CONST(m, "BUTTON_X1MASK", SDL_BUTTON_X1MASK);
    MAKE_PY_CONST(m, "BUTTON_X2MASK", SDL_BUTTON_X2MASK);

    MAKE_PY_CONST(m, "APPMOUSEFOCUS", SDL_APPMOUSEFOCUS);
    MAKE_PY_CONST(m, "APPINPUTFOCUS", SDL_APPINPUTFOCUS);
    MAKE_PY_CONST(m, "APPACTIVE", SDL_APPACTIVE);

    sdl_VersionInfo_register_type(m);

    sdl_Color_register_type(m);

    sdl_Rect_register_type(m);

    sdl_Palette_register_type(m);

    sdl_PixelFormat_register_type(m);

    sdl_VideoInfo_register_type(m);

    sdl_Surface_register_type(m);

    sdl_keysym_register_type(m);
}
