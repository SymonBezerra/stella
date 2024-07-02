#ifndef LUA_PARSER_PYTHON_HPP
#define LUA_PARSER_PYTHON_HPP

#include <memory>
#include <python3.11/Python.h>

#include "../cpp_src/lua_parser.tpp"
#include "../cpp_src/graph.tpp"
#include "../cpp_src/edge.hpp"

using std::make_unique;
using std::unique_ptr;

typedef struct {
    PyObject_HEAD
    unique_ptr<stella::LuaParser> parser; 
} LuaParserObject;

PyObject *LuaParser_new(PyObject *type, PyObject *args, PyObject *kwds);

int LuaParser_init(LuaParserObject *self, PyObject *args, PyObject *kwds);

void LuaParser_dealloc(LuaParserObject* self);

PyObject* LuaParser_load(LuaParserObject* self, PyObject* args);

extern PyTypeObject LuaParserType;

#endif