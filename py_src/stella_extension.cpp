#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <exception>
#include <sstream>
#include "../cpp_src/stella.hpp"

#include "node.hpp"
#include "edge.hpp"
#include "graph.hpp"
#include "adj_list.hpp"
#include "adj_matrix.hpp"

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

    // Initialize all types
    if (PyType_Ready(&GraphType) < 0 ||
        PyType_Ready(&NodeType) < 0 ||
        PyType_Ready(&BaseEdgeType) < 0 ||
        PyType_Ready(&EdgeType) < 0 ||
        PyType_Ready(&DirectedEdgeType) < 0 ||
        PyType_Ready(&AdjListType) < 0 ||
        PyType_Ready(&DirectedAdjListType) < 0 ||
        PyType_Ready(&AdjMatrixType) < 0 ||
        PyType_Ready(&DirectedAdjMatrixType) < 0) {
        return NULL;
    }

    // Create the stella module
    m = PyModule_Create(&stellaModule);
    if (m == NULL) {
        return NULL;
    }

    // Add objects to the module and increment their references
    Py_INCREF(&NodeType);
    if (PyModule_AddObject(m, "Node", (PyObject *)&NodeType) < 0) {
        Py_DECREF(&NodeType);
        Py_DECREF(m);
        return NULL;
    }

    Py_INCREF(&BaseEdgeType);
    if (PyModule_AddObject(m, "BaseEdge", (PyObject *)&BaseEdgeType) < 0) {
        Py_DECREF(&BaseEdgeType);
        Py_DECREF(m);
        return NULL;
    }

    Py_INCREF(&EdgeType);
    if (PyModule_AddObject(m, "Edge", (PyObject *)&EdgeType) < 0) {
        Py_DECREF(&EdgeType);
        Py_DECREF(m);
        return NULL;
    }

    Py_INCREF(&DirectedEdgeType);
    if (PyModule_AddObject(m, "DirectedEdge", (PyObject *)&DirectedEdgeType) < 0) {
        Py_DECREF(&DirectedEdgeType);
        Py_DECREF(m);
        return NULL;
    }

    Py_INCREF(&AdjListType);
    if (PyModule_AddObject(m, "AdjList", (PyObject *)&AdjListType) < 0) {
        Py_DECREF(&AdjListType);
        Py_DECREF(m);
        return NULL;
    }

    Py_INCREF(&DirectedAdjListType);
    if (PyModule_AddObject(m, "DirectedAdjList", (PyObject *)&DirectedAdjListType) < 0) {
        Py_DECREF(&DirectedAdjListType);
        Py_DECREF(m);
        return NULL;
    }

    Py_INCREF(&GraphType);
    if (PyModule_AddObject(m, "Graph", (PyObject *)&GraphType) < 0) {
        Py_DECREF(&GraphType);
        Py_DECREF(m);
        return NULL;
    }

    Py_INCREF(&AdjMatrixType);
    if (PyModule_AddObject(m, "AdjMatrix", (PyObject *)&AdjMatrixType) < 0) {
        Py_DECREF(&AdjMatrixType);
        Py_DECREF(m);
        return NULL;
    }

    Py_INCREF(&DirectedAdjMatrixType);
    if (PyModule_AddObject(m, "DirectedAdjMatrix", (PyObject *)&DirectedAdjMatrixType) < 0) {
        Py_DECREF(&DirectedAdjMatrixType);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}