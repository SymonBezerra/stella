#include "lua_parser.hpp"
#include "graph.hpp"
#include "adj_list.hpp"
#include "adj_matrix.hpp"

PyObject *LuaParser_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    LuaParserObject* self;
    self = (LuaParserObject *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->parser = nullptr;
    }
    return (PyObject *) self;
}

int LuaParser_init(LuaParserObject *self, PyObject *args, PyObject *kwds) {
    self->parser = make_unique<stella::LuaParser>();
    return 0;
}

void LuaParser_dealloc(LuaParserObject *self) {
    Py_TYPE(self)->tp_free((PyObject *)self);
}

PyObject* LuaParser_load(LuaParserObject* self, PyObject* args) {
    PyObject* filename;
    PyObject* obj;
    if (PyArg_ParseTuple(args, "sO", &filename, &obj)) {
        if (!PyObject_IsSubclass(obj, (PyObject *) &GraphType)) {
            PyErr_SetString(PyExc_TypeError, "Second argument must be a subclass of Graph");
        }

        if(PyObject_IsInstance(obj, (PyObject *) &AdjListType)) {
            try {
                PyObject* filenameStr = PyUnicode_AsEncodedString(filename, "UTF-8", "strict");
                self->parser->load(PyBytes_AS_STRING(filenameStr), ((AdjListObject *)obj)->adjlist.get());
                Py_RETURN_NONE;
            } catch (runtime_error ex) {
                PyErr_SetString(PyExc_RuntimeError, ex.what());
                return NULL;
            }
        } else if(PyObject_IsInstance(obj, (PyObject *) &DirectedAdjListType)) {
            try {
                PyObject* filenameStr = PyUnicode_AsEncodedString(filename, "UTF-8", "strict");
                self->parser->load(PyBytes_AS_STRING(filenameStr), ((DirectedAdjListObject *)obj)->adjlist.get());
                Py_RETURN_NONE;
            } catch (runtime_error ex) {
                PyErr_SetString(PyExc_RuntimeError, ex.what());
                return NULL;
            }
        }
        else if(PyObject_IsInstance(obj, (PyObject *) &AdjMatrixType)) {
            try {
                PyObject* filenameStr = PyUnicode_AsEncodedString(filename, "UTF-8", "strict");
                self->parser->load(PyBytes_AS_STRING(filenameStr), ((AdjMatrixObject *)obj)->adjmatrix.get());
                Py_RETURN_NONE;
            } catch (runtime_error ex) {
                PyErr_SetString(PyExc_RuntimeError, ex.what());
                return NULL;
            }
        }
        else if(PyObject_IsInstance(obj, (PyObject *) &DirectedAdjMatrixType)) {
            try {
                PyObject* filenameStr = PyUnicode_AsEncodedString(filename, "UTF-8", "strict");
                self->parser->load(PyBytes_AS_STRING(filenameStr), ((DirectedAdjMatrixObject *)obj)->adjmatrix.get());
                Py_RETURN_NONE;
            } catch (runtime_error ex) {
                PyErr_SetString(PyExc_RuntimeError, ex.what());
                return NULL;
            }
        } else {
            PyErr_SetString(PyExc_TypeError, "Second argument must be a subclass of Graph");
            return NULL;
        }
    }
    PyErr_SetString(PyExc_ValueError, "Invalid arguments, expected (str, Graph)");
    return NULL;
}

PyMethodDef LuaParser_methods[] = {
    {"load", (PyCFunction)LuaParser_load, METH_VARARGS, "Add an edge to the graph."},
    {NULL, NULL}
};

PyTypeObject LuaParserType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "stella.LuaParser",             /* tp_name */
    sizeof(LuaParserObject),        /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)LuaParser_dealloc,  /* tp_dealloc */
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
    0,        /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,        /* tp_flags */
    "Node object",             /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    LuaParser_methods,         /* tp_methods */
    0,                         /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)LuaParser_init,  /* tp_init */
    0,                         /* tp_alloc */
    LuaParser_new,             /* tp_new */
};