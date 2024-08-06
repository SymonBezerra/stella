#include "node.hpp"

void Node_dealloc(NodeObject *self) {
    delete self->node;
    Py_TYPE(self)->tp_free((PyObject *)self);
}

PyObject *Node_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    NodeObject *self;
    self = (NodeObject *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->node = nullptr;
    }
    return (PyObject *)self;
}

int Node_init(NodeObject *self, PyObject *args, PyObject *kwds) {
    const char *label;
    if (!PyArg_ParseTuple(args, "s", &label)) {
        return -1;
    }
    self->node = new (shared_ptr<stella::Node>) (make_shared<stella::Node>(label));
    return 0;
}

PyObject *Node_label(NodeObject *self, void *closure) {
    return PyUnicode_FromString(self->node->get()->getLabel().c_str());
}

PyGetSetDef Node_properties[] = {
    {"label", (getter)Node_label, NULL, "Node label", NULL},
    {NULL}
};

PyObject *Node_str(NodeObject *self) {
    std::ostringstream oss;
    oss << self->node->get();
    return PyUnicode_FromString(oss.str().c_str());
}

PyObject *Node_richcompare(PyObject* first, PyObject* second, int op) {
    if (
        !PyObject_IsInstance(first, (PyObject *)&NodeType)
        || !PyObject_IsInstance(second, (PyObject *)&NodeType)
    )
        Py_RETURN_NOTIMPLEMENTED;

    bool result = false;
    switch (op) {
        case Py_EQ:
            result = *(((NodeObject *)first)->node) == *(((NodeObject *)second)->node) ;
            break;
        case Py_NE:
            result = *(((NodeObject *)first)->node)  != *(((NodeObject *)second)->node) ;
            break;
        default:
            Py_RETURN_NOTIMPLEMENTED;
    }

    if (result) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

PyTypeObject NodeType = {
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
    Node_richcompare,          /* tp_richcompare */
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