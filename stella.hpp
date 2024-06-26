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
            virtual E* getEdge(string label) = 0;
            virtual N* getNode(string label) = 0;
            virtual std::vector<N*>& getAllNodes() = 0;
            virtual std::map<string, E*>& getAllEdges() = 0;
    };

    template<typename N, typename E>
    class AdjList: Graph<N, E> {
        static_assert(std::is_base_of<Edge, E>::value, "E must be of type stella::Edge for non-directed graphs");
        private:
            vector<N*> nodes;
            map<string, E*> edges;
        public:
            AdjList() {}
            void addNode(N* node) override {
                nodes.push_back(node);
            }
            void addNode(string label) override {
                N* node = new N(label);
                nodes.push_back(node);
            }
            void addEdge(E* edge) override {
                edges.insert({edge->getLabel(), edge});
            }
            void addEdge(string label, string n1, string n2) override {
                N* node1 = getNode(n1);
                N* node2 = getNode(n2);
                E* edge = new E(label, node1, node2);
                edges.insert({edge->getLabel(), edge});
            }
            void addEdge(string label, string n1, string n2, int weight) override {
                N* node1 = getNode(n1);
                N* node2 = getNode(n2);
                E* edge = new E(label, node1, node2, weight);
                edges.insert({edge->getLabel(), edge});
            }
            E* getEdge(string label) override {
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
            std::map<string, E*>& getAllEdges() override {
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
    };
};
#endif
