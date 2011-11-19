
#include "Event.h"

static PyObject *
sdl_Event_init_subevent(int subevent_type, PyObject *toplevel);

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

#define ATTR_ERR() do { \
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
            ATTR_ERR();
            break;
        case SDL_ACTIVEEVENT:
            if (!strcmp(aname, "gain"))
                self->event->active.gain = v;
            else if (!strcmp(aname, "state"))
                self->event->active.state = v;
            else
                ATTR_ERR();
            break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            if (!strcmp(aname, "state"))
                self->event->key.state = v;
            else
                ATTR_ERR();
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
                ATTR_ERR();
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
                ATTR_ERR();
            break;
        case SDL_JOYAXISMOTION:
            if (!strcmp(aname, "which"))
                self->event->jaxis.which = v;
            else if (!strcmp(aname, "axis"))
                self->event->jaxis.axis = v;
            else if (!strcmp(aname, "value"))
                self->event->jaxis.value = v;
            else
                ATTR_ERR();
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
                ATTR_ERR();
            break;
        case SDL_JOYHATMOTION:
            if (!strcmp(aname, "which"))
                self->event->jhat.which = v;
            else if (!strcmp(aname, "hat"))
                self->event->jhat.hat = v;
            else if (!strcmp(aname, "value"))
                self->event->jhat.value = v;
            else
                ATTR_ERR();
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
                ATTR_ERR();
            break;
        case SDL_VIDEORESIZE:
            if (!strcmp(aname, "w"))
                self->event->resize.w = v;
            else if (!strcmp(aname, "h"))
                self->event->resize.h = v;
            else
                ATTR_ERR();
            break;
        case SDL_VIDEOEXPOSE:
            ATTR_ERR();
            break;
        case SDL_QUIT:
            ATTR_ERR();
            break;
        case SDL_USEREVENT:
            if (!strcmp(aname, "code"))
                self->event->user.code = v;
            else
                ATTR_ERR();
            break;
        case SDL_SYSWMEVENT:
            ATTR_ERR();
            break;
    }
    return 0;
}

static PyObject *
sdl_Event_getattro(sdl_Event *self, PyObject *attr)
{
    if (!PyString_Check(attr))
    {
        PyErr_SetString(PyExc_AttributeError, "Invalid attribute name");
        return NULL;
    }
    const char *aname = PyString_AsString(attr);
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

