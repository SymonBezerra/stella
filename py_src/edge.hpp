#ifndef EDGE_PYTHON_HPP
#define EDGE_PYTHON_HPP

#include <memory>
#include <sstream>
#include <python3.11/Python.h>

#include "../cpp_src/stella.hpp"
#include "node.hpp"

using std::make_shared;
using std::shared_ptr;

typedef struct {
    PyObject_HEAD
    shared_ptr<stella::BaseEdge>* edge;
} BaseEdgeObject;

void BaseEdge_dealloc(BaseEdgeObject *self);

PyObject *BaseEdge_new(PyTypeObject *type, PyObject *args, PyObject *kwds);

int BaseEdge_init(BaseEdgeObject *self, PyObject *args, PyObject *kwds);

PyObject *BaseEdge_str(BaseEdgeObject *self);

PyObject *BaseEdge_label(BaseEdgeObject *self);

PyObject *BaseEdge_n1(BaseEdgeObject *self);

PyObject *BaseEdge_n2(BaseEdgeObject *self);

PyObject *BaseEdge_weight(BaseEdgeObject* self);

extern PyTypeObject BaseEdgeType;

typedef struct {
    PyObject_HEAD
    shared_ptr<stella::Edge>* edge;
} EdgeObject;

extern PyTypeObject EdgeType;

/* ******************** DIRECTEDEDGE IMPL STARTED  **************************/

typedef struct {
    PyObject_HEAD
    shared_ptr<stella::DirectedEdge>* edge;
} DirectedEdgeObject;

PyObject *DirectedEdge_str(DirectedEdgeObject *self);

extern PyTypeObject DirectedEdgeType;

#endif