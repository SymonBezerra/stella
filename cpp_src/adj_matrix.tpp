#ifndef ADJ_MATRIX_TPP
#define ADJ_MATRIX_TPP

#include <exception>
#include <map>
#include <memory>

#include "graph.tpp"

using std::invalid_argument;
using std::make_shared;
using std::map;
using std::shared_ptr;

namespace stella {

    template<typename N, typename E>
    class AdjMatrix {
        static_assert(is_base_of<Edge, E>::value, "E must be of type stella::Edge for non-directed graphs");
    protected:
        void pushNode(int size) {
            edges.push_back(vector<map<string, shared_ptr<E>>>{});
            for(int i = size - 1; i >= 0; i--) {
                edges[i].push_back(map<string, shared_ptr<E>>{});
            }
        }
        vector<shared_ptr<N>> nodes;
        vector<vector<map<string, shared_ptr<E>>>> edges;
    public:
        AdjMatrix() {}
        void addNode(shared_ptr<N> node) {
            if (getNode(node->getLabel()))
                throw invalid_argument("Node already exists: " + node->getLabel());
            nodes.push_back(node);
            pushNode(nodes.size());
        }
        void addNode(std::string label) {
            if (getNode(label))
                throw invalid_argument("Node already exists: " + label);
            shared_ptr<N> node = make_shared<N>(label);
            nodes.push_back(node);
            pushNode(nodes.size());
        }
        void addEdge(shared_ptr<E> edge) {
        int n1 = getNodeIndex(edge->getN1()->getLabel());
        int n2 = getNodeIndex(edge->getN2()->getLabel());
        if (n1 < 0 || n2 < 0)
            throw invalid_argument("Node labels not found: "
            + edge->getN1()->getLabel() + " " + edge->getN2()->getLabel());
            if (n1 > n2) edges[n1][n2].insert({edge->getLabel(), edge});
            else if (n1 == n2) edges[n1][0].insert({edge->getLabel(), edge});
            else edges[n2][n1].insert({edge->getLabel(), edge});
        }

        void addEdge(string label, string n1, string n2) {
            int node1 = getNodeIndex(n1);
            int node2 = getNodeIndex(n2);
            if (node1 < 0 || node2 < 0)
                throw invalid_argument("Node labels not found: " + n1 + " " + n2);
            shared_ptr<E> edge = make_shared<E>(label, nodes[node1], nodes[node2], 1);
            if (node1 > node2) edges[node1][node2].insert({edge->getLabel(), edge});
            else if (node2 == node1) edges[node1][0].insert({edge->getLabel(), edge});
            else edges[node2][node1].insert({edge->getLabel(), edge});
        }

        void addEdge(string label, string n1, string n2, int weight) {
            int node1 = getNodeIndex(n1);
            int node2 = getNodeIndex(n2);
            if (node1 < 0 || node2 < 0)
                throw invalid_argument("Node labels not found: " + n1 + " " + n2);
            shared_ptr<E> edge = make_shared<E>(label, nodes[node1], nodes[node2], weight);
            if (node2 > node1) edges[node1][node2].insert({edge->getLabel(), edge});
            else if (node2 == node1) edges[node1][0].insert({edge->getLabel(), edge});
            else edges[node2][node1].insert({edge->getLabel(), edge});
        }
            shared_ptr<N> getNode(std::string label) {
                for (shared_ptr<N> node : nodes) {
                    if (node->getLabel().compare(label) == 0) return node;
                }
                return nullptr;
            }
            int getNodeIndex(std::string label) {
                auto it = nodes.begin();
                int index = 0;
                while (it != nodes.end()) {
                    if ((*it)->getLabel() == label) return index;
                    ++it;
                    ++index;
                }
                return -1;
            }
            std::vector<shared_ptr<N>>& getAllNodes() {
                return nodes;
            }
            std::vector<std::vector<std::map<string, shared_ptr<E>>>>& getAllEdges() {
                return edges;
            }
            // ~AdjMatrix() {
            //     int size = nodes.size();
            //     int dec = 0;
            //     for (int i = 0; i < size; i++, dec++) {
            //         for (int j = 0; j < size - dec; j++) {
            //             map<string, shared_ptr<E>>& edgeMap = edges[i][j];
            //             for (auto it = edgeMap.begin(); it != edgeMap.end(); ++it) {
            //                 delete it->second;
            //             }
            //             edgeMap.clear();
            //         }
            //     }

            //     for (int i = 0; i < nodes.size(); i++)
            //         delete nodes[i];

            //     edges.clear();
            //     nodes.clear();
            // }
    };

    template<typename N, typename E>
    class DirectedAdjMatrix : public AdjMatrix<N, E> {
        static_assert(std::is_base_of<DirectedEdge, E>::value, "E must be of type stella::DirectedEdge for directed graphs");
    protected:
        void pushNode(int size) {
            this->edges.push_back(vector<map<string, shared_ptr<E>>>{});
            for(int i = 0; i < size; i++) {
                if (i != size - 1)
                    this->edges[i].push_back(map<string, shared_ptr<E>>{});
                else
                    for (int j = 0; j < size; j++)
                        this->edges[i].push_back(map<string, shared_ptr<E>>{});
            }
        }
    public:
        DirectedAdjMatrix() : AdjMatrix<N, E>() {}
        void addNode(shared_ptr<N> node) {
            if (this->getNode(node->getLabel()))
                throw invalid_argument("Node already exists: " + node->getLabel());
            this->nodes.push_back(node);
            pushNode(this->nodes.size());
        }
        void addNode(std::string label) {
            if (this->getNode(label))
                throw invalid_argument("Node already exists: " + label);
            shared_ptr<N> node = make_shared<N>(label);
            this->nodes.push_back(node);
            pushNode(this->nodes.size());
        }
        void addEdge(shared_ptr<E> edge) {
            int n1 = this->getNodeIndex(edge->getN1()->getLabel());
            int n2 = this->getNodeIndex(edge->getN2()->getLabel());
            if (n1 < 0 || n2 < 0)
                throw invalid_argument("Node labels not found: "
                    + edge->getN1()->getLabel() + " " + edge->getN2()->getLabel());
            this->edges[n1][n2].insert({edge->getLabel(), edge});
        }
        void addEdge(string label, string n1, string n2) {
            int node1 = this->getNodeIndex(n1);
            int node2 = this->getNodeIndex(n2);
            if (node1 < 0 || node2 < 0)
                throw invalid_argument("Node labels not found: " + n1 + " " + n2);
            shared_ptr<E> edge = make_shared<E>(label, this->nodes[node1], this->nodes[node2], 1);
            this->edges[node1][node2].insert({edge->getLabel(), edge});
        }

        void addEdge(string label, string n1, string n2, int weight) {
            int node1 = this->getNodeIndex(n1);
            int node2 = this->getNodeIndex(n2);
            if (node1 < 0 || node2 < 0)
                throw invalid_argument("Node labels not found: " + n1 + " " + n2);
            shared_ptr<E> edge = make_shared<E>(label, this->nodes[node1], this->nodes[node2], weight);
            this->edges[node1][node2].insert({edge->getLabel(), edge});
        }
        // ~DirectedAdjMatrix() {
        //     int size = this->nodes.size();
        //     for (int i = 0; i < size; i++) {
        //         for (int j = 0; j < size; j++) {
        //             map<string, shared_ptr<E>>& edgeMap = this->edges[i][j];
        //             for (auto it = edgeMap.begin(); it != edgeMap.end(); ++it) {
        //                 delete it->second;
        //             }
        //             edgeMap.clear();
        //         }
        //     }

        //     for (int i = 0; i < this->nodes.size(); i++)
        //         delete this->nodes[i];

        //     this->edges.clear();
        //     this->nodes.clear();
        // }
    };
};

#endif