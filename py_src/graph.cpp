#include "graph.hpp"

int Graph_init(GraphObject *self, PyObject *args, PyObject *kwds) {
    PyErr_SetString(PyExc_TypeError, "Cannot instantiate abstract class 'Graph'");
    return -1;
}

void Graph_dealloc(GraphObject *self) {
    delete self->graph;
    Py_TYPE(self)->tp_free((PyObject *)self);
}

PyObject* Graph_addNode(PyObject *self, PyObject *args) {
    PyErr_SetString(PyExc_NotImplementedError, "addNode method is abstract and must be implemented in derived classes");
    return NULL;
}

PyObject* Graph_addEdge(PyObject *self, PyObject *args) {
    PyErr_SetString(PyExc_NotImplementedError, "addEdge method is abstract and must be implemented in derived classes");
    return NULL;
}

PyObject* Graph_getEdge(PyObject *self, PyObject *args) {
    PyErr_SetString(PyExc_NotImplementedError, "getEdge method is abstract and must be implemented in derived classes");
    return NULL;
}

PyObject* Graph_getNode(PyObject *self, PyObject *args) {
    PyErr_SetString(PyExc_NotImplementedError, "getNode method is abstract and must be implemented in derived classes");
    return NULL;
}

PyObject* Graph_getAllNodes(PyObject *self, PyObject *args) {
    PyErr_SetString(PyExc_NotImplementedError, "getAllNodes method is abstract and must be implemented in derived classes");
    return NULL;
}

PyObject* Graph_getAllEdges(PyObject *self, PyObject *args) {
    PyErr_SetString(PyExc_NotImplementedError, "getAllEdges method is abstract and must be implemented in derived classes");
    return NULL;
}

PyMethodDef Graph_methods[] = {
    {"addNode", (PyCFunction)Graph_addNode, METH_VARARGS, "Add a node to the graph."},
    {"addEdge", (PyCFunction)Graph_addEdge, METH_VARARGS, "Add an edge to the graph."},
    {"getEdge", (PyCFunction)Graph_getEdge, METH_VARARGS, "Get an edge from the graph."},
    {"getNode", (PyCFunction)Graph_getNode, METH_VARARGS, "Get a node from the graph."},
    {"getAllNodes", (PyCFunction)Graph_getAllNodes, METH_NOARGS, "Get all nodes from the graph."},
    {"getAllEdges", (PyCFunction)Graph_getAllEdges, METH_NOARGS, "Get all edges from the graph."},
    {NULL, NULL, 0, NULL}
};

PyTypeObject GraphType = {
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