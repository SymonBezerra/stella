#include "adj_matrix.hpp"

PyObject *AdjMatrix_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    AdjMatrixObject *self;
    self = (AdjMatrixObject *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->adjmatrix= nullptr;
    }
    return (PyObject *)self;
}

int AdjMatrix_init(AdjMatrixObject *self, PyObject *args, PyObject *kwds) {
    self->adjmatrix = make_unique<stella::AdjMatrix<stella::Node, stella::Edge>>();
    return 0;
}

void AdjMatrix_dealloc(AdjMatrixObject* self) {
    Py_TYPE(self)->tp_free((PyObject *) self);
}

PyObject* AdjMatrix_addNode(AdjMatrixObject* self, PyObject* args) {
    PyObject* arg;
    if (PyArg_ParseTuple(args, "s", &arg)) {
        try {
            self->adjmatrix->addNode(make_shared<stella::Node>((const char* )arg));
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
            self->adjmatrix->addNode(*node);
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

PyObject* AdjMatrix_addEdge(AdjMatrixObject* self, PyObject* args) {
    PyObject* arg;
    const char* label;
    const char* n1_label;
    const char* n2_label;
    int weight = 1;


    if (PyArg_ParseTuple(args, "sss|i", &label, &n1_label, &n2_label, &weight)) {
        try {
            self->adjmatrix->addEdge(label, n1_label, n2_label, weight);
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
        shared_ptr<stella::Edge>* edge = edgeObject->edge;
        try {
            self->adjmatrix->addEdge(*edge);
            Py_RETURN_NONE;
        } catch (std::invalid_argument ex) {
            PyErr_SetString(PyExc_RuntimeError, ex.what());
            return NULL;
        }
    }

    PyErr_SetString(PyExc_TypeError, "Invalid arguments for add_edge. Expected Edge object or (str, str, str, [int]).");
    return NULL;
}

PyObject* AdjMatrix_getNode(AdjMatrixObject* self, PyObject* args) {
    const char* label;
    if (!PyArg_ParseTuple(args, "s", &label)) {
        PyErr_SetString(PyExc_ValueError, "No argument for get_node, str expected");
        return NULL;
    }

    shared_ptr<stella::Node> node = self->adjmatrix->getNode(label);
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

PyObject* AdjMatrix_getAllNodes(AdjMatrixObject* self, PyObject* args) {

    std::vector<shared_ptr<stella::Node>>& nodes = self->adjmatrix->getAllNodes();

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
        pyNode->isOwner = false;
        if (!pyNode) {
            Py_DECREF(pyNodes);
            PyErr_SetString(PyExc_RuntimeError, "Failed to build Python value");
            return NULL;
        }
        PyList_Append(pyNodes, (PyObject *)pyNode);
    }
    return pyNodes;
}

PyObject* AdjMatrix_getAllEdges(AdjMatrixObject* self, PyObject* args) {
    auto& edges = self->adjmatrix->getAllEdges();

    PyObject* pyEdges = PyList_New(edges.size());
    if (!pyEdges) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to create Python list");
        return NULL;
    }

    // Iterate over each pair in edges
    for (size_t i = 0; i < edges.size(); ++i) {
        PyObject* innerList = PyList_New(edges[i].size());
        if (!innerList) {
            Py_DECREF(pyEdges);
            PyErr_SetString(PyExc_RuntimeError, "Failed to create inner Python list");
            return NULL;
        }

        // Iterate over each map<string, Edge*> in edges[i]
        size_t j = 0;
        for (const auto& edgeMap : edges[i]) {
            PyObject* pyDict = PyDict_New();
            if (!pyDict) {
                Py_DECREF(innerList);
                Py_DECREF(pyEdges);
                PyErr_SetString(PyExc_RuntimeError, "Failed to create Python dictionary");
                return NULL;
            }

            // Populate the dictionary with string keys and Edge objects
            for (const auto& pair : edgeMap) {
                PyObject* key = PyUnicode_FromString(pair.first.c_str());
                if (!key) {
                    Py_DECREF(pyDict);
                    Py_DECREF(innerList);
                    Py_DECREF(pyEdges);
                    PyErr_SetString(PyExc_RuntimeError, "Failed to convert C++ key to Python string");
                    return NULL;
                }

                EdgeObject* value = PyObject_New(EdgeObject, &EdgeType);
                if (!value) {
                    Py_DECREF(key);
                    Py_DECREF(pyDict);
                    Py_DECREF(innerList);
                    Py_DECREF(pyEdges);
                    PyErr_NoMemory();
                    return NULL;
                }

                value->edge = new shared_ptr<stella::Edge>(pair.second);

                if (PyDict_SetItem(pyDict, key, (PyObject*)value) < 0) {
                    Py_DECREF(key);
                    Py_DECREF(value);
                    Py_DECREF(pyDict);
                    Py_DECREF(innerList);
                    Py_DECREF(pyEdges);
                    PyErr_SetString(PyExc_RuntimeError, "Failed to add item to Python dictionary");
                    return NULL;
                }

                Py_DECREF(key);
                Py_DECREF(value);
            }

            // Add the dictionary to the inner list
            PyList_SET_ITEM(innerList, j++, pyDict); // Steals reference to pyDict
        }

        // Add the inner list to the main list
        PyList_SET_ITEM(pyEdges, i, innerList); // Steals reference to innerList
    }

    return pyEdges;
}

PyGetSetDef AdjMatrix_GetSetDef[] = {
    {"edges", (getter)AdjMatrix_getAllEdges, NULL, "Node label", NULL},
    {"nodes", (getter)AdjMatrix_getAllNodes, NULL, "Node label", NULL},
    {NULL}
};


PyMethodDef AdjMatrix_methods[] = {
    {"add_node", (PyCFunction)AdjMatrix_addNode, METH_VARARGS, "Add a node to the graph."},
    {"add_edge", (PyCFunction)AdjMatrix_addEdge, METH_VARARGS, "Add an edge to the graph."},
    {"get_node", (PyCFunction)AdjMatrix_getNode, METH_VARARGS, "Get a node from the graph."},
    {NULL, NULL, 0, NULL}
};

PyTypeObject AdjMatrixType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "stella.AdjMatrix",          /* tp_name */
    sizeof(AdjMatrixObject),     /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)AdjMatrix_dealloc, /* tp_dealloc */
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
    "AdjMatrix object",          /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    AdjMatrix_methods,           /* tp_methods */
    0,                         /* tp_members */
    AdjMatrix_GetSetDef,        /* tp_getset */
    &GraphType,                 /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)AdjMatrix_init,    /* tp_init */
    0,                         /* tp_alloc */
    AdjMatrix_new,               /* tp_new */
};

PyObject *DirectedAdjMatrix_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    DirectedAdjMatrixObject *self;
    self = (DirectedAdjMatrixObject *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->adjmatrix = nullptr;
    }
    return (PyObject *)self;
}


int DirectedAdjMatrix_init(DirectedAdjMatrixObject *self, PyObject *args, PyObject *kwds) {
    self->adjmatrix =  make_shared<stella::DirectedAdjMatrix<stella::Node, stella::DirectedEdge>>();
    return 0;
};

PyObject* DirectedAdjMatrix_addNode(DirectedAdjMatrixObject* self, PyObject* args) {
    PyObject* arg;
    if (PyArg_ParseTuple(args, "s", &arg)) {
        try {
            self->adjmatrix->addNode(make_shared<stella::Node>((const char* )arg));
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
            self->adjmatrix->addNode(*node);
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

PyObject* DirectedAdjMatrix_addEdge(DirectedAdjMatrixObject* self, PyObject* args) {
    PyObject* arg;
    const char* label;
    const char* n1_label;
    const char* n2_label;
    int weight = 1;


    if (PyArg_ParseTuple(args, "sss|i", &label, &n1_label, &n2_label, &weight)) {
        try {
            self->adjmatrix->addEdge(label, n1_label, n2_label, weight);
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
            self->adjmatrix->addEdge(*edge);
            Py_RETURN_NONE;
        } catch (std::invalid_argument ex) {
            PyErr_SetString(PyExc_RuntimeError, ex.what());
            return NULL;
        }
    }

    PyErr_SetString(PyExc_TypeError, "Invalid arguments for add_edge. Expected Edge object or (str, str, str, [int]).");
    return NULL;
}

PyObject* DirectedAdjMatrix_getAllEdges(DirectedAdjMatrixObject* self, PyObject* args) {
    auto& edges = self->adjmatrix->getAllEdges();

    PyObject* pyEdges = PyList_New(edges.size());
    if (!pyEdges) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to create Python list");
        return NULL;
    }

    // Iterate over each pair in edges
    for (size_t i = 0; i < edges.size(); ++i) {
        PyObject* innerList = PyList_New(edges[i].size());
        if (!innerList) {
            Py_DECREF(pyEdges);
            PyErr_SetString(PyExc_RuntimeError, "Failed to create inner Python list");
            return NULL;
        }

        // Iterate over each map<string, Edge*> in edges[i]
        size_t j = 0;
        for (const auto& edgeMap : edges[i]) {
            PyObject* pyDict = PyDict_New();
            if (!pyDict) {
                Py_DECREF(innerList);
                Py_DECREF(pyEdges);
                PyErr_SetString(PyExc_RuntimeError, "Failed to create Python dictionary");
                return NULL;
            }

            // Populate the dictionary with string keys and Edge objects
            for (const auto& pair : edgeMap) {
                PyObject* key = PyUnicode_FromString(pair.first.c_str());
                if (!key) {
                    Py_DECREF(pyDict);
                    Py_DECREF(innerList);
                    Py_DECREF(pyEdges);
                    PyErr_SetString(PyExc_RuntimeError, "Failed to convert C++ key to Python string");
                    return NULL;
                }

                DirectedEdgeObject* value = PyObject_New(DirectedEdgeObject, &DirectedEdgeType);
                if (!value) {
                    Py_DECREF(key);
                    Py_DECREF(pyDict);
                    Py_DECREF(innerList);
                    Py_DECREF(pyEdges);
                    PyErr_NoMemory();
                    return NULL;
                }

                value->edge = new shared_ptr<stella::DirectedEdge>(pair.second);

                if (PyDict_SetItem(pyDict, key, (PyObject*)value) < 0) {
                    Py_DECREF(key);
                    Py_DECREF(value);
                    Py_DECREF(pyDict);
                    Py_DECREF(innerList);
                    Py_DECREF(pyEdges);
                    PyErr_SetString(PyExc_RuntimeError, "Failed to add item to Python dictionary");
                    return NULL;
                }

                Py_DECREF(key);
                Py_DECREF(value);
            }

            // Add the dictionary to the inner list
            PyList_SET_ITEM(innerList, j++, pyDict); // Steals reference to pyDict
        }

        // Add the inner list to the main list
        PyList_SET_ITEM(pyEdges, i, innerList); // Steals reference to innerList
    }

    return pyEdges;
}

PyGetSetDef DirectedAdjMatrix_GetSetDef[] = {
    {"edges", (getter)DirectedAdjMatrix_getAllEdges, NULL, "Node label", NULL},
    {NULL, NULL}
};

PyMethodDef DirectedAdjMatrix_methods[] = {
    {"add_node", (PyCFunction)DirectedAdjMatrix_addNode, METH_VARARGS, "Add a node to the graph."},
    {"add_edge", (PyCFunction)DirectedAdjMatrix_addEdge, METH_VARARGS, "Add an edge to the graph."},
    {NULL, NULL}
};

PyTypeObject DirectedAdjMatrixType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "stella.DirectedAdjMatrix",          /* tp_name */
    sizeof(DirectedAdjMatrixObject),     /* tp_basicsize */
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
    "DirectedAdjMatrix object",  /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    DirectedAdjMatrix_methods, /* tp_methods */
    0,                         /* tp_members */
    DirectedAdjMatrix_GetSetDef,                         /* tp_getset */
    &AdjMatrixType,            /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)DirectedAdjMatrix_init,    /* tp_init */
    0,                         /* tp_alloc */
    DirectedAdjMatrix_new,       /* tp_new */
};