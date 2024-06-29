#ifndef ADJ_LIST_PYTHON_HPP
#define ADJ_LIST_PYTHON_HPP

#include <memory>
#include <python3.11/Python.h>

#include "../cpp_src/stella.hpp"

#include "graph.hpp"
#include "edge.hpp"

using std::make_unique;
using std::unique_ptr;

typedef struct {
    PyObject_HEAD
    unique_ptr<stella::AdjList<stella::Node, stella::Edge>> adjlist;
} AdjListObject;

PyObject *AdjList_new(PyTypeObject *type, PyObject *args, PyObject *kwds);

int AdjList_init(AdjListObject *self, PyObject *args, PyObject *kwds);

void AdjList_dealloc(AdjListObject* self);

PyObject* AdjList_addNode(AdjListObject* self, PyObject* args);

PyObject* AdjList_addEdge(AdjListObject* self, PyObject* args);

PyObject* AdjList_getEdge(AdjListObject* self, PyObject* args);

PyObject* AdjList_getNode(AdjListObject* self, PyObject* args);

PyObject* AdjList_getAllNodes(AdjListObject* self, PyObject* args);

PyObject* AdjList_getAllEdges(AdjListObject* self, PyObject* args);

extern PyTypeObject AdjListType;

typedef struct {
    PyObject_HEAD
    stella::DirectedAdjList<stella::Node, stella::DirectedEdge> *adjlist;
} DirectedAdjListObject;

PyObject *DirectedAdjList_new(PyTypeObject *type, PyObject *args, PyObject *kwds);

int DirectedAdjList_init(DirectedAdjListObject *self, PyObject *args, PyObject *kwds);

PyObject* DirectedAdjList_addEdge(DirectedAdjListObject* self, PyObject* args);

PyObject* DirectedAdjList_getEdge(DirectedAdjListObject* self, PyObject* args);

PyObject* DirectedAdjList_getAllEdges(DirectedAdjListObject* self, PyObject* args);;

extern PyTypeObject DirectedAdjListType;

#endif