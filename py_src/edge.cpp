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
    if (
        !PyObject_IsInstance((PyObject *)n1_obj, (PyObject *) &NodeType)
        || !PyObject_IsInstance((PyObject *)n2_obj, (PyObject *) &NodeType)
    ) {
        PyErr_SetString(PyExc_TypeError, "n1 and n2 must be of Node type");
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
    shared_ptr<stella::Node> node =  self->edge->get()->getN2();
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

PyObject *BaseEdge_richcompare(PyObject* first, PyObject* second, int op) {
    if (!PyObject_IsInstance(first, (PyObject *)&BaseEdgeType)
    || !PyObject_IsInstance(second, (PyObject *)&BaseEdgeType))
        Py_RETURN_NOTIMPLEMENTED;

    stella::BaseEdge e1 = *(((BaseEdgeObject *)first)->edge->get());
    stella::BaseEdge e2 = *(((BaseEdgeObject *)second)->edge->get());

    bool result = false;
    switch (op) {
        case Py_EQ:
            result = e1 == e2;
            break;
        case Py_NE:
            result = e1 != e2;
            break;
        case Py_GT:
            result = e1 > e2;
            break;
        case Py_GE:
            result = e1 >= e2;
            break;
        case Py_LT:
            result = e1 < e2;
            break;
        case Py_LE:
            result = e1 <= e2;
            break;
        default:
            Py_RETURN_NOTIMPLEMENTED;
    }
    if (result) Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

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
    BaseEdge_richcompare,      /* tp_richcompare */
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

PyObject *DirectedEdge_richcompare(PyObject* first, PyObject* second, int op) {
    if (!PyObject_IsInstance(first, (PyObject *)&DirectedEdgeType)
    || !PyObject_IsInstance(second, (PyObject *)&DirectedEdgeType))
        Py_RETURN_NOTIMPLEMENTED;

    bool result = false;

    stella::DirectedEdge e1 = *(((DirectedEdgeObject *)first)->edge->get());
    stella::DirectedEdge e2 = *(((DirectedEdgeObject *)second)->edge->get());

    switch (op) {
        case Py_EQ:
            result = e1 == e2;
            break;
        case Py_NE:
            result = e1 != e2;
            break;
        case Py_GT:
            result = e1 > e2;
            break;
        case Py_GE:
            result = e1 >= e2;
            break;
        case Py_LT:
            result = e1 < e2;
            break;
        case Py_LE:
            result = e1 <= e2;
            break;
        default:
            Py_RETURN_NOTIMPLEMENTED;
    }
    if (result) Py_RETURN_TRUE;
    Py_RETURN_FALSE;
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
    DirectedEdge_richcompare,  /* tp_richcompare */
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