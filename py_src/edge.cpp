#include "edge.hpp"

void BaseEdge_dealloc(BaseEdgeObject *self) {
    delete self->edge;
    Py_TYPE(self)->tp_free((PyObject *)self);
}

PyObject *BaseEdge_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    BaseEdgeObject *self;
    self = (BaseEdgeObject *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->edge = nullptr;
    }
    return (PyObject *)self;
}

int BaseEdge_init(BaseEdgeObject *self, PyObject *args, PyObject *kwds) {
    const char *label;
    NodeObject *n1_obj, *n2_obj;
    int weight = 1;
    if (!PyArg_ParseTuple(args, "sOO|i", &label, &n1_obj, &n2_obj, &weight)) {
        return -1;
    }
    shared_ptr<stella::Node>* n1 = n1_obj->node;
    shared_ptr<stella::Node>* n2 = n2_obj->node;
    self->edge = new (shared_ptr<stella::BaseEdge>) (make_shared<stella::BaseEdge>(label, *n1, *n2, weight));
    return 0;
}

PyObject *BaseEdge_str(BaseEdgeObject *self) {
    std::ostringstream oss;
    oss << self->edge->get();
    return PyUnicode_FromString(oss.str().c_str());
}

PyObject *BaseEdge_label(BaseEdgeObject *self) {
    return PyUnicode_FromString(self->edge->get()->getLabel().c_str());
}

PyObject *BaseEdge_n1(BaseEdgeObject *self) {
    shared_ptr<stella::Node> node = self->edge->get()->getN1();
    NodeObject* nodeObject = PyObject_New(NodeObject, &NodeType);
    nodeObject->node = new shared_ptr<stella::Node>(node);
    return (PyObject *) nodeObject;
}

PyObject *BaseEdge_n2(BaseEdgeObject *self) {
    shared_ptr<stella::Node> node =  self->edge->get()->getN1();
    NodeObject* nodeObject = PyObject_New(NodeObject, &NodeType);
    nodeObject->node = new shared_ptr<stella::Node>(node);
    return (PyObject *) nodeObject;
}

PyObject *BaseEdge_weight(BaseEdgeObject* self) {
    int weight = self->edge->get()->getWeight();
    return PyLong_FromLong(weight);
}

PyGetSetDef BaseEdge_GetSet[] = {
    {"label", (getter)BaseEdge_label, NULL, "Edge label"},
    {"n1", (getter)BaseEdge_n1, NULL, "Node first node"},
    {"n2", (getter)BaseEdge_n2, NULL, "Node second node"},
    {"weight", (getter)BaseEdge_weight, NULL, "Node weight"},
    {NULL}
};

PyTypeObject BaseEdgeType = {
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

PyTypeObject EdgeType = {
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
    0,                         /* tp_str */
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

PyObject *DirectedEdge_str(DirectedEdgeObject *self) {
    std::ostringstream oss;
    oss << self->edge->get();
    return PyUnicode_FromString(oss.str().c_str());
}

PyTypeObject DirectedEdgeType = {
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