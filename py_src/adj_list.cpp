#include "adj_list.hpp"

PyObject *AdjList_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    AdjListObject *self;
    self = (AdjListObject *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->adjlist = nullptr;
    }
    return (PyObject *)self;
}

int AdjList_init(AdjListObject *self, PyObject *args, PyObject *kwds) {
    self->adjlist = make_unique<stella::AdjList<stella::Node, stella::Edge>>();
    return 0;
}

void AdjList_dealloc(AdjListObject* self) {
    Py_TYPE(self)->tp_free((PyObject *) self);
}

PyObject* AdjList_addNode(AdjListObject* self, PyObject* args) {
    PyObject* arg;
    if (PyArg_ParseTuple(args, "s", &arg)) {
        try {
            ((AdjListObject*) (self))->adjlist->addNode(make_shared<stella::Node>((const char* )arg));
            Py_RETURN_NONE;
        } catch (std::invalid_argument ex) {
            PyErr_SetString(PyExc_RuntimeError, ex.what());
            return NULL;
        }
    }

    PyErr_Clear();

    if (PyArg_ParseTuple(args, "O", &arg)) {
        if (!PyObject_IsInstance(arg, (PyObject* )&NodeType)) {
            PyErr_SetString(PyExc_TypeError, "Object must be of Node type");
            return NULL;
        }
        NodeObject* nodeObj = (NodeObject*) arg;
        shared_ptr<stella::Node>* node = nodeObj->node;
        nodeObj->isOwner = false;
        try {
            ((AdjListObject*) (self))->adjlist->addNode(*node);
            Py_RETURN_NONE;
        } catch (std::invalid_argument ex) {
            PyErr_SetString(PyExc_RuntimeError, ex.what());
            return NULL;
        }
    } else {
        PyErr_SetString(PyExc_TypeError, "Invalid arguments for add_node, expected Node object or str");
        return NULL;
    }
}

PyObject* AdjList_addEdge(AdjListObject* self, PyObject* args) {
    PyObject* arg;
    const char* label;
    const char* n1_label;
    const char* n2_label;
    int weight = 1;


    if (PyArg_ParseTuple(args, "sss|i", &label, &n1_label, &n2_label, &weight)) {
        try {
            ((AdjListObject *) (self))->adjlist->addEdge(label, n1_label, n2_label, weight);
            Py_RETURN_NONE;
        } catch (std::invalid_argument ex) {
            PyErr_SetString(PyExc_RuntimeError, ex.what());
            return NULL;
        }
    }

    PyErr_Clear();

    if (PyArg_ParseTuple(args, "O", &arg)) {
        if (PyObject_IsInstance(arg, (PyObject* )&DirectedEdgeType)) {
            PyErr_SetString(PyExc_TypeError, "Object must be of non-directed Edge type");
            return NULL;
        }
        if (!PyObject_IsInstance(arg, (PyObject* )&EdgeType)) {
            PyErr_SetString(PyExc_TypeError, "Object must be of Edge type");
            return NULL;
        }

        EdgeObject* edgeObject = (EdgeObject*) arg;
        try {
            ((AdjListObject*) (self))->adjlist->addEdge(*(edgeObject->edge));
            Py_RETURN_NONE;
        } catch (std::invalid_argument ex) {
            PyErr_SetString(PyExc_RuntimeError, ex.what());
            return NULL;
        }
    }

    PyErr_SetString(PyExc_TypeError, "Invalid arguments for add_edge. Expected Edge object or (str, str, str, [int]).");
    return NULL;
}

PyObject* AdjList_getEdge(AdjListObject* self, PyObject* args) {
    const char* label;
    if (!PyArg_ParseTuple(args, "s", &label)) {
        PyErr_SetString(PyExc_ValueError, "No argument for get_edge, str expected");
        return NULL;
    }

    shared_ptr<stella::Edge> edge = self->adjlist->getEdge(label);
    if (edge) {
        EdgeObject* pyEdge = PyObject_New(EdgeObject, &EdgeType);
        if (!pyEdge)
            PyErr_NoMemory();
        pyEdge->edge = new shared_ptr<stella::Edge>(edge);
        return (PyObject *) pyEdge;
    }

    Py_RETURN_NONE;
}

PyObject* AdjList_getNode(AdjListObject* self, PyObject* args) {
    const char* label;
    if (!PyArg_ParseTuple(args, "s", &label)) {
        PyErr_SetString(PyExc_ValueError, "No argument for get_node, str expected");
        return NULL;
    }

    shared_ptr<stella::Node> node = self->adjlist->getNode(label);
    if (node) {

        NodeObject* node_obj = PyObject_New(NodeObject, &NodeType);
        if (!node_obj) {
            return PyErr_NoMemory();
        }
        node_obj->node = new shared_ptr<stella::Node>(node);

        return (PyObject*)node_obj;
    }

    Py_RETURN_NONE;
}

PyObject* AdjList_getAllNodes(AdjListObject* self, PyObject* args) {

    std::vector<shared_ptr<stella::Node>>& nodes = ((AdjListObject*) (self))->adjlist->getAllNodes();

    PyObject* pyNodes = PyList_New(0);
    if (!pyNodes) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to create Python list");
        return NULL;
    }

    for (size_t i = 0; i < nodes.size(); ++i) {
        NodeObject* pyNode = PyObject_New(NodeObject, &NodeType);
        if (!pyNode) {
            return PyErr_NoMemory();
        }
        pyNode->node = new shared_ptr<stella::Node>(nodes[i]);
        if (!pyNode) {
            Py_DECREF(pyNodes);
            PyErr_SetString(PyExc_RuntimeError, "Failed to build Python value");
            return NULL;
        }
        PyList_Append(pyNodes, (PyObject *)pyNode);
    }
    return pyNodes;
}

PyObject* AdjList_getAllEdges(AdjListObject* self, PyObject* args) {
    std::map<std::string, shared_ptr<stella::Edge>>& edges = self->adjlist->getAllEdges();
    PyObject* pyEdges = PyDict_New();
    if (!pyEdges) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to create Python dictionary");
        return NULL;
    }

    for (const auto& pair : edges) {
        PyObject* key = PyUnicode_FromString(pair.first.c_str());
        if (!key) {
            Py_DECREF(pyEdges);
            PyErr_SetString(PyExc_RuntimeError, "Failed to convert C++ key to Python string");
            return NULL;
        }

        EdgeObject* value = PyObject_New(EdgeObject, &EdgeType);
        if (!value) {
            Py_DECREF(key);
            Py_DECREF(pyEdges);
            PyErr_NoMemory();
            return NULL;
        }

        value->edge = new shared_ptr<stella::Edge>(pair.second);

        if (PyDict_SetItem(pyEdges, key, (PyObject *)value) < 0) {
            Py_DECREF(key);
            Py_DECREF(value);
            Py_DECREF(pyEdges);
            PyErr_SetString(PyExc_RuntimeError, "Failed to add item to Python dictionary");
            return NULL;
        }

        Py_DECREF(key);
        Py_DECREF(value);
    }

    return pyEdges;
}

PyGetSetDef AdjList_GetSetDef[] = {
    {"edges", (getter)AdjList_getAllEdges, NULL, "Node label", NULL},
    {"nodes", (getter)AdjList_getAllNodes, NULL, "Node label", NULL},
    {NULL}
};


PyMethodDef AdjList_methods[] = {
    {"add_node", (PyCFunction)AdjList_addNode, METH_VARARGS, "Add a node to the graph."},
    {"add_edge", (PyCFunction)AdjList_addEdge, METH_VARARGS, "Add an edge to the graph."},
    {"get_edge", (PyCFunction)AdjList_getEdge, METH_VARARGS, "Get an edge from the graph."},
    {"get_node", (PyCFunction)AdjList_getNode, METH_VARARGS, "Get a node from the graph."},
    {NULL, NULL, 0, NULL}
};

PyTypeObject AdjListType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "stella.AdjList",          /* tp_name */
    sizeof(AdjListObject),     /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)AdjList_dealloc, /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,        /* tp_flags */
    "AdjList object",          /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    AdjList_methods,           /* tp_methods */
    0,                         /* tp_members */
    AdjList_GetSetDef,        /* tp_getset */
    &GraphType,                 /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)AdjList_init,    /* tp_init */
    0,                         /* tp_alloc */
    AdjList_new,               /* tp_new */
};

PyObject *DirectedAdjList_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    DirectedAdjListObject *self;
    self = (DirectedAdjListObject *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->adjlist = nullptr;
    }
    return (PyObject *)self;
}


int DirectedAdjList_init(DirectedAdjListObject *self, PyObject *args, PyObject *kwds) {
    self->adjlist = make_unique<stella::DirectedAdjList<stella::Node, stella::DirectedEdge>>();
    return 0;
};

PyObject* DirectedAdjList_addEdge(DirectedAdjListObject* self, PyObject* args) {
    PyObject* arg;
    const char* label;
    const char* n1_label;
    const char* n2_label;
    int weight = 1;


    if (PyArg_ParseTuple(args, "sss|i", &label, &n1_label, &n2_label, &weight)) {
        try {
            ((DirectedAdjListObject*) (self))->adjlist->addEdge(label, n1_label, n2_label, weight);
            Py_RETURN_NONE;
        } catch (std::invalid_argument ex) {
            PyErr_SetString(PyExc_RuntimeError, ex.what());
            return NULL;
        }
    }

    PyErr_Clear();

    if (PyArg_ParseTuple(args, "O", &arg)) {
        if (!PyObject_IsInstance(arg, (PyObject* )&DirectedEdgeType)) {
            PyErr_SetString(PyExc_TypeError, "Object must be of DirectedEdge type");
            return NULL;
        }

        DirectedEdgeObject* edgeObject = (DirectedEdgeObject*) arg;
        shared_ptr<stella::DirectedEdge>* edge = edgeObject->edge;
        try {
            ((DirectedAdjListObject*) (self))->adjlist->addEdge(*edge);
            Py_RETURN_NONE;
        } catch (std::invalid_argument ex) {
            PyErr_SetString(PyExc_RuntimeError, ex.what());
            return NULL;
        }
    }

    PyErr_SetString(PyExc_TypeError, "Invalid arguments for add_edge. Expected Edge object or (str, str, str, [int]).");
    return NULL;
}

PyObject* DirectedAdjList_getEdge(DirectedAdjListObject* self, PyObject* args) {
    const char* label;
    if (!PyArg_ParseTuple(args, "s", &label)) {
        PyErr_SetString(PyExc_ValueError, "No argument for get_edge, str expected");
        return NULL;
    }

    shared_ptr<stella::DirectedEdge>* edge = new shared_ptr<stella::DirectedEdge>(self->adjlist->getEdge(label));
    if (edge) {
        DirectedEdgeObject* pyEdge = PyObject_New(DirectedEdgeObject, &DirectedEdgeType);
        if (!pyEdge)
            PyErr_NoMemory();
        pyEdge->edge = edge;
        return (PyObject *) pyEdge;
    }

    Py_RETURN_NONE;
}

PyObject* DirectedAdjList_getAllEdges(DirectedAdjListObject* self, PyObject* args) {
    std::map<std::string, shared_ptr<stella::DirectedEdge>>& edges = self->adjlist->getAllEdges();
    PyObject* pyEdges = PyDict_New();
    if (!pyEdges) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to create Python dictionary");
        return NULL;
    }

    for (const auto& pair : edges) {
        PyObject* key = PyUnicode_FromString(pair.first.c_str());
        if (!key) {
            Py_DECREF(pyEdges);
            PyErr_SetString(PyExc_RuntimeError, "Failed to convert C++ key to Python string");
            return NULL;
        }

        DirectedEdgeObject* value = PyObject_New(DirectedEdgeObject, &DirectedEdgeType);
        if (!value) {
            Py_DECREF(key);
            Py_DECREF(pyEdges);
            PyErr_NoMemory();
            return NULL;
        }

        value->edge = new shared_ptr<stella::DirectedEdge>(pair.second);

        if (PyDict_SetItem(pyEdges, key, (PyObject *)value) < 0) {
            Py_DECREF(key);
            Py_DECREF(value);
            Py_DECREF(pyEdges);
            PyErr_SetString(PyExc_RuntimeError, "Failed to add item to Python dictionary");
            return NULL;
        }

        Py_DECREF(key);
        Py_DECREF(value);
    }

    return pyEdges;
};

PyMethodDef DirectedAdjList_methods[] = {
    {"add_edge", (PyCFunction)DirectedAdjList_addEdge, METH_VARARGS, "Add an edge to the graph."},
    {"get_edge", (PyCFunction)DirectedAdjList_getEdge, METH_VARARGS, "Get an edge from the graph."},
    {NULL, NULL}
};

PyGetSetDef DirectedAdjList_GetSetDef[] = {
    {"edges", (getter)DirectedAdjList_getAllEdges, NULL, "Node label", NULL},
    {NULL}
};

PyTypeObject DirectedAdjListType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "stella.DirectedAdjList",          /* tp_name */
    sizeof(DirectedAdjListObject),     /* tp_basicsize */
    0,                         /* tp_itemsize */
    0,                         /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,        /* tp_flags */
    "DirectedAdjList object",  /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    DirectedAdjList_methods,   /* tp_methods */
    0,                         /* tp_members */
    DirectedAdjList_GetSetDef, /* tp_getset */
    &AdjListType,              /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)DirectedAdjList_init,    /* tp_init */
    0,                         /* tp_alloc */
    DirectedAdjList_new,       /* tp_new */
};