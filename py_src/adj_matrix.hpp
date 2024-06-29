#ifndef ADJ_MATRIX_PYTHON_HPP
#define ADJ_MATRIX_PYTHON_HPP

#include<memory>

#include <python3.11/Python.h>

#include "../cpp_src/stella.hpp"

#include "graph.hpp"
#include "edge.hpp"

using std::make_shared;
using std::shared_ptr;

typedef struct {
    PyObject_HEAD
    shared_ptr<stella::AdjMatrix<stella::Node, stella::Edge>> adjmatrix;
} AdjMatrixObject;

PyObject *AdjMatrix_new(PyTypeObject *type, PyObject *args, PyObject *kwds);

int AdjMatrix_init(AdjMatrixObject *self, PyObject *args, PyObject *kwds);

void AdjMatrix_dealloc(AdjMatrixObject* self);

PyObject* AdjMatrix_addNode(AdjMatrixObject* self, PyObject* args);

PyObject* AdjMatrix_addEdge(AdjMatrixObject* self, PyObject* args);

PyObject* AdjMatrix_getNode(AdjMatrixObject* self, PyObject* args);

PyObject* AdjMatrix_getAllNodes(AdjMatrixObject* self, PyObject* args);

PyObject* AdjMatrix_getAllEdges(AdjMatrixObject* self, PyObject* args);

extern PyTypeObject AdjMatrixType;

typedef struct {
    PyObject_HEAD
    shared_ptr<stella::DirectedAdjMatrix<stella::Node, stella::DirectedEdge>> adjmatrix;
} DirectedAdjMatrixObject;

PyObject *DirectedAdjMatrix_new(PyTypeObject *type, PyObject *args, PyObject *kwds);

int DirectedAdjMatrix_init(DirectedAdjMatrixObject *self, PyObject *args, PyObject *kwds);

PyObject* DirectedAdjMatrix_addNode(DirectedAdjMatrixObject* self, PyObject* args);

PyObject* DirectedAdjMatrix_addEdge(DirectedAdjMatrixObject* self, PyObject* args);

PyObject* DirectedAdjMatrix_getAllEdges(DirectedAdjMatrixObject* self, PyObject* args);

extern PyTypeObject DirectedAdjMatrixType;

#endif