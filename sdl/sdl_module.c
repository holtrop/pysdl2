
#include <Python.h>
#include <SDL/SDL.h>
#include "sdl_module.h"

#define _MAKE_CONST(m, pfx, sym) \
    do { \
        PyObject *io = PyInt_FromLong(pfx ## sym); \
        PyDict_SetItemString((m), #sym, io); \
        Py_DECREF(io); \
    } while(0)
#define MAKE_CONST(m, sym) _MAKE_CONST(m, SDL_, sym)
#define MAKE_CONSTK(m, sym) _MAKE_CONST(m, SDL, sym)

#define PYFUNC(name, desc) \
    static const char name ## _doc[] = desc; \
    static PyObject *pysdl_ ## name(PyObject *self, PyObject *args)

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

PYFUNC(WasInit, "Check which subsystems are initialized")
{
    Uint32 flags;
    if (!PyArg_ParseTuple(args, "I", &flags))
        return NULL;
    int rc = SDL_WasInit(flags);
    return Py_BuildValue("i", rc);
}

#define METH_REF(name) {#name, pysdl_ ## name, METH_VARARGS, name ## _doc}
static PyMethodDef sdl_methods[] = {
    METH_REF(GetError),
    METH_REF(Init),
    METH_REF(InitSubSystem),
    METH_REF(Quit),
    METH_REF(QuitSubSystem),
    METH_REF(SetError),
    METH_REF(WasInit),
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initSDL(void)
{
    PyObject *m = Py_InitModule(PYSDL_MODULE_NAME, sdl_methods);
    if (m == NULL)
        return;

    PyObject *md = PyModule_GetDict(m);
    if (md == NULL)
        return;

    MAKE_CONST(md, INIT_TIMER);
    MAKE_CONST(md, INIT_VIDEO);
    MAKE_CONST(md, INIT_AUDIO);
    MAKE_CONST(md, INIT_CDROM);
    MAKE_CONST(md, INIT_JOYSTICK);
    MAKE_CONST(md, INIT_EVERYTHING);
    MAKE_CONST(md, INIT_NOPARACHUTE);
    MAKE_CONST(md, INIT_EVENTTHREAD);

#undef ENOMEM
    MAKE_CONST(md, ENOMEM);
    MAKE_CONST(md, EFREAD);
    MAKE_CONST(md, EFWRITE);
    MAKE_CONST(md, EFSEEK);
    MAKE_CONST(md, UNSUPPORTED);
    MAKE_CONST(md, LASTERROR);

    MAKE_CONSTK(md, K_UNKNOWN);
    MAKE_CONSTK(md, K_FIRST);
    MAKE_CONSTK(md, K_BACKSPACE);
    MAKE_CONSTK(md, K_TAB);
    MAKE_CONSTK(md, K_CLEAR);
    MAKE_CONSTK(md, K_RETURN);
    MAKE_CONSTK(md, K_PAUSE);
    MAKE_CONSTK(md, K_ESCAPE);
    MAKE_CONSTK(md, K_SPACE);
    MAKE_CONSTK(md, K_EXCLAIM);
    MAKE_CONSTK(md, K_QUOTEDBL);
    MAKE_CONSTK(md, K_HASH);
    MAKE_CONSTK(md, K_DOLLAR);
    MAKE_CONSTK(md, K_AMPERSAND);
    MAKE_CONSTK(md, K_QUOTE);
    MAKE_CONSTK(md, K_LEFTPAREN);
    MAKE_CONSTK(md, K_RIGHTPAREN);
    MAKE_CONSTK(md, K_ASTERISK);
    MAKE_CONSTK(md, K_PLUS);
    MAKE_CONSTK(md, K_COMMA);
    MAKE_CONSTK(md, K_MINUS);
    MAKE_CONSTK(md, K_PERIOD);
    MAKE_CONSTK(md, K_SLASH);
    MAKE_CONSTK(md, K_0);
    MAKE_CONSTK(md, K_1);
    MAKE_CONSTK(md, K_2);
    MAKE_CONSTK(md, K_3);
    MAKE_CONSTK(md, K_4);
    MAKE_CONSTK(md, K_5);
    MAKE_CONSTK(md, K_6);
    MAKE_CONSTK(md, K_7);
    MAKE_CONSTK(md, K_8);
    MAKE_CONSTK(md, K_9);
    MAKE_CONSTK(md, K_COLON);
    MAKE_CONSTK(md, K_SEMICOLON);
    MAKE_CONSTK(md, K_LESS);
    MAKE_CONSTK(md, K_EQUALS);
    MAKE_CONSTK(md, K_GREATER);
    MAKE_CONSTK(md, K_QUESTION);
    MAKE_CONSTK(md, K_AT);

    MAKE_CONSTK(md, K_LEFTBRACKET);
    MAKE_CONSTK(md, K_BACKSLASH);
    MAKE_CONSTK(md, K_RIGHTBRACKET);
    MAKE_CONSTK(md, K_CARET);
    MAKE_CONSTK(md, K_UNDERSCORE);
    MAKE_CONSTK(md, K_BACKQUOTE);
    MAKE_CONSTK(md, K_a);
    MAKE_CONSTK(md, K_b);
    MAKE_CONSTK(md, K_c);
    MAKE_CONSTK(md, K_d);
    MAKE_CONSTK(md, K_e);
    MAKE_CONSTK(md, K_f);
    MAKE_CONSTK(md, K_g);
    MAKE_CONSTK(md, K_h);
    MAKE_CONSTK(md, K_i);
    MAKE_CONSTK(md, K_j);
    MAKE_CONSTK(md, K_k);
    MAKE_CONSTK(md, K_l);
    MAKE_CONSTK(md, K_m);
    MAKE_CONSTK(md, K_n);
    MAKE_CONSTK(md, K_o);
    MAKE_CONSTK(md, K_p);
    MAKE_CONSTK(md, K_q);
    MAKE_CONSTK(md, K_r);
    MAKE_CONSTK(md, K_s);
    MAKE_CONSTK(md, K_t);
    MAKE_CONSTK(md, K_u);
    MAKE_CONSTK(md, K_v);
    MAKE_CONSTK(md, K_w);
    MAKE_CONSTK(md, K_x);
    MAKE_CONSTK(md, K_y);
    MAKE_CONSTK(md, K_z);
    MAKE_CONSTK(md, K_DELETE);

    MAKE_CONSTK(md, K_WORLD_0);
    MAKE_CONSTK(md, K_WORLD_1);
    MAKE_CONSTK(md, K_WORLD_2);
    MAKE_CONSTK(md, K_WORLD_3);
    MAKE_CONSTK(md, K_WORLD_4);
    MAKE_CONSTK(md, K_WORLD_5);
    MAKE_CONSTK(md, K_WORLD_6);
    MAKE_CONSTK(md, K_WORLD_7);
    MAKE_CONSTK(md, K_WORLD_8);
    MAKE_CONSTK(md, K_WORLD_9);
    MAKE_CONSTK(md, K_WORLD_10);
    MAKE_CONSTK(md, K_WORLD_11);
    MAKE_CONSTK(md, K_WORLD_12);
    MAKE_CONSTK(md, K_WORLD_13);
    MAKE_CONSTK(md, K_WORLD_14);
    MAKE_CONSTK(md, K_WORLD_15);
    MAKE_CONSTK(md, K_WORLD_16);
    MAKE_CONSTK(md, K_WORLD_17);
    MAKE_CONSTK(md, K_WORLD_18);
    MAKE_CONSTK(md, K_WORLD_19);
    MAKE_CONSTK(md, K_WORLD_20);
    MAKE_CONSTK(md, K_WORLD_21);
    MAKE_CONSTK(md, K_WORLD_22);
    MAKE_CONSTK(md, K_WORLD_23);
    MAKE_CONSTK(md, K_WORLD_24);
    MAKE_CONSTK(md, K_WORLD_25);
    MAKE_CONSTK(md, K_WORLD_26);
    MAKE_CONSTK(md, K_WORLD_27);
    MAKE_CONSTK(md, K_WORLD_28);
    MAKE_CONSTK(md, K_WORLD_29);
    MAKE_CONSTK(md, K_WORLD_30);
    MAKE_CONSTK(md, K_WORLD_31);
    MAKE_CONSTK(md, K_WORLD_32);
    MAKE_CONSTK(md, K_WORLD_33);
    MAKE_CONSTK(md, K_WORLD_34);
    MAKE_CONSTK(md, K_WORLD_35);
    MAKE_CONSTK(md, K_WORLD_36);
    MAKE_CONSTK(md, K_WORLD_37);
    MAKE_CONSTK(md, K_WORLD_38);
    MAKE_CONSTK(md, K_WORLD_39);
    MAKE_CONSTK(md, K_WORLD_40);
    MAKE_CONSTK(md, K_WORLD_41);
    MAKE_CONSTK(md, K_WORLD_42);
    MAKE_CONSTK(md, K_WORLD_43);
    MAKE_CONSTK(md, K_WORLD_44);
    MAKE_CONSTK(md, K_WORLD_45);
    MAKE_CONSTK(md, K_WORLD_46);
    MAKE_CONSTK(md, K_WORLD_47);
    MAKE_CONSTK(md, K_WORLD_48);
    MAKE_CONSTK(md, K_WORLD_49);
    MAKE_CONSTK(md, K_WORLD_50);
    MAKE_CONSTK(md, K_WORLD_51);
    MAKE_CONSTK(md, K_WORLD_52);
    MAKE_CONSTK(md, K_WORLD_53);
    MAKE_CONSTK(md, K_WORLD_54);
    MAKE_CONSTK(md, K_WORLD_55);
    MAKE_CONSTK(md, K_WORLD_56);
    MAKE_CONSTK(md, K_WORLD_57);
    MAKE_CONSTK(md, K_WORLD_58);
    MAKE_CONSTK(md, K_WORLD_59);
    MAKE_CONSTK(md, K_WORLD_60);
    MAKE_CONSTK(md, K_WORLD_61);
    MAKE_CONSTK(md, K_WORLD_62);
    MAKE_CONSTK(md, K_WORLD_63);
    MAKE_CONSTK(md, K_WORLD_64);
    MAKE_CONSTK(md, K_WORLD_65);
    MAKE_CONSTK(md, K_WORLD_66);
    MAKE_CONSTK(md, K_WORLD_67);
    MAKE_CONSTK(md, K_WORLD_68);
    MAKE_CONSTK(md, K_WORLD_69);
    MAKE_CONSTK(md, K_WORLD_70);
    MAKE_CONSTK(md, K_WORLD_71);
    MAKE_CONSTK(md, K_WORLD_72);
    MAKE_CONSTK(md, K_WORLD_73);
    MAKE_CONSTK(md, K_WORLD_74);
    MAKE_CONSTK(md, K_WORLD_75);
    MAKE_CONSTK(md, K_WORLD_76);
    MAKE_CONSTK(md, K_WORLD_77);
    MAKE_CONSTK(md, K_WORLD_78);
    MAKE_CONSTK(md, K_WORLD_79);
    MAKE_CONSTK(md, K_WORLD_80);
    MAKE_CONSTK(md, K_WORLD_81);
    MAKE_CONSTK(md, K_WORLD_82);
    MAKE_CONSTK(md, K_WORLD_83);
    MAKE_CONSTK(md, K_WORLD_84);
    MAKE_CONSTK(md, K_WORLD_85);
    MAKE_CONSTK(md, K_WORLD_86);
    MAKE_CONSTK(md, K_WORLD_87);
    MAKE_CONSTK(md, K_WORLD_88);
    MAKE_CONSTK(md, K_WORLD_89);
    MAKE_CONSTK(md, K_WORLD_90);
    MAKE_CONSTK(md, K_WORLD_91);
    MAKE_CONSTK(md, K_WORLD_92);
    MAKE_CONSTK(md, K_WORLD_93);
    MAKE_CONSTK(md, K_WORLD_94);
    MAKE_CONSTK(md, K_WORLD_95);

    MAKE_CONSTK(md, K_KP0);
    MAKE_CONSTK(md, K_KP1);
    MAKE_CONSTK(md, K_KP2);
    MAKE_CONSTK(md, K_KP3);
    MAKE_CONSTK(md, K_KP4);
    MAKE_CONSTK(md, K_KP5);
    MAKE_CONSTK(md, K_KP6);
    MAKE_CONSTK(md, K_KP7);
    MAKE_CONSTK(md, K_KP8);
    MAKE_CONSTK(md, K_KP9);
    MAKE_CONSTK(md, K_KP_PERIOD);
    MAKE_CONSTK(md, K_KP_DIVIDE);
    MAKE_CONSTK(md, K_KP_MULTIPLY);
    MAKE_CONSTK(md, K_KP_MINUS);
    MAKE_CONSTK(md, K_KP_PLUS);
    MAKE_CONSTK(md, K_KP_ENTER);
    MAKE_CONSTK(md, K_KP_EQUALS);

    MAKE_CONSTK(md, K_UP);
    MAKE_CONSTK(md, K_DOWN);
    MAKE_CONSTK(md, K_RIGHT);
    MAKE_CONSTK(md, K_LEFT);
    MAKE_CONSTK(md, K_INSERT);
    MAKE_CONSTK(md, K_HOME);
    MAKE_CONSTK(md, K_END);
    MAKE_CONSTK(md, K_PAGEUP);
    MAKE_CONSTK(md, K_PAGEDOWN);

    MAKE_CONSTK(md, K_F1);
    MAKE_CONSTK(md, K_F2);
    MAKE_CONSTK(md, K_F3);
    MAKE_CONSTK(md, K_F4);
    MAKE_CONSTK(md, K_F5);
    MAKE_CONSTK(md, K_F6);
    MAKE_CONSTK(md, K_F7);
    MAKE_CONSTK(md, K_F8);
    MAKE_CONSTK(md, K_F9);
    MAKE_CONSTK(md, K_F10);
    MAKE_CONSTK(md, K_F11);
    MAKE_CONSTK(md, K_F12);
    MAKE_CONSTK(md, K_F13);
    MAKE_CONSTK(md, K_F14);
    MAKE_CONSTK(md, K_F15);

    MAKE_CONSTK(md, K_NUMLOCK);
    MAKE_CONSTK(md, K_CAPSLOCK);
    MAKE_CONSTK(md, K_SCROLLOCK);
    MAKE_CONSTK(md, K_RSHIFT);
    MAKE_CONSTK(md, K_LSHIFT);
    MAKE_CONSTK(md, K_RCTRL);
    MAKE_CONSTK(md, K_LCTRL);
    MAKE_CONSTK(md, K_RALT);
    MAKE_CONSTK(md, K_LALT);
    MAKE_CONSTK(md, K_RMETA);
    MAKE_CONSTK(md, K_LMETA);
    MAKE_CONSTK(md, K_LSUPER);
    MAKE_CONSTK(md, K_RSUPER);
    MAKE_CONSTK(md, K_MODE);
    MAKE_CONSTK(md, K_COMPOSE);

    MAKE_CONSTK(md, K_HELP);
    MAKE_CONSTK(md, K_PRINT);
    MAKE_CONSTK(md, K_SYSREQ);
    MAKE_CONSTK(md, K_BREAK);
    MAKE_CONSTK(md, K_MENU);
    MAKE_CONSTK(md, K_POWER);
    MAKE_CONSTK(md, K_EURO);
    MAKE_CONSTK(md, K_UNDO);

    MAKE_CONSTK(md, K_LAST);
}
