#ifndef LUA_PARSER_HPP
#define LUA_PARSER_HPP

#include <exception>
#include <memory>
#include <type_traits>

/*
    TO USE THIS PARSER, READ THIS:
    You must install a Lua runtime, or download the Lua source and add the header directory
    manually. For the "official version" of Stella, i.e. this repository, we will keep for now
    the version 5.4. Future updates can modify the version use.

    For compilation, use only the flag `-llua5.4` (or other version)
*/
#include <lua5.4/lua.h>
#include <lua5.4/lauxlib.h>
#include <lua5.4/luaconf.h>
#include "node.hpp"
#include "edge.hpp"
#include "graph.tpp"

using std::is_base_of;
using std::unique_ptr;
using std::make_unique;
using std::runtime_error;

namespace stella {
    class LuaParser {
        private:
            lua_State* L;
        public:
            LuaParser() {
                L = luaL_newstate();
                if (L == nullptr) throw runtime_error("LuaParser failed to initialize");
            }
            template<typename G>
            void load(const string& filename, G* graph) {
                // static_assert(is_base_of<Graph<typename G::NodeType, typename G::EdgeType>, G>::value, "G must be derived from stella::Graph");
                if (luaL_dofile(L, filename.c_str()) != LUA_OK) {
                    throw runtime_error(lua_tostring(L, -1));
                }
                // Get nodes from Lua
                lua_getglobal(L, "nodes");
                if (lua_istable(L, -1)) {
                    lua_pushnil(L); // First key
                    while (lua_next(L, -2) != 0) {
                        std::string nodeLabel = lua_tostring(L, -1);
                        graph->addNode(nodeLabel);
                        lua_pop(L, 1); // Remove value, keep key for next iteration
                    }
                }
                else throw std::runtime_error("LuaParser syntax error: missing nodes table");
                lua_pop(L, 1); // Remove nodes table

                // Get edges from Lua
                lua_getglobal(L, "edges");
                if (lua_istable(L, -1)) {
                    lua_pushnil(L); // First key
                    while (lua_next(L, -2) != 0) {
                        const char* edgeLabel = lua_tostring(L, -2);
                        if (edgeLabel == nullptr) throw runtime_error("Lua parse syntax error, missing edge label");

                        lua_pushstring(L, "n1");
                        lua_gettable(L, -2);
                        const char* n1 = lua_tostring(L, -1);
                        if (n1 == nullptr) throw runtime_error("Lua parse syntax error, missing n1 label");
                        lua_pop(L, 1);

                        lua_pushstring(L, "n2");
                        lua_gettable(L, -2);
                        const char* n2 = lua_tostring(L, -1);
                        if (n2 == nullptr) throw runtime_error("Lua parse syntax error, missing n1 label");
                        lua_pop(L, 1);

                        lua_pushstring(L, "weight");
                        lua_gettable(L, -2);
                        int weight = lua_tointeger(L, -1);
                        if (weight == 0) weight++;
                        lua_pop(L, 1);

                        graph->addEdge(edgeLabel, n1, n2, weight);
                        lua_pop(L, 1); // Remove edge table, keep key for next iteration
                    }
                } else throw std::runtime_error("LuaParser syntax error: missing edges table");
                lua_pop(L, 1); // Remove edges table
            }
            ~LuaParser() {
                lua_close(L);
            }
    };

};

#endif