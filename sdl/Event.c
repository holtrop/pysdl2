
#include "Event.h"
#include "keysym.h"

/* Forward declarations */
static PyObject *
sdl_Event_init_subevent(int subevent_type, PyObject *toplevel);
static PyObject *
sdl_Event_getattro(sdl_Event *self, PyObject *attr);

/* init is called when an Event object is constructed by the user from Python.
 * So, we are going to create a top-level Event object and allocate memory
 * for an SDL_Event structure which we will own. */
static int
sdl_Event_init(sdl_Event *self, PyObject *args, PyObject *kwargs)
{
    self->event = calloc(1, sizeof(SDL_Event));
    self->subevent_type = SDL_NOEVENT; /* this is the top-level event object */
    self->toplevel = NULL;
    return 0;
}

#define SETATTR_ERR() do { \
    PyErr_SetString(PyExc_AttributeError, "Invalid attribute"); \
    return -1; \
} while (0)

static int
sdl_Event_setattro(sdl_Event *self, PyObject *attr, PyObject *val)
{
    if (!PyString_Check(attr))
    {
        PyErr_SetString(PyExc_AttributeError, "Invalid attribute name");
        return -1;
    }
    const char *aname = PyString_AsString(attr);
    if (!PyInt_Check(val))
    {
        PyErr_SetString(PyExc_ValueError, "Invalid parameter");
        return -1;
    }
    long v = PyInt_AsLong(val);

    if (!strcmp(aname, "type"))
    {
        self->event->type = v;
        return 0;
    }
    switch (self->subevent_type)
    {
        case SDL_NOEVENT:       /* top-level event object */
            SETATTR_ERR();
            break;
        case SDL_ACTIVEEVENT:
            if (!strcmp(aname, "gain"))
                self->event->active.gain = v;
            else if (!strcmp(aname, "state"))
                self->event->active.state = v;
            else
                SETATTR_ERR();
            break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            if (!strcmp(aname, "state"))
                self->event->key.state = v;
            else
                SETATTR_ERR();
            break;
        case SDL_MOUSEMOTION:
            if (!strcmp(aname, "state"))
                self->event->motion.state = v;
            else if (!strcmp(aname, "x"))
                self->event->motion.x = v;
            else if (!strcmp(aname, "y"))
                self->event->motion.y = v;
            else if (!strcmp(aname, "xrel"))
                self->event->motion.xrel = v;
            else if (!strcmp(aname, "yrel"))
                self->event->motion.yrel = v;
            else
                SETATTR_ERR();
            break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            if (!strcmp(aname, "which"))
                self->event->button.which = v;
            else if (!strcmp(aname, "button"))
                self->event->button.button = v;
            else if (!strcmp(aname, "state"))
                self->event->button.state = v;
            else if (!strcmp(aname, "x"))
                self->event->button.x = v;
            else if (!strcmp(aname, "y"))
                self->event->button.y = v;
            else
                SETATTR_ERR();
            break;
        case SDL_JOYAXISMOTION:
            if (!strcmp(aname, "which"))
                self->event->jaxis.which = v;
            else if (!strcmp(aname, "axis"))
                self->event->jaxis.axis = v;
            else if (!strcmp(aname, "value"))
                self->event->jaxis.value = v;
            else
                SETATTR_ERR();
            break;
        case SDL_JOYBALLMOTION:
            if (!strcmp(aname, "which"))
                self->event->jball.which = v;
            else if (!strcmp(aname, "ball"))
                self->event->jball.ball = v;
            else if (!strcmp(aname, "xrel"))
                self->event->jball.xrel = v;
            else if (!strcmp(aname, "yrel"))
                self->event->jball.yrel = v;
            else
                SETATTR_ERR();
            break;
        case SDL_JOYHATMOTION:
            if (!strcmp(aname, "which"))
                self->event->jhat.which = v;
            else if (!strcmp(aname, "hat"))
                self->event->jhat.hat = v;
            else if (!strcmp(aname, "value"))
                self->event->jhat.value = v;
            else
                SETATTR_ERR();
            break;
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            if (!strcmp(aname, "which"))
                self->event->jbutton.which = v;
            else if (!strcmp(aname, "button"))
                self->event->jbutton.button = v;
            else if (!strcmp(aname, "state"))
                self->event->jbutton.state = v;
            else
                SETATTR_ERR();
            break;
        case SDL_VIDEORESIZE:
            if (!strcmp(aname, "w"))
                self->event->resize.w = v;
            else if (!strcmp(aname, "h"))
                self->event->resize.h = v;
            else
                SETATTR_ERR();
            break;
        case SDL_VIDEOEXPOSE:
            SETATTR_ERR();
            break;
        case SDL_QUIT:
            SETATTR_ERR();
            break;
        case SDL_USEREVENT:
            if (!strcmp(aname, "code"))
                self->event->user.code = v;
            else
                SETATTR_ERR();
            break;
        case SDL_SYSWMEVENT:
            SETATTR_ERR();
            break;
        default:
            PyErr_SetString(PyExc_SystemError, "Invalid subevent_type");
            return -1;
    }
    return 0;
}

static void
sdl_Event_dealloc(sdl_Event *self)
{
    if (self->subevent_type == SDL_NOEVENT)
    {
        /* we're deallocating the top-level event object */
        free(self->event);
    }
    else
    {
        /* we're deallocating a subevent object */
        Py_DECREF(self->toplevel);
    }
}

PyTypeObject sdl_EventType = {
    PyObject_HEAD_INIT(NULL)
    0,                              /* ob_size */
    "SDL.Event",                    /* tp_name */
    sizeof(sdl_Event),              /* tp_basicsize */
    0,                              /* tp_itemsize */
    (destructor)sdl_Event_dealloc,  /* tp_dealloc */
    0,                              /* tp_print */
    0,                              /* tp_getattr */
    0,                              /* tp_setattr */
    0,                              /* tp_compare */
    0,                              /* tp_repr */
    0,                              /* tp_as_number */
    0,                              /* tp_as_sequence */
    0,                              /* tp_as_mapping */
    0,                              /* tp_hash  */
    0,                              /* tp_call */
    0,                              /* tp_str */
    (getattrofunc)sdl_Event_getattro,/* tp_getattro */
    (setattrofunc)sdl_Event_setattro,/* tp_setattro */
    0,                              /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   /* tp_flags */
    "SDL Event Structure",          /* tp_doc */
    0,                              /* tp_traverse */
    0,                              /* tp_clear */
    0,                              /* tp_richcompare */
    0,                              /* tp_weaklistoffset */
    0,                              /* tp_iter */
    0,                              /* tp_iternext */
    0,                              /* tp_methods */
    0,                              /* tp_members */
    0,                              /* tp_getset */
    0,                              /* tp_base */
    0,                              /* tp_dict */
    0,                              /* tp_descr_get */
    0,                              /* tp_descr_set */
    0,                              /* tp_dictoffset */
    (initproc)sdl_Event_init,       /* tp_init */
    0,                              /* tp_alloc */
    PyType_GenericNew,              /* tp_new */
};

#define GETATTR_ERR() do { \
    PyErr_SetString(PyExc_AttributeError, "Invalid attribute"); \
    return NULL; \
} while (0)

#define RET_SUBEVENT(toplevel_o, subtype) \
    return sdl_Event_init_subevent(subtype, (PyObject *) toplevel_o)

static PyObject *
sdl_Event_getattro(sdl_Event *self, PyObject *attr)
{
    if (!PyString_Check(attr))
    {
        PyErr_SetString(PyExc_AttributeError, "Invalid attribute name");
        return NULL;
    }
    const char *aname = PyString_AsString(attr);
    if (!strcmp(aname, "type"))
        return Py_BuildValue("I", self->event->type);
    switch (self->subevent_type)
    {
        case SDL_NOEVENT:       /* top-level event object */
            if (!strcmp(aname, "active"))
                RET_SUBEVENT(self, SDL_ACTIVEEVENT);
            else if (!strcmp(aname, "key"))
                RET_SUBEVENT(self, SDL_KEYDOWN);
            else if (!strcmp(aname, "motion"))
                RET_SUBEVENT(self, SDL_MOUSEMOTION);
            else if (!strcmp(aname, "button"))
                RET_SUBEVENT(self, SDL_MOUSEBUTTONDOWN);
            else if (!strcmp(aname, "jaxis"))
                RET_SUBEVENT(self, SDL_JOYAXISMOTION);
            else if (!strcmp(aname, "jball"))
                RET_SUBEVENT(self, SDL_JOYBALLMOTION);
            else if (!strcmp(aname, "jhat"))
                RET_SUBEVENT(self, SDL_JOYHATMOTION);
            else if (!strcmp(aname, "jbutton"))
                RET_SUBEVENT(self, SDL_JOYBUTTONDOWN);
            else if (!strcmp(aname, "resize"))
                RET_SUBEVENT(self, SDL_VIDEORESIZE);
            else if (!strcmp(aname, "expose"))
                RET_SUBEVENT(self, SDL_VIDEOEXPOSE);
            else if (!strcmp(aname, "quit"))
                RET_SUBEVENT(self, SDL_QUIT);
            else if (!strcmp(aname, "user"))
                RET_SUBEVENT(self, SDL_USEREVENT);
            else
                GETATTR_ERR();
            break;
        case SDL_ACTIVEEVENT:
            if (!strcmp(aname, "gain"))
                return Py_BuildValue("I", self->event->active.gain);
            else if (!strcmp(aname, "state"))
                return Py_BuildValue("I", self->event->active.state);
            else
                GETATTR_ERR();
            break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            if (!strcmp(aname, "state"))
                return Py_BuildValue("I", self->event->key.state);
            else if (!strcmp(aname, "keysym"))
                return sdl_keysym_from_SDL_keysym_and_event(
                        &self->event->key.keysym, self);
            else
                GETATTR_ERR();
            break;
        case SDL_MOUSEMOTION:
            if (!strcmp(aname, "state"))
                return Py_BuildValue("I", self->event->motion.state);
            else if (!strcmp(aname, "x"))
                return Py_BuildValue("I", self->event->motion.x);
            else if (!strcmp(aname, "y"))
                return Py_BuildValue("I", self->event->motion.y);
            else if (!strcmp(aname, "xrel"))
                return Py_BuildValue("I", self->event->motion.xrel);
            else if (!strcmp(aname, "yrel"))
                return Py_BuildValue("I", self->event->motion.yrel);
            else
                GETATTR_ERR();
            break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            if (!strcmp(aname, "which"))
                return Py_BuildValue("I", self->event->button.which);
            else if (!strcmp(aname, "button"))
                return Py_BuildValue("I", self->event->button.button);
            else if (!strcmp(aname, "state"))
                return Py_BuildValue("I", self->event->button.state);
            else if (!strcmp(aname, "x"))
                return Py_BuildValue("I", self->event->button.x);
            else if (!strcmp(aname, "y"))
                return Py_BuildValue("I", self->event->button.y);
            else
                GETATTR_ERR();
            break;
        case SDL_JOYAXISMOTION:
            if (!strcmp(aname, "which"))
                return Py_BuildValue("I", self->event->jaxis.which);
            else if (!strcmp(aname, "axis"))
                return Py_BuildValue("I", self->event->jaxis.axis);
            else if (!strcmp(aname, "value"))
                return Py_BuildValue("i", self->event->jaxis.value);
            else
                GETATTR_ERR();
            break;
        case SDL_JOYBALLMOTION:
            if (!strcmp(aname, "which"))
                return Py_BuildValue("I", self->event->jball.which);
            else if (!strcmp(aname, "ball"))
                return Py_BuildValue("I", self->event->jball.ball);
            else if (!strcmp(aname, "xrel"))
                return Py_BuildValue("i", self->event->jball.xrel);
            else if (!strcmp(aname, "yrel"))
                return Py_BuildValue("i", self->event->jball.yrel);
            else
                GETATTR_ERR();
            break;
        case SDL_JOYHATMOTION:
            if (!strcmp(aname, "which"))
                return Py_BuildValue("I", self->event->jhat.which);
            else if (!strcmp(aname, "hat"))
                return Py_BuildValue("I", self->event->jhat.hat);
            else if (!strcmp(aname, "value"))
                return Py_BuildValue("I", self->event->jhat.value);
            else
                GETATTR_ERR();
            break;
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            if (!strcmp(aname, "which"))
                Py_BuildValue("I", self->event->jbutton.which);
            else if (!strcmp(aname, "button"))
                Py_BuildValue("I", self->event->jbutton.button);
            else if (!strcmp(aname, "state"))
                Py_BuildValue("I", self->event->jbutton.state);
            else
                GETATTR_ERR();
            break;
        case SDL_VIDEORESIZE:
            if (!strcmp(aname, "w"))
                return Py_BuildValue("i", self->event->resize.w);
            else if (!strcmp(aname, "h"))
                return Py_BuildValue("i", self->event->resize.h);
            else
                GETATTR_ERR();
            break;
        case SDL_VIDEOEXPOSE:
            GETATTR_ERR();
            break;
        case SDL_QUIT:
            GETATTR_ERR();
            break;
        case SDL_USEREVENT:
            if (!strcmp(aname, "code"))
                return Py_BuildValue("i", self->event->user.code);
            else
                GETATTR_ERR();
            break;
        case SDL_SYSWMEVENT:
            GETATTR_ERR();
            break;
    }
    PyErr_SetString(PyExc_SystemError, "Invalid subevent_type");
    return NULL;
}

void sdl_Event_register_type(PyObject *module)
{
    if (PyType_Ready(&sdl_EventType) < 0)
    {
        fprintf(stderr, "Error initializing sdl_EventType\n");
        return;
    }

    Py_INCREF(&sdl_EventType);
    PyModule_AddObject(module, "Event", (PyObject *) &sdl_EventType);
}

PyObject *sdl_Event_get_type(void)
{
    return (PyObject *) &sdl_EventType;
}

SDL_Event *sdl_Event_get_SDL_Event(PyObject *event)
{
    return ((sdl_Event *) event)->event;
}

/* This function steals ownership of the event pointer */
PyObject *sdl_Event_from_SDL_Event(SDL_Event *event)
{
    sdl_Event *sdl_event = PyObject_New(sdl_Event, &sdl_EventType);
    return (PyObject *) sdl_event;
}

/* Create a sdl_Event python object for a "subevent" */
static PyObject *
sdl_Event_init_subevent(int subevent_type, PyObject *toplevel)
{
    sdl_Event *sdl_event = PyObject_New(sdl_Event, &sdl_EventType);
    sdl_event->subevent_type = subevent_type;
    sdl_event->toplevel = toplevel;
    sdl_event->event = sdl_Event_get_SDL_Event(toplevel);
    Py_INCREF(toplevel);
    return (PyObject *) sdl_event;
}

