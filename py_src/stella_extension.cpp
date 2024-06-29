#define PY_SSIZE_T_CLEAN
#include <python3.11/Python.h>
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
    if (PyType_Ready(&GraphType) < 0 ||
        PyType_Ready(&NodeType) < 0 ||
        PyType_Ready(&BaseEdgeType) < 0 ||
        PyType_Ready(&EdgeType) < 0 ||
        PyType_Ready(&DirectedEdgeType) < 0 ||
        PyType_Ready(&AdjListType) < 0 ||
        PyType_Ready(&DirectedAdjListType) < 0 ||
        // PyType_Ready(&AdjMatrixType) < 0 ||
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

    Py_INCREF(&AdjMatrixType);
    PyModule_AddObject(m, "AdjMatrix", (PyObject *)&AdjMatrixType);

    Py_INCREF(&DirectedAdjMatrixType);
    if (PyModule_AddObject(m, "DirectedAdjMatrix", (PyObject *)&DirectedAdjMatrixType) < 0) {
        Py_DECREF(&DirectedAdjMatrixType);
        Py_DECREF(m);
        return NULL;
    }


    return m;
}