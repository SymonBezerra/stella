#ifndef ADJ_LIST_TPP
#define ADJ_LIST_TPP

#include <exception>
#include <map>
#include <memory>

#include "graph.tpp"

using std::invalid_argument;
using std::map;
using std::make_shared;
using std::shared_ptr;

namespace stella {
    template<typename N, typename E>
    class AdjList: public Graph<N, E> {
        static_assert(is_base_of<Edge, E>::value, "E must be of type stella::Edge for non-directed graphs");
        protected:
            vector<shared_ptr<N>> nodes;
            map<string, shared_ptr<E>> edges;
        public:
            AdjList() {}
            void addNode(shared_ptr<N> node) override {
                if (getNode(node->getLabel()))
                    throw invalid_argument("Node already exists: " + node->getLabel());
                nodes.push_back(node);
            }
            void addNode(string label) override {
                if (getNode(label))
                    throw invalid_argument("Node already exists: " + label);
                shared_ptr<N> node = make_shared<N>(label);
                nodes.push_back(node);
            }
            void addEdge(shared_ptr<E> edge) override {
                if (getEdge(edge->getLabel()))
                    throw invalid_argument("Edge already exists: " + edge->getLabel());
                edges.insert({edge->getLabel(), edge});
            }
            void addEdge(string label, string n1, string n2) override {
                if (getEdge(label))
                    throw invalid_argument("Edge already exists: " + label);
                shared_ptr<N> node1 = getNode(n1);
                shared_ptr<N> node2 = getNode(n2);
                if (!node1 || !node2)
                    throw invalid_argument("Node labels not found: " + n1 + " " + n2);
                shared_ptr<E> edge = make_shared<E>(label, node1, node2);
                edges.insert({label, edge});
            }
            void addEdge(string label, string n1, string n2, int weight) override {
                if (getEdge(label))
                    throw invalid_argument("Edge already exists: " + label);
                shared_ptr<N> node1 = getNode(n1);
                shared_ptr<N> node2 = getNode(n2);
                if (!node1 || !node2)
                    throw invalid_argument("Node labels not found: " + n1 + " " + n2);
                shared_ptr<E> edge = make_shared<E>(label, node1, node2, weight);
                edges.insert({edge->getLabel(), edge});
            }
            shared_ptr<E> getEdge(string label) {
                auto it = edges.find(label);
                if (it != edges.end()) return it->second;
                return nullptr;
            }
            shared_ptr<N> getNode(string label) override {
                for (shared_ptr<N> node: nodes) {
                    if (node->getLabel().compare(label) == 0) return node;
                } return nullptr;
            }
            std::vector<shared_ptr<N>>& getAllNodes() override {
                return nodes;
            }
            std::map<string, shared_ptr<E>>& getAllEdges() {
                return edges;
            }
            // ~AdjList() {
            //     for (auto pair : edges)
            //         delete pair.second;
            //     for (int i = 0; i < nodes.size(); i++)
            //         delete nodes[i];
            //     nodes.clear();
            //     edges.clear();
            // }
    };

    template<typename N, typename E>
    class DirectedAdjList: public AdjList<N, E> {
        static_assert(is_base_of<DirectedEdge, E>::value, "E must be of type stella::DirectedEdge for directed graphs");
        public:
            DirectedAdjList(): AdjList<N, E>() {}
            map<string, shared_ptr<E>>& getAllEdges() {
                return this->edges;
            }
    };
};

#endif