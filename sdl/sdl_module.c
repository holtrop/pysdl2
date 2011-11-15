
#include <Python.h>
#include "pysdl.h"
#include "sdl_module.h"
#include "VersionInfo.h"
#include "Color.h"

static PyObject *VersionInfoFromSDL_version(const SDL_version *ver)
{
    PyObject *lst = Py_BuildValue("()");
    PyObject *dict = Py_BuildValue("{s:i,s:i,s:i}",
            "major", ver->major,
            "minor", ver->minor,
            "patch", ver->patch);
    PyObject *ver_obj = PyObject_Call((PyObject *) &sdl_VersionInfoType,
            lst,
            dict);
    Py_DECREF(dict);
    Py_DECREF(lst);
    return ver_obj;
}

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
    return VersionInfoFromSDL_version(ver);
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
    return VersionInfoFromSDL_version(&ver);
}

PYFUNC(WasInit, "Check which subsystems are initialized")
{
    Uint32 flags;
    if (!PyArg_ParseTuple(args, "I", &flags))
        return NULL;
    int rc = SDL_WasInit(flags);
    return Py_BuildValue("i", rc);
}

static PyMethodDef sdl_methods[] = {
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

    MAKE_PY_CONST(md, "INIT_TIMER", SDL_INIT_TIMER);
    MAKE_PY_CONST(md, "INIT_VIDEO", SDL_INIT_VIDEO);
    MAKE_PY_CONST(md, "INIT_AUDIO", SDL_INIT_AUDIO);
    MAKE_PY_CONST(md, "INIT_CDROM", SDL_INIT_CDROM);
    MAKE_PY_CONST(md, "INIT_JOYSTICK", SDL_INIT_JOYSTICK);
    MAKE_PY_CONST(md, "INIT_EVERYTHING", SDL_INIT_EVERYTHING);
    MAKE_PY_CONST(md, "INIT_NOPARACHUTE", SDL_INIT_NOPARACHUTE);
    MAKE_PY_CONST(md, "INIT_EVENTTHREAD", SDL_INIT_EVENTTHREAD);

    MAKE_PY_CONST(md, "ENOMEM", SDL_ENOMEM);
    MAKE_PY_CONST(md, "EFREAD", SDL_EFREAD);
    MAKE_PY_CONST(md, "EFWRITE", SDL_EFWRITE);
    MAKE_PY_CONST(md, "EFSEEK", SDL_EFSEEK);
    MAKE_PY_CONST(md, "UNSUPPORTED", SDL_UNSUPPORTED);
    MAKE_PY_CONST(md, "LASTERROR", SDL_LASTERROR);

    MAKE_PY_CONST(md, "K_UNKNOWN", SDLK_UNKNOWN);
    MAKE_PY_CONST(md, "K_FIRST", SDLK_FIRST);
    MAKE_PY_CONST(md, "K_BACKSPACE", SDLK_BACKSPACE);
    MAKE_PY_CONST(md, "K_TAB", SDLK_TAB);
    MAKE_PY_CONST(md, "K_CLEAR", SDLK_CLEAR);
    MAKE_PY_CONST(md, "K_RETURN", SDLK_RETURN);
    MAKE_PY_CONST(md, "K_PAUSE", SDLK_PAUSE);
    MAKE_PY_CONST(md, "K_ESCAPE", SDLK_ESCAPE);
    MAKE_PY_CONST(md, "K_SPACE", SDLK_SPACE);
    MAKE_PY_CONST(md, "K_EXCLAIM", SDLK_EXCLAIM);
    MAKE_PY_CONST(md, "K_QUOTEDBL", SDLK_QUOTEDBL);
    MAKE_PY_CONST(md, "K_HASH", SDLK_HASH);
    MAKE_PY_CONST(md, "K_DOLLAR", SDLK_DOLLAR);
    MAKE_PY_CONST(md, "K_AMPERSAND", SDLK_AMPERSAND);
    MAKE_PY_CONST(md, "K_QUOTE", SDLK_QUOTE);
    MAKE_PY_CONST(md, "K_LEFTPAREN", SDLK_LEFTPAREN);
    MAKE_PY_CONST(md, "K_RIGHTPAREN", SDLK_RIGHTPAREN);
    MAKE_PY_CONST(md, "K_ASTERISK", SDLK_ASTERISK);
    MAKE_PY_CONST(md, "K_PLUS", SDLK_PLUS);
    MAKE_PY_CONST(md, "K_COMMA", SDLK_COMMA);
    MAKE_PY_CONST(md, "K_MINUS", SDLK_MINUS);
    MAKE_PY_CONST(md, "K_PERIOD", SDLK_PERIOD);
    MAKE_PY_CONST(md, "K_SLASH", SDLK_SLASH);
    MAKE_PY_CONST(md, "K_0", SDLK_0);
    MAKE_PY_CONST(md, "K_1", SDLK_1);
    MAKE_PY_CONST(md, "K_2", SDLK_2);
    MAKE_PY_CONST(md, "K_3", SDLK_3);
    MAKE_PY_CONST(md, "K_4", SDLK_4);
    MAKE_PY_CONST(md, "K_5", SDLK_5);
    MAKE_PY_CONST(md, "K_6", SDLK_6);
    MAKE_PY_CONST(md, "K_7", SDLK_7);
    MAKE_PY_CONST(md, "K_8", SDLK_8);
    MAKE_PY_CONST(md, "K_9", SDLK_9);
    MAKE_PY_CONST(md, "K_COLON", SDLK_COLON);
    MAKE_PY_CONST(md, "K_SEMICOLON", SDLK_SEMICOLON);
    MAKE_PY_CONST(md, "K_LESS", SDLK_LESS);
    MAKE_PY_CONST(md, "K_EQUALS", SDLK_EQUALS);
    MAKE_PY_CONST(md, "K_GREATER", SDLK_GREATER);
    MAKE_PY_CONST(md, "K_QUESTION", SDLK_QUESTION);
    MAKE_PY_CONST(md, "K_AT", SDLK_AT);

    MAKE_PY_CONST(md, "K_LEFTBRACKET", SDLK_LEFTBRACKET);
    MAKE_PY_CONST(md, "K_BACKSLASH", SDLK_BACKSLASH);
    MAKE_PY_CONST(md, "K_RIGHTBRACKET", SDLK_RIGHTBRACKET);
    MAKE_PY_CONST(md, "K_CARET", SDLK_CARET);
    MAKE_PY_CONST(md, "K_UNDERSCORE", SDLK_UNDERSCORE);
    MAKE_PY_CONST(md, "K_BACKQUOTE", SDLK_BACKQUOTE);
    MAKE_PY_CONST(md, "K_a", SDLK_a);
    MAKE_PY_CONST(md, "K_b", SDLK_b);
    MAKE_PY_CONST(md, "K_c", SDLK_c);
    MAKE_PY_CONST(md, "K_d", SDLK_d);
    MAKE_PY_CONST(md, "K_e", SDLK_e);
    MAKE_PY_CONST(md, "K_f", SDLK_f);
    MAKE_PY_CONST(md, "K_g", SDLK_g);
    MAKE_PY_CONST(md, "K_h", SDLK_h);
    MAKE_PY_CONST(md, "K_i", SDLK_i);
    MAKE_PY_CONST(md, "K_j", SDLK_j);
    MAKE_PY_CONST(md, "K_k", SDLK_k);
    MAKE_PY_CONST(md, "K_l", SDLK_l);
    MAKE_PY_CONST(md, "K_m", SDLK_m);
    MAKE_PY_CONST(md, "K_n", SDLK_n);
    MAKE_PY_CONST(md, "K_o", SDLK_o);
    MAKE_PY_CONST(md, "K_p", SDLK_p);
    MAKE_PY_CONST(md, "K_q", SDLK_q);
    MAKE_PY_CONST(md, "K_r", SDLK_r);
    MAKE_PY_CONST(md, "K_s", SDLK_s);
    MAKE_PY_CONST(md, "K_t", SDLK_t);
    MAKE_PY_CONST(md, "K_u", SDLK_u);
    MAKE_PY_CONST(md, "K_v", SDLK_v);
    MAKE_PY_CONST(md, "K_w", SDLK_w);
    MAKE_PY_CONST(md, "K_x", SDLK_x);
    MAKE_PY_CONST(md, "K_y", SDLK_y);
    MAKE_PY_CONST(md, "K_z", SDLK_z);
    MAKE_PY_CONST(md, "K_DELETE", SDLK_DELETE);

    MAKE_PY_CONST(md, "K_WORLD_0", SDLK_WORLD_0);
    MAKE_PY_CONST(md, "K_WORLD_1", SDLK_WORLD_1);
    MAKE_PY_CONST(md, "K_WORLD_2", SDLK_WORLD_2);
    MAKE_PY_CONST(md, "K_WORLD_3", SDLK_WORLD_3);
    MAKE_PY_CONST(md, "K_WORLD_4", SDLK_WORLD_4);
    MAKE_PY_CONST(md, "K_WORLD_5", SDLK_WORLD_5);
    MAKE_PY_CONST(md, "K_WORLD_6", SDLK_WORLD_6);
    MAKE_PY_CONST(md, "K_WORLD_7", SDLK_WORLD_7);
    MAKE_PY_CONST(md, "K_WORLD_8", SDLK_WORLD_8);
    MAKE_PY_CONST(md, "K_WORLD_9", SDLK_WORLD_9);
    MAKE_PY_CONST(md, "K_WORLD_10", SDLK_WORLD_10);
    MAKE_PY_CONST(md, "K_WORLD_11", SDLK_WORLD_11);
    MAKE_PY_CONST(md, "K_WORLD_12", SDLK_WORLD_12);
    MAKE_PY_CONST(md, "K_WORLD_13", SDLK_WORLD_13);
    MAKE_PY_CONST(md, "K_WORLD_14", SDLK_WORLD_14);
    MAKE_PY_CONST(md, "K_WORLD_15", SDLK_WORLD_15);
    MAKE_PY_CONST(md, "K_WORLD_16", SDLK_WORLD_16);
    MAKE_PY_CONST(md, "K_WORLD_17", SDLK_WORLD_17);
    MAKE_PY_CONST(md, "K_WORLD_18", SDLK_WORLD_18);
    MAKE_PY_CONST(md, "K_WORLD_19", SDLK_WORLD_19);
    MAKE_PY_CONST(md, "K_WORLD_20", SDLK_WORLD_20);
    MAKE_PY_CONST(md, "K_WORLD_21", SDLK_WORLD_21);
    MAKE_PY_CONST(md, "K_WORLD_22", SDLK_WORLD_22);
    MAKE_PY_CONST(md, "K_WORLD_23", SDLK_WORLD_23);
    MAKE_PY_CONST(md, "K_WORLD_24", SDLK_WORLD_24);
    MAKE_PY_CONST(md, "K_WORLD_25", SDLK_WORLD_25);
    MAKE_PY_CONST(md, "K_WORLD_26", SDLK_WORLD_26);
    MAKE_PY_CONST(md, "K_WORLD_27", SDLK_WORLD_27);
    MAKE_PY_CONST(md, "K_WORLD_28", SDLK_WORLD_28);
    MAKE_PY_CONST(md, "K_WORLD_29", SDLK_WORLD_29);
    MAKE_PY_CONST(md, "K_WORLD_30", SDLK_WORLD_30);
    MAKE_PY_CONST(md, "K_WORLD_31", SDLK_WORLD_31);
    MAKE_PY_CONST(md, "K_WORLD_32", SDLK_WORLD_32);
    MAKE_PY_CONST(md, "K_WORLD_33", SDLK_WORLD_33);
    MAKE_PY_CONST(md, "K_WORLD_34", SDLK_WORLD_34);
    MAKE_PY_CONST(md, "K_WORLD_35", SDLK_WORLD_35);
    MAKE_PY_CONST(md, "K_WORLD_36", SDLK_WORLD_36);
    MAKE_PY_CONST(md, "K_WORLD_37", SDLK_WORLD_37);
    MAKE_PY_CONST(md, "K_WORLD_38", SDLK_WORLD_38);
    MAKE_PY_CONST(md, "K_WORLD_39", SDLK_WORLD_39);
    MAKE_PY_CONST(md, "K_WORLD_40", SDLK_WORLD_40);
    MAKE_PY_CONST(md, "K_WORLD_41", SDLK_WORLD_41);
    MAKE_PY_CONST(md, "K_WORLD_42", SDLK_WORLD_42);
    MAKE_PY_CONST(md, "K_WORLD_43", SDLK_WORLD_43);
    MAKE_PY_CONST(md, "K_WORLD_44", SDLK_WORLD_44);
    MAKE_PY_CONST(md, "K_WORLD_45", SDLK_WORLD_45);
    MAKE_PY_CONST(md, "K_WORLD_46", SDLK_WORLD_46);
    MAKE_PY_CONST(md, "K_WORLD_47", SDLK_WORLD_47);
    MAKE_PY_CONST(md, "K_WORLD_48", SDLK_WORLD_48);
    MAKE_PY_CONST(md, "K_WORLD_49", SDLK_WORLD_49);
    MAKE_PY_CONST(md, "K_WORLD_50", SDLK_WORLD_50);
    MAKE_PY_CONST(md, "K_WORLD_51", SDLK_WORLD_51);
    MAKE_PY_CONST(md, "K_WORLD_52", SDLK_WORLD_52);
    MAKE_PY_CONST(md, "K_WORLD_53", SDLK_WORLD_53);
    MAKE_PY_CONST(md, "K_WORLD_54", SDLK_WORLD_54);
    MAKE_PY_CONST(md, "K_WORLD_55", SDLK_WORLD_55);
    MAKE_PY_CONST(md, "K_WORLD_56", SDLK_WORLD_56);
    MAKE_PY_CONST(md, "K_WORLD_57", SDLK_WORLD_57);
    MAKE_PY_CONST(md, "K_WORLD_58", SDLK_WORLD_58);
    MAKE_PY_CONST(md, "K_WORLD_59", SDLK_WORLD_59);
    MAKE_PY_CONST(md, "K_WORLD_60", SDLK_WORLD_60);
    MAKE_PY_CONST(md, "K_WORLD_61", SDLK_WORLD_61);
    MAKE_PY_CONST(md, "K_WORLD_62", SDLK_WORLD_62);
    MAKE_PY_CONST(md, "K_WORLD_63", SDLK_WORLD_63);
    MAKE_PY_CONST(md, "K_WORLD_64", SDLK_WORLD_64);
    MAKE_PY_CONST(md, "K_WORLD_65", SDLK_WORLD_65);
    MAKE_PY_CONST(md, "K_WORLD_66", SDLK_WORLD_66);
    MAKE_PY_CONST(md, "K_WORLD_67", SDLK_WORLD_67);
    MAKE_PY_CONST(md, "K_WORLD_68", SDLK_WORLD_68);
    MAKE_PY_CONST(md, "K_WORLD_69", SDLK_WORLD_69);
    MAKE_PY_CONST(md, "K_WORLD_70", SDLK_WORLD_70);
    MAKE_PY_CONST(md, "K_WORLD_71", SDLK_WORLD_71);
    MAKE_PY_CONST(md, "K_WORLD_72", SDLK_WORLD_72);
    MAKE_PY_CONST(md, "K_WORLD_73", SDLK_WORLD_73);
    MAKE_PY_CONST(md, "K_WORLD_74", SDLK_WORLD_74);
    MAKE_PY_CONST(md, "K_WORLD_75", SDLK_WORLD_75);
    MAKE_PY_CONST(md, "K_WORLD_76", SDLK_WORLD_76);
    MAKE_PY_CONST(md, "K_WORLD_77", SDLK_WORLD_77);
    MAKE_PY_CONST(md, "K_WORLD_78", SDLK_WORLD_78);
    MAKE_PY_CONST(md, "K_WORLD_79", SDLK_WORLD_79);
    MAKE_PY_CONST(md, "K_WORLD_80", SDLK_WORLD_80);
    MAKE_PY_CONST(md, "K_WORLD_81", SDLK_WORLD_81);
    MAKE_PY_CONST(md, "K_WORLD_82", SDLK_WORLD_82);
    MAKE_PY_CONST(md, "K_WORLD_83", SDLK_WORLD_83);
    MAKE_PY_CONST(md, "K_WORLD_84", SDLK_WORLD_84);
    MAKE_PY_CONST(md, "K_WORLD_85", SDLK_WORLD_85);
    MAKE_PY_CONST(md, "K_WORLD_86", SDLK_WORLD_86);
    MAKE_PY_CONST(md, "K_WORLD_87", SDLK_WORLD_87);
    MAKE_PY_CONST(md, "K_WORLD_88", SDLK_WORLD_88);
    MAKE_PY_CONST(md, "K_WORLD_89", SDLK_WORLD_89);
    MAKE_PY_CONST(md, "K_WORLD_90", SDLK_WORLD_90);
    MAKE_PY_CONST(md, "K_WORLD_91", SDLK_WORLD_91);
    MAKE_PY_CONST(md, "K_WORLD_92", SDLK_WORLD_92);
    MAKE_PY_CONST(md, "K_WORLD_93", SDLK_WORLD_93);
    MAKE_PY_CONST(md, "K_WORLD_94", SDLK_WORLD_94);
    MAKE_PY_CONST(md, "K_WORLD_95", SDLK_WORLD_95);

    MAKE_PY_CONST(md, "K_KP0", SDLK_KP0);
    MAKE_PY_CONST(md, "K_KP1", SDLK_KP1);
    MAKE_PY_CONST(md, "K_KP2", SDLK_KP2);
    MAKE_PY_CONST(md, "K_KP3", SDLK_KP3);
    MAKE_PY_CONST(md, "K_KP4", SDLK_KP4);
    MAKE_PY_CONST(md, "K_KP5", SDLK_KP5);
    MAKE_PY_CONST(md, "K_KP6", SDLK_KP6);
    MAKE_PY_CONST(md, "K_KP7", SDLK_KP7);
    MAKE_PY_CONST(md, "K_KP8", SDLK_KP8);
    MAKE_PY_CONST(md, "K_KP9", SDLK_KP9);
    MAKE_PY_CONST(md, "K_KP_PERIOD", SDLK_KP_PERIOD);
    MAKE_PY_CONST(md, "K_KP_DIVIDE", SDLK_KP_DIVIDE);
    MAKE_PY_CONST(md, "K_KP_MULTIPLY", SDLK_KP_MULTIPLY);
    MAKE_PY_CONST(md, "K_KP_MINUS", SDLK_KP_MINUS);
    MAKE_PY_CONST(md, "K_KP_PLUS", SDLK_KP_PLUS);
    MAKE_PY_CONST(md, "K_KP_ENTER", SDLK_KP_ENTER);
    MAKE_PY_CONST(md, "K_KP_EQUALS", SDLK_KP_EQUALS);

    MAKE_PY_CONST(md, "K_UP", SDLK_UP);
    MAKE_PY_CONST(md, "K_DOWN", SDLK_DOWN);
    MAKE_PY_CONST(md, "K_RIGHT", SDLK_RIGHT);
    MAKE_PY_CONST(md, "K_LEFT", SDLK_LEFT);
    MAKE_PY_CONST(md, "K_INSERT", SDLK_INSERT);
    MAKE_PY_CONST(md, "K_HOME", SDLK_HOME);
    MAKE_PY_CONST(md, "K_END", SDLK_END);
    MAKE_PY_CONST(md, "K_PAGEUP", SDLK_PAGEUP);
    MAKE_PY_CONST(md, "K_PAGEDOWN", SDLK_PAGEDOWN);

    MAKE_PY_CONST(md, "K_F1", SDLK_F1);
    MAKE_PY_CONST(md, "K_F2", SDLK_F2);
    MAKE_PY_CONST(md, "K_F3", SDLK_F3);
    MAKE_PY_CONST(md, "K_F4", SDLK_F4);
    MAKE_PY_CONST(md, "K_F5", SDLK_F5);
    MAKE_PY_CONST(md, "K_F6", SDLK_F6);
    MAKE_PY_CONST(md, "K_F7", SDLK_F7);
    MAKE_PY_CONST(md, "K_F8", SDLK_F8);
    MAKE_PY_CONST(md, "K_F9", SDLK_F9);
    MAKE_PY_CONST(md, "K_F10", SDLK_F10);
    MAKE_PY_CONST(md, "K_F11", SDLK_F11);
    MAKE_PY_CONST(md, "K_F12", SDLK_F12);
    MAKE_PY_CONST(md, "K_F13", SDLK_F13);
    MAKE_PY_CONST(md, "K_F14", SDLK_F14);
    MAKE_PY_CONST(md, "K_F15", SDLK_F15);

    MAKE_PY_CONST(md, "K_NUMLOCK", SDLK_NUMLOCK);
    MAKE_PY_CONST(md, "K_CAPSLOCK", SDLK_CAPSLOCK);
    MAKE_PY_CONST(md, "K_SCROLLOCK", SDLK_SCROLLOCK);
    MAKE_PY_CONST(md, "K_RSHIFT", SDLK_RSHIFT);
    MAKE_PY_CONST(md, "K_LSHIFT", SDLK_LSHIFT);
    MAKE_PY_CONST(md, "K_RCTRL", SDLK_RCTRL);
    MAKE_PY_CONST(md, "K_LCTRL", SDLK_LCTRL);
    MAKE_PY_CONST(md, "K_RALT", SDLK_RALT);
    MAKE_PY_CONST(md, "K_LALT", SDLK_LALT);
    MAKE_PY_CONST(md, "K_RMETA", SDLK_RMETA);
    MAKE_PY_CONST(md, "K_LMETA", SDLK_LMETA);
    MAKE_PY_CONST(md, "K_LSUPER", SDLK_LSUPER);
    MAKE_PY_CONST(md, "K_RSUPER", SDLK_RSUPER);
    MAKE_PY_CONST(md, "K_MODE", SDLK_MODE);
    MAKE_PY_CONST(md, "K_COMPOSE", SDLK_COMPOSE);

    MAKE_PY_CONST(md, "K_HELP", SDLK_HELP);
    MAKE_PY_CONST(md, "K_PRINT", SDLK_PRINT);
    MAKE_PY_CONST(md, "K_SYSREQ", SDLK_SYSREQ);
    MAKE_PY_CONST(md, "K_BREAK", SDLK_BREAK);
    MAKE_PY_CONST(md, "K_MENU", SDLK_MENU);
    MAKE_PY_CONST(md, "K_POWER", SDLK_POWER);
    MAKE_PY_CONST(md, "K_EURO", SDLK_EURO);
    MAKE_PY_CONST(md, "K_UNDO", SDLK_UNDO);

    MAKE_PY_CONST(md, "K_LAST", SDLK_LAST);

    MAKE_PY_CONST(md, "KMOD_NONE", KMOD_NONE);
    MAKE_PY_CONST(md, "KMOD_LSHIFT", KMOD_LSHIFT);
    MAKE_PY_CONST(md, "KMOD_RSHIFT", KMOD_RSHIFT);
    MAKE_PY_CONST(md, "KMOD_LCTRL", KMOD_LCTRL);
    MAKE_PY_CONST(md, "KMOD_RCTRL", KMOD_RCTRL);
    MAKE_PY_CONST(md, "KMOD_LALT", KMOD_LALT);
    MAKE_PY_CONST(md, "KMOD_RALT", KMOD_RALT);
    MAKE_PY_CONST(md, "KMOD_LMETA", KMOD_LMETA);
    MAKE_PY_CONST(md, "KMOD_RMETA", KMOD_RMETA);
    MAKE_PY_CONST(md, "KMOD_NUM", KMOD_NUM);
    MAKE_PY_CONST(md, "KMOD_CAPS", KMOD_CAPS);
    MAKE_PY_CONST(md, "KMOD_MODE", KMOD_MODE);
    MAKE_PY_CONST(md, "KMOD_RESERVED", KMOD_RESERVED);

    MAKE_PY_CONST(md, "KMOD_CTRL", KMOD_CTRL);
    MAKE_PY_CONST(md, "KMOD_SHIFT", KMOD_SHIFT);
    MAKE_PY_CONST(md, "KMOD_ALT", KMOD_ALT);
    MAKE_PY_CONST(md, "KMOD_META", KMOD_META);

    MAKE_PY_CONST(md, "ANYFORMAT", SDL_ANYFORMAT);
    MAKE_PY_CONST(md, "ASYNCBLIT", SDL_ASYNCBLIT);
    MAKE_PY_CONST(md, "DOUBLEBUF", SDL_DOUBLEBUF);
    MAKE_PY_CONST(md, "HWACCEL", SDL_HWACCEL);
    MAKE_PY_CONST(md, "HWPALETTE", SDL_HWPALETTE);
    MAKE_PY_CONST(md, "HWSURFACE", SDL_HWSURFACE);
    MAKE_PY_CONST(md, "FULLSCREEN", SDL_FULLSCREEN);
    MAKE_PY_CONST(md, "OPENGL", SDL_OPENGL);
    MAKE_PY_CONST(md, "OPENGLBLIT", SDL_OPENGLBLIT);
    MAKE_PY_CONST(md, "RESIZABLE", SDL_RESIZABLE);
    MAKE_PY_CONST(md, "RLEACCEL", SDL_RLEACCEL);
    MAKE_PY_CONST(md, "SRCALPHA", SDL_SRCALPHA);
    MAKE_PY_CONST(md, "SRCCOLORKEY", SDL_SRCCOLORKEY);
    MAKE_PY_CONST(md, "SWSURFACE", SDL_SWSURFACE);
    MAKE_PY_CONST(md, "PREALLOC", SDL_PREALLOC);

    /* create sdl_VersionInfoType type */
    if (PyType_Ready(&sdl_VersionInfoType) < 0)
    {
        fprintf(stderr, "Error initializing sdl_VersionInfoType\n");
        return;
    }

    Py_INCREF(&sdl_VersionInfoType);
    PyModule_AddObject(m, "VersionInfo", (PyObject *) &sdl_VersionInfoType);

    /* create sdl_ColorType type */
    if (PyType_Ready(&sdl_ColorType) < 0)
    {
        fprintf(stderr, "Error initializing sdl_ColorType\n");
        return;
    }

    Py_INCREF(&sdl_ColorType);
    PyModule_AddObject(m, "Color", (PyObject *) &sdl_ColorType);
}
