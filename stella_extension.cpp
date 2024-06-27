/*
    Module extension developed for CPython (Python's default implementation).
    Stella (tribute to "Stella Maris", an ancient Latin title for the Blessed Virgin Mary)
    is a library for teaching Graph Theory on Computer Science courses,
    and other programming related subjects. It is meant to be efficient on performance,
    using C++ as its base, but also comes with a Python wrapper.
    One of its core features is the individuality of nodes, edges, and graphs
    as object-oriented implementations, favouring a comprehensible and extensible code.
    Made by Symon Bezerra and Henrique do Nascimento Cunha. Published under MIT license.
    Based on Bibgrafo library, by Henrique do Nascimento Cunha.

    This code was made with aid of ChatGPT.

    Copyright © 2021 Henrique do Nascimento Cunha

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software
    and associated documentation files (the “Software”), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge, publish, distribute,
    sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
    is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies
    or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
    INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
    PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
    FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
    ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.

*/

#define PY_SSIZE_T_CLEAN
#include <python3.11/Python.h>
#include <exception>
#include <sstream>
#include "stella.hpp"

typedef struct {
    PyObject_HEAD
    stella::Node *node;
    bool isOwner = true;
} NodeObject;

static void Node_dealloc(NodeObject *self) {
    if (self->isOwner) delete self->node;
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyObject *Node_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    NodeObject *self;
    self = (NodeObject *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->node = nullptr;
    }
    return (PyObject *)self;
}

static int Node_init(NodeObject *self, PyObject *args, PyObject *kwds) {
    const char *label;
    if (!PyArg_ParseTuple(args, "s", &label)) {
        return -1;
    }
    self->node = new stella::Node(label);
    return 0;
}

static PyObject *Node_label(NodeObject *self, void *closure) {
    return PyUnicode_FromString(self->node->getLabel().c_str());
}

static PyGetSetDef Node_properties[] = {
    {"label", (getter)Node_label, NULL, "Node label", NULL},
    {NULL}
};

static PyObject *Node_str(NodeObject *self) {
    return PyUnicode_FromString(self->node->getLabel().c_str());
}

static PyTypeObject NodeType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "stella.Node",             /* tp_name */
    sizeof(NodeObject),        /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)Node_dealloc,  /* tp_dealloc */
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
    (reprfunc)Node_str,        /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,        /* tp_flags */
    "Node object",             /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    0,                         /* tp_methods */
    0,                         /* tp_members */
    Node_properties,           /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Node_init,       /* tp_init */
    0,                         /* tp_alloc */
    Node_new,                  /* tp_new */
};

// Define a struct to represent the Python object for BaseEdge
typedef struct {
    PyObject_HEAD
    stella::BaseEdge *edge;
    bool isOwner = false;
} BaseEdgeObject;

typedef struct {
    PyObject_HEAD
    stella::Edge *edge;
    bool isOwner = false;
} EdgeObject;

typedef struct {
    PyObject_HEAD
    stella::DirectedEdge *edge;
    bool isOwner = false;
} DirectedEdgeObject;

static void BaseEdge_dealloc(BaseEdgeObject *self) {
    if (self->isOwner) delete self->edge;
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyObject *BaseEdge_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    BaseEdgeObject *self;
    self = (BaseEdgeObject *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->edge = nullptr;
    }
    return (PyObject *)self;
}

static int BaseEdge_init(BaseEdgeObject *self, PyObject *args, PyObject *kwds) {
    const char *label;
    NodeObject *n1_obj, *n2_obj;
    int weight = 1;
    if (!PyArg_ParseTuple(args, "sOO|i", &label, &n1_obj, &n2_obj, &weight)) {
        return -1;
    }
    stella::Node *n1 = (stella::Node *) n1_obj->node;
    stella::Node *n2 = (stella::Node *) n2_obj->node;
    self->edge = new stella::BaseEdge(label, n1, n2, weight);
    return 0;
}

static PyObject *BaseEdge_str(BaseEdgeObject *self) {
    std::ostringstream oss;
    oss << self->edge;
    return PyUnicode_FromString(oss.str().c_str());
}

static PyObject *BaseEdge_label(BaseEdgeObject *self) {
    return PyUnicode_FromString(self->edge->getLabel().c_str());
}

static PyObject *BaseEdge_n1(BaseEdgeObject *self) {
    stella::Node* node = (stella::Node*) self->edge->getN1();
    NodeObject* nodeObject = PyObject_New(NodeObject, &NodeType);
    nodeObject->node = node;
    nodeObject->isOwner = false;
    return (PyObject *) nodeObject;
}

static PyObject *BaseEdge_n2(BaseEdgeObject *self) {
    stella::Node *node = (stella::Node *) self->edge->getN1();
    NodeObject* nodeObject = PyObject_New(NodeObject, &NodeType);
    nodeObject->node = node;
    nodeObject->isOwner = false;
    return (PyObject *) nodeObject;
}

static PyObject *BaseEdge_weight(BaseEdgeObject* self) {
    int weight = self->edge->getWeight();
    return PyLong_FromLong(weight);
}

static PyObject *Edge_str(EdgeObject *self) {
    std::ostringstream oss;
    oss << self->edge;
    return PyUnicode_FromString(oss.str().c_str());
}

static PyObject *DirectedEdge_str(DirectedEdgeObject *self) {
    std::ostringstream oss;
    oss << self->edge;
    return PyUnicode_FromString(oss.str().c_str());
}

static PyGetSetDef BaseEdge_GetSet[] = {
    {"label", (getter)BaseEdge_label, NULL, "Edge label"},
    {"n1", (getter)BaseEdge_n1, NULL, "Node first node"},
    {"n2", (getter)BaseEdge_n2, NULL, "Node second node"},
    {"weight", (getter)BaseEdge_weight, NULL, "Node weight"},
    {NULL}
};

static PyTypeObject BaseEdgeType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "stella.BaseEdge",         /* tp_name */
    sizeof(BaseEdgeObject),    /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)BaseEdge_dealloc,  /* tp_dealloc */
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
    (reprfunc)BaseEdge_str,    /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_IS_ABSTRACT,        /* tp_flags */
    "BaseEdge object",         /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    0,                         /* tp_methods */
    0,                         /* tp_members */
    BaseEdge_GetSet,           /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)BaseEdge_init,   /* tp_init */
    0,                         /* tp_alloc */
    BaseEdge_new,              /* tp_new */
};

static PyTypeObject EdgeType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "stella.Edge",             /* tp_name */
    sizeof(EdgeObject),        /* tp_basicsize */
    0,                         /* tp_itemsize */
    0,
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
    (reprfunc)Edge_str,        /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,        /* tp_flags */
    "Edge object",             /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    0,                         /* tp_methods */
    0,                         /* tp_members */
    0,                         /* tp_getset */
    &BaseEdgeType,             /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    0,                         /* tp_init */
    0,                         /* tp_alloc */
    0,                         /* tp_new */
};

// DirectedEdge type definition
static PyTypeObject DirectedEdgeType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "stella.DirectedEdge",     /* tp_name */
    sizeof(DirectedEdgeObject),/* tp_basicsize */
    0,                         /* tp_itemsize */
    0,
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
    (reprfunc)DirectedEdge_str,/* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,        /* tp_flags */
    "DirectedEdge object",     /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    0,                         /* tp_methods */
    0,                         /* tp_members */
    0,                         /* tp_getset */
    &EdgeType,                 /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    0,                         /* tp_init */
    0,                         /* tp_alloc */
    0,                         /* tp_new */
};

static PyObject *StellaError;

typedef struct {
    PyObject_HEAD
    stella::Graph<stella::Node, stella::BaseEdge> *graph;
} GraphObject;

typedef struct {
    PyObject_HEAD
    stella::AdjList<stella::Node, stella::Edge> *adjlist;
} AdjListObject;

typedef struct {
    PyObject_HEAD
    stella::DirectedAdjList<stella::Node, stella::DirectedEdge> *adjlist;
} DirectedAdjListObject;

typedef struct {
    PyObject_HEAD
    stella::AdjMatrix<stella::Node, stella::Edge> *adjmatrix;
} AdjMatrixObject;

typedef struct {
    PyObject_HEAD
    stella::DirectedAdjMatrix<stella::Node, stella::DirectedEdge> *adjmatrix;
} DirectedAdjMatrixObject;

static int Graph_init(GraphObject *self, PyObject *args, PyObject *kwds) {
    PyErr_SetString(PyExc_TypeError, "Cannot instantiate abstract class 'Graph'");
    return -1;
}

static void Graph_dealloc(GraphObject *self) {
    delete self->graph;
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyObject* Graph_addNode(PyObject *self, PyObject *args) {
    PyErr_SetString(PyExc_NotImplementedError, "addNode method is abstract and must be implemented in derived classes");
    return NULL;
}

static PyObject* Graph_addEdge(PyObject *self, PyObject *args) {
    PyErr_SetString(PyExc_NotImplementedError, "addEdge method is abstract and must be implemented in derived classes");
    return NULL;
}

static PyObject* Graph_getEdge(PyObject *self, PyObject *args) {
    PyErr_SetString(PyExc_NotImplementedError, "getEdge method is abstract and must be implemented in derived classes");
    return NULL;
}

static PyObject* Graph_getNode(PyObject *self, PyObject *args) {
    PyErr_SetString(PyExc_NotImplementedError, "getNode method is abstract and must be implemented in derived classes");
    return NULL;
}

static PyObject* Graph_getAllNodes(PyObject *self, PyObject *args) {
    PyErr_SetString(PyExc_NotImplementedError, "getAllNodes method is abstract and must be implemented in derived classes");
    return NULL;
}

static PyObject* Graph_getAllEdges(PyObject *self, PyObject *args) {
    PyErr_SetString(PyExc_NotImplementedError, "getAllEdges method is abstract and must be implemented in derived classes");
    return NULL;
}


static PyMethodDef Graph_methods[] = {
    {"addNode", (PyCFunction)Graph_addNode, METH_VARARGS, "Add a node to the graph."},
    {"addEdge", (PyCFunction)Graph_addEdge, METH_VARARGS, "Add an edge to the graph."},
    {"getEdge", (PyCFunction)Graph_getEdge, METH_VARARGS, "Get an edge from the graph."},
    {"getNode", (PyCFunction)Graph_getNode, METH_VARARGS, "Get a node from the graph."},
    {"getAllNodes", (PyCFunction)Graph_getAllNodes, METH_NOARGS, "Get all nodes from the graph."},
    {"getAllEdges", (PyCFunction)Graph_getAllEdges, METH_NOARGS, "Get all edges from the graph."},
    {NULL, NULL, 0, NULL}
};

static PyTypeObject GraphType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "stella.Graph",            /* tp_name */
    sizeof(GraphObject),       /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)Graph_dealloc, /* tp_dealloc */
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
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_IS_ABSTRACT, /* tp_flags */
    "Graph object",            /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    Graph_methods,             /* tp_methods */
    0,                         /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Graph_init,      /* tp_init */
    0,                         /* tp_alloc */
    PyType_GenericNew,         /* tp_new */
};

static PyObject *AdjList_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    AdjListObject *self;
    self = (AdjListObject *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->adjlist = nullptr;
    }
    return (PyObject *)self;
}

static int AdjList_init(AdjListObject *self, PyObject *args, PyObject *kwds) {
    self->adjlist = new stella::AdjList<stella::Node, stella::Edge>();
    return 0;
}

static void AdjList_dealloc(AdjListObject* self) {
    delete self->adjlist;
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject* AdjList_addNode(AdjListObject* self, PyObject* args) {
    PyObject* arg;
    if (PyArg_ParseTuple(args, "s", &arg)) {
        try {
            ((AdjListObject*) (self))->adjlist->addNode(new stella::Node((const char* )arg));
            Py_RETURN_NONE;
        } catch (std::invalid_argument ex) {
            PyErr_SetString(StellaError, ex.what());
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
        stella::Node* node = nodeObj->node;
        nodeObj->isOwner = false;
        try {
            ((AdjListObject*) (self))->adjlist->addNode(node);
            Py_RETURN_NONE;
        } catch (std::invalid_argument ex) {
            PyErr_SetString(StellaError, ex.what());
            return NULL;
        }
    } else {
        PyErr_SetString(PyExc_TypeError, "Invalid arguments for addEdge, expected Node object or str");
        return NULL;
    }
}

static PyObject* AdjList_addEdge(AdjListObject* self, PyObject* args) {
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
            PyErr_SetString(StellaError, ex.what());
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
        stella::Edge* edge = edgeObject->edge;
        edgeObject->isOwner = false;
        try {
            ((AdjListObject*) (self))->adjlist->addEdge(edge);
            Py_RETURN_NONE;
        } catch (std::invalid_argument ex) {
            PyErr_SetString(StellaError, ex.what());
            return NULL;
        }
    }

    PyErr_SetString(PyExc_TypeError, "Invalid arguments for add_edge. Expected Edge object or (str, str, str, [int]).");
    return NULL;
}

static PyObject* DirectedAdjList_addEdge(DirectedAdjListObject* self, PyObject* args) {
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
            PyErr_SetString(StellaError, ex.what());
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
        stella::DirectedEdge* edge = edgeObject->edge;
        edgeObject->isOwner = false;
        try {
            ((DirectedAdjListObject*) (self))->adjlist->addEdge(edge);
            Py_RETURN_NONE;
        } catch (std::invalid_argument ex) {
            PyErr_SetString(StellaError, ex.what());
            return NULL;
        }
    }

    PyErr_SetString(PyExc_TypeError, "Invalid arguments for add_edge. Expected Edge object or (str, str, str, [int]).");
    return NULL;
}

static PyObject* AdjList_getEdge(AdjListObject* self, PyObject* args) {
    const char* label;
    if (!PyArg_ParseTuple(args, "s", &label)) {
        PyErr_SetString(PyExc_ValueError, "No argument for get_edge, str expected");
        return NULL;
    }

    stella::Edge* edge = ((AdjListObject*) (self))->adjlist->getEdge(label);
    if (edge) {
        EdgeObject* pyEdge = PyObject_New(EdgeObject, &EdgeType);
        if (!pyEdge)
            PyErr_NoMemory();
        pyEdge->edge = edge;
        pyEdge->isOwner = false;
        return (PyObject *) pyEdge;
    }

    Py_RETURN_NONE;
}

static PyObject* AdjList_getNode(AdjListObject* self, PyObject* args) {
    const char* label;
    if (!PyArg_ParseTuple(args, "s", &label)) {
        PyErr_SetString(PyExc_ValueError, "No argument for get_node, str expected");
        return NULL;
    }

    stella:: Node* node = ((AdjListObject*) (self))->adjlist->getNode(label);
    if (node) {

        NodeObject* node_obj = PyObject_New(NodeObject, &NodeType);
        if (!node_obj) {
            return PyErr_NoMemory();
        }
        node_obj->node = node;
        node_obj->isOwner = false;

        return (PyObject*)node_obj;
    }

    Py_RETURN_NONE;
}

static PyObject* AdjList_getAllNodes(AdjListObject* self, PyObject* args) {

    std::vector<stella::Node*>& nodes = ((AdjListObject*) (self))->adjlist->getAllNodes();

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
        pyNode->node = nodes[i];
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

static PyObject* AdjList_getAllEdges(AdjListObject* self, PyObject* args) {
    std::map<std::string, stella::Edge*>& edges = self->adjlist->getAllEdges();
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

        value->edge = pair.second;
        value->isOwner = false;

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

static PyObject* DirectedAdjList_getAllEdges(DirectedAdjListObject* self, PyObject* args) {
    std::map<std::string, stella::DirectedEdge*>& edges = self->adjlist->getAllEdges();
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

        DirectedEdgeObject* value = PyObject_New(DirectedEdgeObject, &EdgeType);
        if (!value) {
            Py_DECREF(key);
            Py_DECREF(pyEdges);
            PyErr_NoMemory();
            return NULL;
        }

        value->edge = pair.second;
        value->isOwner = false;

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

static PyGetSetDef AdjList_GetSetDef[] = {
    {"edges", (getter)AdjList_getAllEdges, NULL, "Node label", NULL},
    {"nodes", (getter)AdjList_getAllNodes, NULL, "Node label", NULL},
    {NULL}
};


static PyMethodDef AdjList_methods[] = {
    {"add_node", (PyCFunction)AdjList_addNode, METH_VARARGS, "Add a node to the graph."},
    {"add_edge", (PyCFunction)AdjList_addEdge, METH_VARARGS, "Add an edge to the graph."},
    {"get_edge", (PyCFunction)AdjList_getEdge, METH_VARARGS, "Get an edge from the graph."},
    {"get_node", (PyCFunction)AdjList_getNode, METH_VARARGS, "Get a node from the graph."},
    {NULL, NULL, 0, NULL}
};

static PyTypeObject AdjListType = {
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

static PyObject *DirectedAdjList_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    DirectedAdjListObject *self;
    self = (DirectedAdjListObject *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->adjlist = nullptr;
    }
    return (PyObject *)self;
}


static int DirectedAdjList_init(DirectedAdjListObject *self, PyObject *args, PyObject *kwds) {
    self->adjlist = new stella::DirectedAdjList<stella::Node, stella::DirectedEdge>();
    return 0;
};

static PyMethodDef DirectedAdjList_methods[] = {
    {"add_edge", (PyCFunction)DirectedAdjList_addEdge, METH_VARARGS, "Add an edge to the graph."},
    {NULL, NULL}
};

static PyGetSetDef DirectedAdjList_GetSetDef[] = {
    {"edges", (getter)DirectedAdjList_getAllEdges, NULL, "Node label", NULL},
    {NULL}
};

static PyTypeObject DirectedAdjListType = {
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

static PyObject *AdjMatrix_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    AdjMatrixObject *self;
    self = (AdjMatrixObject *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->adjmatrix= nullptr;
    }
    return (PyObject *)self;
}

static int AdjMatrix_init(AdjMatrixObject *self, PyObject *args, PyObject *kwds) {
    self->adjmatrix = new stella::AdjMatrix<stella::Node, stella::Edge>();
    return 0;
}

static void AdjMatrix_dealloc(AdjMatrixObject* self) {
    delete self->adjmatrix;
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject* AdjMatrix_addNode(AdjMatrixObject* self, PyObject* args) {
    PyObject* arg;
    if (PyArg_ParseTuple(args, "s", &arg)) {
        try {
            self->adjmatrix->addNode(new stella::Node((const char* )arg));
            Py_RETURN_NONE;
        } catch (std::invalid_argument ex) {
            PyErr_SetString(StellaError, ex.what());
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
        stella::Node* node = nodeObj->node;
        nodeObj->isOwner = false;
        try {
            self->adjmatrix->addNode(node);
            Py_RETURN_NONE;
        } catch (std::invalid_argument ex) {
            PyErr_SetString(StellaError, ex.what());
            return NULL;
        }
    } else {
        PyErr_SetString(PyExc_TypeError, "Invalid arguments for add_node, expected Node object or str");
        return NULL;
    }
}

static PyObject* AdjMatrix_addEdge(AdjMatrixObject* self, PyObject* args) {
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
            PyErr_SetString(StellaError, ex.what());
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
        stella::Edge* edge = edgeObject->edge;
        edgeObject->isOwner = false;
        try {
            self->adjmatrix->addEdge(edge);
            Py_RETURN_NONE;
        } catch (std::invalid_argument ex) {
            PyErr_SetString(StellaError, ex.what());
            return NULL;
        }
    }

    PyErr_SetString(PyExc_TypeError, "Invalid arguments for add_edge. Expected Edge object or (str, str, str, [int]).");
    return NULL;
}

static PyObject* AdjMatrix_getNode(AdjMatrixObject* self, PyObject* args) {
    const char* label;
    if (!PyArg_ParseTuple(args, "s", &label)) {
        PyErr_SetString(PyExc_ValueError, "No argument for get_node, str expected");
        return NULL;
    }

    stella:: Node* node = self->adjmatrix->getNode(label);
    if (node) {

        NodeObject* node_obj = PyObject_New(NodeObject, &NodeType);
        if (!node_obj) {
            return PyErr_NoMemory();
        }
        node_obj->node = node;
        node_obj->isOwner = false;

        return (PyObject*)node_obj;
    }

    Py_RETURN_NONE;
}

static PyObject* AdjMatrix_getAllNodes(AdjMatrixObject* self, PyObject* args) {

    std::vector<stella::Node*>& nodes = self->adjmatrix->getAllNodes();

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
        pyNode->node = nodes[i];
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

static PyObject* AdjMatrix_getAllEdges(AdjMatrixObject* self, PyObject* args) {
    std::vector<std::vector<std::map<std::string, stella::Edge*>>>& edges = self->adjmatrix->getAllEdges();

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

                value->edge = pair.second;
                value->isOwner = false;

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

static PyGetSetDef AdjMatrix_GetSetDef[] = {
    {"edges", (getter)AdjMatrix_getAllEdges, NULL, "Node label", NULL},
    {"nodes", (getter)AdjMatrix_getAllNodes, NULL, "Node label", NULL},
    {NULL}
};


static PyMethodDef AdjMatrix_methods[] = {
    {"add_node", (PyCFunction)AdjMatrix_addNode, METH_VARARGS, "Add a node to the graph."},
    {"add_edge", (PyCFunction)AdjMatrix_addEdge, METH_VARARGS, "Add an edge to the graph."},
    {"get_node", (PyCFunction)AdjMatrix_getNode, METH_VARARGS, "Get a node from the graph."},
    {NULL, NULL, 0, NULL}
};

static PyTypeObject AdjMatrixType = {
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

static PyObject *DirectedAdjMatrix_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    DirectedAdjMatrixObject *self;
    self = (DirectedAdjMatrixObject *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->adjmatrix = nullptr;
    }
    return (PyObject *)self;
}


static int DirectedAdjMatrix_init(DirectedAdjMatrixObject *self, PyObject *args, PyObject *kwds) {
    self->adjmatrix = new stella::DirectedAdjMatrix<stella::Node, stella::DirectedEdge>();
    return 0;
};

static PyObject* DirectedAdjMatrix_addEdge(DirectedAdjMatrixObject* self, PyObject* args) {
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
            PyErr_SetString(StellaError, ex.what());
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
        stella::DirectedEdge* edge = edgeObject->edge;
        edgeObject->isOwner = false;
        try {
            self->adjmatrix->addEdge(edge);
            Py_RETURN_NONE;
        } catch (std::invalid_argument ex) {
            PyErr_SetString(StellaError, ex.what());
            return NULL;
        }
    }

    PyErr_SetString(PyExc_TypeError, "Invalid arguments for add_edge. Expected Edge object or (str, str, str, [int]).");
    return NULL;
}

static PyObject* DirectedAdjMatrix_getAllEdges(DirectedAdjMatrixObject* self, PyObject* args) {
    std::vector<std::vector<std::map<std::string, stella::DirectedEdge*>>>& edges = self->adjmatrix->getAllEdges();

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

                value->edge = pair.second;
                value->isOwner = false;

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

static PyGetSetDef DirectedAdjMatrix_GetSetDef[] = {
    {"edges", (getter)DirectedAdjMatrix_getAllEdges, NULL, "Node label", NULL},
    {NULL, NULL}
};

static PyMethodDef DirectedAdjMatrix_methods[] = {
    {"add_edge", (PyCFunction)DirectedAdjMatrix_addEdge, METH_VARARGS, "Add an edge to the graph."},
    {NULL, NULL}
};

static PyTypeObject DirectedAdjMatrixType = {
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


static PyModuleDef stellaModule = {
    PyModuleDef_HEAD_INIT,
    "stella",
    "Python extension module for stella",
    -1,
    NULL, NULL, NULL, NULL, NULL
};


PyMODINIT_FUNC
PyInit_stella(void) {
    PyObject *m;
    if (PyType_Ready(&GraphType) < 0 ||
        PyType_Ready(&NodeType) < 0 ||
        PyType_Ready(&BaseEdgeType) < 0 ||
        PyType_Ready(&EdgeType) < 0 ||
        PyType_Ready(&DirectedEdgeType) < 0 ||
        PyType_Ready(&AdjListType) < 0 ||
        PyType_Ready(&DirectedAdjListType) < 0 ||
        PyType_Ready(&AdjMatrixType) < 0 ||
        PyType_Ready(&DirectedAdjMatrixType) < 0)
        return NULL;

    m = PyModule_Create(&stellaModule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&NodeType);
    PyModule_AddObject(m, "Node", (PyObject *)&NodeType);

    Py_INCREF(&BaseEdgeType);
    PyModule_AddObject(m, "BaseEdge", (PyObject *)&BaseEdgeType);

    Py_INCREF(&EdgeType);
    PyModule_AddObject(m, "Edge", (PyObject *)&EdgeType);

    Py_INCREF(&DirectedEdgeType);
    PyModule_AddObject(m, "DirectedEdge", (PyObject *)&DirectedEdgeType);

    Py_INCREF(&AdjListType);
    PyModule_AddObject(m, "AdjList", (PyObject *)&AdjListType);

    Py_INCREF(&DirectedAdjListType);
    PyModule_AddObject(m, "DirectedAdjList", (PyObject *)&DirectedAdjListType);

    Py_INCREF(&GraphType);
    PyModule_AddObject(m, "Graph", (PyObject *)&GraphType);

    StellaError = PyErr_NewException("stella.StellaError", NULL, NULL);
    if (StellaError == NULL) {
        Py_DECREF(m);
        return NULL;
    }

    PyModule_AddObject(m, "StellaError", StellaError);

    Py_INCREF(&AdjMatrixType);
    PyModule_AddObject(m, "AdjMatrix", (PyObject *)&AdjMatrixType);

    Py_INCREF(&DirectedAdjMatrixType);
    PyModule_AddObject(m, "DirectedAdjMatrix", (PyObject *)&DirectedAdjMatrixType);

    return m;
}