#ifndef GRAPH_PYTHON_HPP
#define GRAPH_PYTHON_HPP

#include <memory>
#include <python3.11/Python.h>
#include "../cpp_src/stella.hpp"

using std::shared_ptr;

typedef struct {
    PyObject_HEAD
    shared_ptr<stella::Graph<stella::Node, stella::BaseEdge>> graph;
} GraphObject;

int Graph_init(GraphObject *self, PyObject *args, PyObject *kwds);

void Graph_dealloc(GraphObject *self);

PyObject* Graph_addNode(PyObject *self, PyObject *args);

PyObject* Graph_addEdge(PyObject *self, PyObject *args);

PyObject* Graph_getEdge(PyObject *self, PyObject *args);

PyObject* Graph_getNode(PyObject *self, PyObject *args);

extern PyTypeObject GraphType;

#endif