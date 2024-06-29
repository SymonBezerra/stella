#ifndef ADJ_LIST_TPP
#define ADJ_LIST_TPP

#include <exception>
#include <map>

#include "graph.tpp"

using std::invalid_argument;
using std::map;

namespace stella {
    template<typename N, typename E>
    class AdjList: public Graph<N, E> {
        static_assert(is_base_of<Edge, E>::value, "E must be of type stella::Edge for non-directed graphs");
        protected:
            vector<N*> nodes;
            map<string, E*> edges;
        public:
            AdjList() {}
            void addNode(N* node) override {
                if (getNode(node->getLabel()))
                    throw invalid_argument("Node already exists: " + node->getLabel());
                nodes.push_back(node);
            }
            void addNode(string label) override {
                if (getNode(label))
                    throw invalid_argument("Node already exists: " + label);
                N* node = new N(label);
                nodes.push_back(node);
            }
            void addEdge(E* edge) override {
                if (getEdge(edge->getLabel()))
                    throw invalid_argument("Edge already exists: " + edge->getLabel());
                edges.insert({edge->getLabel(), edge});
            }
            void addEdge(string label, string n1, string n2) override {
                if (getEdge(label))
                    throw invalid_argument("Edge already exists: " + label);
                N* node1 = getNode(n1);
                N* node2 = getNode(n2);
                if (!node1 || !node2)
                    throw invalid_argument("Node labels not found: " + n1 + " " + n2);
                E* edge = new E(label, node1, node2);
                edges.insert({label, edge});
            }
            void addEdge(string label, string n1, string n2, int weight) override {
                if (getEdge(label))
                    throw invalid_argument("Edge already exists: " + label);
                N* node1 = getNode(n1);
                N* node2 = getNode(n2);
                if (!node1 || !node2)
                    throw invalid_argument("Node labels not found: " + n1 + " " + n2);
                E* edge = new E(label, node1, node2, weight);
                edges.insert({edge->getLabel(), edge});
            }
            E* getEdge(string label) {
                auto it = edges.find(label);
                if (it != edges.end()) return it->second;
                return nullptr;
            }
            N* getNode(string label) override {
                for (N* node: nodes) {
                    if (node->getLabel().compare(label) == 0) return node;
                } return nullptr;
            }
            std::vector<N*>& getAllNodes() override {
                return nodes;
            }
            std::map<string, E*>& getAllEdges() {
                return edges;
            }
            ~AdjList() {
                for (auto pair : edges)
                    delete pair.second;
                for (int i = 0; i < nodes.size(); i++)
                    delete nodes[i];
                nodes.clear();
                edges.clear();
            }
    };

    template<typename N, typename E>
    class DirectedAdjList: public AdjList<N, E> {
        static_assert(is_base_of<DirectedEdge, E>::value, "E must be of type stella::DirectedEdge for directed graphs");
        public:
            DirectedAdjList(): AdjList<N, E>() {}
            map<string, E*>& getAllEdges() {
                return this->edges;
            }
    };
};

#endif