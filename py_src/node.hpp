#ifndef NODE_PYTHON_HPP
#define NODE_PYTHON_HPP

#include <memory>
#include <python3.11/Python.h>
#include "../cpp_src/stella.hpp"

using std::make_shared;
using std::shared_ptr;

typedef struct {
    PyObject_HEAD
    shared_ptr<stella::Node> node;
    bool isOwner;
} NodeObject;

void Node_dealloc(NodeObject *self);
PyObject *Node_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
int Node_init(NodeObject *self, PyObject *args, PyObject *kwds);
PyObject *Node_label(NodeObject *self, void *closure);
PyObject *Node_str(NodeObject *self);

extern PyTypeObject NodeType;

#endif
