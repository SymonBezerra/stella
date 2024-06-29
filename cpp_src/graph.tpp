#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <type_traits>

#include "node.hpp"

using std::is_base_of;
using std::string;
using std::vector;

namespace stella {
    template <typename N, typename E>
    class Graph {
        static_assert(is_base_of<Node, N>::value, "N must be of type stella::Node");
        public:
            virtual void addNode(N* node) = 0;
            virtual void addNode(string label) = 0;
            virtual void addEdge(E* edge) = 0;
            virtual void addEdge(string label, string n1, string n2) = 0;
            virtual void addEdge(string label, string n1, string n2, int weight) = 0;
            virtual N* getNode(string label) = 0;
            virtual vector<N*>& getAllNodes() = 0;
    };
}

#endif