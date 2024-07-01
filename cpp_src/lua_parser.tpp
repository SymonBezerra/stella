#ifndef LUA_PARSER_HPP
#define LUA_PARSER_HPP

#include <exception>
#include <memory>
#include <type_traits>

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
    template<typename G>
    class LuaParser {
                static_assert(is_base_of<Graph<typename G::NodeType, typename G::EdgeType>, G>::value, "G must be derived from stella::Graph");
        private:
            lua_State* L;
            unique_ptr<G> graph;
        public:
            LuaParser() {
                graph = make_unique<G>();
                L = luaL_newstate();
                if (L == nullptr) throw runtime_error("LuaParser failed to initialize");
            }
            void load(const string& filename) {
                if (luaL_dofile(L, filename.c_str()) != LUA_OK) {
                    throw std::runtime_error(lua_tostring(L, -1));
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
                        std::string edgeLabel = lua_tostring(L, -2);

                        lua_pushstring(L, "n1");
                        lua_gettable(L, -2);
                        std::string n1 = lua_tostring(L, -1);
                        lua_pop(L, 1);

                        lua_pushstring(L, "n2");
                        lua_gettable(L, -2);
                        std::string n2 = lua_tostring(L, -1);
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
            unique_ptr<G>& getGraph() {
                return graph;
            }
            ~LuaParser() {
                lua_close(L);
            }
    };

};

#endif