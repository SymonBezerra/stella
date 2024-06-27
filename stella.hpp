/* 
    Stella (tribute to "Stella Maris", an ancient Latin title for the Blessed Virgin Mary) 
    is a library for teaching Graph Theory on Computer Science courses,
    and other programming related subjects.
    One of its core features is the individuality of nodes, edges, and graphs
    as object-oriented implementations, favouring a comprehensible and extensible code.
    Made by Symon Bezerra and Henrique do Nascimento Cunha. Published under MIT license.
    Based on Bibgrafo library, by Henrique do Nascimento Cunha.

    This code was made with aid of ChatGPT.

    Copyright © 2021 Henrique do Nascimento Cunha

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software
    and associated documentation files (the “Software”), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge, publish, distribute,
    sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
    is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies
    or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
    INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
    PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
    FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
    ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.

*/

#ifndef STELLA_H
#define STELLA_H

#include <iostream>
#include <map>
#include <string>
#include <type_traits>
#include <vector>

using std::string;
using std::vector;
using std::map;

namespace stella {
    class Node {
        protected:
            const string label;
        public:
            Node(string label): label(label) {}
            string getLabel() const {
                return label;
            }
            friend std::ostream& operator<<(std::ostream& os, Node* obj) {
                os << obj->label;
                return os;
            }
    };

    // abstract class
    class BaseEdge {
        public:
            const string label;
            const Node* n1;
            const Node* n2;
            const int weight;
            BaseEdge(string label, Node* n1,
                Node* n2, int weight): label(label), n1(n1),
                n2(n2), weight(weight) {}
            BaseEdge(string label, Node* n1,
                Node* n2): label(label), n1(n1),
                n2(n2), weight(1) {}
            string getLabel() const {
                return label;
            }
            const Node* getN1() const {
                return n1;
            }
            const Node* getN2() const {
                return n2;
            }
            int getWeight() const {
                return weight;
            }
            friend std::ostream& operator<<(std::ostream& os, BaseEdge* object) {
                os << object->label << ": " << object->n1->getLabel() << "-"
                << object->n2->getLabel() << "(" << object->weight << ")";
                return os;
            }
    };

    class Edge: public BaseEdge {
        public:
            Edge(string label, Node* n1,
                Node* n2, int weight): BaseEdge(label, n1, n2, weight) {}
            Edge(string label, Node* n1,
                Node* n2): BaseEdge(label, n1, n2) {}
    };

    class DirectedEdge: public Edge {
        public:
            DirectedEdge(string label, Node* n1,
                Node* n2, int weight): Edge(label, n1, n2, weight) {}
            DirectedEdge(string label, Node* n1,
                Node* n2): Edge(label, n1,n2) {} 
            friend std::ostream& operator<<(std::ostream& os, DirectedEdge* object) {
                os << object->label << ": " << object->n1->getLabel() << "->"
                << object->n2->getLabel() << "(" << object->weight << ")";
                return os;
            }
    };

    template <typename N, typename E>
    class Graph {
        static_assert(std::is_base_of<Node, N>::value, "N must be of type stella::Node");
        public:
            virtual void addNode(N* node) = 0;
            virtual void addNode(string label) = 0;
            virtual void addEdge(E* edge) = 0;
            virtual void addEdge(string label, string n1, string n2) = 0;
            virtual void addEdge(string label, string n1, string n2, int weight) = 0;
            // virtual E* getEdge(string label) = 0;
            virtual N* getNode(string label) = 0;
            virtual std::vector<N*>& getAllNodes() = 0;
            // virtual std::map<string, E*>& getAllEdges() = 0;
    };

    template<typename N, typename E>
    class AdjList: public Graph<N, E> {
        static_assert(std::is_base_of<Edge, E>::value, "E must be of type stella::Edge for non-directed graphs");
        protected:
            vector<N*> nodes;
            map<string, E*> edges;
        public:
            AdjList() {}
            void addNode(N* node) override {
                if (getNode(node->getLabel()))
                    throw std::invalid_argument("Node already exists: " + node->getLabel());
                nodes.push_back(node);
            }
            void addNode(string label) override {
                if (getNode(label))
                    throw std::invalid_argument("Node already exists: " + label);
                N* node = new N(label);
                nodes.push_back(node);
            }
            void addEdge(E* edge) override {
                if (getEdge(edge->getLabel()))
                    throw std::invalid_argument("Edge already exists: " + edge->getLabel());
                edges.insert({edge->getLabel(), edge});
            }
            void addEdge(string label, string n1, string n2) override {
                if (getEdge(label))
                    throw std::invalid_argument("Edge already exists: " + label);
                N* node1 = getNode(n1);
                N* node2 = getNode(n2);
                if (!node1 || !node2)
                    throw std::invalid_argument("Node labels not found: " + n1 + " " + n2);
                E* edge = new E(label, node1, node2);
                edges.insert({label, edge});
            }
            void addEdge(string label, string n1, string n2, int weight) override {
                if (getEdge(label))
                    throw std::invalid_argument("Edge already exists: " + label);
                N* node1 = getNode(n1);
                N* node2 = getNode(n2);
                if (!node1 || !node2)
                    throw std::invalid_argument("Node labels not found: " + n1 + " " + n2);
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
        static_assert(std::is_base_of<DirectedEdge, E>::value, "E must be of type stella::DirectedEdge for directed graphs");
        public:
            DirectedAdjList(): AdjList<N, E>() {}
            std::map<string, E*>& getAllEdges() {
                return this->edges;
            }
    };

    template<typename N, typename E>
    class AdjMatrix {
        static_assert(std::is_base_of<Edge, E>::value, "E must be of type stella::Edge for non-directed graphs");
    protected:
        void pushNode(int size) {
            edges.push_back(vector<map<string, E*>>{});
            for(int i = size - 1; i >= 0; i--) {
                edges[i].push_back(map<string, E*>{});
            }
        }
        vector<N*> nodes;
        vector<vector<map<string, E*>>> edges;
    public:
        AdjMatrix() {}
        void addNode(N* node) {
            if (getNode(node->getLabel()))
                throw std::invalid_argument("Node already exists: " + node->getLabel());
            nodes.push_back(node);
            pushNode(nodes.size());
        }
        void addNode(std::string label) {
            if (getNode(label))
                throw std::invalid_argument("Node already exists: " + label);
            N* node = new N(label);
            nodes.push_back(node);
            pushNode(nodes.size());
        }
        void addEdge(E* edge) {
        int n1 = getNodeIndex(edge->getN1()->getLabel());
        int n2 = getNodeIndex(edge->getN2()->getLabel());
        if (n1 < 0 || n2 < 0)
            throw std::invalid_argument("Node labels not found: "
            + edge->getN1()->getLabel() + " " + edge->getN2()->getLabel());
            if (n1 > n2) edges[n1][n2].insert({edge->getLabel(), edge});
            else if (n1 == n2) edges[n1][0].insert({edge->getLabel(), edge});
            else edges[n2][n1].insert({edge->getLabel(), edge});
        }

        void addEdge(string label, string n1, string n2) {
            int node1 = getNodeIndex(n1);
            int node2 = getNodeIndex(n2);
            if (node1 < 0 || node2 < 0)
                throw std::invalid_argument("Node labels not found: " + n1 + " " + n2);
            E* edge = new E(label, nodes[node1], nodes[node2], 1);
            if (node1 > node2) edges[node1][node2].insert({edge->getLabel(), edge});
            else if (node2 == node1) edges[node1][0].insert({edge->getLabel(), edge});
            else edges[node2][node1].insert({edge->getLabel(), edge});
        }

        void addEdge(string label, string n1, string n2, int weight) {
            int node1 = getNodeIndex(n1);
            int node2 = getNodeIndex(n2);
            if (node1 < 0 || node2 < 0)
                throw std::invalid_argument("Node labels not found: " + n1 + " " + n2);
            E* edge = new E(label, nodes[node1], nodes[node2], weight);
            if (node2 > node1) edges[node1][node2].insert({edge->getLabel(), edge});
            else if (node2 == node1) edges[node1][0].insert({edge->getLabel(), edge});
            else edges[node2][node1].insert({edge->getLabel(), edge});
        }
            N* getNode(std::string label) {
                for (N* node : nodes) {
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
            std::vector<N*>& getAllNodes() {
                return nodes;
            }
            std::vector<std::vector<std::map<string, E*>>>& getAllEdges() {
                return edges;
            }
            ~AdjMatrix() {
                int size = nodes.size();
                int dec = 0;
                for (int i = 0; i < size; i++, dec++) {
                    for (int j = 0; j < size - dec; j++) {
                        map<string, E*>& edgeMap = edges[i][j];
                        for (auto it = edgeMap.begin(); it != edgeMap.end(); ++it) {
                            delete it->second;
                        }
                        edgeMap.clear();
                    }
                }

                for (int i = 0; i < nodes.size(); i++)
                    delete nodes[i];

                edges.clear();
                nodes.clear();
            }
    };

    template<typename N, typename E>
    class DirectedAdjMatrix : public AdjMatrix<N, E> {
        static_assert(std::is_base_of<DirectedEdge, E>::value, "E must be of type stella::DirectedEdge for directed graphs");
    protected:
        void pushNode(int size) {
            this->edges.push_back(vector<map<string, E*>>{});
            for(int i = 0; i < size; i++) {
                if (i != size - 1)
                    this->edges[i].push_back(map<string, E*>{});
                else
                    for (int j = 0; j < size; j++)
                        this->edges[i].push_back(map<string, E*>{});
            }
        }
    public:
        DirectedAdjMatrix() : AdjMatrix<N, E>() {}
        void addNode(N* node) {
            if (this->getNode(node->getLabel()))
                throw std::invalid_argument("Node already exists: " + node->getLabel());
            this->nodes.push_back(node);
            pushNode(this->nodes.size());
        }
        void addNode(std::string label) {
            if (this->getNode(label))
                throw std::invalid_argument("Node already exists: " + label);
            N* node = new N(label);
            this->nodes.push_back(node);
            pushNode(this->nodes.size());
        }
        void addEdge(E* edge) {
            int n1 = this->getNodeIndex(edge->getN1()->getLabel());
            int n2 = this->getNodeIndex(edge->getN2()->getLabel());
            if (n1 < 0 || n2 < 0)
                throw std::invalid_argument("Node labels not found: "
                    + edge->getN1()->getLabel() + " " + edge->getN2()->getLabel());
            this->edges[n1][n2].insert({edge->getLabel(), edge});
        }
        void addEdge(string label, string n1, string n2) {
            int node1 = this->getNodeIndex(n1);
            int node2 = this->getNodeIndex(n2);
            if (node1 < 0 || node2 < 0)
                throw std::invalid_argument("Node labels not found: " + n1 + " " + n2);
            E* edge = new E(label, this->nodes[node1], this->nodes[node2], 1);
            this->edges[node1][node2].insert({edge->getLabel(), edge});
        }

        void addEdge(string label, string n1, string n2, int weight) {
            int node1 = this->getNodeIndex(n1);
            int node2 = this->getNodeIndex(n2);
            if (node1 < 0 || node2 < 0)
                throw std::invalid_argument("Node labels not found: " + n1 + " " + n2);
            E* edge = new E(label, this->nodes[node1], this->nodes[node2], weight);
            this->edges[node1][node2].insert({edge->getLabel(), edge});
        }
        ~DirectedAdjMatrix() {
            int size = this->nodes.size();
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    map<string, E*>& edgeMap = this->edges[i][j];
                    for (auto it = edgeMap.begin(); it != edgeMap.end(); ++it) {
                        delete it->second;
                    }
                    edgeMap.clear();
                }
            }

            for (int i = 0; i < this->nodes.size(); i++)
                delete this->nodes[i];

            this->edges.clear();
            this->nodes.clear();
        }
    };
};
#endif
